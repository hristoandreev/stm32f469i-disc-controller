#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

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
    void updateAccessPoints(char *str);
    void hideProgress();
//    virtual void handleDragEvent(const DragEvent &evt);
//    virtual void handleClickEvent(const ClickEvent& evt);
protected:

private:
    /*
    * Callback Declarations
    */
    touchgfx::Callback<Screen1View, int16_t> scrollListItemSelectedCallback;
    touchgfx::Callback<Screen1View, const touchgfx::AbstractButton&> apPassSetBtnOkCallback;
    touchgfx::Callback<Screen1View, const touchgfx::AbstractButton&> apPassSetBtnCancelCallback;

    touchgfx::Callback<Screen1View, const char*> webResCompleteCallback;

    /*
     * Callback Handler Declarations
     */
    void scrollListItemSelectedHandler(int16_t itemSelected);
    void apPassSetBtnOkCallbackHandler(const touchgfx::AbstractButton& src);
    void apPassSetBtnCancelCallbackHandler(const touchgfx::AbstractButton& src);

    void webResCompleteCallbackHandler(const char* res);

    static const int APINFO_SIZE = 128;
    struct APinfo {
        char SSID[256];
        int RSSI;
        char AuthMode[60];
        char PairwiseCipher[60];
        char GroupCipher[60];
        bool isKnow;
    } ap_info[APINFO_SIZE];

    size_t itemCnt;
};

#endif // SCREEN1VIEW_HPP
