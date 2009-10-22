// Includes
#include "bitmaploader.h"
#include "amigascreen.h"
#include "amigawindow.h"
#include "superbitmap.h"
#include "bitmapio.h"
#include "bitmap.h"

void BitmapLoader::startup() {

	// Call base startup method
	Woopsi::startup();

	// Create screen
	AmigaScreen* screen = new AmigaScreen("BMP Loader Screen", Gadget::GADGET_DRAGGABLE, AmigaScreen::AMIGA_SCREEN_SHOW_DEPTH | AmigaScreen::AMIGA_SCREEN_SHOW_FLIP);
	woopsiApplication->addGadget(screen);

	// Add window
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "BMP Loader Window", Gadget::GADGET_DRAGGABLE, AmigaWindow::AMIGA_WINDOW_SHOW_CLOSE | AmigaWindow::AMIGA_WINDOW_SHOW_DEPTH);
	screen->addGadget(window);

	// Get available area within window
	Gadget::Rect rect;
	window->getClientRect(rect);
	
	if (fatInitDefault()) {
	
		SuperBitmap* superBitmap = new SuperBitmap(rect.x, rect.y, rect.width, rect.height, 256, 192, false);
		window->addGadget(superBitmap);
		
		Bitmap* bitmap = BitmapIO::loadBMP("/logo.bmp");
		
		superBitmap->drawBitmap(0, 0, 256, 192, bitmap->getBitmap(), 0, 0, 256, 192);
	}

	// Ensure Woopsi can draw itself
	enableDrawing();
	
	// Draw GUI
	redraw();
}

void BitmapLoader::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}
