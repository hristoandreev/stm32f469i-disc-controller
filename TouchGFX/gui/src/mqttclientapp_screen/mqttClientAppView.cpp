#include <gui/mqttclientapp_screen/mqttClientAppView.hpp>

#ifndef SIMULATOR
#include "logg.h"
#endif

mqttClientAppView::mqttClientAppView() :
        mqttBrokerEditTouchCallback(this, &mqttClientAppView::mqttBrokerEditTouchCallbackHandler),
        mqttPortEditTouchCallback(this, &mqttClientAppView::mqttPortEditTouchCallbackHandler),
        mqttUserEditTouchCallback(this, &mqttClientAppView::mqttUserEditTouchCallbackHandler),
        mqttPasswordEditTouchCallback(this, &mqttClientAppView::mqttPasswordEditTouchCallbackHandler),
        mqttTopicEditTouchCallback(this, &mqttClientAppView::mqttTopicEditTouchCallbackHandler),
        keyboardEnterPressed(this, &mqttClientAppView::keyboardEnterPressedHandler),
        keyboardCancelPressed(this, &mqttClientAppView::keyboardCancelPressedHandler),
        editType(BROKER),
        statusText(TypedText(T_MQTTDISCONNECTED).getText()),
        mqttStatus(DISCONNECTED),
        mqttOldStatus(DISCONNECTED),
        invalidate(false)
#ifndef SIMULATOR
        ,mqtt_client_p(nullptr)
#endif
{
    keyboard.setPosition(0, 0, 800, 480);
    brokerContainer.setEditTouchHandler(mqttBrokerEditTouchCallback);
    portContainer.setEditTouchHandler(mqttPortEditTouchCallback);
    userContainer.setEditTouchHandler(mqttUserEditTouchCallback);
    passwordContainer.setEditTouchHandler(mqttPasswordEditTouchCallback);
    topicContainer.setEditTouchHandler(mqttTopicEditTouchCallback);
    keyboard.setEnterAction(&keyboardEnterPressed);
    keyboard.setCancelAction(&keyboardCancelPressed);
}

void mqttClientAppView::setupScreen()
{
    mqttClientAppViewBase::setupScreen();

    (void)Unicode::strncpy(statusLabelBuffer, statusText, Unicode::strlen(statusText) + 1);
    statusLabel.resizeToCurrentText();

    connectProgress.setValue(0);
    connectProgress.setVisible(false);

    Unicode::UnicodeChar *port = nullptr;
    (void)Unicode::fromUTF8(reinterpret_cast<const uint8_t *>("1883"), port, Unicode::strlen("1883"));
    portContainer.setText(port);
}

void mqttClientAppView::tearDownScreen()
{
    mqttClientAppViewBase::tearDownScreen();
}

void mqttClientAppView::mqttBrokerEditTouchCallbackHandler(const AbstractButtonContainer &src) {
    add(keyboard);
    Unicode::UnicodeChar *buff = keyboard.getBuffer();
    uint16_t len = brokerContainer.getText(buff);
    keyboard.setBufferPosition(len);
    editType = BROKER;
    keyboard.invalidate();
}

void mqttClientAppView::mqttPortEditTouchCallbackHandler(const AbstractButtonContainer &src) {
    add(keyboard);
    Unicode::UnicodeChar *buff = keyboard.getBuffer();
    uint16_t len = portContainer.getText(buff);
    keyboard.setBufferPosition(len);
    editType = PORT;
    keyboard.invalidate();
}

void mqttClientAppView::mqttUserEditTouchCallbackHandler(const AbstractButtonContainer &src) {
    add(keyboard);
    Unicode::UnicodeChar *buff = keyboard.getBuffer();
    uint16_t len = userContainer.getText(buff);
    keyboard.setBufferPosition(len);
    editType = USER;
    keyboard.invalidate();
}

void mqttClientAppView::mqttPasswordEditTouchCallbackHandler(const AbstractButtonContainer &src) {
    add(keyboard);
    Unicode::UnicodeChar *buff = keyboard.getBuffer();
    uint16_t len = passwordContainer.getText(buff);
    keyboard.setBufferPosition(len);
    editType = PASSWORD;
    keyboard.invalidate();
}

void mqttClientAppView::mqttTopicEditTouchCallbackHandler(const AbstractButtonContainer &src) {
    add(keyboard);
    Unicode::UnicodeChar *buff = keyboard.getBuffer();
    uint16_t len = topicContainer.getText(buff);
    keyboard.setBufferPosition(len);
    editType = TOPIC;
    keyboard.invalidate();
}

void mqttClientAppView::keyboardEnterPressedHandler() {
    remove(keyboard);
    keyboard.invalidate();
    Unicode::UnicodeChar *buff = keyboard.getBuffer();

    switch (editType) {
        case BROKER:
            brokerContainer.setText(buff);
            brokerContainer.invalidate();
            break;
        case PORT:
            portContainer.setText(buff);
            portContainer.invalidate();
            break;
        case USER:
            userContainer.setText(buff);
            userContainer.invalidate();
            break;
        case PASSWORD:
            passwordContainer.setText(buff);
            passwordContainer.invalidate();
            break;
        case TOPIC:
            topicContainer.setText(buff);
            topicContainer.invalidate();
            break;
    }
}

