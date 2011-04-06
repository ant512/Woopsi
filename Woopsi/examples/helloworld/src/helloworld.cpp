// Includes
#include "helloworld.h"
#include "amigascreen.h"
#include "amigawindow.h"
#include "textbox.h"

void HelloWorld::startup() {

	// Create screen
	AmigaScreen* screen = new AmigaScreen("Hello World Screen", true, true);
	woopsiApplication->addGadget(screen);

	// Add window
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "Hello World Window", true, true);
	screen->addGadget(window);

	// Get available area within window
	Rect rect;
	window->getClientRect(rect);
	
	// Add textbox
	TextBox* textbox = new TextBox(rect.x, rect.y, rect.width, rect.height, "Hello World!");
	window->addGadget(textbox);
}

void HelloWorld::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}
