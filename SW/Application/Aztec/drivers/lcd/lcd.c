#include "lcd.h"

#define LCD_D7		PINA7
#define LCD_D6		PINA6
#define LCD_D5		PINA5
#define LCD_D4		PINA4

#define RS         PINA1
#define EN         PINA2
#define BACK_LIGHT PINA0
#define LCD_PORT   PORTA

#define COMMAND_MODE()	LCD_PORT &= ~(1<<RS)
#define DATA_MODE()	    LCD_PORT |= (1<<RS)

static void flash(void);

static void init_gpio(void);
static void execute_lcd_init_sequence(void);
void lcd_init_device(void)
{
    init_gpio();
    execute_lcd_init_sequence();
}

static void init_gpio()
{
    DDRA = 0xff;
}

static void execute_lcd_init_sequence(void)
{
    COMMAND_MODE();
	LCD_PORT |= (1<<LCD_D5) | (1<<LCD_D4);
	LCD_PORT &= ~((1<<LCD_D6) | (1<<LCD_D7));	// 0b0011xxxx
	
	// 1)
	_delay_ms(20);
	flash();
	
	// 2)
	_delay_ms(5);
	flash();
	// 3)
	_delay_ms(1);
	flash();
	// 4)
	LCD_PORT &= ~(1<<LCD_D4);	// 0b0010xxxx 
	_delay_ms(5);
	flash();
	// 5)
	_delay_ms(5);

	lcd_send_command(0b00101000);	// N=1(2 rows), F=0 (5*8 matrix)
	lcd_send_command(0b00001000);	// display off, cursor&blinking off
	lcd_send_command(LCD_CLEAR);	// clear screen, return cursor home
	lcd_send_command(0b00000110);	// inc cursor right when writing, don't shift screen

	lcd_send_command(0b00001100);	// display back on
}

static void flash(void)
{
	LCD_PORT |= (1<<EN);
	asm volatile("nop \n \
                nop \n \
                nop \n");
	LCD_PORT &= ~(1<<EN);
}

void lcd_send_command(uint8_t command)
{
	COMMAND_MODE();
	LCD_PORT &= 0x0f;
	LCD_PORT |= (command & 0xf0);
	flash();
	
	LCD_PORT &= 0x0f;
	LCD_PORT |= ((command<<4) & 0xf0);
	flash();
	
	_delay_ms(1);
}

void lcd_write(uint8_t data)
	{
		DATA_MODE();
		LCD_PORT &= 0x0f;
		LCD_PORT |= (data & 0xf0);
		flash();
		
		LCD_PORT &= 0x0f;
		LCD_PORT |= ((data<<4) & 0xf0);
		flash();
		
		_delay_us(50);
	}