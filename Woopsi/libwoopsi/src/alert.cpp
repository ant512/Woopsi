#include "alert.h"
#include "button.h"
#include "multilinetextbox.h"
#include "text.h"
#include "woopsistring.h"

using namespace WoopsiUI;

Alert::Alert(s16 x, s16 y, u16 width, u16 height, const WoopsiString& title, const WoopsiString& text, GadgetStyle* style) : AmigaWindow(x, y, width, height, title, GADGET_DRAGGABLE, AMIGA_WINDOW_SHOW_DEPTH, style) {

	// Define OK button text
	WoopsiString buttonText = "OK";

	// Padding around the main text
	u8 padding = 2;

	Rect rect;
	getClientRect(rect);

	// Calculate button dimensions
	Rect buttonRect;
	buttonRect.width = getFont()->getStringWidth(buttonText) + (padding << 1);
	buttonRect.height = getFont()->getHeight() + (padding << 1);
	buttonRect.x = rect.x + ((rect.width / 2) - (buttonRect.width / 2));
	buttonRect.y = (rect.y + rect.height) - (buttonRect.height + padding);

	// Create OK button
	_button = new Button(buttonRect.x, buttonRect.y, buttonRect.width, buttonRect.height, buttonText);
	_button->addGadgetEventHandler(this);
	addGadget(_button);

	// Create textbox
	_textBox = new MultiLineTextBox(rect.x + padding, rect.y + padding, rect.width - (padding << 1), rect.height - (buttonRect.height + (padding << 2)), text, GADGET_DRAGGABLE, 100, style);
	addGadget(_textBox);
}

void Alert::handleReleaseEvent(const GadgetEventArgs& e) {
	if (e.getSource() == _button) {
		close();
	} else {
		AmigaWindow::handleReleaseEvent(e);
	}
}
