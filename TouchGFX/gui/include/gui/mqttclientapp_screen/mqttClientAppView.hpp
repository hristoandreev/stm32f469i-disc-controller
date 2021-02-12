#ifndef MQTTCLIENTAPPVIEW_HPP
#define MQTTCLIENTAPPVIEW_HPP

#include <gui_generated/mqttclientapp_screen/mqttClientAppViewBase.hpp>
#include <gui/mqttclientapp_screen/mqttClientAppPresenter.hpp>
#include <gui/common/CustomKeyboard.hpp>
//#include <touchgfx/widgets/ButtonWithLabel.hpp>

#ifndef SIMULATOR
#include "lwip/apps/mqtt.h"
#endif

class mqttClientAppView : public mqttClientAppViewBase
{
public:
    mqttClientAppView();
    virtual ~mqttClientAppView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void handleTickEvent();
protected:
    CustomKeyboard keyboard;

    void mqttConnectToBroker() override;
    void subscribeTopic() override;
    void unsubscribeTopic() override;

private:
    enum editTextType {
        BROKER,
        PORT,
        USER,
        PASSWORD,
        TOPIC,
    };

    enum mqttStatus {
        DISCONNECTED,
        CONNECTED,
        CONNECTING,
    };

    touchgfx::Callback<mqttClientAppView, const touchgfx::AbstractButtonContainer&> mqttBrokerEditTouchCallback;
    touchgfx::Callback<mqttClientAppView, const touchgfx::AbstractButtonContainer&> mqttPortEditTouchCallback;
    touchgfx::Callback<mqttClientAppView, const touchgfx::AbstractButtonContainer&> mqttUserEditTouchCallback;
    touchgfx::Callback<mqttClientAppView, const touchgfx::AbstractButtonContainer&> mqttPasswordEditTouchCallback;
    touchgfx::Callback<mqttClientAppView, const touchgfx::AbstractButtonContainer&> mqttTopicEditTouchCallback;

    touchgfx::Callback<mqttClientAppView> keyboardEnterPressed;
    touchgfx::Callback<mqttClientAppView> keyboardCancelPressed;

    void mqttBrokerEditTouchCallbackHandler(const touchgfx::AbstractButtonContainer& src);
    void mqttPortEditTouchCallbackHandler(const touchgfx::AbstractButtonContainer& src);
    void mqttUserEditTouchCallbackHandler(const touchgfx::AbstractButtonContainer& src);
    void mqttPasswordEditTouchCallbackHandler(const touchgfx::AbstractButtonContainer& src);
    void mqttTopicEditTouchCallbackHandler(const touchgfx::AbstractButtonContainer& src);

    void keyboardEnterPressedHandler();
    void keyboardCancelPressedHandler();

    editTextType editType;
    const Unicode::UnicodeChar *statusText;
    mqttStatus mqttStatus, mqttOldStatus;
    bool invalidate;
#ifndef SIMULATOR
    mqtt_client_t *mqtt_client_p;
#endif

};

#endif // MQTTCLIENTAPPVIEW_HPP