void mqttClientAppView::keyboardCancelPressedHandler() {
    remove(keyboard);
    keyboard.invalidate();
}

void mqttClientAppView::mqttConnectToBroker() {
    mqttClientAppViewBase::mqttConnectToBroker();

    ip_addr_t mqtt_host;
//    IP4_ADDR(&mqtt_host, 192, 168, 0, 104);

    Unicode::UnicodeChar u_user[128];
    Unicode::UnicodeChar u_pass[128];
    Unicode::UnicodeChar u_host[128];
    uint16_t len = userContainer.getText(u_user);
    uint8_t user[len + 1];
    (void)Unicode::toUTF8(u_user, user, len + 1);

    len = passwordContainer.getText(u_pass);
    uint8_t pass[len + 1];
    (void)Unicode::toUTF8(u_pass, pass, len + 1);

    len = brokerContainer.getText(u_host);
    uint8_t host[len + 1];
    (void)Unicode::toUTF8(u_host, host, len + 1);

    mqtt_host.addr = inet_addr(reinterpret_cast<const char *>(host));
    if(mqtt_host.addr == IPADDR_NONE) {
        struct hostent *hostent_p;
        hostent_p = lwip_gethostbyname(reinterpret_cast<const char *>(host));
        if(hostent_p == nullptr) {
            // TODO: show message box to inform wats happen.
            return;
        }
        mqtt_host.addr = inet_addr(hostent_p->h_addr_list[0]); // TODO: check is this work correctly!!!
    }

    if(strlen(reinterpret_cast<const char *>(host)) <= 0) {
        // TODO: show message box to inform wats happen.
        return;
    }

#ifndef SIMULATOR
    if(mqttStatus == DISCONNECTED) {
        struct mqtt_connect_client_info_t info = {
                "stm32f469i-disc-controller",
                (strlen(reinterpret_cast<const char *>(user)) > 0)
                                ? reinterpret_cast<const char *>(user) : nullptr,
                (strlen(reinterpret_cast<const char *>(pass)) > 0)
                                ? reinterpret_cast<const char *>(pass) : nullptr,
                100,
                nullptr,
                nullptr,
                0,
                0};

        (void)Unicode::strncpy(clientIDLabelBuffer, info.client_id, CLIENTIDLABEL_SIZE);
        clientIDLabel.resizeToCurrentText();
        clientIDLabel.invalidate();

        mqtt_client_p = mqtt_client_new();
        DBG_ASSERT(mqtt_client_p != nullptr, "Cannot create MQTT object!!!");

        err_t res = mqtt_client_connect(mqtt_client_p, &mqtt_host, MQTT_PORT,
             [](mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
                 auto this_p = reinterpret_cast<mqttClientAppView *>(arg);
                 switch (status) {
                     /** Accepted */
                     case MQTT_CONNECT_ACCEPTED:
                         LOG_I(LOG_DBG_ON, "mqtt_client_connect",
                               "Connected to MQTT broker.");
                         this_p->mqttStatus = CONNECTED;
                         this_p->statusText = TypedText(T_MQTTCONNECTED).getText();
                         this_p->connectBtn.setLabelText(TypedText(T_MQTTDISCONNECT));
                         this_p->connectProgress.setVisible(false);
                         this_p->mqttDataBuffer[0] = 0; // remove all items.
                         this_p->mqttData.resizeToCurrentText();
                         mqtt_set_inpub_callback(this_p->mqtt_client_p, [](void *arg, const char *topic, u32_t tot_len) {
                                 auto this_p = reinterpret_cast<mqttClientAppView *>(arg);
                                 Unicode::UnicodeChar dst[128];
                                 Unicode::UnicodeChar *p;
                                 uint16_t level = &this_p->mqttDataBuffer[Unicode::strlen(this_p->mqttDataBuffer)] - this_p->mqttDataBuffer;
                                 p = ((level + tot_len + strlen(topic) + 4) > MQTTDATA_SIZE)    // 4 for format string.
                                         ? this_p->mqttDataBuffer : &this_p->mqttDataBuffer[Unicode::strlen(this_p->mqttDataBuffer)];
                                 (void)Unicode::fromUTF8(reinterpret_cast<const uint8_t *>(topic), dst, sizeof(dst));
                                 (void)Unicode::snprintf(p, MQTTDATA_SIZE - level, "[%s]: ", dst);
                                 if(tot_len > 0) this_p->invalidate = true;
                             },
                             [](void *arg, const u8_t *data, u16_t len, u8_t flags) {
                                 auto this_p = reinterpret_cast<mqttClientAppView *>(arg);
                                 Unicode::UnicodeChar dst[512];
                                 uint16_t level = &this_p->mqttDataBuffer[Unicode::strlen(this_p->mqttDataBuffer)] - this_p->mqttDataBuffer;
                                 (void)Unicode::fromUTF8(reinterpret_cast<const uint8_t *>(data), dst, sizeof(dst));
                                 (void)Unicode::snprintf(&this_p->mqttDataBuffer[Unicode::strlen(this_p->mqttDataBuffer)], MQTTDATA_SIZE - level - 1, "%s\n", dst);
                                 this_p->mqttData.resizeToCurrentText();
                                 this_p->invalidate = true;
                             }, this_p);
                         break;
                         /** Refused protocol version */
                     case MQTT_CONNECT_REFUSED_PROTOCOL_VERSION:
                         LOG_I(LOG_DBG_ON, "mqtt_client_connect",
                               "Connect to MQTT broker refused protocol version.");
                         break;
                         /** Refused identifier */
                     case MQTT_CONNECT_REFUSED_IDENTIFIER:
                         LOG_I(LOG_DBG_ON, "mqtt_client_connect",
                               "Connect to MQTT broker refused identifier.");
                         break;
                         /** Refused server */
                     case MQTT_CONNECT_REFUSED_SERVER:
                         LOG_I(LOG_DBG_ON, "mqtt_client_connect",
                               "Connect to MQTT broker refused server.");
                         break;
                         /** Refused user credentials */
                     case MQTT_CONNECT_REFUSED_USERNAME_PASS:
                         LOG_I(LOG_DBG_ON, "mqtt_client_connect",
                               "Connect to MQTT broker refused username or pass.");
                         break;
                         /** Refused not authorized */
                     case MQTT_CONNECT_REFUSED_NOT_AUTHORIZED_:
                         LOG_I(LOG_DBG_ON, "mqtt_client_connect",
                               "Connect to MQTT broker not authorized.");
                         break;
                         /** Disconnected */
                     case MQTT_CONNECT_DISCONNECTED:
                         if(this_p->mqtt_client_p != nullptr) {
                             mqtt_client_free(this_p->mqtt_client_p);
                             this_p->mqtt_client_p = nullptr;
                         }
                         LOG_I(LOG_DBG_ON, "mqtt_client_connect",
                               "Disconnected from MQTT broker.");
                         this_p->mqttStatus = DISCONNECTED;
                         this_p->statusText = TypedText(T_MQTTDISCONNECTED).getText();
                         this_p->connectBtn.setLabelText(TypedText(T_MQTTCONNECT));
                         this_p->connectProgress.setVisible(false);
                         break;
                         /** Timeout */
                     case MQTT_CONNECT_TIMEOUT:
                         LOG_I(LOG_DBG_ON, "mqtt_client_connect",
                               "Connect timeout to MQTT broker.");
                         break;
                     default:
                         break;
                 }
             }, this, &info);

        if(res != ERR_OK) return;

        mqttStatus = CONNECTING;
        statusText = TypedText(T_MQTTCONNECTING).getText();
        connectProgress.setVisible(true);
    } else if(mqttStatus == CONNECTED) {
        mqtt_disconnect(mqtt_client_p);
        if(mqtt_client_p != nullptr) {
            mqtt_client_free(mqtt_client_p);
            mqtt_client_p = nullptr;
        }
        mqttStatus = DISCONNECTED;
        statusText = TypedText(T_MQTTDISCONNECTED).getText();
        connectBtn.setLabelText(TypedText(T_MQTTCONNECT));
        connectProgress.setVisible(false);
    } else {
        return;
    }
#endif
}

