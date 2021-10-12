#include "../kernel/kernel.h"

#if (CONFIG_NUM_OF_THREADS > CONFIG_MAX_THREADS || CONFIG_NUM_OF_THREADS == 0)
    #error "Number of maximum threads shall not exceed 255."
#endif

#define SWITCH_THREAD() TIMER2_COMP_vect()

typedef struct thread_t
{
    register_t *stack_pointer; /* must be first field */
    register_t *stack_bottom;
    uint8_t state;
    uint16_t wait_roundabouts;
#if CONFIG_THREADS_QUERY_STATE == TRUE
    thread_address_t id;
#endif
    struct thread_t *next;
}thread_t;


typedef struct thread_control_block_t
{
    thread_t *current_thread;  /* must be first field */
    thread_t *prev_thread;
    uint8_t active_threads; /* = not deleted */
    thread_t thread[CONFIG_NUM_OF_THREADS];
}thread_control_block_t;

static thread_control_block_t tcb = {0};

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
extern void kernel_panic(void);

/*
    * OCRn = (F_CPU*T/(2*prescaler)) - 1

    T should be in microseconds, it gets devided by 1,000,000 to get us.
*/
#if   F_CPU == 8000000UL
    #define MAX_US_TICK (8000u)
#elif F_CPU == 16000000UL
    #define MAX_US_TICK (4000u) /* (2*256*256)/(16*10^6) */
#else
    #error "MCU clock frequency must be either 8MHz or 16MHz!"
#endif
#define MIN_US_TICK (100u) // this number is based on the fact that thread switching takes about 15 microsec
#define PRESCALER (256u)

