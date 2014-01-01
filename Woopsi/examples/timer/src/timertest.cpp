// Includes
#include "timertest.h"
#include "woopsiheaders.h"

void TimerTest::startup() {
	
	/* Code below creates the output screen and associated gadgets */
	// Create screen
	AmigaScreen* outScreen = new AmigaScreen("Ouput Screen", false, false);
	woopsiApplication->addGadget(outScreen);
	
	// Add output window
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "Timer Test Window", false, false);
	outScreen->addGadget(window);

	// Get available area within window
	Rect rect;
	window->getClientRect(rect);
	
	// Add textbox
	_output = new TextBox(rect.x, rect.y, rect.width, rect.height, "", 0);
	window->addGadget(_output);
	
	// Create timer
	_timer = new WoopsiTimer(10, true);
	_timer->setGadgetEventHandler(this);
	window->addGadget(_timer);
	_timer->start();
	
	// Set up value we're going to print to the screen
	_text = 'a';
}

void TimerTest::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}

void TimerTest::handleActionEvent(Gadget& source) {

	// Check which gadget fired the event - we're only interested in the timer
	if (&source == _timer) {
	
		// Append key value to output box if the last key was not a modifier
		_output->setText(_text);
		
		// Move to next character
		_text++;
		
		// Wrap character around if necessary;
		if (_text > 'z') _text = 'a';
	}
}
