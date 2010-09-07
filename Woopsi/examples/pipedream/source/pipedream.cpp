// Includes
#include "pipedream.h"
#include "amigascreen.h"
#include "amigawindow.h"
#include "constants.h"

void PipeDream::startup() {

	// Create screen
	AmigaScreen* screen = new AmigaScreen("PipeDream Screen", Gadget::GADGET_DRAGGABLE, AmigaScreen::AMIGA_SCREEN_SHOW_DEPTH | AmigaScreen::AMIGA_SCREEN_SHOW_FLIP);
	woopsiApplication->addGadget(screen);

	// Add window
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "PipeDream Window", Gadget::GADGET_DRAGGABLE, AmigaWindow::AMIGA_WINDOW_SHOW_CLOSE | AmigaWindow::AMIGA_WINDOW_SHOW_DEPTH);
	screen->addGadget(window);

	// Get available area within window
	Rect rect;
	window->getClientRect(rect);
	
	_grid = new PipeButtonGrid(rect.x, rect.y, 6, 6);
	_grid->setRefcon(1);
	window->addGadget(_grid);
	
	_timer = new WoopsiTimer(20, true);
	_timer->addGadgetEventHandler(this);
	window->addGadget(_timer);
	_timer->setRefcon(2);
	_timer->start();
	
	// Ensure Woopsi can draw itself
	enableDrawing();
}

void PipeDream::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}

void PipeDream::handleActionEvent(const GadgetEventArgs& e) {
	if (e.getSource() == _timer) {
		if (!_grid->increaseFlow(1)) {
		
			// Game over
			while(1);
		}
	}
}
