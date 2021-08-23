#include "../kernel/kernel.h"
#include "../kernel/error_handling.h"

#if (CONFIG_NUM_OF_THREADS > CONFIG_MAX_THREADS || CONFIG_NUM_OF_THREADS == 0)
    #error "Number of maximum threads shall not exceed 255."
#endif

#define SWITCH_THREAD() TIMER2_COMP_vect()

/* THREAD STATES */
#define RUNNING ('X')
#define WAITING ('W')
#define READY   ('R')
#define DELETED ('D')

typedef struct Thread
{
    Register *stack_pointer; /* must be first field */
    Register *stack_bottom;
    uint8_t state;
    uint16_t wait_roundabouts;
    struct Thread *next;
}Thread;


typedef struct ThreadControlBlock
{
    Thread *current_thread;  /* must be first field */
    Thread *prev_thread;
    uint8_t active_threads; /* = not deleted */
    Thread thread[CONFIG_NUM_OF_THREADS];
}ThreadControlBlock;

static ThreadControlBlock tcb = {0};

#define RESET_SYSTICK_TIMER() TCNT2 = 0

#define RESTORE_CONTEXT() \
    asm volatile(" \
                lds R26, tcb \n\
                lds R27, tcb+1 \n\
                ld R31, X+ \n\
                out __SP_L__, R31 \n\
                ld R31, X \n\
                out __SP_H__, R31 \n\
                pop R31 \n\
                out  __SREG__, R31  \n\
                pop R31 \n  \
                pop R30 \n  \
                pop R29 \n  \
                pop R28 \n  \
                pop R27 \n  \
                pop R26 \n  \
                pop R25 \n  \
                pop R24 \n  \
                pop R23 \n  \
                pop R22 \n  \
                pop R21 \n  \
                pop R20 \n  \
                pop R19 \n  \
                pop R18 \n  \
                pop R17 \n  \
                pop R16 \n  \
                pop R15 \n  \
                pop R14 \n  \
                pop R13 \n  \
                pop R12 \n  \
                pop R11 \n  \
                pop R10 \n  \
                pop R9 \n  \
                pop R8 \n  \
                pop R7 \n  \
                pop R6 \n  \
                pop R5 \n  \
                pop R4 \n  \
                pop R3 \n  \
                pop R2 \n  \
                pop R1 \n  \
                pop R0 \n  \
                "); 

#define STORE_CONTEXT()\
    asm volatile(" \
                push R0 \n  \
                push R1 \n  \
                push R2 \n  \
                push R3 \n  \
                push R4 \n  \
                push R5 \n  \
                push R6 \n  \
                push R7 \n  \
                push R8 \n  \
                push R9 \n  \
                push R10 \n  \
                push R11 \n  \
                push R12 \n  \
                push R13 \n  \
                push R14 \n  \
                push R15 \n  \
                push R16 \n  \
                push R17 \n  \
                push R18 \n  \
                push R19 \n  \
                push R20 \n  \
                push R21 \n  \
                push R22 \n  \
                push R23 \n  \
                push R24 \n  \
                push R25 \n  \
                push R26 \n  \
                push R27 \n  \
                push R28 \n  \
                push R29 \n  \
                push R30 \n  \
                push R31 \n  \
                in R31, __SREG__ \n  \
                push R31 \n  \
                lds R26, tcb \n\
                lds R27, tcb+1 \n\
                in R31, __SP_L__ \n\
                st X+, R31 \n\
                in R31, __SP_H__ \n\
                st X, R31 \n\
                ");

extern void disable_systick(void);

/*
    * OCRn = (F_CPU*T/(2*prescaler)) - 1

    T should be in microseconds, it gets devided by 1,000,000 to get us.
*/
#if   F_CPU == 8000000UL
    #define MAX_MS_TICK (8000u)
#elif F_CPU == 16000000UL
    #define MAX_MS_TICK (4000u) /* (2*256*256)/(16*10^6) */
#else
    #error "MCU clock must be either 8MHz or 16MHz!"
#endif
#define MIN_MS_TICK (100u)
#define PRESCALER (256u)

