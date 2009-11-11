// Includes
#include "woopsiheaders.h"

using namespace WoopsiUI;

int main(int argc, char* argv[]) {

	// Create Woopsi instance
	woopsiApplication = new Woopsi();

	// Start up Woopsi application
	woopsiApplication->startup();
	
	// Create screen
	AmigaScreen* screen = new AmigaScreen("Hello World Screen", Gadget::GADGET_DRAGGABLE, AmigaScreen::AMIGA_SCREEN_SHOW_DEPTH | AmigaScreen::AMIGA_SCREEN_SHOW_FLIP);
	woopsiApplication->addGadget(screen);

	// Add window
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "Hello World Window", Gadget::GADGET_DRAGGABLE, AmigaWindow::AMIGA_WINDOW_SHOW_CLOSE | AmigaWindow::AMIGA_WINDOW_SHOW_DEPTH);
	screen->addGadget(window);

	// Get available area within window
	Rect rect;
	window->getClientRect(rect);
	
	// Add textbox
	TextBox* textbox = new TextBox(rect.x, rect.y, rect.width, rect.height, "Hello World!");
	window->addGadget(textbox);

	// Ensure Woopsi can draw itself
	woopsiApplication->enableDrawing();
	
	// Draw GUI
	woopsiApplication->redraw();
	
	// Run Woopsi
	woopsiApplication->goModal();
	
	// Finish up
	woopsiApplication->shutdown();
	
	return 0;
}
