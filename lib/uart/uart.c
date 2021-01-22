//
// Created by hristo on 1/20/21.
//

#include "uart.h"
#include "logg.h"
#include "cmsis_os2.h"

static struct uart *inst[10] = {};

void uartInit(struct uart *uart_p) {
    uart_p->xQueue_in = xQueueCreate(uart_p->iq_size_bytes, sizeof(uint8_t));
    uart_p->xQueue_out = xQueueCreate(uart_p->oq_size_bytes, sizeof(uint8_t));
    /* Check the pointers is not NULL. */
    configASSERT(uart_p->xQueue_in != NULL || uart_p->xQueue_out != NULL);

    uart_p->mutex = osMutexNew(NULL);
    configASSERT(uart_p->mutex != NULL);

    xQueueReset(uart_p->xQueue_in);
    xQueueReset(uart_p->xQueue_out);

    switch((int)uart_p->huart->Instance) {
#ifdef USART1
        case (int)USART1: inst[0] = uart_p; break;
#endif
#ifdef USART2
        case (int)USART2: inst[1] = uart_p; break;
#endif
#ifdef USART3
        case (int)USART3: inst[2] = uart_p; break;
#endif
#ifdef UART4
        case (int)UART4: inst[3] = uart_p; break;
#endif
#ifdef UART5
        case (int)UART5: inst[4] = uart_p; break;
#endif
#ifdef USART6
        case (int)USART6: inst[5] = uart_p; break;
#endif
#ifdef UART7
        case (int)UART7: inst[6] = uart_p; break;
#endif
#ifdef UART8
        case (int)UART8: inst[7] = uart_p; break;
#endif
        default: configASSERT(0);
    }
}

void uartStartReceiving(struct uart *uart_p) {
    configASSERT(uart_p);
    configASSERT(uart_p->huart);
    configASSERT(uart_p->xQueue_in);

    xQueueReset(uart_p->xQueue_in);
    (void)HAL_UART_Receive_IT(uart_p->huart, &uart_p->in_data, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    struct uart *uart_p;
    BaseType_t xHigherPriorityTaskWokenByPost;

    switch((int)huart->Instance) {
#ifdef USART1
        case (int)USART1: uart_p = inst[0]; break;
#endif
#ifdef USART2
        case (int)USART2: uart_p = inst[1]; break;
#endif
#ifdef USART3
        case (int)USART3: uart_p = inst[2]; break;
#endif
#ifdef UART4
        case (int)UART4: uart_p = inst[3]; break;
#endif
#ifdef UART5
        case (int)UART5: uart_p = inst[4]; break;
#endif
#ifdef USART6
        case (int)USART6: uart_p = inst[5]; break;
#endif
#ifdef UART7
        case (int)UART7: uart_p = inst[6]; break;
#endif
#ifdef UART8
        case (int)UART8: uart_p = inst[7]; break;
#endif
        default: configASSERT(0);
    }

    // We have not woken a task at the start of the ISR.
    xHigherPriorityTaskWokenByPost = pdFALSE;
    xQueueSendFromISR(uart_p->xQueue_in, &uart_p->in_data, &xHigherPriorityTaskWokenByPost);
    (void)HAL_UART_Receive_IT(huart, &uart_p->in_data, 1);
}

int uartRead(struct uart *uart_p, void *data_p, size_t size, uint32_t timeout) {
    size_t bytes = 0;
    uint8_t *p = (uint8_t *) data_p;
    uint32_t tickstart;

    configASSERT(uart_p);
    configASSERT(data_p);
    configASSERT(uart_p->xQueue_in);

    /* Init tickstart for timeout management */
    tickstart = HAL_GetTick();

    while(1) {
        if ((timeout == 0U) || ((HAL_GetTick() - tickstart) > timeout)) {
            return bytes;
        }

        BaseType_t res = xQueueReceive(uart_p->xQueue_in, p, 10 / portTICK_RATE_MS);
        if(res != 0) {
            bytes++;
            p++;
            tickstart = HAL_GetTick();
            if(bytes >= size)
                return bytes;
        }
    }
}

int uartWrite(struct uart *uart_p, void *data_p, size_t size, uint32_t timeout) {
    BaseType_t res;
    int i = 0;
    uint8_t *p = (uint8_t *) data_p;

    configASSERT(uart_p);
    configASSERT(data_p);
    configASSERT(uart_p->xQueue_out);
    configASSERT(uart_p->huart);

    if(osMutexAcquire(uart_p->mutex, 100) == osOK) {
        /* Check that a Tx process is not already ongoing */
        if (uart_p->huart->gState == HAL_UART_STATE_READY) {
            uart_p->out_data = p[0];
            i = 1;
        }

        for (; i < size; i++) {
            res = xQueueSend(uart_p->xQueue_out, &p[i], 10 / portTICK_RATE_MS);
            if (res != pdPASS) goto end;
        }

        end:
        (void)osMutexRelease(uart_p->mutex);
        HAL_UART_Transmit_IT(uart_p->huart, &uart_p->out_data, 1);

        return i;
    }

    return 0;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    struct uart *uart_p;
    BaseType_t xHigherPriorityTaskWokenByPost;
    BaseType_t res;

    switch((int)huart->Instance) {
#ifdef USART1
        case (int)USART1: uart_p = inst[0]; break;
#endif
#ifdef USART2
        case (int)USART2: uart_p = inst[1]; break;
#endif
#ifdef USART3
        case (int)USART3: uart_p = inst[2]; break;
#endif
#ifdef UART4
        case (int)UART4: uart_p = inst[3]; break;
#endif
#ifdef UART5
        case (int)UART5: uart_p = inst[4]; break;
#endif
#ifdef USART6
        case (int)USART6: uart_p = inst[5]; break;
#endif
#ifdef UART7
        case (int)UART7: uart_p = inst[6]; break;
#endif
#ifdef UART8
        case (int)UART8: uart_p = inst[7]; break;
#endif
        default: configASSERT(0);
    }

    // We have not woken a task at the start of the ISR.
    xHigherPriorityTaskWokenByPost = pdFALSE;
    res = xQueueReceiveFromISR(uart_p->xQueue_out, &uart_p->out_data, &xHigherPriorityTaskWokenByPost);
    if(res != pdPASS) return;

    (void)HAL_UART_Transmit_IT(huart, &uart_p->out_data, 1);
}