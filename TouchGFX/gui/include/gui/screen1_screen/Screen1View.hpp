#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

#ifndef SIMULATOR
#include "web.h"
#endif

class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void wifiScrollListUpdateItem(wifiItemContainer& item, int16_t itemIndex);
    virtual void handleTickEvent();
    void updateApScanningProgress(int value);
    void updateAccessPoints(const char *str);
    void hideProgress();
    void wifiScrollListClean();
protected:

private:
    /*
    * Callback Declarations
    */
    touchgfx::Callback<Screen1View, int16_t> scrollListItemSelectedCallback;
    touchgfx::Callback<Screen1View, const touchgfx::AbstractButton&> apPassSetBtnOkCallback;
    touchgfx::Callback<Screen1View, const touchgfx::AbstractButton&> apPassSetBtnCancelCallback;
#ifndef SIMULATOR
    touchgfx::Callback<Screen1View, HTTPResult, const char*> webApConnectCompleteCallback;
    touchgfx::Callback<Screen1View, HTTPResult, const char*> webGetCurrApInfoCompleteCallback;
    touchgfx::Callback<Screen1View, HTTPResult, const char*> webGetABVBGCompleteCallback;
#endif
    /*
     * Callback Handler Declarations
     */
    void scrollListItemSelectedHandler(int16_t itemSelected);
    void apPassSetBtnOkCallbackHandler(const touchgfx::AbstractButton& src);
    void apPassSetBtnCancelCallbackHandler(const touchgfx::AbstractButton& src);
#ifndef SIMULATOR
    void webApConnectCompleteCallbackHandler(HTTPResult status, const char* res);
    void webGetCurrApInfoCompleteCallbackHandler(HTTPResult status, const char* res);
    void webGetABVBGCompleteCallbackHandler(HTTPResult status, const char* res);
#endif
    /*
     * Private methods.
     */
    int getCurrAccessPointInfo(const char *str);
    void updateAPInfo(const char *str);
    void setQualityTitleBarIcon(int quality);
    void dogReset();
    void setDigitalClock();

    /*
     * Member data.
     */
    static const int AP_INFO_SIZE = 128;
    static const int AP_SSID_SIZE = 256;
    static const int AP_AUTH_MODE_SIZE = 60;
    static const int AP_PAIR_CIPHER_SIZE = 60;
    static const int AP_CROUP_CIPHER_SIZE = 60;
    static const int AP_BSSID_SIZE = 30;
    static const int AP_IP_MASK_GATEWAY_SIZE = 20;

    static const int NO_CONNECTION = -1000;

    struct APinfo {
        char SSID[AP_SSID_SIZE];
        int RSSI;
        char AuthMode[AP_AUTH_MODE_SIZE];
        char PairwiseCipher[AP_PAIR_CIPHER_SIZE];
        char GroupCipher[AP_CROUP_CIPHER_SIZE];
        bool isKnow;
    } ap_info[AP_INFO_SIZE];

    struct CurrApInfo {
        bool connected;
        char SSID[AP_SSID_SIZE];
        int RSSI;
        int channel;
        int secondaryChannel;
        char BSSID[AP_BSSID_SIZE];
        char AuthMode[AP_AUTH_MODE_SIZE];
        char IP[AP_IP_MASK_GATEWAY_SIZE];
        char Mask[AP_IP_MASK_GATEWAY_SIZE];
        char Gateway[AP_IP_MASK_GATEWAY_SIZE];
    }curr_ap_info;

    size_t itemCnt;
    int scanPeriod;
    int tickCount;
    int update_gadget;
    int digitalClockCount;
    int digitalSeconds;
    int digitalMinutes;
    int digitalHours;
    bool scanDone;
};

#endif // SCREEN1VIEW_HPP
