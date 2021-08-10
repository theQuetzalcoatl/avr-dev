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
    lcd_turn_backligh_on();
    while(1){
        key = get_pressed_key();
        switch(key)
        {
            case KEYPAD_0:
                lcd_write('0');
            break;

            case KEYPAD_1:
                lcd_write('1');
            break;

            case KEYPAD_2:
                lcd_write('2');
            break;

            case KEYPAD_3:
                lcd_write('3');
            break;

            case KEYPAD_4:
                lcd_write('4');
            break;

            case KEYPAD_5:
                lcd_write('5');
            break;

            case KEYPAD_6:
                lcd_write('6');
            break;

            case KEYPAD_7:
                lcd_write('7');
            break;

            case KEYPAD_8:
                lcd_write('8');
            break;

            case KEYPAD_9:
                lcd_write('9');
            break;

            case KEYPAD_ASTERISK:
                lcd_write('*');
            break;

            case KEYPAD_POUND:
                lcd_send_command(LCD_CLEAR);
            break;
        }
        _delay_ms(50);
    }
}

int main(void)
{
    uint8_t thread_1_stack[MIN_STACK_SIZE+4];
    kernel_register_thread(thread_1, thread_1_stack, MIN_STACK_SIZE+4);
    
    uint8_t thread_2_stack[MIN_STACK_SIZE+4];
    kernel_register_thread(sys_alive_led, thread_2_stack, MIN_STACK_SIZE+4);
    
    uint8_t thread_3_stack[MIN_STACK_SIZE+30];
    kernel_register_thread(thread_3, thread_3_stack, MIN_STACK_SIZE+30);

    kernel_init_os();

    return 0;
}