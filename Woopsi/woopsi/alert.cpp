#include "alert.h"
#include "button.h"
#include "multilinetextbox.h"
#include "text.h"

using namespace WoopsiUI;

Alert::Alert(s16 x, s16 y, u16 width, u16 height, const char* title, const char* text, FontBase* font) : AmigaWindow(x, y, width, height, title, GADGET_DRAGGABLE, AMIGA_WINDOW_SHOW_DEPTH, font) {

	// Define OK button text
	const char* buttonText = "OK";

	// Padding around the main text
	u8 padding = 2;

	Rect rect;
	getClientRect(rect);

	// Calculate button dimensions
	Rect buttonRect;
	buttonRect.width = _font->getStringWidth(buttonText) + (padding << 1);
	buttonRect.height = _font->getHeight() + (padding << 1);
	buttonRect.x = rect.x + ((rect.width / 2) - (buttonRect.width / 2));
	buttonRect.y = (rect.y + rect.height) - (buttonRect.height + padding);

	// Create OK button
	_button = new Button(buttonRect.x, buttonRect.y, buttonRect.width, buttonRect.height, buttonText);
	_button->setEventHandler(this);
	addGadget(_button);

	// Create textbox
	_textBox = new MultiLineTextBox(rect.x + padding, rect.y + padding, rect.width - (padding << 1), rect.height - (buttonRect.height + (padding << 2)), text, GADGET_DRAGGABLE, 100, font);
	addGadget(_textBox);
}

bool Alert::handleEvent(const EventArgs& e) {
	// Only handle release events
	if (e.type == EVENT_RELEASE) {
		if (e.gadget != NULL) {
			if (e.gadget == _button) {
				close();
				return true;
			}
		}
	}

	// Handle other window events
	return AmigaWindow::handleEvent(e);
}
