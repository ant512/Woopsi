// Includes
#include "keytest.h"
#include "woopsiheaders.h"

void KeyTest::startup() {

	// Call base startup method
	Woopsi::startup();
	
	/* Code below creates the output screen and associated gadgets */
	// Create screen
	AmigaScreen* outScreen = new AmigaScreen("Ouput Screen", Gadget::GADGET_PERMEABLE, AmigaScreen::AMIGA_SCREEN_SHOW_FLIP);
	woopsiApplication->addGadget(outScreen);
	
	// Move output screen to top display
	outScreen->flipToTopScreen();

	// Add output window
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "Key Test Window", Gadget::GADGET_DRAGGABLE, AmigaScreen::AMIGA_SCREEN_SHOW_FLIP);
	outScreen->addGadget(window);

	// Get available area within window
	Rect rect;
	window->getClientRect(rect);
	
	// Add textbox
	_output = new MultiLineTextBox(rect.x, rect.y, rect.width, rect.height, "", 0, 0);
	_output->showCursor();
	window->addGadget(_output);
	
	/* Code below creates input screen and associated gadgets */
	// Create screen
	AmigaScreen* inScreen = new AmigaScreen("Input Screen", Gadget::GADGET_PERMEABLE, AmigaScreen::AMIGA_SCREEN_SHOW_FLIP);
	woopsiApplication->addGadget(inScreen);

	// Create keyboard
	_keyboard = new WoopsiKeyboard(0, 13, 256, 179, "Keyboard", Gadget::GADGET_DRAGGABLE, 0);
	_keyboard->addKeyboardEventHandler(this);
	inScreen->addGadget(_keyboard);
	
	// Ensure Woopsi can draw itself
	enableDrawing();
	
	// Draw GUI
	redraw();
}

void KeyTest::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}

void KeyTest::handleKeyboardPressEvent(const KeyboardEventArgs& e) {
	processKey(e.getKey());
}

void KeyTest::handleKeyboardRepeatEvent(const KeyboardEventArgs& e) {
	processKey(e.getKey());
}

void KeyTest::processKey(const WoopsiKey* key) {
	// Append key value to output box if the last key was not a modifier
	if (key->getValue() != '\0') {
		// Not modifier; append value
		_output->insertTextAtCursor(key->getValue());
	} else if (key->getKeyType() == WoopsiKey::KEY_BACKSPACE) {
		// Delete last character
		_output->removeText(_output->getTextLength() - 1);
	}
}
