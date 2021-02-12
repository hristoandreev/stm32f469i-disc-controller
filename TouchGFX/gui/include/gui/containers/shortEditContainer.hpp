#ifndef SHORTEDITCONTAINER_HPP
#define SHORTEDITCONTAINER_HPP

#include <gui_generated/containers/shortEditContainerBase.hpp>

class shortEditContainer : public shortEditContainerBase
{
public:
    shortEditContainer();
    virtual ~shortEditContainer() {}

    virtual void initialize();
    void setEditTouchHandler(GenericCallback<const AbstractButtonContainer&>& callback);
    void setText(Unicode::UnicodeChar *text);
    uint16_t getText(Unicode::UnicodeChar *text);
protected:
};

#endif // SHORTEDITCONTAINER_HPP
