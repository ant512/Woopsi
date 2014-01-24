// Includes
#include "pipedream.h"
#include "amigascreen.h"
#include "amigawindow.h"
#include "constants.h"

void PipeDream::startup() {

	// Create screen
	AmigaScreen* screen = new AmigaScreen("PipeDream Screen", true, true);
	woopsiApplication->addGadget(screen);

	// Add window
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "PipeDream Window", true, true);
	screen->addGadget(window);

	// Get available area within window
	Rect rect;
	window->getClientRect(rect);
	
	_grid = new PipeButtonGrid(rect.x + 1, rect.y + 1, 8, 8);
	_grid->setRefcon(1);
	window->addGadget(_grid);
	
	_flowTimer = new WoopsiTimer(20, true);
	_flowTimer->setGadgetEventHandler(this);
	window->addGadget(_flowTimer);
	_flowTimer->setRefcon(2);
	_flowTimer->start();
	
	_redrawTimer = new WoopsiTimer(1, true);
	_redrawTimer->setGadgetEventHandler(this);
	window->addGadget(_redrawTimer);
	_redrawTimer->setRefcon(3);
	_redrawTimer->start();
}

void PipeDream::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}

void PipeDream::handleActionEvent(Gadget& source) {
	if (&source == _flowTimer) {
		if (!_grid->increaseFlow(5)) {
		
			// Game over
			//while(1) {
			//	Hardware::waitForVBlank();
			//}
		}
	} else if (&source == _redrawTimer) {
		_grid->redrawActiveButton();
	}
}
