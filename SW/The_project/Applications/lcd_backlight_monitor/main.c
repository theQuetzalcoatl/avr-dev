#include "lcd_backlight_monitor.h"

#define BACKLIGHT_ON_DURATION_SEC (10u)

register_t lcd_backlight_monitor_stack[CONFIG_MIN_STACK_SIZE+28];
void lcd_backlight_monitor_main(void)
{
    wait_ms(100);
    while(lease(DEV_LCD_LIGHT) != NO_ERROR){;}
    uint32_t t1 = 0;
    uint32_t t2 = 0;

    for(;;){
        t2 = get_uptime();

        if(keypad_get_pressed_key_nonblocking() != KEYPAD_NO_NUM){
            t1 = t2; // resetting the timer 
            lcd_turn_backligh_on();
        }
        
        if(t2 - t1 > BACKLIGHT_ON_DURATION_SEC){
            t1 = t2;
            lcd_turn_backligh_off();
        }
    }
    release(DEV_LCD_LIGHT);
}