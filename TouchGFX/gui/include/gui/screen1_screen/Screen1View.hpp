#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>
#include "modalMessage.h"

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
    Callback<Screen1View, int16_t> scrollListItemSelectedCallback;
    void scrollListItemSelectedHandler(int16_t itemSelected);

    static const int APINFO_SIZE = 128;
    struct APinfo {
        char SSID[128];
        int RSSI;
        char AuthMode[30];
        char PairwiseCipher[30];
        char GroupCipher[30];
        bool isKnow;
    } ap_info[APINFO_SIZE];

    modalMessage message;
};

#endif // SCREEN1VIEW_HPP
