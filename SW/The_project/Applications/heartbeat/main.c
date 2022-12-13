#include "heartbeat.h"

register_t heartbeat_stack[CONFIG_MIN_STACK_SIZE+10];
void main_heartbeat(void)
{
    lease(DEV_LED1);
    for(;;){
        led_1_toggle();
        wait_us(500000);
    }
    release(DEV_LED1);
}