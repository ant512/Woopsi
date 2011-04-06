// Includes
#include "wpaint.h"
#include "woopsiheaders.h"
#include "canvas.h"
#include "toolbox.h"

void WPaint::startup() {

	// Create screen
	Screen* screen = new Screen("WPaint Screen");
	woopsiApplication->addGadget(screen);

	// Add canvas
	Canvas* canvas = new Canvas();
	screen->addGadget(canvas);
	
	// Add toolbox
	Toolbox* toolbox = new Toolbox(canvas);
	screen->addGadget(toolbox);
}

void WPaint::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}
