#include "../kernel/kernel.h"

static void signal_morse_sos_forever();
void kernel_panic(void)
{
    disable_systick();
    signal_morse_sos_forever();
}

static void signal_morse_sos_forever()
{
    DDRC |= 1<<LED4;
    while(1){
        for(uint8_t i = 3; i; --i){
            PORTC |= 1<<LED4;
            _delay_ms(100);
            PORTC &= ~(1<<LED4);
            _delay_ms(100);
        }
        for(uint8_t i = 3; i; --i){
            PORTC |= 1<<LED4;
            _delay_ms(300);
            PORTC &= ~(1<<LED4);
            _delay_ms(100);
        }
        for(uint8_t i = 3; i; --i){
            PORTC |= 1<<LED4;
            _delay_ms(100);
            PORTC &= ~(1<<LED4);
            _delay_ms(100);
        }
        _delay_ms(500);
    }
}