// Includes
#include "windowless.h"
#include "screen.h"
#include "amigawindow.h"
#include "textbox.h"

void Windowless::startup() {

	// Call base startup method
	Woopsi::startup();

	// Create screen
	Screen* screen = new Screen("Windowless Screen", Gadget::GADGET_DRAGGABLE);
	woopsiApplication->addGadget(screen);

	// Get available area within window
	Rect rect;
	screen->getClientRect(rect);
	
	// Add textbox
	TextBox* textbox = new TextBox(rect.x, rect.y, rect.width, rect.height, "Hello World!");
	screen->addGadget(textbox);

	// Ensure Woopsi can draw itself
	enableDrawing();
	
	// Draw GUI
	redraw();
}

void Windowless::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}
