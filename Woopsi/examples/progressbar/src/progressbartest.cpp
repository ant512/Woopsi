// Includes
#include "progressbartest.h"
#include "woopsiheaders.h"
#include "debug.h"

void ProgressBarTest::startup() {

	// Create screen
	AmigaScreen* screen = new AmigaScreen("Test Screen", true, true);
	woopsiApplication->addGadget(screen);

	// Add window
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "Test Window", true, true);
	screen->addGadget(window);

	// Get available area within window
	Rect rect;
	window->getClientRect(rect);
	
	// Add progress bar
	_progressBar = new ProgressBar(rect.x + (rect.width / 4), rect.y + (rect.height / 4), rect.width / 2, rect.height / 4);
	_progressBar->setMinimumValue(0);
	_progressBar->setMaximumValue(100);
	window->addGadget(_progressBar);
	
	// Add timer
	_timer = new WoopsiTimer(10, true);
	_timer->setGadgetEventHandler(this);
	addGadget(_timer);
	
	// Start the timer
	_timer->start();
}

void ProgressBarTest::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}

void ProgressBarTest::handleActionEvent(Gadget& source) {

	// Short version of the event handler since we know that only one gadget can
	// possibly call this
	_progressBar->setValue(_progressBar->getValue() + (rand() % 10));
}
