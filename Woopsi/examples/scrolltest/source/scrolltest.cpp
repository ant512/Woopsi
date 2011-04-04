// Includes
#include "scrolltest.h"
#include "woopsiheaders.h"
#include "testpanel.h"

void ScrollTest::startup() {

	// Create screen
	AmigaScreen* screen = new AmigaScreen("ScrollTest Screen", true, true);
	woopsiApplication->addGadget(screen);

	// Add window
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "ScrollTest Window", true, true);
	screen->addGadget(window);

	// Get available area within window
	Rect rect;
	window->getClientRect(rect);

	// Add test panel
	TestPanel* panel = new TestPanel(rect.x, rect.y, rect.width, rect.height);
	window->addGadget(panel);
}

void ScrollTest::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}
