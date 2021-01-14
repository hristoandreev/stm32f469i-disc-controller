#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#ifndef SIMULATOR
#include "cmsis_os2.h"
#include "web.h"
#include "https_client_task.h"
extern osMessageQueueId_t web_queue_requestHandle;
//extern osMessageQueueId_t web_queue_responseHandle;
#endif

Model::Model() : modelListener(nullptr),
                 is_http_ready(true),
                 is_first(true),
                 tickCount(0),
                 scanPeriod(0),
                 is_scanning(true),
                 progress(0) {
    in_data = new char[in_data_size];
#ifndef SIMULATOR
    https_APsScan_response_Handle = osMessageQueueNew (1, sizeof(uint16_t), nullptr);
#endif
}

/* Tick ~ 16ms*/
void Model::tick() {
#ifndef SIMULATOR
//#if 0
    webState state;
    osStatus_t status;
    struct web_pkg web{};

    /* Timeout ~ 4s */
    scanPeriod++;
    if(scanPeriod == 250) {
        is_scanning = true;
    }

    /* Timeout ~ 16s */
    tickCount++;
    if(tickCount >= 1000) {
        tickCount = 0;
        is_http_ready = true;
        (void)osMessageQueueReset(web_queue_requestHandle);
        (void)osMessageQueueReset(https_APsScan_response_Handle);
    }

    if(is_scanning) {
        modelListener->updateProgress(progress++);

        if (is_http_ready) {
//            if (is_first) {
//                status = osMessageQueueGet(web_queue_responseHandle, &state, nullptr, 1);
//                if (status != osOK) return;
//                if (state != httpc_READY && state != httpc_OK) return;
//                is_first = false;
//            }

            web.uri = "http://192.168.3.1/wifi?cmd=scan";
            web.type = GET;
            web.in_buff = in_data;
            web.in_buff_size = in_data_size;
            web.out_buff = nullptr;
            web.out_buff_size = 0;
            web.msgQHandler =  https_APsScan_response_Handle;
            status = osMessageQueuePut(web_queue_requestHandle, &web, 0U, 1U);
            if (status == osOK) {
                is_http_ready = false;
            } else return;
        }

        if (osMessageQueueGetCount(https_APsScan_response_Handle)) {
            status = osMessageQueueGet(https_APsScan_response_Handle, &state, nullptr, 1U);
            if (status == osOK) {
                is_http_ready = true;
                tickCount = 0;
                if (state == web_OK) {
                    is_scanning = false;
                    scanPeriod = 0;
                    progress = 0;
                    modelListener->updateProgress(progress);
                    modelListener->hideProgress();
                    modelListener->wifiScanningDone();
                }
            }
        }
    }
#endif
}
