//
// Created by hristo on 1/14/21.
//

#ifndef _MODALMESSAGE_H
#define _MODALMESSAGE_H

#include <touchgfx/widgets/ButtonWithLabel.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/containers/ModalWindow.hpp>
#include <touchgfx/widgets/BoxWithBorder.hpp>
#include <touchgfx/Screen.hpp>

class modalMessage : public touchgfx::Container {
public:
    modalMessage();

    void show(const char *msgText, const char *titleText);

private:
    touchgfx::ModalWindow modal;
    touchgfx::ButtonWithLabel buttonCancel;
    touchgfx::ButtonWithLabel buttonOk;
    touchgfx::TextAreaWithOneWildcard title;
    touchgfx::TextAreaWithOneWildcard message;

    /*
    * Callback Declarations
    */
    touchgfx::Callback<modalMessage, const touchgfx::AbstractButton&> buttonOkCallback;
    touchgfx::Callback<modalMessage, const touchgfx::AbstractButton&> buttonCancelCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonOkCallbackHandler(const touchgfx::AbstractButton& src);
    void buttonCancelCallbackHandler(const touchgfx::AbstractButton& src);

    /*
     * Wildcard Buffers
     */
    static const uint16_t TITLE_SIZE = 128;
    touchgfx::Unicode::UnicodeChar titleBuffer[TITLE_SIZE];
    static const uint16_t MESSAGE_SIZE = 256;
    touchgfx::Unicode::UnicodeChar messageBuffer[MESSAGE_SIZE];
};


#endif //_MODALMESSAGE_H
