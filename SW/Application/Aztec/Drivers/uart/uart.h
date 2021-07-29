#ifndef UART_H
#define UART_H

#include "../../kernel/kernel.h"


typedef struct Uart
{
    uint8_t mode;
    uint8_t num_of_bits;
    uint8_t parity;
    uint8_t num_of_stop_bits;
    uint32_t baud_rate;
}Uart;


/* MODES */
#define NORMAL_MODE         (0u)
#define DOUBLE_SPEED_MODE   (1u)

#define BIT_5               (2u)
#define BIT_6               (3u)
#define BIT_7               (4u)
#define BIT_8               (5u)
#define BIT_9               (6u)

#define NO_PARITY           (7u)
#define EVEN_PARITY         (8u)
#define ODD_PARITY          (9u)

#define ONE_STOP_BIT        (10u)
#define TWO_STOP_BITS       (11u)


/* ERROR CODES */
#define ERR_UART_INVALID_MODE           (1u | 0x80)
#define ERR_UART_OUT_OF_LIMIT_BAUD_RATE (2u | 0x80)
#define ERR_UART_INVALID_PARITY         (3U | 0x80)
#define ERR_UART_INVALID_STOP_BITS      (4u | 0x80)

extern uint8_t uart_init_device(const Uart * const uart);
extern void uart_send(char c);

#endif /* UART_H */