#include <gui/containers/shortEditContainer.hpp>

shortEditContainer::shortEditContainer()
{

}

void shortEditContainer::initialize()
{
    shortEditContainerBase::initialize();
}

void shortEditContainer::setEditTouchHandler(GenericCallback<const AbstractButtonContainer &> &callback) {
    editTopicTouch.setAction(callback);
}

void shortEditContainer::setText(Unicode::UnicodeChar *text) {
    uint16_t len = Unicode::strlen(text);
    (void)Unicode::strncpy(editTopicBuffer, text, len + 1);
}

uint16_t shortEditContainer::getText(Unicode::UnicodeChar *text) {
    uint16_t len = Unicode::strlen(editTopicBuffer);
    if(len == 0) {
        text[0] = 0;
        return 0;
    }
    (void)Unicode::strncpy(text, editTopicBuffer, len + 1);
    return len;
}