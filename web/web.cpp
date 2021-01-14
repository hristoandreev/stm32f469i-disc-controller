//
// Created by hristo on 1/5/21.
//

#include <FreeRTOS.h>
#include "https_client_task.h"
#include "web.h"

HTTPClient web::web_client;
osMessageQueueId_t web::web_queue_requestHandle;

void HTTPSClient_Task(void *argument) {
    web::web_service(argument);
}

__attribute__((noreturn))
void web::web_service(void *arg) {
    web_queue_requestHandle = reinterpret_cast<osMessageQueueId_t> (arg);

    osStatus_t res;
    HTTPResult http_res;
    webState state;
    struct web_pkg web_pkg {};

    while(true) {
        res = osMessageQueueGet(web_queue_requestHandle, &web_pkg, nullptr, osWaitForever);
        if(res != osOK) continue;

        HTTPText in_text(web_pkg.in_buff, web_pkg.in_buff_size);

        switch (web_pkg.type) {
            case GET:
                http_res = web_client.get(web_pkg.uri, &in_text);
                break;
            case POST: {
                HTTPText out_text(web_pkg.out_buff, web_pkg.out_buff_size);
                http_res = web_client.post(web_pkg.uri, out_text, &in_text);
                break;
            }
            case PUT: {
                HTTPText out_text(web_pkg.out_buff, web_pkg.out_buff_size);
                http_res = web_client.put(web_pkg.uri, out_text, &in_text);
                break;
            }
            case DELETE:
                http_res = web_client.del(web_pkg.uri, &in_text);
                break;
            default:
                http_res = HTTP_ERROR;
                break;
        }

        if(http_res != HTTP_OK) state = web_ERROR;
        else state = web_OK;

        (void)osMessageQueuePut(web_pkg.msgQHandler, &state, 24, 1000);
    }
}

webState web::web_get(const char *uri, const char *in_buff, size_t in_buff_size, int timeout) {
    struct web_pkg web_req {};
    StaticQueue_t stQueue;
    uint16_t mem;
    osStatus_t status;
    webState state;
    osMessageQueueAttr_t web_queue_response_attributes = {
            .name = "web_queue_response",
            .cb_mem = &stQueue,
            .cb_size = sizeof(stQueue),
            .mq_mem = &mem,
            .mq_size = sizeof(mem)
    };

    web_req.uri = uri;
    web_req.type = GET;
    web_req.in_buff = const_cast<char *>(in_buff);
    web_req.in_buff_size = in_buff_size;
    web_req.out_buff = nullptr;
    web_req.out_buff_size = 0;
    web_req.msgQHandler =  osMessageQueueNew (1, sizeof(mem), &web_queue_response_attributes);

    status = osMessageQueuePut(web_queue_requestHandle, &web_req, 0U, 100);
    if(status != osOK) return web_ERROR;

    status = osMessageQueueGet(web_req.msgQHandler, &state, nullptr, timeout);
    if(status != osOK) return web_ERROR;

    return state;
}
