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
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "Key Test Window", Gadget::GADGET_DRAGGABLE, 0);
	outScreen->addGadget(window);

	// Get available area within window
	Gadget::Rect rect;
	window->getClientRect(rect);
	
	// Add textbox
	_output = new MultiLineTextBox(rect.x, rect.y, rect.width, rect.height, "", 0, 0);
	window->addGadget(_output);
	_output->setTextAlignmentVert(MultiLineTextBox::TEXT_ALIGNMENT_VERT_BOTTOM);
	
	/* Code below creates input screen and associated gadgets */
	// Create screen
	AmigaScreen* inScreen = new AmigaScreen("Input Screen", Gadget::GADGET_PERMEABLE, AmigaScreen::AMIGA_SCREEN_SHOW_FLIP);
	woopsiApplication->addGadget(inScreen);

	// Create keyboard
	_keyboard = new WoopsiKeyboard(0, 0, 256, 192, "Keyboard", Gadget::GADGET_DRAGGABLE, 0);
	_keyboard->setEventHandler(this);
	inScreen->addGadget(_keyboard);
	
	// Ensure Woopsi can draw itself
	enableDrawing();
	
	// Draw GUI
	draw();
}

void KeyTest::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}

bool KeyTest::handleEvent(const EventArgs& e) {
	if (e.gadget != NULL) {
		if (e.gadget == _keyboard) {
			if (e.type == EVENT_ACTION) {
				const WoopsiKey* key = _keyboard->getLastKeyClicked();
				
				// Append key value to output box if the last key was not a modifier
				if (key->getValue() != '\0') {
					// Not modifier; append value
					_output->appendText(key->getValue());
				} else if (key->getKeyType() == WoopsiKey::KEY_BACKSPACE) {
					// Delete last character
					_output->removeText(_output->getTextLength() - 1);
				}
			}
		}
	}
	
	return 0;
}
