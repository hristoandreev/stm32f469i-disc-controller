//
// Created by hristo on 1/7/21.
//

#ifndef STM32F469_DISC_ESP32_WEB_H
#define STM32F469_DISC_ESP32_WEB_H

#include <cmsis_os2.h>

typedef enum {
    web_OK,
    web_ERROR,
    web_BUSY,
    web_READY,
}webState;

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
    void *reqCallback;
};

class web {
public:

    webState get(const char *uri, const char *in_buff, size_t in_buff_size, void *callback);

    static void web_service(void *arg);

    static const size_t RESPONSE_BUFF_SIZE = 4096;
private:
    static osMessageQueueId_t web_queue_requestHandle;
//    static HTTPClient web_client;
    static char response[RESPONSE_BUFF_SIZE];
};

#endif //STM32F469_DISC_ESP32_WEB_H
