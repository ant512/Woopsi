// Includes
#include "defaultstyledemo.h"
#include "amigascreen.h"
#include "amigawindow.h"
#include "textbox.h"
#include "woopsifuncs.h"
#include "fonts/garamond10.h"

void DefaultStyleDemo::startup() {
	
	// Change default style so that all gadgets are created with non-standard
	// colours, etc
	
	// Change the font
	delete defaultGadgetStyle->font;
	defaultGadgetStyle->font = new Garamond10();
	
	// Ensure that this object's font pointer does not point at the deleted font
	setFont(defaultGadgetStyle->font);
	
	// Change the colours
	defaultGadgetStyle->colours.back = woopsiRGB(5, 5, 5);
	defaultGadgetStyle->colours.shine = woopsiRGB(10, 31, 10);
	defaultGadgetStyle->colours.dark = woopsiRGB(5, 10, 5);
	defaultGadgetStyle->colours.highlight = woopsiRGB(25, 31, 25);
	defaultGadgetStyle->colours.shadow = woopsiRGB(0, 0, 0);
	defaultGadgetStyle->colours.fill = woopsiRGB(10, 0, 0);
	defaultGadgetStyle->colours.text = woopsiRGB(0, 31, 0);

	// Create screen
	AmigaScreen* screen = new AmigaScreen("Hello World Screen", true, true);
	woopsiApplication->addGadget(screen);

	// Add window
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "Hello World Window", true, true);
	screen->addGadget(window);

	// Get available area within window
	Rect rect;
	window->getClientRect(rect);
	
	// Add textbox
	TextBox* textbox = new TextBox(rect.x, rect.y, rect.width, rect.height, "Hello World!");
	window->addGadget(textbox);
}

void DefaultStyleDemo::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}
