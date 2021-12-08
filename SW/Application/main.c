#include "Aztec/kernel/kernel.h"

register_t thread_1_stack[MIN_STACK_SIZE+10];
void thread_1(void)
{
    lease(DEV_UART);
    while(1){
        if(button_get_state(BUTTON_2) == PIN_LOW) uart_putc('U');
    }
}

register_t thread_2_stack[MIN_STACK_SIZE+10];
void heartbeat(void)
{
    lease(DEV_LED1);
    for(;;){
        led_1_toggle();
        wait_us(500000);
    }
    release(DEV_LED1);
}

register_t thread_3_stack[MIN_STACK_SIZE+10];
void thread_3(void)
{
    char key;
    uint16_t freq = 0;
    wait_ms(10);
    while(lease(DEV_LCD) == K_ERR_INVALID_DEVICE_ACCESS){;}
    lease(DEV_BUZZER);
    
    lcd_turn_backligh_on();

    while(1){
        for(int i = 4; i; --i){
            key = keypad_get_pressed_key();

            lcd_write(key + '0');
            freq = freq*10;
            freq += key;
            
            wait_ms(120);
        }
        lcd_send_command(LCD_CLEAR);
        if(freq == 0)buzzer_off();
        else buzzer_buzz(freq);
        freq = 0;
    }
    release(DEV_LCD);
}

register_t thread_4_stack[MIN_STACK_SIZE + 10];
void thread_4(void)
{
    lease(DEV_LED2);
    for(;;){
#if CONFIG_THREADS_QUERY_STATE == TRUE
        if(WAITING == get_thread_state(thread_3)) led_2_on();
        else led_2_off();
#else
        lease(DEV_LED4);
        led_4_toggle();
        wait_ms(200);
#endif
    }
    release(DEV_LED2);
}

register_t kv_stack[MIN_STACK_SIZE + 10];
void display_kernel_version(void)
{
    lease(DEV_LCD);
    lcd_turn_backligh_on();
    lcd_print("  ");
    lcd_print("Aztec "KERNEL_VERSION);
    wait_ms(2000);
    lcd_send_command(LCD_CLEAR);
    exit_(); // TEST: not releasing device before exiting
}



int main(void)
{
    register_thread(thread_1, thread_1_stack, sizeof(thread_1_stack));
    register_thread(heartbeat, thread_2_stack, sizeof(thread_2_stack));
    register_thread(thread_3, thread_3_stack, sizeof(thread_3_stack));
    register_thread(thread_4, thread_4_stack, sizeof(thread_4_stack));
    register_thread(display_kernel_version, kv_stack, sizeof(kv_stack));

    start_os();

    while(1){;} // just in case
 
    return 0;
}