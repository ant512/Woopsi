// Includes
#include "timertest.h"
#include "woopsiheaders.h"

void TimerTest::startup() {

	// Call base startup method
	Woopsi::startup();
	
	/* Code below creates the output screen and associated gadgets */
	// Create screen
	AmigaScreen* outScreen = new AmigaScreen("Ouput Screen", 0, 0);
	woopsiApplication->addGadget(outScreen);
	
	// Add output window
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "Timer Test Window", 0, 0);
	outScreen->addGadget(window);

	// Get available area within window
	Gadget::Rect rect;
	window->getClientRect(rect);
	
	// Add textbox
	_output = new TextBox(rect.x, rect.y, rect.width, rect.height, "", 0);
	window->addGadget(_output);
	
	// Create timer
	_timer = new WoopsiTimer(10, true);
	_timer->setEventHandler(this);
	window->addGadget(_timer);
	_timer->start();
	
	// Set up value we're going to print to the screen
	_text = 'a';

	// Ensure Woopsi can draw itself
	enableDrawing();
	
	// Draw GUI
	draw();
}

void TimerTest::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}

bool TimerTest::handleEvent(const EventArgs& e) {

	// Check that a valid gadget fired the event
	if (e.gadget != NULL) {
	
		// Check which gadget fired the event - we're only interested in the timer
		if (e.gadget == _timer) {
		
			// Check which event we've got to process; we're only interested in the
			// action event
			if (e.type == EVENT_ACTION) {

				// Append key value to output box if the last key was not a modifier
				_output->setText(_text);
				
				// Move to next character
				_text++;
				
				// Wrap character around if necessary;
				if (_text > 'z') _text = 'a';
				
				return true;
			}
		}
	}
	
	return false;
}
