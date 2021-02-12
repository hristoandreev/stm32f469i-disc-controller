#include <gui/common/CustomKeyboard.hpp>
#include <touchgfx/Screen.hpp>

//extern Unicode::UnicodeChar keyboardBuffer[2][18];
//extern uint8_t keyboardSelection;
//extern Screen *currScreen;

CustomKeyboard::CustomKeyboard() : keyboard(),
    modeBtnTextArea(),
    capslockPressed(this, &CustomKeyboard::capslockPressedHandler),
    backspacePressed(this, &CustomKeyboard::backspacePressedHandler),
    modePressed(this, &CustomKeyboard::modePressedHandler),
    keyPressed(this, &CustomKeyboard::keyPressedhandler),
    settingsPressed(this, &CustomKeyboard::settingsPressedHandler),
//    enterPressed(this, &CustomKeyboard::enterPressedHandler),
//    cancelPressed(this, &CustomKeyboard::cancelPressedHandler),
    alphaKeys(true),
    uppercaseKeys(false),
    firstCharacterEntry(false)
{
    //Set the callbacks for the callback areas of the keyboard and set its layout.
    layout.callbackAreaArray[0].callback = &capslockPressed;
    layout.callbackAreaArray[1].callback = &backspacePressed;
    layout.callbackAreaArray[2].callback = &modePressed;
    layout.callbackAreaArray[3].callback = &settingsPressed;
    layout.callbackAreaArray[4].callback = nullptr;
    layout.callbackAreaArray[5].callback = nullptr;
    keyboard.setLayout(&layout);
    keyboard.setKeyListener(keyPressed);
    keyboard.setPosition(0, 0, 800, 480);
    keyboard.setTextIndentation();
    //Allocate the buffer associated with keyboard.
    memset(buffer, 0, sizeof(buffer));
    keyboard.setBuffer(buffer, BUFFER_SIZE);

    uppercaseKeys = true;
    firstCharacterEntry = true;

    modeBtnTextArea.setPosition(5, 100 + 79 * 4, 96, 71);
    modeBtnTextArea.setColor(Color::getColorFrom24BitRGB(0xFFU, 0xFFU, 0xFFU));

    setKeyMappingList();

    add(keyboard);
    add(modeBtnTextArea);
}

void CustomKeyboard::setKeyMappingList()
{
    if (alphaKeys)
    {
        modeBtnTextArea.setTypedText(TypedText(T_ALPHAMODE));
        if (uppercaseKeys)
        {
            keyboard.setKeymappingList(&keyMappingListAlphaUpper);
        }
        else
        {
            keyboard.setKeymappingList(&keyMappingListAlphaLower);
        }
    }
    else
    {
        modeBtnTextArea.setTypedText(TypedText(T_NUMMODE));
        if (uppercaseKeys)
        {
            keyboard.setKeymappingList(&keyMappingListNumUpper);
        }
        else
        {
            keyboard.setKeymappingList(&keyMappingListNumLower);
        }
    }
}

void CustomKeyboard::backspacePressedHandler()
{
    uint16_t pos = keyboard.getBufferPosition();
    if (pos > 0)
    {
        //Delete the previous entry in the buffer and decrement the position.
        buffer[pos - 1] = 0;
        keyboard.setBufferPosition(pos - 1);

        //Change keymappings if we have reached the first position.
        if (1 == pos)
        {
            firstCharacterEntry = true;
            uppercaseKeys = true;
            setKeyMappingList();
        }
    }
}

void CustomKeyboard::capslockPressedHandler()
{
    uppercaseKeys = !uppercaseKeys;
    setKeyMappingList();
}

void CustomKeyboard::modePressedHandler()
{
    alphaKeys = !alphaKeys;

    // if we have changed back to alpha and still has no chars in the buffer,
    // we show upper case letters.
    if (firstCharacterEntry && alphaKeys)
    {
        uppercaseKeys = true;
    }
    else
    {
        uppercaseKeys = false;
    }
    setKeyMappingList();
}

void CustomKeyboard::keyPressedhandler(Unicode::UnicodeChar keyChar)
{
    // After the first keypress, the keyboard will shift to lowercase.
    if (firstCharacterEntry && keyChar != 0)
    {
        firstCharacterEntry = false;
        uppercaseKeys = false;
        setKeyMappingList();
    }
}

void CustomKeyboard::setTouchable(bool touch)
{
    Container::setTouchable(touch);
    keyboard.setTouchable(touch);
}

Unicode::UnicodeChar *CustomKeyboard::getBuffer()
{
    return keyboard.getBuffer();
}

void CustomKeyboard::settingsPressedHandler()
{

}

void CustomKeyboard::enterPressedHandler()
{
//    Unicode::UnicodeChar *buff = keyboard.getBuffer();
//    (void)Unicode::strncpy(keyboardBuffer[keyboardSelection], buff, Unicode::strlen(buff) + 1);
}

void CustomKeyboard::cancelPressedHandler()
{

}

void CustomKeyboard::setEnterAction(GenericCallback<> *callback_p) {
    layout.callbackAreaArray[4].callback = callback_p;
}

void CustomKeyboard::setCancelAction(GenericCallback<> *callback_p) {
    layout.callbackAreaArray[5].callback = callback_p;
}

void CustomKeyboard::setBufferPosition(uint16_t newPos) {
    keyboard.setBufferPosition(newPos);
}
