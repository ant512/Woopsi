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
	Rect rect;
	window->getClientRect(rect);
	
	if (fatInitDefault()) {
	
		// Create a superbitmap that we will use to draw to
		SuperBitmap* superBitmap = new SuperBitmap(rect.x, rect.y, rect.width, rect.height, 256, 192, false);
		window->addGadget(superBitmap);
		
		// Load a 16-bit BMP in 555 format with a V3 DIB header
		Bitmap* bitmap = BitmapIO::loadBMP("/logo16_v3.bmp");
		
		// Draw the bitmap to the superbitmap
		superBitmap->getGraphics()->drawBitmap(0, 0, bitmap->getWidth(), bitmap->getHeight(), bitmap, 0, 0);
		
		// Delete the bitmap now we no longer need it
		delete bitmap;
		
		// Load a 16-bit BMP in 555 format with a V4 DIB header
		bitmap = BitmapIO::loadBMP("/logo16_v4.bmp");
		superBitmap->getGraphics()->drawBitmap(bitmap->getWidth(), 0, bitmap->getWidth(), bitmap->getHeight(), bitmap, 0, 0);
		delete bitmap;
		
		// Load a 24-bit BMP with a V3 DIB header
		bitmap = BitmapIO::loadBMP("/logo24_v3.bmp");
		superBitmap->getGraphics()->drawBitmap(bitmap->getWidth() * 2, 0, bitmap->getWidth(), bitmap->getHeight(), bitmap, 0, 0);
		delete bitmap;
		
		// Load a 24-bit BMP with a V4 DIB header
		bitmap = BitmapIO::loadBMP("/logo24_v4.bmp");
		superBitmap->getGraphics()->drawBitmap(bitmap->getWidth() * 3, 0, bitmap->getWidth(), bitmap->getHeight(), bitmap, 0, 0);
		delete bitmap;
		
		// Save the bitmap
		BitmapIO::saveBMP("/bitmaploader.bmp", superBitmap->getBitmap());
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
