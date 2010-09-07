// Includes
#include "fire.h"
#include "amigascreen.h"
#include "amigawindow.h"
#include "woopsitimer.h"
#include "woopsifuncs.h"
#include "bitmap.h"
#include "graphics.h"
#include "button.h"
#include "bitmap.h"

void Fire::startup() {

	// Create screen to host our application
	AmigaScreen* screen = new AmigaScreen("Drawing Screen", Gadget::GADGET_DRAGGABLE, AmigaScreen::AMIGA_SCREEN_SHOW_DEPTH | AmigaScreen::AMIGA_SCREEN_SHOW_FLIP);
	woopsiApplication->addGadget(screen);

	// Add window to the screen
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "Drawing Window", Gadget::GADGET_DRAGGABLE, AmigaWindow::AMIGA_WINDOW_SHOW_CLOSE | AmigaWindow::AMIGA_WINDOW_SHOW_DEPTH);
	screen->addGadget(window);

	// Get available area for child gadgets within window
	Rect rect;
	window->getClientRect(rect);
	
	// Add superbitmap to the window so we have a drawing space
	_superBitmap = new SuperBitmap(rect.x, rect.y, rect.width, rect.height, rect.width, rect.height, true);
	window->addGadget(_superBitmap);
	
	// Get a pointer to the superbitmap's graphics object that we can draw
	_graphics = _superBitmap->getGraphics();
	
	// Clear the bitmap
	_graphics->drawFilledRect(0, 0, _superBitmap->getWidth(), _superBitmap->getHeight(), woopsiRGB(0, 0, 0));
	
	// Add timer to the window set up to trigger an Action event every VBL
	WoopsiTimer* timer = new WoopsiTimer(1, true);
	window->addGadget(timer);
	timer->addGadgetEventHandler(this);
	timer->setRefcon(1);
	timer->start();
	
	// Move the screen to the top display so that the bottom is clear for
	// user controls
	screen->flipToTopScreen();
	
	// Create second screen that will contain the control buttons
	screen = new AmigaScreen("Control Screen", Gadget::GADGET_DRAGGABLE, AmigaScreen::AMIGA_SCREEN_SHOW_DEPTH | AmigaScreen::AMIGA_SCREEN_SHOW_FLIP);
	woopsiApplication->addGadget(screen);

	// Add window to the second screen
	window = new AmigaWindow(0, 13, 256, 179, "Control Window", Gadget::GADGET_DRAGGABLE, AmigaWindow::AMIGA_WINDOW_SHOW_CLOSE | AmigaWindow::AMIGA_WINDOW_SHOW_DEPTH);
	screen->addGadget(window);
	
	// Create the palette
	_fireColours = 94;
	_palette = new u16[_fireColours];
	_palette[0] = woopsiRGB(0, 0, 0);
	
	for (u32 i = 1; i < 32; ++i) {
		_palette[i] = woopsiRGB(i, 0, 0);
		_palette[i + 31] = woopsiRGB(31, i, 0);
		_palette[i + 62] = woopsiRGB(31, 31, i);
	}
	
	// Create the buffers
	_fireWidth = 80;
	_fireHeight = 50;
	_buffer1 = new u16[_fireWidth * _fireHeight];
	_buffer2 = new u16[_fireWidth * _fireHeight];
	
	// Other options
	_intensity = 20;
	_scale = 1;

	// Ensure Woopsi can draw itself
	enableDrawing();
}

void Fire::shutdown() {

	delete[] _palette;
	delete[] _buffer1;
	delete[] _buffer2;

	// Call base shutdown method
	Woopsi::shutdown();
}

void Fire::handleActionEvent(const GadgetEventArgs& e) {

	// Check the refcon of the gadget that raised the event to determine which
	// one it is
	switch (e.getSource()->getRefcon()) {
		case 1:
			// Timer has fired the event
			handleTimer();
			break;
	}
}

void Fire::handleTimer() {
	
	// Generate a random colour
	u32 colour;
	s16 fireX = ((_superBitmap->getWidth() - (_fireWidth * _scale)) / 2) / _scale;
	s16 fireY = (_superBitmap->getHeight() - (_fireHeight * _scale) + 2) / _scale;
	
	// Draw new line of pixels at the bottom of the window
	for (u16 y = 1; y < 3; ++y) {
	for (u16 x = 0; x < _fireWidth; ++x) {

		// Generate a random colour
		colour = _intensity + (rand() % (_fireColours - _intensity));
		
		// Draw to the buffer
		_buffer1[x + ((_fireHeight - y) * _fireWidth)] = colour;
		
		// Draw the new pixel
		//_graphics->drawFilledRect((x + fireX) * _scale, (_fireHeight - y + fireY) * _scale, _scale, _scale, _palette[colour]);
	}
	}
	
	// Draw lines higher up the screen by taking an average of the surrounding
	// pixels
	for (u16 y = _fireHeight - 2; y > 0; --y) {
		for (u16 x = 0; x < _fireWidth; ++x) {
		
			colour = 0;
	
			// Get the pixel to the left of this
			if (x > 0) {
				colour += _buffer1[x - 1 + (y * _fireWidth)];
				
				// Pixels above and below to the left
				if (y < _fireHeight - 1) colour += _buffer1[x - 1 + ((y + 1) * _fireWidth)];
				if (y > 0) colour += _buffer1[x - 1 + ((y - 1) * _fireWidth)];
			}
			
			// Get pixel to the right of this
			if (x < _fireWidth - 1) {
				colour += _buffer1[x + 1 + (y * _fireWidth)];
				
				// Pixels above and below to the right
				if (y < _fireHeight - 1) colour += _buffer1[x + 1 + ((y + 1) * _fireWidth)];
				if (y > 0) colour += _buffer1[x + 1 + ((y - 1) * _fireWidth)];
			}
			
			// Get pixel immediately below this
			if (y < _fireHeight - 1) colour += _buffer1[x + ((y + 1) * _fireWidth)];
			
			// Get the pixel above this
			if (y > 0) colour += _buffer1[x + ((y - 1) * _fireWidth)];
			
			// Average the pixels
			colour >>= 3;
			
			// Decrease the intensity of the colour so that the fire fades out
			if (colour > 0) colour--;

			// Output the pixel to the second buffer
			_buffer2[x + ((y - 1) * _fireWidth)] = colour;
			
			// Draw the new pixel
			_graphics->drawFilledRect((x + fireX) * _scale, (y - 1 + fireY) * _scale, _scale, _scale, _palette[colour]);
		}
	}

	// Swap the buffers so that we work from the latest state next time
	u16* tmp = _buffer1;
	_buffer1 = _buffer2;
	_buffer2 = tmp;
	
	// Redraw the superbitmap to the window
	_superBitmap->markRectsDirty();
}
