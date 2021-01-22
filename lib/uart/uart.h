//
// Created by hristo on 1/20/21.
//

#ifndef STM32F469I_DISC_ESP32_UART_H
#define STM32F469I_DISC_ESP32_UART_H

#include <stm32f4xx_hal.h>
#include "FreeRTOS.h"
#include <queue.h>
#include <cmsis_os2.h>

#if __cplusplus
extern "C" {
#endif

struct uart {
    /* Public members. */
    UART_HandleTypeDef *huart;
    size_t iq_size_bytes;
    size_t oq_size_bytes;

    /* Private members. */
    QueueHandle_t xQueue_in;
    QueueHandle_t xQueue_out;
    osMutexId_t mutex;
    uint8_t in_data;
    uint8_t out_data;
};

void uartInit(struct uart *uart_p);
void uartStartReceiving(struct uart *uart_p);
int uartRead(struct uart *uart_p, void *data_p, size_t size, uint32_t timeout);
int uartWrite(struct uart *uart_p, void *data_p, size_t size, uint32_t timeout);

#if __cplusplus
}
#endif

#endif //STM32F469I_DISC_ESP32_UART_H
