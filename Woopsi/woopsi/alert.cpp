#include "alert.h"

Alert::Alert(s16 x, s16 y, u16 width, u16 height, char* title, char* text, FontBase* font) : SimpleWindow(x, y, width, height, title, GADGET_DRAGGABLE, font) {

	//char* buttonText = "OK";

	//// Padding around the text
	//u8 padding = 2;

	//_textViewer = newTextViewer(2, 2, 200, 20, "text test test");

	//// Calculate the text properties
	//u16 textWidth = _textViewer->getText()->getPixelWidth() + 8;
	//u16 textHeight = _textViewer->getText()->getPixelHeight() + 8;

	////char parp[10];
	////sprintf(parp, "%d:%d", textWidth, textHeight);
	////Debug::output(parp);

	//_textViewer->resize(textWidth, textHeight);

	//// Resize the window
	//resize(textWidth + (getBorderSize() << 1) + 6, textHeight + getTitleHeight() + getBorderSize() + 6);
	//moveTo(128 - (_width >> 1), 96 - (_height >> 1));

	//u8 buttonWidth = (strlen(buttonText) * _font->getWidth()) + (padding << 1);


	////// Resize the textviewer to fit the longest line of text
	////



	////newTextbox(0, 0, textWidth, textHeight, text);
	//_button = newButton((width / 2) - (buttonWidth / 2), textHeight + padding, buttonWidth, textHeight, "OK");
}

Alert::~Alert() {
}

bool Alert::release(s16 x, s16 y) {

	if (_clickedGadget == _button) {
		if (_clickedGadget->checkCollision(x, y)) {
			close();
		}
	}

	return SimpleWindow::release(x, y);
}
