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

modalMessage::modalMessage(uint16_t bmpBackgroundID, uint16_t bmpButtonReleasedID, uint16_t bmpButtonPressedID) :
        buttonOkCallback(this, &modalMessage::buttonOkCallbackHandler),
        buttonCancelCallback(this, &modalMessage::buttonCancelCallbackHandler),
        buttonYesCallback(this, &modalMessage::buttonYesCallbackHandler),
        buttonNoCallback(this, &modalMessage::buttonNoCallbackHandler),
        callback(nullptr),
        BITMAP_BACKGROUND_ID(bmpBackgroundID),
        BITMAP_BUTTONS_RELEASED_ID(bmpButtonReleasedID),
        BITMAP_BUTTONS_PRESSED_ID(bmpButtonPressedID)
{
    displayWidth = HAL::getInstance()->getDisplayWidth();
    displayHeight = HAL::getInstance()->getDisplayHeight();
//    modal.setBackground(touchgfx::BitmapId(BITMAP_MODALWINDOWLBKG_ID));
    modal.setBackground(touchgfx::BitmapId(BITMAP_BACKGROUND_ID));
    modalWidth = modal.getBackgroundWidth();
    modalHeight = modal.getBackgroundHeight();
    backgroundX = (displayWidth - modalWidth) / 2;
    backgroundY = (displayHeight - modalHeight) / 2;
//    modal.setBackground(touchgfx::BitmapId(BITMAP_MODALWINDOWLBKG_ID), 160, 80);
    modal.setBackground(touchgfx::BitmapId(BITMAP_BACKGROUND_ID), backgroundX, backgroundY);
    modal.setShadeColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    modal.setShadeAlpha(150);
    modal.hide();

    title.setXY(35, 13);
    title.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    title.setLinespacing(0);
    title.resizeToCurrentText();
    modal.add(title);

    message.setXY(24, 96);
    message.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    message.setLinespacing(0);
    message.resizeToCurrentText();
    modal.add(message);

    add(modal);
}

/**
 * _________________________________________
 * |  Title                                |
 * |_______________________________________|
 * |                                       |
 * |                                       |
 * |               Message                 |
 * |                                       |
 * |    _____________     _____________    |
 * |   |   Cancel   |    |     OK     |    |
 * |   |____________|    |____________|    |
 * |_______________________________________|
 * @param msgTextId
 * @param titleTextId
 * @param okBtnTextId
 * @param cancelBtnTextId
 * @param callback_p
 */
void modalMessage::showOkCancel(
        touchgfx::TypedTextId msgTextId,
        touchgfx::TypedTextId titleTextId,
        touchgfx::TypedTextId okBtnTextId,
        touchgfx::TypedTextId cancelBtnTextId,
        touchgfx::GenericCallback<buttonSource> *callback_p) {
    buttonCancel.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTONS_RELEASED_ID),
                            touchgfx::Bitmap(BITMAP_BUTTONS_PRESSED_ID));
    int16_t cancelHeight = buttonCancel.getHeight();
    int16_t cancelWidth = buttonCancel.getWidth();
    int16_t cx = (modalWidth / 4) - (cancelWidth / 2);
    int16_t cy = modalHeight - ((modalHeight / 3) - (cancelHeight / 2));
//    buttonCancel.setXY(50, 243);
    buttonCancel.setXY(cx, cy);
    buttonCancel.setLabelText(touchgfx::TypedText(cancelBtnTextId));
    buttonCancel.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    buttonCancel.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    buttonCancel.setAction(buttonCancelCallback);
    modal.add(buttonCancel);

    buttonOk.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTONS_RELEASED_ID),
                        touchgfx::Bitmap(BITMAP_BUTTONS_PRESSED_ID));
    int16_t okHeight = buttonOk.getHeight();
    int16_t okWidth = buttonOk.getWidth();
    int16_t ox = modalWidth - (modalWidth / 4) - (okWidth / 2);
    int16_t oy = modalHeight - ((modalHeight / 3) - (okHeight / 2));
//    buttonOk.setXY(260, 243);
    buttonOk.setXY(ox, oy);
    buttonOk.setLabelText(touchgfx::TypedText(okBtnTextId));
    buttonOk.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    buttonOk.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    buttonOk.setAction(buttonOkCallback);
    modal.add(buttonOk);

    callback = callback_p;

    show(msgTextId, titleTextId);
}

/**
 * _________________________________________
 * |  Title                                |
 * |_______________________________________|
 * |                                       |
 * |                                       |
 * |               Message                 |
 * |                                       |
 * |            _____________              |
 * |           |     OK     |              |
 * |           |____________|              |
 * |_______________________________________|
 * @param msgTextId
 * @param titleTextId
 * @param okBtnTextId
 * @param callback_p
 */
