#include <gui/screen1_screen/Screen1View.hpp>
#include <cstring>
#include <BitmapDatabase.hpp>

#ifndef SIMULATOR
#include "cJSON.h"
#include "web.h"
#include <logg.h>
#include "myTime.h"
#endif

Screen1View::Screen1View() :
    scrollListItemSelectedCallback(this, &Screen1View::scrollListItemSelectedHandler),
    apPassSetBtnOkCallback(this, &Screen1View::apPassSetBtnOkCallbackHandler),
    apPassSetBtnCancelCallback(this, &Screen1View::apPassSetBtnCancelCallbackHandler),
    webApConnectCompleteCallback(this, &Screen1View::webApConnectCompleteCallbackHandler),
    webGetCurrApInfoCompleteCallback(this, &Screen1View::webGetCurrApInfoCompleteCallbackHandler),
    itemCnt(0),
    scanPeriod(0),
    tickCount(0),
    update_gadget(0),
    digitalClockCount(0),
    digitalSeconds(0),
    digitalMinutes(0),
    digitalHours(0),
    scanDone(false)
{
    curr_ap_info.connected = false;
    curr_ap_info.SSID[0] = 0;
    curr_ap_info.RSSI = 0;
    curr_ap_info.channel = 0;
    curr_ap_info.secondaryChannel = 0;
    curr_ap_info.BSSID[0] = 0;
    curr_ap_info.AuthMode[0] = 0;
    curr_ap_info.IP[0] = 0;
    curr_ap_info.Mask[0] = 0;
    curr_ap_info.Gateway[0] = 0;

    wifiScrollList.setItemSelectedCallback(scrollListItemSelectedCallback);
    buttonOk.setAction(apPassSetBtnOkCallback);
    buttonCancel.setAction(apPassSetBtnCancelCallback);
    // Synchronize clock on startup.
    setDigitalClock();
}

void Screen1View::setupScreen() {
    Screen1ViewBase::setupScreen();

}

