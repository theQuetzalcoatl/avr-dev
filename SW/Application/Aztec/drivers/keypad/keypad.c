#include "keypad.h"

#define COL_2 PIND0 
#define ROW_1 PIND1
#define COL_1 PIND2
#define ROW_4 PIND3
#define COL_3 PIND4
#define ROW_3 PIND5
#define ROW_2 PIND6

#define SWITCH_TO(column) PORTD |= 1<<COL_2 | 1<<COL_1 | 1<<COL_3; PORTD &= ~(1<<column)
#define IS_ROW_ACTIVE(row) !(PIND & 1<<row)

/* Device initialization */

static void init_rows_gpio(void);
static void init_columns_gpio(void);
void keypad_init_device(void)
{
    init_columns_gpio();
    init_rows_gpio();
}

static void init_rows_gpio(void)
{
    DDRD &= ~(1<<ROW_1 | 1<<ROW_4 | 1<<ROW_3 | 1<<ROW_2);
	PORTD |= 1<<ROW_1 | 1<<ROW_4 | 1<<ROW_3 | 1<<ROW_2; // activating pullups
}

static void init_columns_gpio(void)
{
    DDRD |= 1<<COL_2 | 1<<COL_1 | 1<<COL_3;
	PORTD |= 1<<COL_2 | 1<<COL_1 | 1<<COL_3;
}

/* Device functionalities */

