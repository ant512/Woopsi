#include "woopsikeyboardscreen.h"
#include "woopsikeyboard.h"
#include "button.h"
#include "woopsi.h"

using namespace WoopsiUI;

WoopsiKeyboardScreen::WoopsiKeyboardScreen(KeyboardEventHandler* opener) : AmigaScreen("Keyboard", 0, 0 ) {
	_flags.canReceiveFocus = false;

	WoopsiKeyboard* keyboard = new WoopsiKeyboard(2, 13, 0);
	keyboard->addKeyboardEventHandler(opener);

	addGadget(keyboard);

	Rect rect;
	getClientRect(rect);

	// Create OK button
	Button* button = new Button(0, 0, 0, 0, "OK");

	// Get button to tell us its desired size
	Rect buttonRect;
	button->getPreferredDimensions(buttonRect);

	// Calculate x and y co-ords
	buttonRect.x = rect.x + ((rect.width - buttonRect.width) >> 1);
	buttonRect.y = (rect.y + rect.height) - buttonRect.height;

	button->changeDimensions(buttonRect.x, buttonRect.y, buttonRect.width, buttonRect.height);
	button->setRefcon(1);
	button->addGadgetEventHandler(this);
	addGadget(button);
}

void WoopsiKeyboardScreen::handleReleaseEvent(const GadgetEventArgs& e) {
	if (e.getSource() != NULL) {
		if (e.getSource()->getRefcon() == 1) {
			woopsiApplication->hideKeyboard();
			return;
		}
	}

	AmigaScreen::handleClickEvent(e);
}
