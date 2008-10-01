#include "woopsikeyboard.h"
#include "button.h"

WoopsiKeyboard::WoopsiKeyboard(s16 x, s16 y, u16 width, u16 height, const char* title, FontBase* font) : AmigaWindow(x, y, width, height, title, GADGET_DRAGGABLE, AMIGA_WINDOW_SHOW_DEPTH, font) {

	// Get available window region
	Rect rect;
	getClientRect(rect);

	// Create buttons
	u8 buttonWidth = 19;
	u8 buttonHeight = 20;
	u8 buttonX = rect.x + 4;
	u8 buttonY = rect.y + 1;

	// 1234567890-=
	addGadget(new Button(buttonX, buttonY, buttonWidth, buttonHeight, "1"));
	addGadget(new Button(buttonX + (1 + buttonWidth), buttonY, buttonWidth, buttonHeight, "2"));
	addGadget(new Button(buttonX + (2 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "3"));
	addGadget(new Button(buttonX + (3 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "4"));
	addGadget(new Button(buttonX + (4 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "5"));
	addGadget(new Button(buttonX + (5 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "6"));
	addGadget(new Button(buttonX + (6 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "7"));
	addGadget(new Button(buttonX + (7 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "8"));
	addGadget(new Button(buttonX + (8 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "9"));
	addGadget(new Button(buttonX + (9 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "0"));
	addGadget(new Button(buttonX + (10 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "-"));
	addGadget(new Button(buttonX + (11 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "="));
	
	buttonY += buttonHeight + 1;

	// QWERTYUIOP Space
	addGadget(new Button(buttonX + (buttonWidth / 2), buttonY, buttonWidth, buttonHeight, "Q"));
	addGadget(new Button(buttonX + (buttonWidth / 2) + (1 + buttonWidth), buttonY, buttonWidth, buttonHeight, "W"));
	addGadget(new Button(buttonX + (buttonWidth / 2) + (2 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "E"));
	addGadget(new Button(buttonX + (buttonWidth / 2) + (3 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "R"));
	addGadget(new Button(buttonX + (buttonWidth / 2) + (4 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "T"));
	addGadget(new Button(buttonX + (buttonWidth / 2) + (5 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "Y"));
	addGadget(new Button(buttonX + (buttonWidth / 2) + (6 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "U"));
	addGadget(new Button(buttonX + (buttonWidth / 2) + (7 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "I"));
	addGadget(new Button(buttonX + (buttonWidth / 2) + (8 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "O"));
	addGadget(new Button(buttonX + (buttonWidth / 2) + (9 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "P"));
	addGadget(new Button(buttonX + (buttonWidth / 2) + (10 * (1 + buttonWidth)), buttonY, 1 + (buttonWidth * 2) - (buttonWidth / 2), buttonHeight, GLYPH_BACKSPACE));

	buttonY += buttonHeight + 1;

	// Caps ASDFGHJKL Return
	addGadget(new Button(buttonX, buttonY, buttonWidth, buttonHeight, "Caps"));
	addGadget(new Button(buttonX + (1 + buttonWidth), buttonY, buttonWidth, buttonHeight, "A"));
	addGadget(new Button(buttonX + (2 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "S"));
	addGadget(new Button(buttonX + (3 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "D"));
	addGadget(new Button(buttonX + (4 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "F"));
	addGadget(new Button(buttonX + (5 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "G"));
	addGadget(new Button(buttonX + (6 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "H"));
	addGadget(new Button(buttonX + (7 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "J"));
	addGadget(new Button(buttonX + (8 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "K"));
	addGadget(new Button(buttonX + (9 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "L"));
	addGadget(new Button(buttonX + (10 * (1 + buttonWidth)), buttonY, 1 + (buttonWidth * 2), buttonHeight, GLYPH_RETURN));

	buttonY += buttonHeight + 1;

	// Shift ZXCVBNM,./
	addGadget(new Button(buttonX, buttonY, (buttonWidth * 2) - (buttonWidth / 2) - 1, buttonHeight, GLYPH_SHIFT));
	addGadget(new Button(buttonX + (buttonWidth / 2) + (1 + buttonWidth), buttonY, buttonWidth, buttonHeight, "Z"));
	addGadget(new Button(buttonX + (buttonWidth / 2) + (2 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "X"));
	addGadget(new Button(buttonX + (buttonWidth / 2) + (3 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "C"));
	addGadget(new Button(buttonX + (buttonWidth / 2) + (4 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "V"));
	addGadget(new Button(buttonX + (buttonWidth / 2) + (5 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "B"));
	addGadget(new Button(buttonX + (buttonWidth / 2) + (6 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "N"));
	addGadget(new Button(buttonX + (buttonWidth / 2) + (7 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "M"));
	addGadget(new Button(buttonX + (buttonWidth / 2) + (8 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, ","));
	addGadget(new Button(buttonX + (buttonWidth / 2) + (9 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "."));
	addGadget(new Button(buttonX + (buttonWidth / 2) + (10 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "/"));

	buttonY += buttonHeight + 1;

	// ;'#
	addGadget(new Button(buttonX + (1 + buttonWidth), buttonY, buttonWidth, buttonHeight, ";"));
	addGadget(new Button(buttonX + (2 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "'"));
	addGadget(new Button(buttonX + (3 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "#"));
	addGadget(new Button(buttonX + (4 * (1 + buttonWidth)), buttonY, 4 + (buttonWidth * 5), buttonHeight, "Space"));
	addGadget(new Button(buttonX + (9 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "["));
	addGadget(new Button(buttonX + (10 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "]"));
	addGadget(new Button(buttonX + (11 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "\\"));

	// Set event handlers
	for (u8 i = 0; i < _gadgets.size(); i++) {
		_gadgets[i]->setEventHandler(this);
	}
}

bool WoopsiKeyboard::handleEvent(const EventArgs& e) {
	// Only handle release events
	if (e.type == EVENT_RELEASE) {
		if (e.gadget != NULL) {
			//if (e.gadget == _button) {
			//	close();
			//	return true;
			//}
		}
	}

	// Handle other window events
	return AmigaWindow::handleEvent(e);
}