static uint8_t init_system_ticking(void)
{
#if CONFIG_SYSTEM_TICK_IN_US > MAX_MS_TICK || CONFIG_SYSTEM_TICK_IN_US < MIN_MS_TICK
    #error "OS's system tick period is out of bounds!"
#endif

    uint16_t us_tick = CONFIG_SYSTEM_TICK_IN_US * 2u; // the formula calculates the period, but we want half of that so it gets multiplied

    uint32_t val = F_CPU/100000u; // 10^5 is used insted of 10^6(to make sec to usec) but I keep the extra digit to be able to use rounding, after that, it is devided by 10, thus 10^6 in total
    val *= (uint32_t)us_tick;
    val /= 2u;
    val /= PRESCALER;
    val += 5u; // rounding
    val /= 10u;
    val -= 1u;
    if(val <= 255u) OCR2 = val;

    TCCR2 |= 1<<WGM21;
    TCCR2 &= ~(1<<WGM20);

    DDRB |= 1<<PINB7;
    PORTB &= ~(1<<PINB7);
    TCCR2 &= ~(1<<COM21);
    TCCR2 |= 1<<COM20;

    TCCR2 |= 1<<CS22;
    TCCR2 &= ~(1<<CS21 | 1<<CS20);
    
    /* NOTE: don't forget to clear the timer reg and interrupt flag before starting the OS */

    TIMSK |= 1<<OCIE2;
    
    return NO_ERROR;
}


static void insert_stack_overflow_detection(void);
static Register *init_stack(const ThreadAddress thread_addr, Register * const stack_start);
uint8_t kernel_register_thread(const ThreadAddress thread_addr,  Register * const stack_start, const StackSize stack_size)
{
    if(tcb.active_threads > CONFIG_NUM_OF_THREADS-1) return K_ERR_THREAD_NUM_OUT_OF_BOUNDS;
    if(stack_size > MAX_STACK_SIZE || stack_size < MIN_STACK_SIZE) return K_ERR_INVALID_STACKSIZE;

    tcb.current_thread = &tcb.thread[tcb.active_threads];

    /* in case of avr-gcc we get the stack_start upside down, starting at bottom address. So it is flipped here */
    *tcb.current_thread = \
        (Thread){
        .stack_bottom = stack_start,
        .stack_pointer = stack_start + stack_size - 1,
        .state = READY,
        .wait_roundabouts = 0,
        .next = &tcb.thread[tcb.active_threads + 1] // safe, because if thread_number=NUM_OF_THREADS-1 this pointer will be changed to the first thread. hread_number=NUM_OF_THREADS case is cought by at the start of function
        };
    
    (void)init_stack(thread_addr, tcb.current_thread->stack_pointer);
    
    ++tcb.active_threads;

    return NO_ERROR;
}

static void insert_stack_overflow_detection()
{
    *(tcb.current_thread->stack_bottom + 1) = 0xBE;
    *(tcb.current_thread->stack_bottom) = 0xEF;
}

/*
    PC[7:0]
    PC[15:8]
    R0
    R1
    ...
    R31
    SREG
*/
static Register *init_stack(const ThreadAddress thread_addr, Register * const stack_start)
{
    Register *stack_pointer = stack_start;

    *stack_pointer = (uint16_t)thread_addr & (uint16_t)0x00ffu; --stack_pointer;
    *stack_pointer = (uint16_t)thread_addr>>8u; --stack_pointer;
    *stack_pointer = 0u; /* R0 */ --stack_pointer;
    *stack_pointer = 0u; /* R1 */ --stack_pointer;
    for(int i = 2; i < 32; ++i, --stack_pointer) *stack_pointer = i; /* R2 - R31 */
    *stack_pointer = SREG | 0x80; /* SREG - with global interrupt enabled */ --stack_pointer;
    tcb.current_thread->stack_pointer = stack_pointer;

    insert_stack_overflow_detection();
    
    return stack_pointer;
}

static void start_scheduling(void)
{
    tcb.current_thread = &tcb.thread[1];
    tcb.current_thread->state = RUNNING;
    tcb.prev_thread = &tcb.thread[0];
    RESTORE_CONTEXT();
    asm volatile("ret \n"); // Note : embedded assembly is needed because the compiler may optimize the function call out, thus not returning here
}

