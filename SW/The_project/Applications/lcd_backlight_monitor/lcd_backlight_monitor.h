#ifndef _LCD_BACKLIGHT_MONITOR_H
#define _LCD_BACKLIGHT_MONITOR_H

#include "kernel.h"

extern void lcd_backlight_monitor_main(void);
extern register_t lcd_backlight_monitor_stack[CONFIG_MIN_STACK_SIZE+28];

#endif /* _LCD_BACKLIGHT_MONITOR_H */