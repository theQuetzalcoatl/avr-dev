#include "../kernel/kernel.h"

#if (NUM_OF_THREADS > MAX_THREADS || NUM_OF_THREADS == 0)
    #error "Number of maximum threads shall not exceed 255."
#endif

typedef struct ThreadControlBlock
{
    Thread thread[NUM_OF_THREADS];
    uint8_t current_thread;
}ThreadControlBlock;

static ThreadControlBlock tcb = {0};

#define RESTORE_CONTEXT()\
    SPL = (uint16_t)tcb.thread[tcb.current_thread].stack_pointer; \
    SPH = (uint16_t)tcb.thread[tcb.current_thread].stack_pointer >> 8; \
    asm volatile(" \
                pop R31 \n\
                out  __SREG__, R31  \n\
                pop R31 \n\
                out  __RAMPZ__, R31  \n\
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
                in R31, __RAMPZ__ \n  \
                push R31 \n  \
                in R31, __SREG__ \n  \
                push R31 \n  \
                "); \
    tcb.thread[tcb.current_thread].stack_pointer = SPL; \
    tcb.thread[tcb.current_thread].stack_pointer = (uint8_t *) ((uint16_t)tcb.thread[tcb.current_thread].stack_pointer | (SPH<<8u));

/*
    * OCRn = (F_CPU*T/2N) - 1

    where N(prescaler) = 1024, T should be in second and then devided by 1000 to get ms.
*/

#define MAX_MS_TICK (32u) /* Maximum possible period is about 32 ms. (2*1024*256)/(16*10^6) */
#define PRESCALER (1024u)

static uint8_t init_system_ticking(const uint8_t ms_tick)
{
    if(ms_tick > MAX_MS_TICK || ms_tick == 0u) return 11; /* checking systick boundaries  */

    uint32_t val = (uint32_t)ms_tick*F_CPU;
    val /= 2u;
    val/= PRESCALER;
    val/= 1000;
    val -= 1u;
    if(val < 256u) OCR0 = val;
    else return ERR_FAILED_INIT_SYSTICK;

    /* CTC mode */
    TCCR0 |= 1<<WGM01;
    TCCR0 &= ~(1<<WGM01);

    /* devide F_CLK by 128 */
    TCCR0 |= 1<<CS02 | 1<<CS01 | 1<<CS00;

    /* enable compare output match interrupt */
    TIMSK |= 1<<OCIE0; 
    /* NOTE: don't forget to clear the timer reg and interrupt flag before starting the OS */
    
    return NO_ERROR;
}


static void insert_stack_overflow_detection(uint8_t curr_thread);
static Register *init_stack(thread_address thread_addr, Register *stack_start, uint8_t curr_thread);
uint8_t kernel_register_thread(thread_address thread_addr, Register *stack_start, StackSize stack_size)
{
    if(tcb.current_thread > NUM_OF_THREADS-1) return GENERAL_ERROR;
    if(stack_size > MAX_STACK_SIZE || stack_size < MIN_STACK_SIZE) return ERR_INVALID_STACKSIZE;

    /* in case of avr-gcc we get the stack_start upside down, starting at bottom address. So it is changed here */
    tcb.thread[tcb.current_thread] = \
        (Thread){
        .stack_bottom = stack_start,
        .stack_pointer = stack_start + stack_size - 1,
        .state = READY,
        .remaining_wait_ticks = 0u
        };
    
    (void)init_stack(thread_addr, tcb.thread[tcb.current_thread].stack_pointer, tcb.current_thread);
    
    ++tcb.current_thread;

    return NO_ERROR;
}

static void insert_stack_overflow_detection(uint8_t curr_thread)
{
    *(tcb.thread[curr_thread].stack_bottom + 1) = 0xBE;
    *(tcb.thread[curr_thread].stack_bottom) = 0xEF;
}

/*
    PC[7:0]
    PC[15:8]
    R0
    R1
    ...
    R31
    RAMPZ
    SREG
*/
static Register *init_stack(thread_address thread_addr, Register *stack_start, uint8_t curr_thread)
{
    Register *stack_pointer = stack_start;

    *stack_pointer = (uint16_t)thread_addr & (uint16_t)0x00ffu; --stack_pointer;
    *stack_pointer = (uint16_t)thread_addr>>8u; --stack_pointer;
    *stack_pointer = 0u; /* R0 */ --stack_pointer;
    *stack_pointer = 0u; /* R1 */ --stack_pointer;
    for(int i = 2; i < 32; ++i, --stack_pointer) *stack_pointer = i; /* R2 - R31 */
    *stack_pointer = RAMPZ; /* RAMPZ */ --stack_pointer;
    *stack_pointer = SREG | 0x80; /* SREG - with global interrupt enabled */ --stack_pointer;
    tcb.thread[curr_thread].stack_pointer = stack_pointer;

    insert_stack_overflow_detection(curr_thread);
    
    return stack_pointer;
}

static uint8_t check_stack_for_overflow(void)
{
    if(*(tcb.thread[tcb.current_thread].stack_bottom + 1) != 0xBE || *(tcb.thread[tcb.current_thread].stack_bottom) != 0xEF) return ERR_STACK_OVERFLOW;
    else return NO_ERROR;
}

static void schedule_next_task(void)
{
    tcb.thread[tcb.current_thread].state = READY;

    while(TRUE){
        if((tcb.current_thread + 1) > (NUM_OF_THREADS-1)) tcb.current_thread = 0;
        else ++tcb.current_thread;

        if(tcb.thread[tcb.current_thread].remaining_wait_ticks == 0){
            tcb.thread[tcb.current_thread].state = RUNNING;
            break;
        }
        else{
            --tcb.thread[tcb.current_thread].remaining_wait_ticks;
            continue;
        }
    }
}

static void start_scheduling(void)
{
    tcb.current_thread = 0;
    RESTORE_CONTEXT();
    asm volatile("ret \n"); // Note : embedded assembly is needed because the compiler may optimize the function call out, thus not returning here
}

uint8_t kernel_init_os(void)
{
    cli();
    uint8_t ret = NO_ERROR;

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
    
    ret |= init_system_ticking(SYSTEM_TICK_IN_MS);

    if(ret != NO_ERROR) return ret;

    TCNT0 = 0;
    TIFR |= 1<<OCF0;
    sei();
    start_scheduling();

    return ret; /* we should never be here */
}

void TIMER0_COMP_vect( void ) __attribute__ ( ( signal, naked ) );
void TIMER0_COMP_vect( void )
{
    // global interrupt flag is disabled
    STORE_CONTEXT();
    if(ERR_STACK_OVERFLOW == check_stack_for_overflow()){ // buzzing with 1kHz
        while(TRUE){
            PORTB ^= 1<<PINB4; 
            _delay_ms(1);
        }
    }
    schedule_next_task();
    RESTORE_CONTEXT();
    asm volatile ("reti"); // enables global interrupt flag
}
