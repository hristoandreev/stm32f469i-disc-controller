/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <touchgfx/Color.hpp>
#include "BitmapDatabase.hpp"
#include <texts/TextKeysAndLanguages.hpp>

Screen1ViewBase::Screen1ViewBase() :
    updateItemCallback(this, &Screen1ViewBase::updateItemCallbackHandler)
{

    touchgfx::CanvasWidgetRenderer::setupBuffer(canvasBuffer, CANVAS_BUFFER_SIZE);

    __background.setPosition(0, 0, 800, 480);
    __background.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));

    bkgImage.setBitmap(touchgfx::Bitmap(BITMAP_D3_MT8100IE_AC_SYSTEM_ID));
    bkgImage.setPosition(0, 0, 800, 480);
    bkgImage.setOffset(0, 0);

    wifiSlideMenu.setup(touchgfx::SlideMenu::SOUTH,
        touchgfx::Bitmap(BITMAP_D3_MT8100IE_AC_SYSTEM_ID),
        touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_LARGE_ID),
        touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_LARGE_PRESSED_ID),
        0, 0, 213, 420);
    wifiSlideMenu.setState(touchgfx::SlideMenu::EXPANDED);
    wifiSlideMenu.setVisiblePixelsWhenCollapsed(40);
    wifiSlideMenu.setHiddenPixelsWhenExpanded(0);
    wifiSlideMenu.setAnimationEasingEquation(touchgfx::EasingEquations::cubicEaseInOut);
    wifiSlideMenu.setAnimationDuration(18);
    wifiSlideMenu.setExpandedStateTimeout(0);
    wifiSlideMenu.setXY(0, 0);

    wifiTitleBox.setPosition(0, 0, 800, 55);
    wifiTitleBox.setColor(touchgfx::Color::getColorFrom24BitRGB(166, 143, 0));
    wifiTitleBox.setBorderColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    wifiTitleBox.setBorderSize(2);
    wifiSlideMenu.add(wifiTitleBox);

    systemTitleBox.setPosition(0, 440, 800, 40);
    systemTitleBox.setColor(touchgfx::Color::getColorFrom24BitRGB(166, 143, 0));
    systemTitleBox.setBorderColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    systemTitleBox.setBorderSize(1);
    wifiSlideMenu.add(systemTitleBox);

    wifiTitleText.setXY(22, 3);
    wifiTitleText.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    wifiTitleText.setLinespacing(0);
    wifiTitleText.setTypedText(touchgfx::TypedText(T_SINGLEUSEID2));
    wifiSlideMenu.add(wifiTitleText);

    wifiScanningProgress.setXY(713, 0);
    wifiScanningProgress.setProgressIndicatorPosition(0, 0, 54, 54);
    wifiScanningProgress.setRange(0, 100);
    wifiScanningProgress.setCenter(27, 27);
    wifiScanningProgress.setRadius(20);
    wifiScanningProgress.setLineWidth(10);
    wifiScanningProgress.setStartEndAngle(0, 360);
    wifiScanningProgress.setCapPrecision(180);
    wifiScanningProgress.setBackground(touchgfx::Bitmap(BITMAP_BLUE_PROGRESSINDICATORS_BG_SMALL_CIRCLE_INDICATOR_BG_LINE_FULL_ID));
    wifiScanningProgressPainter.setBitmap(touchgfx::Bitmap(BITMAP_BLUE_PROGRESSINDICATORS_FILL_SMALL_CIRCLE_INDICATOR_FILL_LINE_FULL_ID));
    wifiScanningProgress.setPainter(wifiScanningProgressPainter);
    wifiScanningProgress.setValue(60);
    wifiSlideMenu.add(wifiScanningProgress);

    wifiListBkg.setPosition(0, 55, 800, 385);
    wifiListBkg.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    wifiListBkg.setBorderColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    wifiListBkg.setBorderSize(1);
    wifiSlideMenu.add(wifiListBkg);

    wifiScrollList.setPosition(0, 55, 800, 385);
    wifiScrollList.setHorizontal(false);
    wifiScrollList.setCircular(false);
    wifiScrollList.setEasingEquation(touchgfx::EasingEquations::backEaseOut);
    wifiScrollList.setSwipeAcceleration(10);
    wifiScrollList.setDragAcceleration(10);
    wifiScrollList.setNumberOfItems(0);
    wifiScrollList.setPadding(0, 0);
    wifiScrollList.setSnapping(true);
    wifiScrollList.setDrawableSize(80, 0);
    wifiScrollList.setDrawables(wifiScrollListListItems, updateItemCallback);
    wifiSlideMenu.add(wifiScrollList);

    digitalClock.setPosition(719, 448, 66, 25);
    digitalClock.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    digitalClock.setTypedText(touchgfx::TypedText(T_DIGITAL_CLOCK));
    digitalClock.displayLeadingZeroForHourIndicator(true);
    digitalClock.setDisplayMode(touchgfx::DigitalClock::DISPLAY_24_HOUR_NO_SECONDS);
    digitalClock.setTime24Hour(23, 40, 0);
    wifiSlideMenu.add(digitalClock);

    wifiStrengthIcon.setXY(672, 444);
    wifiStrengthIcon.setBitmap(touchgfx::Bitmap(BITMAP_WIFI1_32X32_ID));
    wifiSlideMenu.add(wifiStrengthIcon);

    connectAPModalWindow.setBackground(touchgfx::BitmapId(BITMAP_MODALWINDOWLBKG_ID), 160, 80);
    connectAPModalWindow.setShadeColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    connectAPModalWindow.setShadeAlpha(150);
    connectAPModalWindow.hide();

    buttonCancel.setXY(50, 243);
    buttonCancel.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_PRESSED_ID));
    buttonCancel.setLabelText(touchgfx::TypedText(T_SINGLEUSEID9));
    buttonCancel.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    buttonCancel.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    connectAPModalWindow.add(buttonCancel);

    buttonOk.setXY(260, 243);
    buttonOk.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_ID), touchgfx::Bitmap(BITMAP_BLUE_BUTTONS_ROUND_EDGE_SMALL_PRESSED_ID));
    buttonOk.setLabelText(touchgfx::TypedText(T_SINGLEUSEID10));
    buttonOk.setLabelColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    buttonOk.setLabelColorPressed(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    connectAPModalWindow.add(buttonOk);

    title.setXY(35, 13);
    title.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    title.setLinespacing(0);
    titleBuffer[0] = 0;
    title.setWildcard(titleBuffer);
    title.resizeToCurrentText();
    title.setTypedText(touchgfx::TypedText(T_SINGLEUSEID11));
    connectAPModalWindow.add(title);

    boxWithBorder1.setPosition(24, 96, 433, 50);
    boxWithBorder1.setColor(touchgfx::Color::getColorFrom24BitRGB(255, 255, 255));
    boxWithBorder1.setBorderColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    boxWithBorder1.setBorderSize(5);
    connectAPModalWindow.add(boxWithBorder1);

    password.setPosition(36, 96, 410, 50);
    password.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    password.setLinespacing(0);
    passwordBuffer[0] = 0;
    password.setWildcard(passwordBuffer);
    password.setTypedText(touchgfx::TypedText(T_SINGLEUSEID13));
    connectAPModalWindow.add(password);

    textArea1.setXY(35, 65);
    textArea1.setColor(touchgfx::Color::getColorFrom24BitRGB(0, 0, 0));
    textArea1.setLinespacing(0);
    textArea1.setTypedText(touchgfx::TypedText(T_SINGLEUSEID12));
    connectAPModalWindow.add(textArea1);

    add(__background);
    add(bkgImage);
    add(wifiSlideMenu);
    add(connectAPModalWindow);
}

void Screen1ViewBase::setupScreen()
{
    wifiScrollList.initialize();
    for (int i = 0; i < wifiScrollListListItems.getNumberOfDrawables(); i++)
    {
        wifiScrollListListItems[i].initialize();
    }
}

void Screen1ViewBase::updateItemCallbackHandler(touchgfx::DrawableListItemsInterface* items, int16_t containerIndex, int16_t itemIndex)
{
    if (items == &wifiScrollListListItems)
    {
        touchgfx::Drawable* d = items->getDrawable(containerIndex);
        wifiItemContainer* cc = (wifiItemContainer*)d;
        wifiScrollListUpdateItem(*cc, itemIndex);
    }
}
