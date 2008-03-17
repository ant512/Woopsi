#include "alert.h"
#include "button.h"
#include "multilinetextbox.h"
#include "text.h"

Alert::Alert(s16 x, s16 y, u16 width, u16 height, char* title, char* text, FontBase* font) : AmigaWindow(x, y, width, height, title, GADGET_DRAGGABLE, font) {

	// Define OK button text
	char* buttonText = "OK";

	// Padding around the main text
	u8 padding = 2;

	Rect rect;
	getClientRect(rect);

	// Create textbox
	_textBox = new MultiLineTextBox(rect.x + padding, rect.y + padding, rect.width - (padding << 1), 50, text, GADGET_DRAGGABLE, 100, font);
	addGadget(_textBox);

	// Calculate the text dimensions
	u16 textWidth = _textBox->getText()->getPixelWidth() + (padding << 1);
	u16 textHeight = _textBox->getText()->getPixelHeight() + (padding << 1);

	// Limit text dimensions
	if (textWidth > 200) {
		textWidth = 200;
	}

	if (textHeight > 100) {
		textHeight = 100;
	}

	// Resize to match limited dimensions
	_textBox->resize(textWidth, textHeight);

	// Recalculate the text properties for the new dimensions
	textWidth = _textBox->getText()->getPixelWidth() + (padding << 1);
	textHeight = _textBox->getText()->getPixelHeight() + (padding << 1);

	// Calculate button dimensions
	u8 buttonWidth = (strlen(buttonText) * _font->getWidth()) + (padding << 1);
	u8 buttonHeight = _font->getHeight() + (padding << 1);

	//// Resize the window
	resize(textWidth + (getBorderSize() << 1) + 6, textHeight + getTitleHeight() + getBorderSize() + buttonHeight);
	moveTo((SCREEN_WIDTH >> 1) - (_width >> 1), (SCREEN_HEIGHT >> 1) - (_height >> 1));

	// Create OK button
	_button = new Button((width / 2) - (buttonWidth / 2), textHeight + padding, buttonWidth, buttonHeight, buttonText);
	_button->setEventHandler(this);
	addGadget(_button);
}

Alert::~Alert() {
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
	AmigaWindow::handleEvent(e);

	return false;
}
