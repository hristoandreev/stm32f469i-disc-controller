//
// Created by hristo on 1/7/21.
//

#ifndef STM32F469_DISC_ESP32_WEB_H
#define STM32F469_DISC_ESP32_WEB_H

#include <cmsis_os2.h>
#include "HTTPClient.h"

enum webState {
    web_OK,
    web_ERROR,
    web_BUSY,
    web_READY,
};

struct req_pkt;

class web {
public:
    enum req_layer {
        INTERNAL,
        EXTERNAL,
        L_NUM
    };

    static webState get(const char *uri_p, const char *in_buff, size_t in_buff_size, void *callback);

    static void web_service(void *arg);
    static const size_t RESPONSE_BUFF_SIZE = 8192;
    static struct req_pkt request_pkt[L_NUM];

private:
    static const size_t URI_BUFF_SIZE = 128;
    static const size_t REQ_QUEUE_SIZE = 10;

    enum req_type{
        GET,
        POST,
        PUT,
        DELETE,
    };

    struct web_pkg {
        req_type type;
        char uri[web::URI_BUFF_SIZE];
//        char *in_buff;
//        size_t in_buff_size;
        char *out_buff;
        size_t out_buff_size;
        void *reqCallback;
    };
};

struct req_pkt {
    osMessageQueueId_t web_queue_requestHandle;
    char response[web::RESPONSE_BUFF_SIZE];
};

#endif //STM32F469_DISC_ESP32_WEB_H
