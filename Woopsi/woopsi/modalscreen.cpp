#include "modalscreen.h"

ModalScreen::ModalScreen(char* title, FontBase* font) : Screen(title, font) { }

void ModalScreen::draw(Rect clipRect) {
	_flags.erased = false;
	erase();
}
