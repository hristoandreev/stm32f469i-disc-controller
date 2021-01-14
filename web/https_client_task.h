//
// Created by hristo on 1/5/21.
//

#ifndef STM32F469_DISC_ESP32_HRISTO_H
#define STM32F469_DISC_ESP32_HRISTO_H

#include <cmsis_os2.h>

#if __cplusplus
extern "C" {
#endif

typedef enum {
    GET,
    POST,
    PUT,
    DELETE,
}req_type;

struct web_pkg {
    req_type type;
    const char *uri;
    char *in_buff;
    size_t in_buff_size;
    char *out_buff;
    size_t out_buff_size;
    osMessageQueueId_t msgQHandler;
};

void HTTPSClient_Task(void *argument);

#if __cplusplus
}
#endif

#endif //STM32F469_DISC_ESP32_HRISTO_H