static uint8_t init_system_ticking(void)
{
#if CONFIG_SYSTEM_TICK_IN_US > MAX_US_TICK || CONFIG_SYSTEM_TICK_IN_US < MIN_US_TICK
    #error "OS's system tick period is out of bounds!"
#endif

    uint16_t us_tick = CONFIG_SYSTEM_TICK_IN_US * 2u; // the formula calculates the period, but we want half of that so it gets multiplied

    uint32_t val = F_CPU/100000u; // 10^5 is used instead of 10^6(to make sec to usec) but I keep the extra digit to be able to use rounding, after that, it is devided by 10, thus 10^6 in total
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

static void start_scheduling(void)
{
    tcb.current_thread = &tcb.thread[0];
    tcb.current_thread->state = RUNNING;
    tcb.prev_thread = &tcb.thread[0];
    RESTORE_CONTEXT();
    asm volatile("ret"); // the compiler may optimize the function call out, thus we would not return here
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

void disable_systick(void)
{
    TCCR2 &= ~(1<<CS22 | 1<<CS21 | 1<<CS20);
    TIMSK &= ~(1<<OCIE2);
}

    /******************* SYSTEM CALLS, PUBLIC APIs *******************/

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

void kernel_wait_us(const uint32_t us)
{
    if(us != 0){
    KERNEL_ENTER_ATOMIC();
        uint32_t tmp = us/(CONFIG_SYSTEM_TICK_IN_US*tcb.active_threads);
        if(tmp == 0) tcb.current_thread->wait_roundabouts = 1;
        else if(tmp < (uint16_t)~0) tcb.current_thread->wait_roundabouts = tmp;
        else tcb.current_thread->wait_roundabouts = ~0;
        tcb.current_thread->state = WAITING;
    KERNEL_EXIT_ATOMIC();
        
        thread_t volatile *curr = tcb.current_thread;
        while(curr->state == WAITING){;}
    }
}

void kernel_wait_ms(const uint16_t ms)
{
    kernel_wait_us((uint32_t)ms*1000);
}

static void init_device_drivers(void);
static void make_threadlist_circular(void);
static void sort_thread_list_descending(void);
static void link_thread_list(void);

k_error_t kernel_start_os(void)
{
    KERNEL_ENTER_ATOMIC();
    uint8_t ret = NO_ERROR;

    if(tcb.active_threads == 0 || tcb.active_threads > CONFIG_NUM_OF_THREADS) return K_ERR_THREAD_NUM_OUT_OF_BOUNDS;

    ret = init_system_ticking();
    if(ret != NO_ERROR) return ret;

    init_device_drivers();

#if CONFIG_THREADS_QUERY_STATE == TRUE
    sort_thread_list_descending();
    link_thread_list();
#endif
    make_threadlist_circular();

    RESET_SYSTICK_TIMER();
    if(TIFR & (1<<OCF2)) TIFR |= 1<<OCF2;
    KERNEL_EXIT_ATOMIC();
    start_scheduling();

    return ret;
}

static void make_threadlist_circular(void)
{
    tcb.thread[tcb.active_threads-1].next = &tcb.thread[0];
}

static void init_device_drivers(void)
{
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
}

#if CONFIG_THREADS_QUERY_STATE == TRUE
static void sort_thread_list_descending(void)
{
    thread_t temp = {0};
    if(tcb.active_threads > 1){
        // for now it is a simple bubblesort 
        for(uint8_t i = 0; i < tcb.active_threads; ++i){
            for(uint8_t ii = 0; ii < tcb.active_threads - i -1; ++ii){
                if(tcb.thread[ii].id < tcb.thread[ii+1].id){
                    temp = tcb.thread[ii];
                    tcb.thread[ii] = tcb.thread[ii+1];
                    tcb.thread[ii+1] = temp;
                }
            }
        }
    }
}

static void link_thread_list(void)
{
    for(uint8_t i = 0; i < tcb.active_threads-1; ++i) tcb.thread[i].next = &tcb.thread[i+1];
}

k_error_t kernel_get_thread_state(const thread_address_t th_addr)
{
#if (CONFIG_NUM_OF_THREADS > 1)
        uint8_t max = 0;
        uint8_t min = CONFIG_NUM_OF_THREADS-1;
        uint8_t middle = CONFIG_NUM_OF_THREADS / 2;

        if(tcb.thread[max].id == th_addr) return tcb.thread[max].state;
        else if(tcb.thread[min].id == th_addr) return tcb.thread[min].state;

        for(; middle != max && middle != min; middle = (max + min)/2){
            if(tcb.thread[middle].id == th_addr) return tcb.thread[middle].state;
            else if(tcb.thread[middle].id > th_addr) max = middle;
            else min = middle;
            middle = (min + max)/2;
        }
        return K_ERR_THREAD_NOT_FOUND;
#else
    return tcb.current_thread->state;
#endif
}

#endif

static void init_stack(const thread_address_t thread_addr, register_t * const stack_start);
static k_error_t check_if_stack_is_already_registered(register_t * const stack_start);

k_error_t kernel_register_thread(const thread_address_t thread_addr,  register_t * const stack_start, const stack_size_t stack_size)
{
    /* error checking */
    if(tcb.active_threads > CONFIG_NUM_OF_THREADS-1) return K_ERR_THREAD_NUM_OUT_OF_BOUNDS;
    if(stack_size > MAX_STACK_SIZE || stack_size < MIN_STACK_SIZE) return K_ERR_INVALID_STACKSIZE;
    if(tcb.active_threads > 0){
        if(check_if_stack_is_already_registered(stack_start) == K_ERR_ALREADY_REGISTERED_STACK) return K_ERR_ALREADY_REGISTERED_STACK;
    }

    tcb.current_thread = &tcb.thread[tcb.active_threads];

    /* in case of avr-gcc we get the stack_start upside down, starting at bottom address. So it is flipped here */
    *tcb.current_thread = \
        (thread_t){
        .stack_bottom = stack_start,
        .stack_pointer = stack_start + stack_size - 1,
        .state = READY,
        .wait_roundabouts = 0,
#if CONFIG_THREADS_QUERY_STATE == TRUE
        .id = thread_addr,
        .next = 0 
#else
        .next = &tcb.thread[tcb.active_threads+1] // safe, because if thread_number=NUM_OF_THREADS-1 this pointer will be changed to the first thread. hread_number=NUM_OF_THREADS case is cought by at the start of function
#endif
        };
    
    init_stack(thread_addr, tcb.current_thread->stack_pointer);
    
    ++tcb.active_threads;

    return NO_ERROR;
}

/* CONTEXT

    PC[7:0]
    PC[15:8]
    R0
    R1
    ...
    R31
    SREG
*/
static void insert_stack_overflow_detection_bytes(void);

static void init_stack(const thread_address_t thread_addr, register_t * const stack_start)
{
    register_t *stack_pointer = stack_start;

    *stack_pointer = (uint16_t)thread_addr & (uint16_t)0x00ffu; --stack_pointer;
    *stack_pointer = (uint16_t)thread_addr>>8u; --stack_pointer;
    *stack_pointer = 0u; /* R0 */ --stack_pointer;
    *stack_pointer = 0u; /* R1 */ --stack_pointer;
    for(int i = 2; i <= 31; ++i, --stack_pointer) *stack_pointer = i; /* R2 - R31 */
    *stack_pointer = SREG | 0x80; /* SREG - with global interrupt enabled */ --stack_pointer;
    tcb.current_thread->stack_pointer = stack_pointer;

    insert_stack_overflow_detection_bytes();   
}

static void insert_stack_overflow_detection_bytes(void)
{
    *(tcb.current_thread->stack_bottom + 1) = 0xBE;
    *(tcb.current_thread->stack_bottom) = 0xEF;
}

static k_error_t check_if_stack_is_already_registered(register_t * const stack_start)
{
    for(int8_t i = tcb.active_threads-1; i >= 0; --i){  
        if(tcb.thread[i].stack_bottom == stack_start) return K_ERR_ALREADY_REGISTERED_STACK;
    }
    return NO_ERROR;
}


/*
 * kernel.c
 * This file is part of Aztec
 *
 * Copyright (C) 2021 - theQuetzalcoatl
 *
 * Aztec is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Aztec is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Aztec. If not, see <http://www.gnu.org/licenses/>.
 */