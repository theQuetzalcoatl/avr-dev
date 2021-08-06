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

void sys_alive_led(void)
{
    for(; ;){
        toggle_led(LED1);
        _delay_ms(100);
    }
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
    uint8_t thread_1_stack[MIN_STACK_SIZE+4];
    kernel_register_thread(thread_1, thread_1_stack, MIN_STACK_SIZE+4);
    
    uint8_t thread_2_stack[MIN_STACK_SIZE+2];
    kernel_register_thread(sys_alive_led, thread_2_stack, MIN_STACK_SIZE+2);
    
    uint8_t thread_3_stack[MIN_STACK_SIZE+30];
    kernel_register_thread(thread_3, thread_3_stack, MIN_STACK_SIZE+30);
    kernel_init_os();

    return 0;
}