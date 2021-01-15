//
// Created by hristo on 1/14/21.
//

#include "modalMessage.h"
#include <touchgfx/Color.hpp>
#include <touchgfx/Bitmap.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Unicode.hpp>
#include <cstring>

using namespace touchgfx;

modalMessage::modalMessage() :
        buttonOkCallback(this, &modalMessage::buttonOkCallbackHandler),
        buttonCancelCallback(this, &modalMessage::buttonCancelCallbackHandler)
{
    modal.setBackground(touchgfx::BitmapId(BITMAP_MODALWINDOWLBKG_ID), 160, 80);
    modal.setShadeColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    modal.setShadeAlpha(150);
    modal.hide();

    buttonCancel.setXY(50, 243);
    buttonCancel.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_PRESSED_ID));
    buttonCancel.setLabelText(touchgfx::TypedText(T_SINGLEUSEID9));
    buttonCancel.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    buttonCancel.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    buttonCancel.setAction(buttonCancelCallback);
    modal.add(buttonCancel);

    buttonOk.setXY(260, 243);
    buttonOk.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_PRESSED_ID));
    buttonOk.setLabelText(touchgfx::TypedText(T_SINGLEUSEID10));
    buttonOk.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    buttonOk.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    buttonOk.setAction(buttonOkCallback);
    modal.add(buttonOk);

    title.setXY(35, 13);
    title.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    title.setLinespacing(0);
    titleBuffer[0] = 0;
//    (void)Unicode::strncpy(titleBuffer, "Title", TITLE_SIZE);
    title.setWildcard(titleBuffer);
    title.resizeToCurrentText();
    title.setTypedText(touchgfx::TypedText(T_SINGLEUSEID12));
    modal.add(title);

    message.setXY(24, 96);
    message.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    message.setLinespacing(0);
    messageBuffer[0] = 0;
    message.setWildcard(messageBuffer);
    message.resizeToCurrentText();
    message.setTypedText(touchgfx::TypedText(T_SINGLEUSEID12));
    modal.add(message);

    add(modal);
}

void modalMessage::show(const char *msgText, const char *titleText = "Title") {
    (void)Unicode::strncpy(titleBuffer, titleText, strlen(titleText));
//    (void)Unicode::snprintf(titleBuffer, TITLE_SIZE, "%s", titleBuffer);
    (void)Unicode::strncpy(messageBuffer, msgText, strlen(msgText));
//    (void)Unicode::snprintf(messageBuffer, MESSAGE_SIZE, "%s", messageBuffer);

    modal.show();
    title.invalidate();
    message.invalidate();
    modal.invalidate();
}

void modalMessage::buttonOkCallbackHandler(const AbstractButton &src) {
    modal.hide();
    modal.invalidate();
}

void modalMessage::buttonCancelCallbackHandler(const AbstractButton &src) {
    modal.hide();
    modal.invalidate();
}

