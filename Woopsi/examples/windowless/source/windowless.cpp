// Includes
#include "windowless.h"
#include "screen.h"
#include "amigawindow.h"
#include "textbox.h"

void Windowless::startup() {

	// Create screen
	Screen* screen = new Screen("Windowless Screen", Gadget::GADGET_DRAGGABLE);
	woopsiApplication->addGadget(screen);

	// Get available area within window
	Rect rect;
	screen->getClientRect(rect);
	
	// Add textbox
	TextBox* textbox = new TextBox(rect.x, rect.y, rect.width, rect.height, "Hello World!");
	screen->addGadget(textbox);
}

void Windowless::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}
