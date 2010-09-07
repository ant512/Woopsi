// Includes
#include "wpaint.h"
#include "woopsiheaders.h"
#include "canvas.h"
#include "toolbox.h"

void WPaint::startup() {

	// Create screen
	Screen* screen = new Screen("WPaint Screen", 0);
	woopsiApplication->addGadget(screen);

	// Add canvas
	Canvas* canvas = new Canvas();
	screen->addGadget(canvas);
	
	// Add toolbox
	Toolbox* toolbox = new Toolbox(canvas);
	screen->addGadget(toolbox);

	// Ensure Woopsi can draw itself
	enableDrawing();
}

void WPaint::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}
