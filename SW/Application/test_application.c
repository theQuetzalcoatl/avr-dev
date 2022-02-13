#include "Aztec/kernel/kernel.h"

register_t halt_thread_stack[CONFIG_MIN_STACK_SIZE + 10];
void halt_thread(void)
{
    while(1){
        if(button_get_state(BUTTON_3) == PRESSED) reboot();
        //if(button_get_state(BUTTON_3) == PRESSED) halt_system();
    }
}

register_t thread_1_stack[CONFIG_MIN_STACK_SIZE+10];
void thread_1(void)
{
    lease(DEV_UART);
    while(1){
        if(button_get_state(BUTTON_2) == PIN_LOW) uart_putc('U');
    }
}

register_t heartbeat_stack[CONFIG_MIN_STACK_SIZE+10];
void heartbeat(void)
{
    lease(DEV_LED1);
    for(;;){
        led_1_toggle();
        wait_us(500000);
    }
    release(DEV_LED1);
}

register_t thread_3_stack[CONFIG_MIN_STACK_SIZE+20];
void thread_3(void)
{
    wait_ms(10);
    while(lease(DEV_LCD) == K_ERR_INVALID_DEVICE_ACCESS){;}
    lease(DEV_ADC);
    
    char buff[5] = {0};
    adc_t val = 0;
    
    lcd_turn_backligh_on();

    while(1){
        adc_convert(&val);
        itoa(val, buff, 10);
        lcd_move_cursor(1,1);
        lcd_print(buff);
        lcd_move_cursor(5,2);
        lcd_print(buff);
        wait_ms(200);
        lcd_send_command(LCD_CLEAR);
    }
    release(DEV_LCD);
}

register_t thread_4_stack[CONFIG_MIN_STACK_SIZE + 10];
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

register_t kv_stack[CONFIG_MIN_STACK_SIZE + 10];
void display_kernel_version(void)
{
    lease(DEV_LCD);
    lcd_turn_backligh_on();
    lcd_print("  ");
    lcd_print("Aztec "KERNEL_VERSION " ");
    lcd_write(get_num_of_threads() + '0');
    wait_ms(2000);
    lcd_send_command(LCD_CLEAR);
    exit_(); // TEST: not releasing device before exiting, OS does it
}



int main(void)
{
    init_device(button_init_device, DEV_BUTTON);
    init_device(led_init_device, DEV_LED1); /* TEST: many devices can have the same init function */
    init_device(led_init_device, DEV_LED2);
    init_device(led_init_device, DEV_LED3);
    init_device(led_init_device, DEV_LED4);
    init_device(lcd_init_device, DEV_LCD);
    init_device(uart_init_device, DEV_UART);
    init_device(buzzer_init_device, DEV_BUZZER);
    init_device(adc_init_device, DEV_ADC);
    keypad_init_device(); /* TEST: devices can be handled outside of the kernels supervision */

    register_thread(thread_1, thread_1_stack, sizeof(thread_1_stack));
    register_thread(heartbeat, heartbeat_stack, sizeof(heartbeat_stack));
    register_thread(thread_3, thread_3_stack, sizeof(thread_3_stack));
    register_thread(thread_4, thread_4_stack, sizeof(thread_4_stack));
    register_thread(display_kernel_version, kv_stack, sizeof(kv_stack));
    register_thread(halt_thread, halt_thread_stack, sizeof(halt_thread_stack));

    start_os();

    while(1){;} // just in case
 
    return 0;
}