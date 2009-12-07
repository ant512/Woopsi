// Includes
#include "packedfonts.h"
#include "amigascreen.h"
#include "amigawindow.h"
#include "textbox.h"

#include "arial9.h"
#include "bankgothic12.h"
#include "courier.h"
#include "couriermono.h"
#include "garamond10.h"
#include "kartika10.h"
#include "latha.h"
#include "latha15.h"
#include "lucida6.h"
#include "lucida10.h"
#include "lucidacalligraphy14.h"
#include "macdemo.h"
#include "ocrfont8.h"
#include "poorrichard12.h"
#include "script9b.h"
#include "timesnewroman9.h"
#include "trebuchet8.h"

void PackedFonts::startup() {

	// Call base startup method
	Woopsi::startup();

	// Create screen
	AmigaScreen* screen = new AmigaScreen("Hello World Screen", Gadget::GADGET_DRAGGABLE, AmigaScreen::AMIGA_SCREEN_SHOW_DEPTH | AmigaScreen::AMIGA_SCREEN_SHOW_FLIP);
	woopsiApplication->addGadget(screen);

	// Add window
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "Hello World Window", Gadget::GADGET_DRAGGABLE, AmigaWindow::AMIGA_WINDOW_SHOW_CLOSE | AmigaWindow::AMIGA_WINDOW_SHOW_DEPTH);
	screen->addGadget(window);

	// Get available area within window
	Rect rect;
	window->getClientRect(rect);
	
	// Add textbox
	TextBox* textbox = new TextBox(rect.x, rect.y, rect.width, rect.height, "Hello World!", new Kartika10());
	window->addGadget(textbox);

	// Ensure Woopsi can draw itself
	enableDrawing();
	
	// Draw GUI
	redraw();
}

void PackedFonts::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}
