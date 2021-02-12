#ifndef LONGEDITCONTAINER_HPP
#define LONGEDITCONTAINER_HPP

#include <gui_generated/containers/longEditContainerBase.hpp>

class longEditContainer : public longEditContainerBase
{
public:
    longEditContainer();
    virtual ~longEditContainer() {}

    virtual void initialize();
    void setEditTouchHandler(GenericCallback<const AbstractButtonContainer&>& callback);
    void setText(Unicode::UnicodeChar *text);
    uint16_t getText(Unicode::UnicodeChar *text);
protected:
};

#endif // LONGEDITCONTAINER_HPP
