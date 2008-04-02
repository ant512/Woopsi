#include "button.h"

Button::Button(s16 x, s16 y, u16 width, u16 height, char* text, FontBase* font) : Textbox(x, y, width, height, text, font) {
	_outline = OUTLINE_CLICK_DEPENDENT;
}

Button::Button(s16 x, s16 y, u16 width, u16 height, char letter, FontBase* font) : Textbox(x, y, width, height, letter, font) {
	_outline = OUTLINE_CLICK_DEPENDENT;
}
