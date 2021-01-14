#ifndef WIFIITEMCONTAINER_HPP
#define WIFIITEMCONTAINER_HPP

#include <gui_generated/containers/wifiItemContainerBase.hpp>

class wifiItemContainer : public wifiItemContainerBase
{
public:
    wifiItemContainer();
    virtual ~wifiItemContainer() {}

    virtual void initialize();

    void setAccessPointStrengthLockIcon(BitmapId bitmap) {
        accessPointStrengthIcon.setBitmap(touchgfx::Bitmap(bitmap));
        accessPointLockIcon.setVisible(true);
    }

    void setAccessPointStrengthUnlockIcon(BitmapId bitmap) {
        accessPointStrengthIcon.setBitmap(touchgfx::Bitmap(bitmap));
        accessPointLockIcon.setVisible(false);
    }

    void setAccessPointName(const char *name) {
        (void)Unicode::strncpy(accessPointNameBuffer, name, ACCESSPOINTNAME_SIZE);
        (void)Unicode::snprintf(accessPointNameBuffer, ACCESSPOINTNAME_SIZE, "%s", accessPointNameBuffer);
        accessPointName.resizeToCurrentText();
//        accessPointName.invalidate();
    }

    void setAccessPointStrength(int strength) {
        (void)Unicode::snprintf(accessPointStranghtBuffer, ACCESSPOINTSTRANGHT_SIZE, "%d", strength);
        accessPointStranght.resizeToCurrentText();
//        accessPointStranght.invalidate();
    }

    void setAccessPointAuthMode(const char *authMode) {
        (void)Unicode::strncpy(accessPointAuthModeBuffer, authMode, ACCESSPOINTAUTHMODE_SIZE);
        (void)Unicode::snprintf(accessPointAuthModeBuffer, ACCESSPOINTAUTHMODE_SIZE, "%s", accessPointAuthModeBuffer);
        accessPointAuthMode.resizeToCurrentText();
//        accessPointAuthMode.invalidate();
    }
protected:
};

#endif // WIFIITEMCONTAINER_HPP
