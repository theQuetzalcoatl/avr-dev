#include "Aztec/kernel/kernel.h"

void thread_1(void)
{
    for(;;){
        if(button_get_state(BUTTON_2) == PIN_LOW){
            uart_send('U');
            _delay_ms(120);
        }
    }
}

void sys_alive_led(void)
{
    for(;;){
        toggle_led(LED1);
        _delay_ms(100);
    }
}

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
            _delay_ms(50);
        }
        lcd_send_command(LCD_CLEAR);
        if(freq == 0)buzzer_off();
        else buzzer_buzz(freq);
        freq = 0;
    }
}

void thread_4(void)
{
    for(int i = 10; i; --i){
        toggle_led(LED2);
        _delay_ms(201);
    }
    kernel_exit();
}

int main(void)
{
    uint8_t thread_1_stack[MIN_STACK_SIZE+4];
    kernel_register_thread(thread_1, thread_1_stack, MIN_STACK_SIZE+4);
    
    uint8_t thread_2_stack[MIN_STACK_SIZE+4];
    kernel_register_thread(sys_alive_led, thread_2_stack, MIN_STACK_SIZE+4);
    
    uint8_t thread_3_stack[MIN_STACK_SIZE+30];
    kernel_register_thread(thread_3, thread_3_stack, MIN_STACK_SIZE+30);

    uint8_t thread_4_stack[MIN_STACK_SIZE + 2];
    kernel_register_thread(thread_4, thread_4_stack, MIN_STACK_SIZE+2);

    kernel_init_os();

    while(1){;} // just in case
 
    return 0;
}