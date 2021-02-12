#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#ifndef SIMULATOR
#include "logg.h"
#endif

Model::Model() : modelListener(nullptr),
                 is_http_ready(true),
                 tickCount(0),
                 scanPeriod(MS_TO_TICK(6000) - 1),
                 is_scanning(true),
                 progress(0)
#ifndef SIMULATOR
                 ,webResCompleteCallback(this, &Model::webResCompleteCallbackHandler)
#endif
{

}

/* Tick ~ 16ms*/
void Model::tick() {
#ifndef SIMULATOR
    /* Period ~ 6s */
    scanPeriod++;
    if(scanPeriod == MS_TO_TICK(6000)) {
        is_scanning = true;
        if (is_http_ready) {
            webState state = web::get("http://192.168.3.1/wifi?cmd=scan", nullptr, 0, &webResCompleteCallback);
            if(state != web_OK) {
//                tickCount = 0;
                scanPeriod = 0;
            } else {
                LOG_I(LOG_DBG_ON, "Model::tick", "Scanning available access points...");
                is_http_ready = false;
            }
        }
    }

    /* Timeout ~ 16s */
    tickCount++;
    if(tickCount >= MS_TO_TICK(16000)) {
        tickCount = 0;
        scanPeriod = 0;
        is_scanning = false;
        is_http_ready = true;
        modelListener->clearAPsList();
        LOG_W(LOG_DBG_ON, "Model::tick", "Timeout expired!");
    }

    if(is_scanning) {
        modelListener->updateProgress(progress++);
    } else {
        if(progress) {
            progress = 0;
            modelListener->hideProgress();
        }
    }
#endif
}

#ifndef SIMULATOR
void Model::webResCompleteCallbackHandler(HTTPResult status, const char *res) {
    is_http_ready = true;
    tickCount = 0;
    scanPeriod = 0;
    if (HTTP_OK == status) {
        is_scanning = false;
        LOG_I(LOG_DBG_ON, "Model::webResCompleteCallbackHandler", "Scanning done.");
        modelListener->wifiScanningDone(res);
    }
}
#endif