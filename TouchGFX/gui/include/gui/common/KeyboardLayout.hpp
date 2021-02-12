#ifndef KEYBOARD_LAYOUT
#define KEYBOARD_LAYOUT

#include <touchgfx/widgets/Keyboard.hpp>
#include <touchgfx/hal/Types.hpp>
#include <fonts/ApplicationFontProvider.hpp>
#include "BitmapDatabase.hpp"

using namespace touchgfx;

/**
 * Array specifying the keys used in the CustomKeyboard.
 */
static const Keyboard::Key keyArray[41] =
{
    { 1, Rect(4,               104,          73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 2, Rect(4 + 80,          104,          73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 3, Rect(4 + 80 * 2,      104,          73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 4, Rect(4 + 80 * 3,      104,          73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 5, Rect(4 + 80 * 4,      104,          73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 6, Rect(4 + 80 * 5,      104,          73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 7, Rect(4 + 80 * 6,      104,          73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 8, Rect(4 + 80 * 7,      104,          73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    { 9, Rect(4 + 80 * 8,      104,          73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {10, Rect(4 + 80 * 9,      104,          73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},

    {11, Rect(4,               104 + 76,     73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {12, Rect(4 + 80,          104 + 76,     73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {13, Rect(4 + 80 * 2,      104 + 76,     73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {14, Rect(4 + 80 * 3,      104 + 76,     73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {15, Rect(4 + 80 * 4,      104 + 76,     73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {16, Rect(4 + 80 * 5,      104 + 76,     73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {17, Rect(4 + 80 * 6,      104 + 76,     73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {18, Rect(4 + 80 * 7,      104 + 76,     73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {19, Rect(4 + 80 * 8,      104 + 76,     73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {20, Rect(4 + 80 * 9,      104 + 76,     73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},

    {21, Rect(4 + 40,          104 + 76 * 2, 73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {22, Rect(4 + 40 + 80,     104 + 76 * 2, 73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {23, Rect(4 + 40 + 80 * 2, 104 + 76 * 2, 73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {24, Rect(4 + 40 + 80 * 3, 104 + 76 * 2, 73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {25, Rect(4 + 40 + 80 * 4, 104 + 76 * 2, 73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {26, Rect(4 + 40 + 80 * 5, 104 + 76 * 2, 73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {27, Rect(4 + 40 + 80 * 6, 104 + 76 * 2, 73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {28, Rect(4 + 40 + 80 * 7, 104 + 76 * 2, 73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {29, Rect(4 + 40 + 80 * 8, 104 + 76 * 2, 73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},

    {30, Rect(4 + 40 + 80,     104 + 76 * 3, 73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {31, Rect(4 + 40 + 80 * 2, 104 + 76 * 3, 73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {32, Rect(4 + 40 + 80 * 3, 104 + 76 * 3, 73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {33, Rect(4 + 40 + 80 * 4, 104 + 76 * 3, 73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {34, Rect(4 + 40 + 80 * 5, 104 + 76 * 3, 73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {35, Rect(4 + 40 + 80 * 6, 104 + 76 * 3, 73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},
    {36, Rect(4 + 40 + 80 * 7, 104 + 76 * 3, 73, 70), BITMAP_KEYBOARD_KEY_HIGHLIGHTED_ID},

    {37, Rect(41 + 73 * 2, 104 + 76 * 4, 66, 71), BITMAP_KEYBOARD_KEY_PUNCTUATION_HIGHLIGHTED_ID},
    {38, Rect(40 + 73 * 3, 104 + 76 * 4, 66, 71), BITMAP_KEYBOARD_KEY_PUNCTUATION_HIGHLIGHTED_ID},
    {39, Rect(15 + 80 * 4, 104 + 76 * 4, 132, 70), BITMAP_KEYBOARD_SPACE_HIGHLIGHTED_ID},
    {40, Rect(37 + 73 * 6, 104 + 76 * 4, 66, 71), BITMAP_KEYBOARD_KEY_PUNCTUATION_HIGHLIGHTED_ID},
    {41, Rect(36 + 73 * 7, 104 + 76 * 4, 66, 71), BITMAP_KEYBOARD_KEY_PUNCTUATION_HIGHLIGHTED_ID},
//    {31, Rect(25 + 40 + 80 * 2, 115 + 80 * 3, 252, 40), BITMAP_KEYBOARD_SPACE_HIGHLIGHTED_ID},
//    {32, Rect(25 + 40 + 80 * 3, 115 + 80 * 3, 252, 40), BITMAP_KEYBOARD_SPACE_HIGHLIGHTED_ID},
//    {33, Rect(25 + 40 + 80 * 4, 115 + 80 * 3, 252, 40), BITMAP_KEYBOARD_SPACE_HIGHLIGHTED_ID},
//    {34, Rect(25 + 40 + 80 * 5, 115 + 80 * 3, 252, 40), BITMAP_KEYBOARD_SPACE_HIGHLIGHTED_ID},
//    {35, Rect(25 + 40 + 80 * 6, 115 + 80 * 3, 252, 40), BITMAP_KEYBOARD_SPACE_HIGHLIGHTED_ID},
};

/**
 * Callback areas for the special buttons on the CustomKeyboard.
 */
static Keyboard::CallbackArea callbackAreas[6] =
{
    {Rect(4,           104 + 76 * 3, 96, 71), 0, BITMAP_KEYBOARD_KEY_SHIFT_HIGHLIGHTED_ID},     // caps-lock
    {Rect(700,         104 + 76 * 3, 96, 71), 0, BITMAP_KEYBOARD_KEY_DELETE_HIGHLIGHTED_ID},    // backspace
    {Rect(4,           104 + 76 * 4, 96, 71), 0, BITMAP_KEYBOARD_KEY_NUM_HIGHLIGHTED_ID},       // mode
    {Rect(34 + 73 * 1, 104 + 76 * 4, 74, 71), 0, BITMAP_KEYBOARD_KEY_SETTINGS_HIGHLIGHTED_ID},  // settings
    {Rect(35 + 73 * 8, 104 + 76 * 4, 74, 71), 0, BITMAP_KEYBOARD_KEY_ENTER_HIGHLIGHTED_ID},     // enter
    {Rect(43 + 73 * 9, 104 + 76 * 4, 96, 71), 0, BITMAP_KEYBOARD_KEY_CANCEL_HIGHLIGHTED_ID},    // keyboard hide
};

/**
 * The layout for the CustomKeyboard.
 */
static const Keyboard::Layout layout =
{
    BITMAP_KEYBOARD_BACKGROUND_ID,
    keyArray,
    41,
    callbackAreas,
    6,
    Rect(30, 12, 11 * 60, 80),
    TypedText(T_ENTEREDTEXT),
#if !defined(USE_BPP) || USE_BPP==16
    0,
#elif USE_BPP==24
    0,
#elif USE_BPP==4
    0,
#elif USE_BPP==2
    0,
#else
#error Unknown USE_BPP
#endif
    Typography::KEYBOARD,
    0xFFFFFFU
};

#endif
