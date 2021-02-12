#include <gui/containers/longEditContainer.hpp>

longEditContainer::longEditContainer()
{

}

void longEditContainer::initialize()
{
    longEditContainerBase::initialize();
}

void longEditContainer::setEditTouchHandler(GenericCallback<const AbstractButtonContainer&>& callback) {
    editTextTouch.setAction(callback);
}

void longEditContainer::setText(Unicode::UnicodeChar *text) {
    uint16_t len = Unicode::strlen(text);
    (void)Unicode::strncpy(editTextBuffer, text, len + 1);
}

uint16_t longEditContainer::getText(Unicode::UnicodeChar *text) {
    uint16_t len = Unicode::strlen(editTextBuffer);
    if(len == 0) {
        text[0] = 0;
        return 0;
    }
    (void)Unicode::strncpy(text, editTextBuffer, len + 1);
    return len;
}