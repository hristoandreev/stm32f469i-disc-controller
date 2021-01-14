//
// Created by hristo on 1/7/21.
//

#ifndef STM32F469_DISC_ESP32_WEB_H
#define STM32F469_DISC_ESP32_WEB_H

#include <cmsis_os2.h>
#include "HTTPClient.h"

typedef enum {
    web_OK,
    web_ERROR,
    web_BUSY,
    web_READY,
}webState;

class web {
public:

    webState web_get(const char *uri, const char *in_buff, size_t in_buff_size, int timeout);

    static void web_service(void *arg);
private:
    static osMessageQueueId_t web_queue_requestHandle;
    static HTTPClient web_client;
};

#endif //STM32F469_DISC_ESP32_WEB_H
