#include "Aztec/kernel/kernel.h"

register_t thread_1_stack[MIN_STACK_SIZE+6];
void thread_1(void)
{
    while(1){
        if(button_get_state(BUTTON_2) == PIN_LOW) uart_send('U');
    }
}

register_t thread_2_stack[MIN_STACK_SIZE+6];
void heartbeat(void)
{
    device_id_t id = led_1_lease();
    for(;;){
        led_1_toggle(id);
        kernel_wait_us(500000);
    }
    led_1_release();
}

register_t thread_3_stack[MIN_STACK_SIZE+6];
void thread_3(void)
{
    char key;
    volatile uint16_t freq = 0;
    lcd_turn_backligh_on();

    while(1){
        for(int i = 4; i; --i){
            key = keypad_get_pressed_key();
            switch(key)
            {
                case KEYPAD_0:
                    lcd_write('0');
                    freq = freq*10;
                break;

                case KEYPAD_1:
                    lcd_write('1');
                    freq = freq*10;
                    freq += 1;
                break;

                case KEYPAD_2:
                    lcd_write('2');
                    freq = freq*10;
                    freq += 2;
                break;

                case KEYPAD_3:
                    lcd_write('3');
                    freq = freq*10;
                    freq += 3;
                break;

                case KEYPAD_4:
                    lcd_write('4');
                    freq = freq*10;
                    freq += 4;
                break;

                case KEYPAD_5:
                    lcd_write('5');
                    freq = freq*10;
                    freq += 5;
                break;

                case KEYPAD_6:
                    lcd_write('6');
                    freq = freq*10;
                    freq += 6;
                break;

                case KEYPAD_7:
                    lcd_write('7');
                    freq = freq*10;
                    freq += 7;
                break;

                case KEYPAD_8:
                    lcd_write('8');
                    freq = freq*10;
                    freq += 8;
                break;

                case KEYPAD_9:
                    lcd_write('9');
                    freq = freq*10;
                    freq += 9;
                break;

                case KEYPAD_ASTERISK:
                case KEYPAD_POUND:
                break;
            }
            kernel_wait_ms(120);
        }
        lcd_send_command(LCD_CLEAR);
        if(freq == 0)buzzer_off();
        else buzzer_buzz(freq);
        freq = 0;
    }
}

register_t thread_4_stack[MIN_STACK_SIZE + 6];
void thread_4(void)
{
    device_id_t id = led_2_lease();
    for(;;){
#if CONFIG_THREADS_QUERY_STATE == TRUE
        if(WAITING == kernel_get_thread_state(thread_3)) led_2_on(id);
        else led_2_off(id);
#else
        toggle_led(LED4);
        kernel_wait_ms(200);
#endif
    }
    led_2_release();
}

int main(void)
{
    kernel_register_thread(thread_1, thread_1_stack, MIN_STACK_SIZE+6);
    kernel_register_thread(heartbeat, thread_2_stack, MIN_STACK_SIZE+6);
    kernel_register_thread(thread_3, thread_3_stack, MIN_STACK_SIZE+6);
    kernel_register_thread(thread_4, thread_4_stack, MIN_STACK_SIZE+6);

    kernel_start_os();

    while(1){;} // just in case
 
    return 0;
}