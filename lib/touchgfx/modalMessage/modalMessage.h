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

enum buttonSource {
    srcOK,
    srcCANCEL,
    srcYES,
    srcNO,
};

typedef void (*buttonHandler)(buttonSource src);

class modalMessage : public touchgfx::Container {
public:
    modalMessage(uint16_t bmpBackgroundID, uint16_t bmpButtonReleasedID, uint16_t bmpButtonPressedID);

    void showOkCancel(
            touchgfx::TypedTextId msgTextId,
            touchgfx::TypedTextId titleTextId,
            touchgfx::TypedTextId okBtnTextId,
            touchgfx::TypedTextId cancelBtnTextId,
            touchgfx::GenericCallback<buttonSource> *callback_p);

    void showOk(
            touchgfx::TypedTextId msgTextId,
            touchgfx::TypedTextId titleTextId,
            touchgfx::TypedTextId okBtnTextId,
            touchgfx::GenericCallback<buttonSource> *callback_p);

    void showYesNo(
            touchgfx::TypedTextId msgTextId,
            touchgfx::TypedTextId titleTextId,
            touchgfx::TypedTextId yesBtnTextId,
            touchgfx::TypedTextId noBtnTextId,
            touchgfx::GenericCallback<buttonSource> *callback_p);

    void showYesNoCancel(
            touchgfx::TypedTextId msgTextId,
            touchgfx::TypedTextId titleTextId,
            touchgfx::TypedTextId yesBtnTextId,
            touchgfx::TypedTextId noBtnTextId,
            touchgfx::TypedTextId cancelBtnTextId,
            touchgfx::GenericCallback<buttonSource> *callback_p);

private:
    touchgfx::ModalWindow modal;
    touchgfx::ButtonWithLabel buttonCancel;
    touchgfx::ButtonWithLabel buttonOk;
    touchgfx::ButtonWithLabel buttonYes;
    touchgfx::ButtonWithLabel buttonNo;
    touchgfx::TextAreaWithOneWildcard title;
    touchgfx::TextAreaWithOneWildcard message;

    /*
    * Callback Declarations
    */
    touchgfx::Callback<modalMessage, const touchgfx::AbstractButton&> buttonOkCallback;
    touchgfx::Callback<modalMessage, const touchgfx::AbstractButton&> buttonCancelCallback;

    touchgfx::Callback<modalMessage, const touchgfx::AbstractButton&> buttonYesCallback;
    touchgfx::Callback<modalMessage, const touchgfx::AbstractButton&> buttonNoCallback;

    touchgfx::GenericCallback<buttonSource> *callback;

    /*
     * Callback Handler Declarations
     */
    void buttonOkCallbackHandler(const touchgfx::AbstractButton& src);
    void buttonCancelCallbackHandler(const touchgfx::AbstractButton& src);

    void buttonYesCallbackHandler(const touchgfx::AbstractButton& src);
    void buttonNoCallbackHandler(const touchgfx::AbstractButton& src);

    void show(touchgfx::TypedTextId msgTextId, touchgfx::TypedTextId titleTextId);

    uint16_t displayWidth;
    uint16_t displayHeight;
    uint16_t modalWidth;
    uint16_t modalHeight;
    int16_t backgroundX;
    int16_t backgroundY;

    uint16_t BITMAP_BACKGROUND_ID;
    uint16_t BITMAP_BUTTONS_RELEASED_ID;
    uint16_t BITMAP_BUTTONS_PRESSED_ID;
};


#endif //_MODALMESSAGE_H
