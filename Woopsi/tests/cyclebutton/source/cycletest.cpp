// Includes
#include "cycletest.h"
#include "amigascreen.h"
#include "amigawindow.h"
#include "cyclebutton.h"
#include "debug.h"

void CycleTest::startup() {

	// Call base startup method
	Woopsi::startup();

	// Create screen
	AmigaScreen* screen = new AmigaScreen("Test Screen", Gadget::GADGET_DRAGGABLE, AmigaScreen::AMIGA_SCREEN_SHOW_DEPTH | AmigaScreen::AMIGA_SCREEN_SHOW_FLIP);
	woopsiApplication->addGadget(screen);

	// Add window
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "Test Window", Gadget::GADGET_DRAGGABLE, AmigaWindow::AMIGA_WINDOW_SHOW_CLOSE | AmigaWindow::AMIGA_WINDOW_SHOW_DEPTH);
	screen->addGadget(window);

	// Add cycle button
	CycleButton* cyclebutton = new CycleButton(30, 30, 100, 20);
	cyclebutton->addOption("Option 1", 1);
	cyclebutton->addOption("Option 2", 2);
	cyclebutton->addOption("Option 3", 3);
	cyclebutton->addOption("Option 4", 4);
	cyclebutton->addOption("Option 5", 5);
	cyclebutton->addOption("Option 6", 6);
	cyclebutton->addOption("Option 7", 7);
	cyclebutton->addOption("Option 8", 8);
	cyclebutton->addOption("Option 9", 9);
	cyclebutton->addOption("Option 10", 10);
	window->addGadget(cyclebutton);

	// Get preferred dimensions for cycle button and resize
	Gadget::Rect rect;
	cyclebutton->getPreferredDimensions(rect);
	cyclebutton->resize(rect.width, rect.height);
	
	// Ensure Woopsi can draw itself
	enableDrawing();
	
	// Draw GUI
	redraw();
}

void CycleTest::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}
