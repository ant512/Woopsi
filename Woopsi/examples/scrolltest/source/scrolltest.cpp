// Includes
#include "scrolltest.h"
#include "woopsiheaders.h"
#include "testpanel.h"

void ScrollTest::startup() {

	// Create screen
	AmigaScreen* screen = new AmigaScreen("ScrollTest Screen", Gadget::GADGET_DRAGGABLE, AmigaScreen::AMIGA_SCREEN_SHOW_DEPTH | AmigaScreen::AMIGA_SCREEN_SHOW_FLIP);
	woopsiApplication->addGadget(screen);

	// Add window
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "ScrollTest Window", Gadget::GADGET_DRAGGABLE, AmigaWindow::AMIGA_WINDOW_SHOW_CLOSE | AmigaWindow::AMIGA_WINDOW_SHOW_DEPTH);
	screen->addGadget(window);

	// Get available area within window
	Rect rect;
	window->getClientRect(rect);

	// Add test panel
	TestPanel* panel = new TestPanel(rect.x, rect.y, rect.width, rect.height, 0);
	window->addGadget(panel);
}

void ScrollTest::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}
