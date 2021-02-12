//
// Created by hristo on 1/5/21.
//

#include <TouchGFXGeneratedHAL.hpp>
#include <logg.h>
#include "https_client_task.h"
#include "web.h"

struct req_pkt web::request_pkt[web::L_NUM];

void HTTPSClientExternal_Task(void *argument) {
    (void) argument;
//    LOG_D(LOG_DBG_ON, "HTTPSClientExternal_Task", "Task started.");
    web::web_service(&web::request_pkt[web::EXTERNAL]);
}

void HTTPSClientInternal_Task(void *argument) {
    (void) argument;
//    LOG_D(LOG_DBG_ON, "HTTPSClientInternal_Task", "Task started.");
    web::web_service(&web::request_pkt[web::INTERNAL]);
}

__attribute__((noreturn))
void web::web_service(void *arg) {
    auto *ctx = reinterpret_cast<struct req_pkt *>(arg);
    osStatus_t res;
    HTTPResult http_res;
    HTTPClient http_client;
    struct web_pkg web_pkg {};
    touchgfx::GenericCallback<HTTPResult, const char*> *p;

    ctx->web_queue_requestHandle = osMessageQueueNew (web::REQ_QUEUE_SIZE, sizeof(struct web_pkg), nullptr);
    DBG_ASSERT(ctx->web_queue_requestHandle != nullptr, "Out of memory!!!");

    while(true) {
        res = osMessageQueueGet(ctx->web_queue_requestHandle, &web_pkg, nullptr, osWaitForever);
        if(res != osOK) continue;

        p = reinterpret_cast<touchgfx::GenericCallback<HTTPResult, const char *> *>(web_pkg.reqCallback);

        HTTPText in_text(ctx->response, RESPONSE_BUFF_SIZE);

        switch (web_pkg.type) {
            case GET:
                http_res = http_client.get(web_pkg.uri, &in_text);
                break;
            case POST: {
                HTTPText out_text(web_pkg.out_buff, web_pkg.out_buff_size);
                http_res = http_client.post(web_pkg.uri, out_text, &in_text);
                break;
            }
            case PUT: {
                HTTPText out_text(web_pkg.out_buff, web_pkg.out_buff_size);
                http_res = http_client.put(web_pkg.uri, out_text, &in_text);
                break;
            }
            case DELETE:
                http_res = http_client.del(web_pkg.uri, &in_text);
                break;
            default:
                http_res = HTTP_ERROR;
                break;
        }

        if(p != nullptr)
            p->execute(http_res, ctx->response);
    }
}

/**
 * @brief   GET request function.
 *
 * @param[in] uri_p     pointer to the URI
 * @param[in] callback  pointer to the response function
 *
 * @return              The operation status.
 * @retval web_OK       operation succeeded.
 * @retval web_ERROR    operation failed.
 *
 */
webState web::get(const char *uri_p, const char *in_buff, size_t in_buff_size, void *callback) {
    struct web_pkg web_req {};
    osStatus_t status;
    osMessageQueueId_t q;

    if(nullptr == uri_p) return web_ERROR;
    DBG_ASSERT(strlen(uri_p) < web::URI_BUFF_SIZE, "URI is too long!!!");

    char *p = strstr(uri_p, "192.168.3.1");
    if(p != nullptr) q = request_pkt[INTERNAL].web_queue_requestHandle;
    else q = request_pkt[EXTERNAL].web_queue_requestHandle;

    DBG_ASSERT(q != nullptr, "Queue is not created!!!");

    (void)strcpy(web_req.uri, uri_p);
    web_req.type = GET;
//    web_req.in_buff = const_cast<char *>(response);
//    web_req.in_buff = const_cast<char *>(in_buff);
//    web_req.in_buff_size = RESPONSE_BUFF_SIZE;
//    web_req.in_buff_size = in_buff_size;
    web_req.out_buff = nullptr; // TODO: Problem.
    web_req.out_buff_size = 0;
    web_req.reqCallback = callback;

    status = osMessageQueuePut(q, &web_req, 0U, 1);
    if(status != osOK) return web_ERROR;

    return web_OK;
}
