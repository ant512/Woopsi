#include "woopsikeyboard.h"
#include "button.h"

WoopsiKeyboard::WoopsiKeyboard(s16 x, s16 y, u16 width, u16 height, const char* title, FontBase* font) : AmigaWindow(x, y, width, height, title, GADGET_DRAGGABLE, AMIGA_WINDOW_SHOW_DEPTH, font) {

	// Create buttons
	u8 buttonWidth = 20;
	u8 buttonHeight = 20;

	Rect rect;
	getClientRect(rect);

	// Numbers
	addGadget(new Button(rect.x + 1, rect.y + 1, buttonWidth, buttonHeight, "1"));
	addGadget(new Button(rect.x + 1 + (1 + buttonWidth), rect.y + 1, buttonWidth, buttonHeight, "2"));
	addGadget(new Button(rect.x + 1 + (2 * (1 + buttonWidth)), rect.y + 1, buttonWidth, buttonHeight, "3"));
	addGadget(new Button(rect.x + 1 + (3 * (1 + buttonWidth)), rect.y + 1, buttonWidth, buttonHeight, "4"));
	addGadget(new Button(rect.x + 1 + (4 * (1 + buttonWidth)), rect.y + 1, buttonWidth, buttonHeight, "5"));
	addGadget(new Button(rect.x + 1 + (5 * (1 + buttonWidth)), rect.y + 1, buttonWidth, buttonHeight, "6"));
	addGadget(new Button(rect.x + 1 + (6 * (1 + buttonWidth)), rect.y + 1, buttonWidth, buttonHeight, "7"));
	addGadget(new Button(rect.x + 1 + (7 * (1 + buttonWidth)), rect.y + 1, buttonWidth, buttonHeight, "8"));
	addGadget(new Button(rect.x + 1 + (8 * (1 + buttonWidth)), rect.y + 1, buttonWidth, buttonHeight, "9"));
	addGadget(new Button(rect.x + 1 + (9 * (1 + buttonWidth)), rect.y + 1, buttonWidth, buttonHeight, "0"));
	addGadget(new Button(rect.x + 1 + (10 * (1 + buttonWidth)), rect.y + 1, buttonWidth, buttonHeight, "-"));
	addGadget(new Button(rect.x + 1 + (11 * (1 + buttonWidth)), rect.y + 1, buttonWidth, buttonHeight, "+"));
	
	// QWERTYUIOP
	addGadget(new Button(rect.x + 1 + (buttonWidth / 2), rect.y + 1 + (1 + buttonHeight), buttonWidth, buttonHeight, "Q"));
	addGadget(new Button(rect.x + 1 + (buttonWidth / 2) + (1 + buttonWidth), rect.y + 1 + (1 + buttonHeight), buttonWidth, buttonHeight, "W"));
	addGadget(new Button(rect.x + 1 + (buttonWidth / 2) + (2 * (1 + buttonWidth)), rect.y + 1 + (1 + buttonHeight), buttonWidth, buttonHeight, "E"));
	addGadget(new Button(rect.x + 1 + (buttonWidth / 2) + (3 * (1 + buttonWidth)), rect.y + 1 + (1 + buttonHeight), buttonWidth, buttonHeight, "R"));
	addGadget(new Button(rect.x + 1 + (buttonWidth / 2) + (4 * (1 + buttonWidth)), rect.y + 1 + (1 + buttonHeight), buttonWidth, buttonHeight, "T"));
	addGadget(new Button(rect.x + 1 + (buttonWidth / 2) + (5 * (1 + buttonWidth)), rect.y + 1 + (1 + buttonHeight), buttonWidth, buttonHeight, "Y"));
	addGadget(new Button(rect.x + 1 + (buttonWidth / 2) + (6 * (1 + buttonWidth)), rect.y + 1 + (1 + buttonHeight), buttonWidth, buttonHeight, "U"));
	addGadget(new Button(rect.x + 1 + (buttonWidth / 2) + (7 * (1 + buttonWidth)), rect.y + 1 + (1 + buttonHeight), buttonWidth, buttonHeight, "I"));
	addGadget(new Button(rect.x + 1 + (buttonWidth / 2) + (8 * (1 + buttonWidth)), rect.y + 1 + (1 + buttonHeight), buttonWidth, buttonHeight, "O"));
	addGadget(new Button(rect.x + 1 + (buttonWidth / 2) + (9 * (1 + buttonWidth)), rect.y + 1 + (1 + buttonHeight), buttonWidth, buttonHeight, "P"));
	addGadget(new Button(rect.x + 1 + (buttonWidth / 2) + (10 * (1 + buttonWidth)), rect.y + 1 + (1 + buttonHeight), buttonWidth, buttonHeight, "["));
	addGadget(new Button(rect.x + 1 + (buttonWidth / 2) + (11 * (1 + buttonWidth)), rect.y + 1 + (1 + buttonHeight), buttonWidth, buttonHeight, "]"));

	// ASDFGHJKL
	addGadget(new Button(rect.x + 2 + buttonWidth, rect.y + 1 + (2 * (1 + buttonWidth)), buttonWidth, buttonHeight, "A"));
	addGadget(new Button(rect.x + 2 + buttonWidth + (1 + buttonWidth), rect.y + 1 + (2 * (1 + buttonWidth)), buttonWidth, buttonHeight, "S"));
	addGadget(new Button(rect.x + 2 + buttonWidth + (2 * (1 + buttonWidth)), rect.y + 1 + (2 * (1 + buttonWidth)), buttonWidth, buttonHeight, "D"));
	addGadget(new Button(rect.x + 2 + buttonWidth + (3 * (1 + buttonWidth)), rect.y + 1 + (2 * (1 + buttonWidth)), buttonWidth, buttonHeight, "F"));
	addGadget(new Button(rect.x + 2 + buttonWidth + (4 * (1 + buttonWidth)), rect.y + 1 + (2 * (1 + buttonWidth)), buttonWidth, buttonHeight, "G"));
	addGadget(new Button(rect.x + 2 + buttonWidth + (5 * (1 + buttonWidth)), rect.y + 1 + (2 * (1 + buttonWidth)), buttonWidth, buttonHeight, "H"));
	addGadget(new Button(rect.x + 2 + buttonWidth + (6 * (1 + buttonWidth)), rect.y + 1 + (2 * (1 + buttonWidth)), buttonWidth, buttonHeight, "J"));
	addGadget(new Button(rect.x + 2 + buttonWidth + (7 * (1 + buttonWidth)), rect.y + 1 + (2 * (1 + buttonWidth)), buttonWidth, buttonHeight, "K"));
	addGadget(new Button(rect.x + 2 + buttonWidth + (8 * (1 + buttonWidth)), rect.y + 1 + (2 * (1 + buttonWidth)), buttonWidth, buttonHeight, "L"));
	addGadget(new Button(rect.x + 2 + buttonWidth + (9 * (1 + buttonWidth)), rect.y + 1 + (2 * (1 + buttonWidth)), buttonWidth, buttonHeight, ";"));
	addGadget(new Button(rect.x + 2 + buttonWidth + (10 * (1 + buttonWidth)), rect.y + 1 + (2 * (1 + buttonWidth)), buttonWidth, buttonHeight, "'"));
	addGadget(new Button(rect.x + 2 + buttonWidth + (11 * (1 + buttonWidth)), rect.y + 1 + (2 * (1 + buttonWidth)), buttonWidth, buttonHeight, "#"));

	// ZXCVBNM
	addGadget(new Button(rect.x + 1 + (buttonWidth / 2), rect.y + 1 + (3 * (1 + buttonWidth)), buttonWidth, buttonHeight, "\\"));
	addGadget(new Button(rect.x + 1 + (buttonWidth / 2) + (1 + buttonWidth), rect.y + 1 + (3 * (1 + buttonWidth)), buttonWidth, buttonHeight, "Z"));
	addGadget(new Button(rect.x + 1 + (buttonWidth / 2) + (2 * (1 + buttonWidth)), rect.y + 1 + (3 * (1 + buttonWidth)), buttonWidth, buttonHeight, "X"));
	addGadget(new Button(rect.x + 1 + (buttonWidth / 2) + (3 * (1 + buttonWidth)), rect.y + 1 + (3 * (1 + buttonWidth)), buttonWidth, buttonHeight, "C"));
	addGadget(new Button(rect.x + 1 + (buttonWidth / 2) + (4 * (1 + buttonWidth)), rect.y + 1 + (3 * (1 + buttonWidth)), buttonWidth, buttonHeight, "V"));
	addGadget(new Button(rect.x + 1 + (buttonWidth / 2) + (5 * (1 + buttonWidth)), rect.y + 1 + (3 * (1 + buttonWidth)), buttonWidth, buttonHeight, "B"));
	addGadget(new Button(rect.x + 1 + (buttonWidth / 2) + (6 * (1 + buttonWidth)), rect.y + 1 + (3 * (1 + buttonWidth)), buttonWidth, buttonHeight, "N"));
	addGadget(new Button(rect.x + 1 + (buttonWidth / 2) + (7 * (1 + buttonWidth)), rect.y + 1 + (3 * (1 + buttonWidth)), buttonWidth, buttonHeight, "M"));
	addGadget(new Button(rect.x + 1 + (buttonWidth / 2) + (8 * (1 + buttonWidth)), rect.y + 1 + (3 * (1 + buttonWidth)), buttonWidth, buttonHeight, ","));
	addGadget(new Button(rect.x + 1 + (buttonWidth / 2) + (9 * (1 + buttonWidth)), rect.y + 1 + (3 * (1 + buttonWidth)), buttonWidth, buttonHeight, "."));
	addGadget(new Button(rect.x + 1 + (buttonWidth / 2) + (10 * (1 + buttonWidth)), rect.y + 1 + (3 * (1 + buttonWidth)), buttonWidth, buttonHeight, "/"));

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