void mqttClientAppView::handleTickEvent() {
    if(mqttOldStatus != mqttStatus) {
        mqttOldStatus = mqttStatus;
        (void) Unicode::strncpy(statusLabelBuffer, statusText, Unicode::strlen(statusText) + 1);
//        statusLabel.resizeToCurrentText();
        statusLabel.invalidate();
        connectProgress.invalidate();
        connectBtn.invalidate();
    }

    if(invalidate) {
        invalidate = false;
        mqttData.invalidate();
    }
}

void mqttClientAppView::subscribeTopic() {
    mqttClientAppViewBase::subscribeTopic();
    Unicode::UnicodeChar u_topic[128];
    uint16_t len = topicContainer.getText(u_topic);
    uint8_t topic[len + 1];
    (void)Unicode::toUTF8(u_topic, topic, len + 1);
    mqtt_subscribe(mqtt_client_p, reinterpret_cast<const char *>(topic), 2, [](void *arg, err_t err) {
            (void)arg;
            (void)err;
//            auto this_p = reinterpret_cast<mqttClientAppView *>(arg);
        }, this);
}

void mqttClientAppView::unsubscribeTopic() {
    mqttClientAppViewBase::unsubscribeTopic();
    Unicode::UnicodeChar u_topic[128];
    uint16_t len = topicContainer.getText(u_topic);
    uint8_t topic[len + 1];
    (void)Unicode::toUTF8(u_topic, topic, len + 1);
    mqtt_unsubscribe(mqtt_client_p, reinterpret_cast<const char *>(topic), [](void *arg, err_t err) {
            (void)arg;
            (void)err;
//            auto this_p = reinterpret_cast<mqttClientAppView *>(arg);
        }, this);
}
