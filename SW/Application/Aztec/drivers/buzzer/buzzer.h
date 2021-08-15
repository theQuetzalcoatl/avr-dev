#ifndef BUZZER_H
#define BUZZER_H

#include "../../kernel/kernel.h"

#define BUZZER_ERR_INVALID_REGISTER_VAL (1u)
#define BUZZER_ERR_OUT_OF_BOUNDS_FREQ_REQUEST (2u)

extern void buzzer_init_device(void);
extern uint8_t buzzer_buzz(uint16_t frequency_Hz);
extern void buzzer_off(void);

#endif /* BUZZER_H */