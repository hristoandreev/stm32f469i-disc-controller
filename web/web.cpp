//
// Created by hristo on 1/5/21.
//

#include <TouchGFXGeneratedHAL.hpp>
#include <logg.h>
#include "https_client_task.h"
#include "HTTPClient.h"
#include "web.h"

typedef enum {
    GET,
    POST,
    PUT,
    DELETE,
}req_type;

struct web_pkg {
    req_type type;
    char uri[web::URI_BUFF_SIZE];
    char *in_buff;
    size_t in_buff_size;
    char *out_buff;
    size_t out_buff_size;
    void *reqCallback;
};

osMessageQueueId_t web::web_queue_requestHandle;
char web::response[web::RESPONSE_BUFF_SIZE];
//char web::uri[web::URI_BUFF_SIZE];

void HTTPSClient_Task(void *argument) {
    LOG_D(LOG_DBG_ON, "HTTPSClient_Task", "Task started.");
    web::web_service(argument);
}

__attribute__((noreturn))
void web::web_service(void *arg) {
    osStatus_t res;
    HTTPResult http_res;
    HTTPClient web_client;
    struct web_pkg web_pkg {};
    touchgfx::GenericCallback<const char*> *p;

    web_queue_requestHandle = osMessageQueueNew (web::REQ_QUEUE_SIZE, sizeof(struct web_pkg), nullptr);
    DBG_ASSERT(web_queue_requestHandle != nullptr, "Out of memory!!!");

    while(true) {
        res = osMessageQueueGet(web_queue_requestHandle, &web_pkg, nullptr, osWaitForever);
        if(res != osOK) continue;

        p = reinterpret_cast<touchgfx::GenericCallback<const char *> *>(web_pkg.reqCallback);

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

        if(p == nullptr) continue;

        if(http_res != HTTP_OK) p->execute(nullptr);
        else p->execute(web_pkg.in_buff);
    }
}

webState web::get(const char *uri_p, const char *in_buff, size_t in_buff_size, void *callback) {
    struct web_pkg web_req {};
    osStatus_t status;

    if(nullptr == uri_p) return web_ERROR;
    DBG_ASSERT(strlen(uri_p) < web::URI_BUFF_SIZE, "URI too long!!!");

    (void)strcpy(web_req.uri, uri_p);
    web_req.type = GET;
    web_req.in_buff = const_cast<char *>(response);
//    web_req.in_buff = const_cast<char *>(in_buff);
    web_req.in_buff_size = RESPONSE_BUFF_SIZE;
//    web_req.in_buff_size = in_buff_size;
    web_req.out_buff = nullptr; // TODO: Problem.
    web_req.out_buff_size = 0;
    web_req.reqCallback = callback;

    status = osMessageQueuePut(web_queue_requestHandle, &web_req, 0U, 1);
    if(status != osOK) return web_ERROR;

    return web_OK;
}
