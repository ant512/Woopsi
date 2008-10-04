// Includes
#include "keytest.h"
#include "woopsiheaders.h"

void KeyTest::startup() {

	// Call base startup method
	Woopsi::startup();

	
	/* Code below creates the output screen and associated gadgets */
	// Create screen
	AmigaScreen* outScreen = new AmigaScreen("Ouput Screen", 0, 0);
	woopsiApplication->addGadget(outScreen);
	
	// Move output screen to top display
	outScreen->flipToTopScreen();

	// Add output window
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "Key Test Window", Gadget::GADGET_DRAGGABLE, AmigaWindow::AMIGA_WINDOW_SHOW_CLOSE | AmigaWindow::AMIGA_WINDOW_SHOW_DEPTH);
	outScreen->addGadget(window);

	// Get available area within window
	Gadget::Rect rect;
	window->getClientRect(rect);
	
	// Add textbox
	_output = new TextBox(rect.x, rect.y, rect.width, rect.height, "");
	window->addGadget(_output);
	
	/* Code below creates input screen and associated gadgets */
	// Create screen
	AmigaScreen* inScreen = new AmigaScreen("Input Screen", 0, 0);
	woopsiApplication->addGadget(inScreen);

	// Create keyboard
	_keyboard = new WoopsiKeyboard(0, 0, 256, 192, "Keyboard");
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
				_output->addText(_keyboard->getLastKeyClicked()->getText());
			}
		}
	}
	
	return 0;
}
