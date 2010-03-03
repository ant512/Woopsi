// Includes
#include "dimmedscreentest.h"
#include "woopsiheaders.h"

void DimmedScreenTest::startup() {

	// Create screen
	AmigaScreen* screen = new AmigaScreen("Test Screen", Gadget::GADGET_DRAGGABLE, AmigaScreen::AMIGA_SCREEN_SHOW_DEPTH | AmigaScreen::AMIGA_SCREEN_SHOW_FLIP);
	woopsiApplication->addGadget(screen);

	// Add window
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "Test Window", Gadget::GADGET_DRAGGABLE, AmigaWindow::AMIGA_WINDOW_SHOW_CLOSE | AmigaWindow::AMIGA_WINDOW_SHOW_DEPTH);
	screen->addGadget(window);

	// Get available area within window
	Rect rect;
	window->getClientRect(rect);
	
	// Add textbox
	TextBox* textbox = new TextBox(rect.x, rect.y, rect.width, rect.height, "Hello World!");
	window->addGadget(textbox);
	
	// Add dimmed screen
	_dimmedScreen = new DimmedScreen();
	woopsiApplication->addGadget(_dimmedScreen);
	
	// Add alert window to dimmed screen
	Alert* alert = new Alert(30, 30, 100, 100, "Test", "Test");
	alert->addGadgetEventHandler(this);
	_dimmedScreen->addGadget(alert);

	// Ensure Woopsi can draw itself
	enableDrawing();
	
	// Draw GUI
	redraw();
	
	// Make alert window modal
	alert->goModal();
}

void DimmedScreenTest::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}

void DimmedScreenTest::handleCloseEvent(const GadgetEventArgs& e) {

	// Alert window has closed, so close the dimmed screen
	_dimmedScreen->close();
	_dimmedScreen = NULL;
}
