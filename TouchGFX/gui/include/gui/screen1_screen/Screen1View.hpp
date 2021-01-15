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
    void updateAccessPoints(char *str);
    virtual void wifiScrollListUpdateItem(wifiItemContainer& item, int16_t itemIndex);
    void updateApScanningProgress(int value);
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

    /*
     * Callback Handler Declarations
     */
    void scrollListItemSelectedHandler(int16_t itemSelected);
    void apPassSetBtnOkCallbackHandler(const touchgfx::AbstractButton& src);
    void apPassSetBtnCancelCallbackHandler(const touchgfx::AbstractButton& src);

    static const int APINFO_SIZE = 128;
    struct APinfo {
        char SSID[128];
        int RSSI;
        char AuthMode[30];
        char PairwiseCipher[30];
        char GroupCipher[30];
        bool isKnow;
    } ap_info[APINFO_SIZE];


};

#endif // SCREEN1VIEW_HPP
