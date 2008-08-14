// Includes
#include "dimmedscreentest.h"
#include "woopsiheaders.h"
#include "dimmedscreen.h"

void DimmedScreenTest::startup() {

	// Call base startup method
	Woopsi::startup();

	// Create screen
	AmigaScreen* screen = new AmigaScreen("Test Screen", Gadget::GADGET_DRAGGABLE, AmigaScreen::AMIGA_SCREEN_SHOW_DEPTH | AmigaScreen::AMIGA_SCREEN_SHOW_FLIP);
	woopsiApplication->addGadget(screen);

	// Add window
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "Test Window", Gadget::GADGET_DRAGGABLE, AmigaWindow::AMIGA_WINDOW_SHOW_CLOSE | AmigaWindow::AMIGA_WINDOW_SHOW_DEPTH);
	screen->addGadget(window);

	// Get available area within window
	Gadget::Rect rect;
	window->getClientRect(rect);
	
	// Add textbox
	TextBox* textbox = new TextBox(rect.x, rect.y, rect.width, rect.height, "Hello World!");
	window->addGadget(textbox);
	
	// Add dimmed screen
	_dimmedScreen = new DimmedScreen();
	woopsiApplication->addGadget(_dimmedScreen);
	
	// Add alert window to dimmed screen
	Alert* alert = new Alert(30, 30, 100, 100, "Test", "Test");
	alert->setEventHandler(this);
	_dimmedScreen->addGadget(alert);

	// Ensure Woopsi can draw itself
	enableDrawing();
	
	// Draw GUI
	draw();
	
	// Make alert window modal
	alert->goModal();
}

void DimmedScreenTest::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}

bool DimmedScreenTest::handleEvent(const EventArgs& e) {

	// Short version of the event handler - since we know that only one gadget can
	// possibly call this, we can just check for the close event without bothering with the
	// other checks.
	if (e.type == EVENT_CLOSE) {
	
		// Alert window has closed, so close the dimmed screen
		_dimmedScreen->close();
		_dimmedScreen = NULL;
		
		return true;
	}
	
	return false;
}
