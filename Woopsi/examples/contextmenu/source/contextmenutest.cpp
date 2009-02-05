// Includes
#include "contextmenutest.h"
#include "woopsiheaders.h"
#include <stdio.h>

void ContextMenuTest::startup() {

	// Call base startup method
	Woopsi::startup();
	
	/* Code below creates the output screen and associated gadgets */
	// Create screen
	AmigaScreen* outScreen = new AmigaScreen("Ouput Screen", 0, 0);
	woopsiApplication->addGadget(outScreen);
	
	// Add output window
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "Context Menu Test Window", 0, 0);
	outScreen->addGadget(window);

	// Get available area within window
	Gadget::Rect rect;
	window->getClientRect(rect);
	
	// Add textbox
	_output = new MultiLineTextBox(rect.x, rect.y, rect.width, rect.height, "", 0);
	window->addGadget(_output);
	_output->setEventHandler(this);
	
	// Create context menu items for the textbox
	_output->addContextMenuItem("Context Menu", 0);
	_output->addContextMenuItem("Test 1", 1);
	_output->addContextMenuItem("Test 2", 2);
	_output->addContextMenuItem("Test 3", 3);

	// Ensure Woopsi can draw itself
	enableDrawing();
	
	// Draw GUI
	draw();
}

void ContextMenuTest::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}

bool ContextMenuTest::handleEvent(const EventArgs& e) {

	// Check that a valid gadget fired the event
	if (e.gadget != NULL) {
	
		// Check which gadget fired the event - we're only interested in the textbox
		if (e.gadget == _output) {
		
			// Check which event we've got to process; we're only interested in the
			// context menu event
			if (e.type == EVENT_CONTEXT_MENU_SELECTION) {

				// Append value of context menu item to output textbox
				char buffer[10];

				sprintf(buffer, "%d", woopsiApplication->getContextMenu()->getValue());
				_output->appendText("Menu item selected: ");
				_output->appendText(buffer);
				_output->appendText('\n');
				
				return true;
			}
		}
	}
	
	return false;
}
