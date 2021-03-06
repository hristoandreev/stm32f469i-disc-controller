/* DO NOT EDIT THIS FILE */
/* This file is autogenerated by the text-database code generator */

#ifndef APPLICATIONFONTPROVIDER_HPP
#define APPLICATIONFONTPROVIDER_HPP

#include <touchgfx/FontManager.hpp>

namespace touchgfx
{
    class FlashDataReader;
}

struct Typography
{
    static const touchgfx::FontId DEFAULT = 0;
    static const touchgfx::FontId LARGE = 1;
    static const touchgfx::FontId SMALL = 2;
    static const touchgfx::FontId APS_SSID = 3;
    static const touchgfx::FontId APS_RSSI = 4;
    static const touchgfx::FontId CLOCK = 5;
    static const touchgfx::FontId MODALWINDOWTITLE = 6;
    static const touchgfx::FontId APP_NAME = 7;
    static const touchgfx::FontId MODE = 8;
    static const touchgfx::FontId KEYBOARD = 9;
    static const touchgfx::FontId DISPLAY = 10;
    static const touchgfx::FontId EDITTEXT = 11;
};

struct TypographyFontIndex
{
    static const touchgfx::FontId DEFAULT = 0;          // verdana_20_4bpp
    static const touchgfx::FontId LARGE = 1;            // verdana_40_4bpp
    static const touchgfx::FontId SMALL = 2;            // verdana_10_4bpp
    static const touchgfx::FontId APS_SSID = 3;         // verdana_30_4bpp
    static const touchgfx::FontId APS_RSSI = 0;         // verdana_20_4bpp
    static const touchgfx::FontId CLOCK = 4;            // verdanab_20_4bpp
    static const touchgfx::FontId MODALWINDOWTITLE = 0; // verdana_20_4bpp
    static const touchgfx::FontId APP_NAME = 5;         // verdanab_15_4bpp
    static const touchgfx::FontId MODE = 6;             // verdanab_40_4bpp
    static const touchgfx::FontId KEYBOARD = 6;         // verdanab_40_4bpp
    static const touchgfx::FontId DISPLAY = 7;          // verdanab_60_4bpp
    static const touchgfx::FontId EDITTEXT = 3;         // verdana_30_4bpp
    static const uint16_t NUMBER_OF_FONTS = 8;
};

class ApplicationFontProvider : public touchgfx::FontProvider
{
public:
    virtual touchgfx::Font* getFont(touchgfx::FontId typography);

    static void setFlashReader(touchgfx::FlashDataReader* /* flashReader */) { }
    static touchgfx::FlashDataReader* getFlashReader() { return 0; }
};

#endif // APPLICATIONFONTPROVIDER_HPP
