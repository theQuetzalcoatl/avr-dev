#ifndef LCD_H
#define LCD_H

#include "../../kernel/kernel.h"

    /* COMMANDS */
#define LCD_CLEAR (uint8_t)(0b00000001u)

extern void lcd_init_device(void);
extern void lcd_send_command(uint8_t command);
extern void lcd_write(uint8_t data);

#endif  /* LCD_H */