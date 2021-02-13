#include <gui/screen1_screen/Screen1View.hpp>
#include <cstring>
#include <string>
#include <BitmapDatabase.hpp>
#include "modalMessage.h"
#include <texts/TextKeysAndLanguages.hpp>

#ifndef SIMULATOR
#include "cJSON.h"
#endif

Screen1View::Screen1View() :
    scrollListItemSelectedCallback(this, &Screen1View::scrollListItemSelectedHandler),
    message(BITMAP_MODALWINDOWLBKG_ID, BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_ID, BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_PRESSED_ID)
{
    wifiScrollList.setItemSelectedCallback(scrollListItemSelectedCallback);
    message.setPosition(0, 0, 800, 480);
    add(message);
}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
//    setItemSelectedCallback(updateApScanningProgress);
//    wifiScanningProgress.setVisible(false);
//    wifiScanningProgress.setStartEndAngle(0, 720);
//    message.showOk(T_SINGLEUSEID12, T_SINGLEUSEID12, T_SINGLEUSEID10);
    message.showOkCancel(T_SINGLEUSEID12, T_SINGLEUSEID12,
                         T_SINGLEUSEID10, T_SINGLEUSEID9, nullptr);
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
}

void Screen1View::wifiScrollListUpdateItem(wifiItemContainer& item, int16_t itemIndex) {
    auto apStrength = ap_info[itemIndex].RSSI;
    auto AuthMode = ap_info[itemIndex].AuthMode;
    BitmapId bmpID;

    if(apStrength > -50) bmpID = BITMAP_WIFI5_64X64_ID;
    else if(apStrength <= -50 && apStrength >= -60) bmpID = BITMAP_WIFI4_64X64_ID;
    else if(apStrength <= -60 && apStrength >= -70) bmpID = BITMAP_WIFI3_64X64_ID;
    else if(apStrength < -70) bmpID = BITMAP_WIFI2_64X64_ID;
    else bmpID = BITMAP_WIFI1_64X64_ID;

    if(strcmp(AuthMode, "Unknown") == 0) item.setAccessPointStrengthUnlockIcon(bmpID);
    else item.setAccessPointStrengthLockIcon(bmpID);

    item.setAccessPointName(ap_info[itemIndex].SSID);
    item.setAccessPointStrength(apStrength);
    item.setAccessPointAuthMode(AuthMode);
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
    size_t itemCnt = 0;
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

//    wifiScrollList.removeAll();
    wifiScrollList.setNumberOfItems(itemCnt);
//    scrollableContainer1.
    cJSON_Delete(scan_json);
#endif
}

void Screen1View::scrollListItemSelectedHandler(int16_t itemSelected) {
//    char *ssid = ap_info[itemSelected].SSID;
//    (void)Unicode::strncpy(titleBuffer, ssid, strlen(ssid));
//    (void)Unicode::snprintf(titleBuffer, TITLE_SIZE, "%s", titleBuffer);
//    title.resizeToCurrentText();
//    connectAPModalWindow.show();
//    connectAPModalWindow.invalidate();
//    message.show("Puu deeba", "Mama mu deeba!");
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
