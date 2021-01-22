//
// Created by ivan on 16.5.2019 г..
//
#include "dbg.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "uart.h"

extern UART_HandleTypeDef huart3;

struct uart dbg_uart = {
        .huart = &huart3,
        .iq_size_bytes = 1,
        .oq_size_bytes = 1024
};

int dbg_print_va(const char *fmt, va_list ap) {
    char *obuf = NULL;
    int len = vsnprintf(obuf, 0, fmt, ap);
    if (len <= 0) {
        return len;
    }
    obuf = malloc(len + 1);
    len = vsnprintf(obuf, len+1, fmt, ap);
    int res = uartWrite(&dbg_uart, obuf, len, 200);
    free(obuf);
    return res;
}

int dbgPrint(const char *fmt, ...) {
    va_list args;
    int r = -1;

    va_start(args, fmt);
    r = dbg_print_va(fmt, args);
    va_end(args);

    return r;
}

int dbgInit(void) {
    uartInit(&dbg_uart);
    return 1;
}

int dbgDeInit(void) {
    return 0;
}

__attribute__((noreturn))
void dbg_abort(void) {
    taskDISABLE_INTERRUPTS();
    while (1);
}
