#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

#ifndef SIMULATOR
#include "web.h"
#endif

Model::Model() : modelListener(nullptr),
                 is_http_ready(true),
                 tickCount(0),
                 scanPeriod(0),
                 is_scanning(true),
                 progress(0),
                 webResCompleteCallback(this, &Model::webResCompleteCallbackHandler)
{
//    in_data = new char[in_data_size];
}

/* Tick ~ 16ms*/
void Model::tick() {
#ifndef SIMULATOR
    /* Period ~ 4s */
    scanPeriod++;
    if(scanPeriod == 250) {
        is_scanning = true;
    }

    /* Timeout ~ 16s */
    tickCount++;
    if(tickCount >= 1000) {
        tickCount = 0;
        scanPeriod = 0;
        is_scanning = false;
        is_http_ready = true;
    }

    if(is_scanning) {
        modelListener->updateProgress(progress++);
        if (is_http_ready) {
            web web;
            (void)web.get("http://192.168.3.1/wifi?cmd=scan", nullptr, 0, &webResCompleteCallback);
            is_http_ready = false;
        }
    } else {
        if(progress) {
            progress = 0;
            modelListener->hideProgress();
        }
    }
#endif
}

void Model::webResCompleteCallbackHandler(const char *res) {
    is_http_ready = true;
    tickCount = 0;
    if (res != nullptr) {
        scanPeriod = 0;
        is_scanning = false;
        modelListener->wifiScanningDone(const_cast<char *>(res));
    }
}
