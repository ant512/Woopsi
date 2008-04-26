// Includes
#include "helloworld.h"
#include "woopsiheaders.h"

void HelloWorld::startup() {

	// Call base startup method
	Woopsi::startup();

	// Create screen
	AmigaScreen* screen = new AmigaScreen("Hello World Screen");
	woopsiApplication->addGadget(screen);

	// Add window
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "Hello World Window", Gadget::GADGET_CLOSABLE | Gadget::GADGET_DRAGGABLE);
	screen->addGadget(window);

	// Get available area within window
	Gadget::Rect rect;
	window->getClientRect(rect);
	
	// Add textbox
	Textbox* textbox = new Textbox(rect.x, rect.y, rect.width, rect.height, "Hello World!");
	window->addGadget(textbox);

	// Ensure Woopsi can draw itself
	enableDrawing();
	
	// Draw GUI
	draw();
}

void HelloWorld::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}