static void make_threadlist_circular(void);
uint8_t kernel_start_os(void)
{
    KERNEL_ENTER_ATOMIC();
    uint8_t ret = NO_ERROR;

    if(tcb.active_threads == 0 || tcb.active_threads > CONFIG_NUM_OF_THREADS) return K_ERR_THREAD_NUM_OUT_OF_BOUNDS;

    ret = init_system_ticking();
    if(ret != NO_ERROR) return ret;

    const Uart uart =
    {
        .baud_rate=9600,
        .mode=NORMAL_MODE,
        .num_of_bits=8,
        .num_of_stop_bits=ONE_STOP_BIT,
        .parity=NO_PARITY
    };

    (void)uart_init_device(&uart);
    button_init_device();
    buzzer_init_device();
    keypad_init_device();
    led_init_device();
    lcd_init_device();

    make_threadlist_circular();

    RESET_SYSTICK_TIMER();
    if(TIFR & (1<<OCF2)) TIFR |= 1<<OCF2;
    KERNEL_EXIT_ATOMIC();
    start_scheduling();

    return ret;
}

static void make_threadlist_circular(void)
{
    tcb.current_thread->next = &tcb.thread[0];
}

static void schedule_next_task(void);
static uint8_t check_stack_for_overflow(void);
void TIMER2_COMP_vect( void ) __attribute__ ( ( signal, naked ) );
void TIMER2_COMP_vect( void )
{
    // global interrupt flag is disabled
    STORE_CONTEXT();
    if(K_ERR_STACK_OVERFLOW == check_stack_for_overflow()) kernel_panic();
    schedule_next_task();
    RESET_SYSTICK_TIMER();
    RESTORE_CONTEXT();
    asm volatile ("reti"); // enables global interrupt flag
}

static uint8_t check_stack_for_overflow(void)
{
    if(*(tcb.current_thread->stack_bottom + 1) != 0xBE || *(tcb.current_thread->stack_bottom) != 0xEF) return K_ERR_STACK_OVERFLOW;
    else return NO_ERROR;
}

static void schedule_next_task(void)
{
    Thread *curr = tcb.current_thread;
    switch(tcb.current_thread->state){
        case WAITING: // we are here because of a regular systick interrupt
            --tcb.current_thread->wait_roundabouts;
            tcb.prev_thread = tcb.current_thread;
            if(tcb.current_thread->wait_roundabouts == 0) tcb.current_thread->state = READY;
            break;

        case RUNNING: // we are here because of a regular systick interrupt
            tcb.current_thread->state = READY;
            tcb.prev_thread = tcb.current_thread;
            break;
        
        case DELETED: // we are here because of an exit syscall
            break;

        case READY:
        default:
            kernel_panic();
    }
    tcb.current_thread = tcb.current_thread->next;
    if(tcb.current_thread->state == READY) tcb.current_thread->state = RUNNING;
    else if(tcb.current_thread->state == WAITING);
    else kernel_panic();
}

static void remove_curr_thread_from_list(void);
void kernel_exit(void)
{
    KERNEL_ENTER_ATOMIC();
    tcb.current_thread->state = DELETED;
    --tcb.active_threads;

    if(tcb.active_threads == 0){
        disable_systick();
        while(1){;}
    }

    remove_curr_thread_from_list();

    SWITCH_THREAD();
}

static void remove_curr_thread_from_list(void)
{
    tcb.prev_thread->next = tcb.current_thread->next;
}

void disable_systick(void)
{
    TCCR2 &= ~(1<<CS22 | 1<<CS21 | 1<<CS20);
    TIMSK &= ~(1<<OCIE2);
}

void kernel_wait_us(uint32_t us)
{
    KERNEL_ENTER_ATOMIC();
    if(us != 0){
        uint32_t tmp = us/(CONFIG_SYSTEM_TICK_IN_US*tcb.active_threads);
        if(tmp == 0) tcb.current_thread->wait_roundabouts = 1;
        else if(tmp < (uint16_t)~0) tcb.current_thread->wait_roundabouts = tmp;
        else tcb.current_thread->wait_roundabouts = ~0;
        tcb.current_thread->state = WAITING;
    KERNEL_EXIT_ATOMIC();
        
        Thread volatile *curr = tcb.current_thread;
        while(curr->state == WAITING){;}
    }
}

void kernel_wait_ms(uint16_t ms)
{
    kernel_wait_us((uint32_t)ms*1000);
}