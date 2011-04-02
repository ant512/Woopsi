// Includes
#include "scrolltest.h"
#include "woopsiheaders.h"
#include "testpanel.h"

void ScrollTest::startup() {

	// Create screen
	AmigaScreen* screen = new AmigaScreen("ScrollTest Screen", Gadget::GADGET_DRAGGABLE, true, true);
	woopsiApplication->addGadget(screen);

	// Add window
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "ScrollTest Window", Gadget::GADGET_DRAGGABLE, true, true);
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
