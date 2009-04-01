// Includes
#include "calendartest.h"
#include "woopsiheaders.h"
#include "debug.h"

void CalendarTest::startup() {

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
	
	// Add calendar
	_calendar = new Calendar(rect.x, rect.y, rect.width, rect.height, 26, 10, 2008, 0);
	_calendar->addGadgetEventHandler(this);
	window->addGadget(_calendar);

	// Ensure Woopsi can draw itself
	enableDrawing();
	
	// Draw GUI
	draw();
}

void CalendarTest::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}

void CalendarTest::handleActionEvent(const GadgetEventArgs& e) {

	// Short version of the event handler since we know that only one gadget can
	// possibly call this
	Debug::printf("%d/%d/%d", _calendar->getDay(), _calendar->getMonth(), _calendar->getYear());
}
