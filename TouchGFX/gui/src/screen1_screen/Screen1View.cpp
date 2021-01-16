#include <gui/screen1_screen/Screen1View.hpp>
#include <cstring>
#include <string>
#include <BitmapDatabase.hpp>

#ifndef SIMULATOR
#include "cJSON.h"
#include "web.h"
#endif

Screen1View::Screen1View() :
    scrollListItemSelectedCallback(this, &Screen1View::scrollListItemSelectedHandler),
    apPassSetBtnOkCallback(this, &Screen1View::apPassSetBtnOkCallbackHandler),
    apPassSetBtnCancelCallback(this, &Screen1View::apPassSetBtnCancelCallbackHandler),
    webResCompleteCallback(this, &Screen1View::webResCompleteCallbackHandler)
{
    wifiScrollList.setItemSelectedCallback(scrollListItemSelectedCallback);
    buttonOk.setAction(apPassSetBtnOkCallback);
    buttonCancel.setAction(apPassSetBtnCancelCallback);
}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
//    for(int i = 0; i < wifiScrollListListItems.getNumberOfDrawables(); ++i)
//        wifiScrollList.itemChanged(i);
}

void Screen1View::tearDownScreen()
{
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

void Screen1View::wifiScrollListUpdateItem(wifiItemContainer& item, int16_t itemIndex) {
    auto apStrength = ap_info[itemIndex].RSSI;
    auto authMode = ap_info[itemIndex].AuthMode;
    auto ssid = ap_info[itemIndex].SSID;
    BitmapId bmpID;

    if(apStrength > -50) bmpID = BITMAP_WIFI5_64X64_ID;
    else if(apStrength <= -50 && apStrength >= -60) bmpID = BITMAP_WIFI4_64X64_ID;
    else if(apStrength <= -60 && apStrength >= -70) bmpID = BITMAP_WIFI3_64X64_ID;
    else if(apStrength < -70) bmpID = BITMAP_WIFI2_64X64_ID;
    else bmpID = BITMAP_WIFI1_64X64_ID;

    if(strcmp(authMode, "Unknown") == 0) item.setAccessPointStrengthUnlockIcon(bmpID);
    else item.setAccessPointStrengthLockIcon(bmpID);

    item.setAccessPointName(ssid);
    item.setAccessPointStrength(apStrength);
    item.setAccessPointAuthMode(authMode);
//    item.invalidate();
}

//void Screen1View::handleDragEvent(const DragEvent &evt) {
//    wifiSlideMenu.resetExpandedStateTimer();
//}

//void Screen1View::handleClickEvent(const ClickEvent& evt) {
////    wifiSlideMenu.resetExpandedStateTimer();
//}

void Screen1View::updateAccessPoints(char *str) {
#ifndef SIMULATOR
    itemCnt = 0;
    cJSON *scan_json = cJSON_Parse(str);
    if(scan_json == nullptr) return;

    const cJSON *status = cJSON_GetObjectItemCaseSensitive(scan_json, "status");
    if(!cJSON_IsString(status) || (status->valuestring == nullptr)) {cJSON_Delete(scan_json); return;}
    if(strcmp(status->valuestring, "success") != 0) {cJSON_Delete(scan_json); return;}

    const cJSON *data = cJSON_GetObjectItemCaseSensitive(scan_json, "data");

    const cJSON *APs = cJSON_GetObjectItemCaseSensitive(data, "APs");
    if(!cJSON_IsArray(APs)) { cJSON_Delete(scan_json); return; }

    cJSON *AP;
    cJSON_ArrayForEach(AP, APs)
    {
        cJSON *SSID = cJSON_GetObjectItemCaseSensitive(AP, "SSID");
        cJSON *RSSI = cJSON_GetObjectItemCaseSensitive(AP, "RSSI");
        cJSON *AuthMode = cJSON_GetObjectItemCaseSensitive(AP, "AuthMode");
        cJSON *PairwiseCipher = cJSON_GetObjectItemCaseSensitive(AP, "PairwiseCipher");
        cJSON *GroupCipher = cJSON_GetObjectItemCaseSensitive(AP, "GroupCipher");
        cJSON *IsKnown = cJSON_GetObjectItemCaseSensitive(AP, "IsKnown");

        if (!cJSON_IsString(SSID) ||
            !cJSON_IsNumber(RSSI) ||
            !cJSON_IsString(AuthMode) ||
            !cJSON_IsString(PairwiseCipher) ||
            !cJSON_IsString(GroupCipher) ||
            !cJSON_IsBool(IsKnown)) {
                cJSON_Delete(scan_json);
                return;
        }
        (void)strcpy(ap_info[itemCnt].SSID, SSID->valuestring);
        (void)strcpy(ap_info[itemCnt].AuthMode, AuthMode->valuestring);
        (void)strcpy(ap_info[itemCnt].PairwiseCipher, PairwiseCipher->valuestring);
        (void)strcpy(ap_info[itemCnt].GroupCipher, GroupCipher->valuestring);
        ap_info[itemCnt].RSSI = RSSI->valueint;
        ap_info[itemCnt].isKnow = IsKnown->valueint;
        itemCnt++;
    }

    cJSON_Delete(scan_json);
//    wifiScrollList.removeAll();
//    wifiScrollList.setNumberOfItems(itemCnt);
//    wifiScrollList.invalidate();
//    scrollableContainer1.
#endif
}

void Screen1View::scrollListItemSelectedHandler(int16_t itemSelected) {
    char *ssid = ap_info[itemSelected].SSID;
    (void)Unicode::fromUTF8(reinterpret_cast<const uint8_t *>(ssid), titleBuffer, TITLE_SIZE);
    title.resizeToCurrentText();
    connectAPModalWindow.show();
    connectAPModalWindow.invalidate();
}

void Screen1View::apPassSetBtnOkCallbackHandler(const AbstractButton &src) {
    uint8_t utf8Password[50];
    uint8_t utf8SSID[128];
    char *uri[512];
    connectAPModalWindow.hide();
    connectAPModalWindow.invalidate();
    (void)Unicode::toUTF8(passwordBuffer, utf8Password, sizeof(utf8Password));
    (void)Unicode::toUTF8(titleBuffer, utf8SSID, sizeof(utf8SSID));
    (void)sprintf(reinterpret_cast<char *>(uri),
                  "http://192.168.3.1/wifi?cmd=connect&ssid=%s&pass=%s",
                  utf8SSID, utf8Password);
//    web web;
//    (void)web.get(reinterpret_cast<const char *>(uri), &webResCompleteCallback);
}

void Screen1View::apPassSetBtnCancelCallbackHandler(const AbstractButton &src) {
    connectAPModalWindow.hide();
    connectAPModalWindow.invalidate();
}

void Screen1View::webResCompleteCallbackHandler(const char *res) {

}

void Screen1View::handleTickEvent() {
    if(itemCnt) {
        wifiScrollList.setNumberOfItems(itemCnt);
        itemCnt = 0;
    }
}
/*
 * "{\"status\":\"success\",
 * \"data\":{
 *      \"APs\":[
 *          {\"SSID\":\"Nochka\",\"RSSI\":-55,\"AuthMode\":\"WPA2_PSK\",\"PairwiseCipher\":\"CCMP\",\"GroupCipher\":\"CCMP\",\"IsKnown\":true},
 *          {\"SSID\":\"Tech_D3855281\",\"RSSI\":-77,\"AuthMode\":\"WPA_WPA2_PSK\",\"PairwiseCipher\":\"TKIP+CCMP\",\"GroupCipher\":\"TKIP\",\"IsKnown\":false},
 *          {\"SSID\":\"TP-LINK_C84C\",\"RSSI\":-80,\"AuthMode\":\"WPA2_PSK\",\"PairwiseCipher\":\"CCMP\",\"GroupCipher\":\"CCMP\",\"IsKnown\":false},
 *          {\"SSID\":\"Tech_D0052074\",\"RSSI\":-81,\"AuthMode\":\"WPA_WPA2_PSK\",\"PairwiseCipher\":\"TKIP+CCMP\",\"GroupCipher\":\"TKIP\",\"IsKnown\":false},
 *          {\"SSID\":\"TomiMancho\",\"RSSI\":-81,\"AuthMode\":\"WPA2_PSK\",\"PairwiseCipher\":\"CCMP\",\"GroupCipher\":\"CCMP\",\"IsKnown\":false},
 *          {\"SSID\":\"cxxxx{}:::::::::::::::::>\",\"RSSI\":-85,\"AuthMode\":\"WPA2_PSK\",\"PairwiseCipher\":\"CCMP\",\"GroupCipher\":\"CCMP\",\"IsKnown\":false},
 *          {\"SSID\":\"Rozi\",\"RSSI\":-85,\"AuthMode\":\"WPA2_PSK\",\"PairwiseCipher\":\"TKIP+CCMP\",\"GroupCipher\":\"TKIP\",\"IsKnown\":false},
 *          {\"SSID\":\"Kukov1\",\"RSSI\":-89,\"AuthMode\":\"WPA2_PSK\",\"PairwiseCipher\":\"CCMP\",\"GroupCipher\":\"CCMP\",\"IsKnown\":false},
 *          {\"SSID\":\"VIVACOM_NET\",\"RSSI\":-91,\"AuthMode\":\"WPA2_PSK\",\"PairwiseCipher\":\"CCMP\",\"GroupCipher\":\"CCMP\",\"IsKnown\":false},
 *          {\"SSID\":\"Konstantin-\",\"RSSI\":-91,\"AuthMode\":\"WPA_WPA2_PSK\",\"PairwiseCipher\":\"TKIP+CCMP\",\"GroupCipher\":\"TKIP\",\"IsKnown\":false},
 *          {\"SSID\":\"Skevovi\",\"RSSI\":-94,\"AuthMode\":\"WPA2_PSK\",\"PairwiseCipher\":\"CCMP\",\"GroupCipher\":\"CCMP\",\"IsKnown\":false},
 *          {\"SSID\":\"6ec410\",\"RSSI\":-94,\"AuthMode\":\"WPA_WPA2_PSK\",\"PairwiseCipher\":\"CCMP\",\"GroupCipher\":\"CCMP\",\"IsKnown\":false},
 *          {\"SSID\":\"PepAlex\",\"RSSI\":-95,\"AuthMode\":\"Unknown\",\"PairwiseCipher\":\"None\",\"GroupCipher\":\"None\",\"IsKnown\":false},
 *          {\"SSID\":\"Tech_D3751135\",\"RSSI\":-95,\"AuthMode\":\"WPA_WPA2_PSK\",\"PairwiseCipher\":\"TKIP+CCMP\",\"GroupCipher\":\"TKIP\",\"IsKnown\":false},
 *          {\"SSID\":\"Rozi\",\"RSSI\":-96,\"AuthMode\":\"WPA2_PSK\",\"PairwiseCipher\":\"TKIP+CCMP\",\"GroupCipher\":\"TKIP\",\"IsKnown\":false}
 *      ]
 *  }
 *}"
 */
