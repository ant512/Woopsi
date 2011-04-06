// Includes
#include "contextmenutest.h"
#include "woopsiheaders.h"
#include <stdio.h>

void ContextMenuTest::startup() {
	
	/* Code below creates the output screen and associated gadgets */
	// Create screen
	AmigaScreen* outScreen = new AmigaScreen("Ouput Screen", false, false);
	woopsiApplication->addGadget(outScreen);
	
	// Add output window
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "Context Menu Test Window", false, false);
	outScreen->addGadget(window);

	// Get available area within window
	Rect rect;
	window->getClientRect(rect);
	
	// Add textbox
	_output = new MultiLineTextBox(rect.x, rect.y, rect.width, rect.height, "", 0);
	window->addGadget(_output);
	_output->addGadgetEventHandler(this);
	
	// Create context menu items for the textbox
	_output->addContextMenuItem("Context Menu", 0);
	_output->addContextMenuItem("Test 1", 1);
	_output->addContextMenuItem("Test 2", 2);
	_output->addContextMenuItem("Test 3", 3);
}

void ContextMenuTest::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}

void ContextMenuTest::handleContextMenuSelectionEvent(const ContextMenuEventArgs& e) {
		
	// Append value of context menu item to output textbox
	char buffer[10];

	sprintf(buffer, "%d", e.getItem()->getValue());
	_output->appendText("Menu item selected: ");
	_output->appendText(buffer);
	_output->appendText('\n');
}