void modalMessage::showOk(
        touchgfx::TypedTextId msgTextId,
        touchgfx::TypedTextId titleTextId,
        touchgfx::TypedTextId okBtnTextId,
        touchgfx::GenericCallback<buttonSource> *callback_p) {
    buttonOk.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTONS_RELEASED_ID),
                        touchgfx::Bitmap(BITMAP_BUTTONS_PRESSED_ID));
    int16_t okHeight = buttonOk.getHeight();
    int16_t okWidth = buttonOk.getWidth();
    int16_t x = (modalWidth / 2) - (okWidth / 2);
    int16_t y = modalHeight - ((modalHeight / 3) - (okHeight / 2));
//    buttonOk.setXY(160, 243);
    buttonOk.setXY(x, y);
    buttonOk.setLabelText(touchgfx::TypedText(okBtnTextId));
    buttonOk.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    buttonOk.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    buttonOk.setAction(buttonOkCallback);
    modal.add(buttonOk);

    callback = callback_p;

    show(msgTextId, titleTextId);
}
/**
 * _________________________________________
 * |  Title                                |
 * |_______________________________________|
 * |                                       |
 * |                                       |
 * |               Message                 |
 * |                                       |
 * |    _____________     _____________    |
 * |   |     No     |    |     Yes    |    |
 * |   |____________|    |____________|    |
 * |_______________________________________|
 * @param msgTextId
 * @param titleTextId
 * @param yesBtnTextId
 * @param noBtnTextId
 * @param callback_p
 */
void modalMessage::showYesNo(
        touchgfx::TypedTextId msgTextId,
        touchgfx::TypedTextId titleTextId,
        touchgfx::TypedTextId yesBtnTextId,
        touchgfx::TypedTextId noBtnTextId,
        touchgfx::GenericCallback<buttonSource> *callback_p) {
    buttonNo.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTONS_RELEASED_ID),
                        touchgfx::Bitmap(BITMAP_BUTTONS_PRESSED_ID));
    int16_t noHeight = buttonCancel.getHeight();
    int16_t noWidth = buttonCancel.getWidth();
    int16_t nx = (modalWidth / 4) - (noWidth / 2);
    int16_t ny = modalHeight - ((modalHeight / 3) - (noHeight / 2));
//    buttonNo.setXY(50, 243);
    buttonNo.setXY(nx, ny);
    buttonNo.setLabelText(touchgfx::TypedText(noBtnTextId));
    buttonNo.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    buttonNo.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    buttonNo.setAction(buttonNoCallback);
    modal.add(buttonNo);

    buttonYes.setBitmaps(touchgfx::Bitmap(BITMAP_BUTTONS_RELEASED_ID),
                         touchgfx::Bitmap(BITMAP_BUTTONS_PRESSED_ID));
    int16_t yesHeight = buttonOk.getHeight();
    int16_t yesWidth = buttonOk.getWidth();
    int16_t yx = modalWidth - (modalWidth / 4) - (yesWidth / 2);
    int16_t yy = modalHeight - ((modalHeight / 3) - (yesHeight / 2));
//    buttonYes.setXY(260, 243);
    buttonYes.setXY(yx, yy);
    buttonYes.setLabelText(touchgfx::TypedText(yesBtnTextId));
    buttonYes.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    buttonYes.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    buttonYes.setAction(buttonYesCallback);
    modal.add(buttonYes);

    callback = callback_p;

    show(msgTextId, titleTextId);
}

/**
 *
 * @param msgTextId
 * @param titleTextId
 * @param yesBtnTextId
 * @param noBtnTextId
 * @param cancelBtnTextId
 * @param callback_p
 */
void modalMessage::showYesNoCancel(touchgfx::TypedTextId msgTextId, touchgfx::TypedTextId titleTextId,
                                   touchgfx::TypedTextId yesBtnTextId, touchgfx::TypedTextId noBtnTextId,
                                   touchgfx::TypedTextId cancelBtnTextId,
                                   touchgfx::GenericCallback<buttonSource> *callback_p) {
    callback = callback_p;
    show(msgTextId, titleTextId);
}

void modalMessage::show(touchgfx::TypedTextId msgTextId, touchgfx::TypedTextId titleTextId) {
    title.setTypedText(touchgfx::TypedText(titleTextId));
    message.setTypedText(touchgfx::TypedText(msgTextId));
    title.resizeToCurrentText();
    message.resizeToCurrentText();
    modal.show();
    modal.invalidate();
}

void modalMessage::buttonOkCallbackHandler(const AbstractButton &src) {
    modal.hide();
    modal.invalidate();
    if(callback)
        callback->execute(srcOK);
}

void modalMessage::buttonCancelCallbackHandler(const AbstractButton &src) {
    modal.hide();
    modal.invalidate();
    if(callback)
        callback->execute(srcCANCEL);
}

void modalMessage::buttonYesCallbackHandler(const AbstractButton &src) {
    modal.hide();
    modal.invalidate();
    if(callback)
        callback->execute(srcYES);
}

void modalMessage::buttonNoCallbackHandler(const AbstractButton &src) {
    modal.hide();
    modal.invalidate();
    if(callback)
        callback->execute(srcNO);
}
