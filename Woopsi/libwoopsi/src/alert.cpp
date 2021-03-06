#include "alert.h"
#include "button.h"
#include "multilinetextbox.h"
#include "woopsistring.h"

using namespace WoopsiUI;

Alert::Alert(s16 x, s16 y, u16 width, u16 height, const WoopsiString& title, const WoopsiString& text, GadgetStyle* style) : AmigaWindow(x, y, width, height, title, false, true, style) {

	Rect rect;
	getClientRect(rect);

	// Adjust rect to leave 2px padding around children
	rect.x += 2;
	rect.y += 2;
	rect.width -= 4;
	rect.height -= 4;

	// Create OK button
	_button = new Button(0, 0, 0, 0, "OK");

	// Get button to tell us its desired size
	Rect buttonRect;
	_button->getPreferredDimensions(buttonRect);

	// Calculate x and y co-ords
	buttonRect.x = rect.x + ((rect.width - buttonRect.width) >> 1);
	buttonRect.y = (rect.y + rect.height) - buttonRect.height;

	_button->changeDimensions(buttonRect.x, buttonRect.y, buttonRect.width, buttonRect.height);

	// Create textbox
	_textBox = new MultiLineTextBox(rect.x, rect.y, rect.width, rect.height - buttonRect.height - 2, text, 100, style);
	_textBox->hideCursor();
	_textBox->disableKeyboardPopup();
	addGadget(_textBox);

	_button->setGadgetEventHandler(this);
	addGadget(_button);
}

void Alert::handleReleaseEvent(Gadget& source, const WoopsiPoint& point) {
	if (&source == _button) {
		close();
	} else {
		AmigaWindow::handleReleaseEvent(*this, point);
	}
}

void Alert::onResize(u16 width, u16 height) {

	// Call base class' method to ensure the basic window resizes correctly
	AmigaWindow::onResize(width, height);

	Rect rect;
	getClientRect(rect);

	// Adjust rect to leave 2px padding around children
	rect.x += 2;
	rect.y += 2;
	rect.width -= 4;
	rect.height -= 4;

	// Calculate button dimensions
	Rect buttonRect;
	_button->getPreferredDimensions(buttonRect);

	// Calculate x and y co-ords
	buttonRect.x = rect.x + ((rect.width - buttonRect.width) >> 1);
	buttonRect.y = (rect.y + rect.height) - buttonRect.height;

	_button->changeDimensions(buttonRect.x, buttonRect.y, buttonRect.width, buttonRect.height);

	// Change textbox dimensions
	_textBox->changeDimensions(rect.x, rect.y, rect.width, rect.height - buttonRect.height - 2);
}