void Screen1View::tearDownScreen() {
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::updateApScanningProgress(int value) {
    wifiScanningProgress.setVisible(true);
    wifiScanningProgress.setValue(value % 100);
    wifiScanningProgress.invalidate();
}

void Screen1View::hideProgress() {
    wifiScanningProgress.setVisible(false);
    wifiScanningProgress.invalidate();
}

void Screen1View::wifiScrollListClean() {
    wifiScrollList.setNumberOfItems(0);
    wifiScrollList.invalidate();
}

void Screen1View::wifiScrollListUpdateItem(wifiItemContainer& item, int16_t itemIndex) {
    auto apStrength = ap_info[itemIndex].RSSI;
    auto authMode = ap_info[itemIndex].AuthMode;
    auto ssid = ap_info[itemIndex].SSID;
    BitmapId bmpID;

    if(apStrength > -50) bmpID = BITMAP_WIFI5_64X64_ID;
    else if(apStrength <= -50 && apStrength > -60) bmpID = BITMAP_WIFI4_64X64_ID;
    else if(apStrength <= -60 && apStrength > -70) bmpID = BITMAP_WIFI3_64X64_ID;
    else bmpID = BITMAP_WIFI2_64X64_ID;

    if(strcmp(authMode, "Unknown") == 0) item.setAccessPointStrengthUnlockIcon(bmpID);
    else item.setAccessPointStrengthLockIcon(bmpID);

    item.setAccessPointName(ssid);
    item.setAccessPointStrength(apStrength);
    item.setAccessPointAuthMode(authMode);
}

/*
 * Update access points.
 */
void Screen1View::updateAccessPoints(char *str) {
#ifndef SIMULATOR
    LOG_I(LOG_DBG_ON, "Screen1View::updateAccessPoints", "Parsing...");
    itemCnt = 0;
    scanDone = false;
    cJSON *scan_json = cJSON_Parse(str);
    if(scan_json == nullptr) return;

    const cJSON *data;
    const cJSON *APs;

    const cJSON *status = cJSON_GetObjectItemCaseSensitive(scan_json, "status");
    if(status == nullptr) goto err;
    if(!cJSON_IsString(status) || (status->valuestring == nullptr)) goto err;
    if(strcmp(status->valuestring, "success") != 0) goto err;

    data = cJSON_GetObjectItemCaseSensitive(scan_json, "data");
    if(data == nullptr) goto err;

    APs = cJSON_GetObjectItemCaseSensitive(data, "APs");
    if(APs == nullptr) goto err;
    if(!cJSON_IsArray(APs)) goto err;

    cJSON *AP;
    cJSON_ArrayForEach(AP, APs)
    {
        cJSON *SSID = cJSON_GetObjectItemCaseSensitive(AP, "SSID");
        cJSON *RSSI = cJSON_GetObjectItemCaseSensitive(AP, "RSSI");
        cJSON *AuthMode = cJSON_GetObjectItemCaseSensitive(AP, "AuthMode");
        cJSON *PairwiseCipher = cJSON_GetObjectItemCaseSensitive(AP, "PairwiseCipher");
        cJSON *GroupCipher = cJSON_GetObjectItemCaseSensitive(AP, "GroupCipher");
        cJSON *IsKnown = cJSON_GetObjectItemCaseSensitive(AP, "IsKnown");

        if (SSID == nullptr || !cJSON_IsString(SSID) ||
            RSSI == nullptr || !cJSON_IsNumber(RSSI) ||
            AuthMode == nullptr || !cJSON_IsString(AuthMode) ||
            PairwiseCipher == nullptr || !cJSON_IsString(PairwiseCipher) ||
            GroupCipher == nullptr || !cJSON_IsString(GroupCipher) ||
            IsKnown == nullptr || !cJSON_IsBool(IsKnown) ||
            itemCnt >= AP_INFO_SIZE) {
                goto err;
        }
        (void)strcpy(ap_info[itemCnt].SSID, SSID->valuestring);
        (void)strcpy(ap_info[itemCnt].AuthMode, AuthMode->valuestring);
        (void)strcpy(ap_info[itemCnt].PairwiseCipher, PairwiseCipher->valuestring);
        (void)strcpy(ap_info[itemCnt].GroupCipher, GroupCipher->valuestring);
        ap_info[itemCnt].RSSI = RSSI->valueint;
        ap_info[itemCnt].isKnow = IsKnown->valueint;
        LOG_I(LOG_DBG_ON, "AP","%s -> %d dbm, %s, %s, %s",  ap_info[itemCnt].SSID,
                                                            ap_info[itemCnt].RSSI,
                                                            ap_info[itemCnt].AuthMode,
                                                            ap_info[itemCnt].PairwiseCipher,
                                                            ap_info[itemCnt].GroupCipher);
        itemCnt++;
    }

    cJSON_Delete(scan_json);
    scanDone = true;
    LOG_I(LOG_DBG_ON, "Screen1View::updateAccessPoints", "Done.");
    LOG_I(LOG_DBG_ON, "Screen1View::updateAccessPoints", "Found %d access points.", itemCnt);
    return;

err:
    cJSON_Delete(scan_json);
    LOG_E(LOG_DBG_ON, "Screen1View::updateAccessPoints", "Pattern error! -> %s", str);
#endif
}

void Screen1View::scrollListItemSelectedHandler(int16_t itemSelected) {
    char *ssid = ap_info[itemSelected].SSID;

    if(curr_ap_info.connected)
        if(strcmp(ssid, curr_ap_info.SSID) == 0)
            return;

    if(!ap_info[itemSelected].isKnow) {
        (void) Unicode::fromUTF8(reinterpret_cast<const uint8_t *>(ssid), titleBuffer, TITLE_SIZE);
        title.resizeToCurrentText();
        connectAPModalWindow.show();
        connectAPModalWindow.invalidate();
    } else {

        char uri[300];
        setQualityTitleBarIcon(NO_CONNECTION);
        (void)sprintf(uri, "http://192.168.3.1/wifi?cmd=connect&ssid=%s", ssid);
#ifndef SIMULATOR
        web web;
        (void)web.get(uri, nullptr, 0, &webApConnectCompleteCallback);
        LOG_I(LOG_DBG_ON, "Screen1View::scrollListItemSelectedHandler", "Connecting to WI-FI...");
#endif
    }
}

void Screen1View::apPassSetBtnOkCallbackHandler(const AbstractButton &src) {
    uint8_t utf8Password[50];
    uint8_t utf8SSID[128];
    char uri[300];

    setQualityTitleBarIcon(NO_CONNECTION);
    connectAPModalWindow.hide();
    connectAPModalWindow.invalidate();

    (void)Unicode::toUTF8(passwordBuffer, utf8Password, sizeof(utf8Password));
    (void)Unicode::toUTF8(titleBuffer, utf8SSID, sizeof(utf8SSID));
    (void)sprintf(uri, "http://192.168.3.1/wifi?cmd=connect&ssid=%s&pass=%s", utf8SSID, utf8Password);
#ifndef SIMULATOR
    web web;
    (void)web.get(uri, nullptr, 0, &webApConnectCompleteCallback);
    LOG_I(LOG_DBG_ON, "Screen1View::apPassSetBtnOkCallbackHandler", "Connecting to WI-FI...");
#endif
}

void Screen1View::apPassSetBtnCancelCallbackHandler(const AbstractButton &src) {
    connectAPModalWindow.hide();
    connectAPModalWindow.invalidate();
}

void Screen1View::webApConnectCompleteCallbackHandler(const char *res) {
    if(nullptr != res) {
#ifndef SIMULATOR
        LOG_I(LOG_DBG_ON, "Screen1View::webApConnectCompleteCallbackHandler", "Done.");
#endif
        updateAPInfo(res);
    }
}

void Screen1View::handleTickEvent() {

    if(scanDone) {
        wifiScrollList.setNumberOfItems(itemCnt); // Update access points scroll list.
#ifndef SIMULATOR
        LOG_I(LOG_DBG_ON, "Screen1View::handleTickEvent", "Refresh access point lists with %d items.", itemCnt);
#endif
        scanDone = false;
    }

    /* Update every ~ 2s */
    update_gadget++;
    if(update_gadget >= MS_TO_TICK(2000)) {
        update_gadget = 0;
        wifiStrengthIcon.invalidate();
#ifndef SIMULATOR
        LOG_I(LOG_DBG_ON, "Screen1View::handleTickEvent", "Refresh WI-FI quality icon.");
#endif
    }

    /* Period ~ 2s */
    scanPeriod++;
    if(scanPeriod == MS_TO_TICK(2000)) {
        scanPeriod = 0;
        dogReset();
#ifndef SIMULATOR
        web web;
        (void)web.get("http://192.168.3.1/wifi?cmd=get_current_ap_info", nullptr, 0, &webGetCurrApInfoCompleteCallback);
        LOG_I(LOG_DBG_ON, "Screen1View::handleTickEvent", "Getting WI-FI info...");
#endif
    }

    /* Timeout ~ 3s */
    tickCount++;
    if(tickCount >= MS_TO_TICK(3000)) {
        dogReset();
        scanPeriod = 0;
#ifndef SIMULATOR
        LOG_W(LOG_DBG_ON, "Screen1View::handleTickEvent", "Timeout expired!");
#endif
        setQualityTitleBarIcon(NO_CONNECTION);
    }

    digitalClockCount++;
    if(digitalClockCount % 60 == 0) {
        if(++digitalSeconds >= 60) {
            digitalSeconds = 0;
            if(++digitalMinutes >= 60) {
                digitalMinutes = 0;
                setDigitalClock();          // Synchronize clock every hour.
                if(++digitalHours >= 24) {
                    digitalHours = 0;
                }
            }
        }
        digitalClock.setTime24Hour(digitalHours, digitalMinutes, 0);
    }
}

void Screen1View::webGetCurrApInfoCompleteCallbackHandler(const char *res) {
    scanPeriod = 0;
    if(nullptr != res) {
#ifndef SIMULATOR
        LOG_I(LOG_DBG_ON, "Screen1View::webGetCurrApInfoCompleteCallbackHandler", "Done.");
#endif
        updateAPInfo(res);
    }
}

int Screen1View::getCurrAccessPointInfo(const char *str) {
#ifndef SIMULATOR
    LOG_I(LOG_DBG_ON, "Screen1View::getCurrAccessPointInfo", "Parsing...");
    cJSON *ap_info_json = cJSON_Parse(str);
    if(ap_info_json == nullptr) return -1;

    const cJSON *data;
    const cJSON *connected;
    const cJSON *APInfo;
    const cJSON *SSID;
    const cJSON *RSSI;
    const cJSON *Channel;
    const cJSON *SecondaryChannel;
    const cJSON *BSSID;
    const cJSON *AuthMode;
    const cJSON *IPInfo;
    const cJSON *IP;
    const cJSON *Mask;
    const cJSON *Gateway;

    const cJSON *status = cJSON_GetObjectItemCaseSensitive(ap_info_json, "status");
    if(!cJSON_IsString(status) || (status->valuestring == nullptr)) goto err;
    if(strcmp(status->valuestring, "success") != 0) goto err;

    data = cJSON_GetObjectItemCaseSensitive(ap_info_json, "data");

    connected = cJSON_GetObjectItemCaseSensitive(data, "connected");
    if(!cJSON_IsBool(connected)) goto err;
    curr_ap_info.connected = (connected->valueint != 0);

    APInfo = cJSON_GetObjectItemCaseSensitive(data, "APInfo");
    if(APInfo == nullptr) goto end;

    SSID = cJSON_GetObjectItemCaseSensitive(APInfo, "SSID");
    if(!cJSON_IsString(SSID) || (SSID->valuestring == nullptr)) goto err;
    (void)strcpy(curr_ap_info.SSID, SSID->valuestring);

    RSSI = cJSON_GetObjectItemCaseSensitive(APInfo, "RSSI");
    if(!cJSON_IsNumber(RSSI)) goto err;
    curr_ap_info.RSSI = RSSI->valueint;

    Channel = cJSON_GetObjectItemCaseSensitive(APInfo, "Channel");
    if(!cJSON_IsNumber(Channel)) goto err;
    curr_ap_info.channel = Channel->valueint;

    SecondaryChannel = cJSON_GetObjectItemCaseSensitive(APInfo, "SecondaryChannel");
    if(!cJSON_IsNumber(SecondaryChannel)) goto err;
    curr_ap_info.secondaryChannel = SecondaryChannel->valueint;

    BSSID = cJSON_GetObjectItemCaseSensitive(APInfo, "BSSID");
    if(!cJSON_IsString(BSSID) || (BSSID->valuestring == nullptr)) goto err;
    (void)strcpy(curr_ap_info.BSSID, BSSID->valuestring);

    AuthMode = cJSON_GetObjectItemCaseSensitive(APInfo, "AuthMode");
    if(!cJSON_IsString(AuthMode) || (AuthMode->valuestring == nullptr)) goto err;
    (void)strcpy(curr_ap_info.AuthMode, AuthMode->valuestring);

    IPInfo = cJSON_GetObjectItemCaseSensitive(data, "IPInfo");

    IP = cJSON_GetObjectItemCaseSensitive(IPInfo, "IP");
    if(!cJSON_IsString(IP) || (IP->valuestring == nullptr)) goto err;
    (void)strcpy(curr_ap_info.IP, IP->valuestring);

    Mask = cJSON_GetObjectItemCaseSensitive(IPInfo, "Mask");
    if(!cJSON_IsString(Mask) || (Mask->valuestring == nullptr)) goto err;
    (void)strcpy(curr_ap_info.Mask, Mask->valuestring);

    Gateway = cJSON_GetObjectItemCaseSensitive(IPInfo, "Gateway");
    if(!cJSON_IsString(Gateway) || (Gateway->valuestring == nullptr)) goto err;
    (void)strcpy(curr_ap_info.Gateway, Gateway->valuestring);

    end:
    cJSON_Delete(ap_info_json);
    LOG_I(LOG_DBG_ON, "Screen1View::getCurrAccessPointInfo", "Done.");
#endif

    return 0;

#ifndef SIMULATOR
    err:
    cJSON_Delete(ap_info_json);
    LOG_E(LOG_DBG_ON, "Screen1View::getCurrAccessPointInfo", "Pattern error! -> %s", str);
    return -1;
#endif
}

void Screen1View::setQualityTitleBarIcon(int quality) {
    BitmapId bmpID = BITMAP_WIFI1_32X32_ID;

    if(quality != NO_CONNECTION) {
        if (quality > -50) bmpID = BITMAP_WIFI5_32X32_ID;
        else if (quality <= -50 && quality > -60) bmpID = BITMAP_WIFI4_32X32_ID;
        else if (quality <= -60 && quality > -70) bmpID = BITMAP_WIFI3_32X32_ID;
        else bmpID = BITMAP_WIFI2_32X32_ID;
    }

    wifiStrengthIcon.setBitmap(touchgfx::Bitmap(bmpID));
}

void Screen1View::updateAPInfo(const char *str) {
    if(getCurrAccessPointInfo(str) >= 0) {
        dogReset();
        if(curr_ap_info.connected) {
            setQualityTitleBarIcon(curr_ap_info.RSSI);
#ifndef SIMULATOR
            LOG_I(LOG_DBG_ON, "Connected to ","%s -> %d dbm, %s, %d, %d, %s,\r\n\tIP: %s\r\n\tMASK: %s\r\n\tGateway: %s",
                  curr_ap_info.SSID, curr_ap_info.RSSI, curr_ap_info.AuthMode, curr_ap_info.channel,
                  curr_ap_info.secondaryChannel, curr_ap_info.BSSID, curr_ap_info.IP, curr_ap_info.Mask,
                  curr_ap_info.Gateway);
#endif
        } else {
            setQualityTitleBarIcon(NO_CONNECTION);
#ifndef SIMULATOR
            LOG_I(LOG_DBG_ON, "Screen1View::updateAPInfo", "WI-FI disconnected.");
#endif
        }
    }
}

void Screen1View::dogReset() {
    tickCount = 0;
}

void Screen1View::setDigitalClock() {
#ifndef SIMULATOR
    struct timeval timeVal {};
    struct tm *nowtm;

    (void)gettimeofday(&timeVal, nullptr);
    nowtm = localtime(&timeVal.tv_sec);
    digitalSeconds = nowtm->tm_sec;
    digitalMinutes = nowtm->tm_min;
    digitalHours = nowtm->tm_hour;
    digitalClock.setTime24Hour(digitalHours, digitalMinutes, 0);
    LOG_I(LOG_DBG_ON, "Screen1View::setDigitalClock", "Clock is synchronized to %02d:%02d", digitalHours, digitalMinutes);
#endif
}
