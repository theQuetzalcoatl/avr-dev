#include "Aztec/kernel/kernel.h"

void thread_1(void)
{
    while(1){
        if(button_get_button_state(BUTTON_2) == PIN_LOW){
            uart_send('U');
            _delay_ms(120);
        }
    }
}

void thread_2(void)
{
    for(int i = 10; i ; --i){
        toggle_led(LED1);
        _delay_ms(100);
    }
    kernel_exit();
}

void thread_3(void)
{
    for(int i = 5; i ; --i){
        toggle_led(LED3);
        _delay_ms(300);
    }
    turn_led_off(LED3);
    kernel_exit();
}

int main(void)
{
    uint8_t thread_1_stack[MIN_STACK_SIZE+30];
    kernel_register_thread(thread_1, thread_1_stack, MIN_STACK_SIZE+30);
    
    uint8_t thread_2_stack[MIN_STACK_SIZE+5];
    kernel_register_thread(thread_2, thread_2_stack, MIN_STACK_SIZE+5);
    
    uint8_t thread_3_stack[MIN_STACK_SIZE+30];
    kernel_register_thread(thread_3, thread_3_stack, MIN_STACK_SIZE+30);

    kernel_init_os();
 
    return 0;
}