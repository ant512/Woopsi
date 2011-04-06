// Includes
#include "gadgetdrawing.h"
#include "amigascreen.h"
#include "woopsitimer.h"
#include "woopsifuncs.h"
#include "graphicsport.h"
#include "button.h"
#include "graphics.h"

void GadgetDrawing::startup() {

	// Create screen to host our application
	AmigaScreen* screen = new AmigaScreen("Drawing Screen", true, true);
	woopsiApplication->addGadget(screen);

	// Add window to the screen
	_window = new AmigaWindow(0, 13, 256, 179, "Drawing Window", true, true);
	screen->addGadget(_window);

	// Get available area for child gadgets within window
	Rect rect;
	_window->getClientRect(rect);
	
	// Add timer to the window set up to trigger an Action event every VBL
	WoopsiTimer* timer = new WoopsiTimer(1, true);
	_window->addGadget(timer);
	timer->addGadgetEventHandler(this);
	timer->setRefcon(1);
	timer->start();
	
	// Move the screen to the top display so that the bottom is clear for
	// user controls
	screen->flipToTopScreen();
	
	// Create second screen that will contain the control buttons
	screen = new AmigaScreen("Control Screen", true, true);
	woopsiApplication->addGadget(screen);

	// Add window to the second screen
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "Control Window", true, true);
	screen->addGadget(window);
	
	// Create array containing labels for all buttons
	WoopsiArray<const char*> buttonText;
	buttonText.push_back("Off");
	buttonText.push_back("Lines");
	buttonText.push_back("Ellipses");
	buttonText.push_back("Filled Ellipses");
	buttonText.push_back("Rects");
	buttonText.push_back("Filled Rects");
	buttonText.push_back("Filled XOR Rects");
	buttonText.push_back("XOR Horiz Lines");
	buttonText.push_back("XOR Vert Lines");
	buttonText.push_back("XOR Rects");
	buttonText.push_back("XOR Pixels");
	buttonText.push_back("Pixels");
	buttonText.push_back("Text");
	buttonText.push_back("Bitmap");
	buttonText.push_back("Bitmap Transparency");
	buttonText.push_back("Dim");
	
	// Calculate button dimensions based on size of window
	u16 buttonWidth = rect.width / 2;
	u16 buttonHeight = rect.height / 10;
	
	// Create all buttons using array of button text as labels
	for (u8 i = 0; i < buttonText.size(); ++i) {
		s16 buttonX = rect.x + ((i / 10) * buttonWidth);
		s16 buttonY = rect.y + ((i % 10) * buttonHeight);
		
		Button* button = new Button(buttonX, buttonY, buttonWidth, buttonHeight, buttonText[i]);
		button->setRefcon(i + 2);
		button->addGadgetEventHandler(this);
		window->addGadget(button);
	}
	
	// Create a bitmap to use to test the bitmap drawing mode
	_bitmap = new Bitmap(100, 20);
	Graphics* gfx = _bitmap->newGraphics();
	gfx->drawFilledRect(0, 0, 100, 20, woopsiRGB(31, 0, 0));
	gfx->drawFilledEllipse(50, 10, 10, 10, woopsiRGB(0, 31, 0));
	gfx->drawText(30, 5, getFont(), "Bitmap", 0, 6, woopsiRGB(0, 0, 31));
	delete gfx;
	
	// Set to line drawing mode
	_mode = DRAW_MODE_OFF;
}

void GadgetDrawing::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}

void GadgetDrawing::handleActionEvent(const GadgetEventArgs& e) {

	// Check the refcon of the gadget that raised the event to determine which
	// one it is
	switch (e.getSource()->getRefcon()) {
		case 1:
			// Timer has fired the event
			handleTimer();
			break;
		case 2:
			// Off button
			_mode = DRAW_MODE_OFF;
			break;
		case 3:
			// Lines button
			_mode = DRAW_MODE_LINE;
			break;
		case 4:
			// Ellipses button
			_mode = DRAW_MODE_ELLIPSE;
			break;
		case 5:
			// Filled ellipses button
			_mode = DRAW_MODE_FILLED_ELLIPSE;
			break;
		case 6:
			// Rects button
			_mode = DRAW_MODE_RECT;
			break;
		case 7:
			// Filled rects button
			_mode = DRAW_MODE_FILLED_RECT;
			break;
		case 8:
			// Filled XOR rects button
			_mode = DRAW_MODE_FILLED_XOR_RECT;
			break;
		case 9:
			// Horiz lines button
			_mode = DRAW_MODE_XOR_HORIZ_LINE;
			break;
		case 10:
			// Vert lines button
			_mode = DRAW_MODE_XOR_VERT_LINE;
			break;
		case 11:
			// XOR rects button
			_mode = DRAW_MODE_XOR_RECT;
			break;
		case 12:
			// XOR pixels button
			_mode = DRAW_MODE_XOR_PIXEL;
			break;
		case 13:
			// Pixels button
			_mode = DRAW_MODE_PIXEL;
			break;
		case 14:
			// Text button
			_mode = DRAW_MODE_TEXT;
			break;
		case 15:
			// Bitmap button
			_mode = DRAW_MODE_BITMAP;
			break;
		case 16:
			// Bitmap transparent button
			_mode = DRAW_MODE_BITMAP_TRANSPARENT;
			break;
		case 17:
			// Dim button
			_mode = DRAW_MODE_DIM;
			break;
	}
}

void GadgetDrawing::handleTimer() {

	// Choose drawing type based on mode
	switch (_mode) {
		case DRAW_MODE_OFF:
			break;
		case DRAW_MODE_LINE:
			drawLine();
			break;
		case DRAW_MODE_ELLIPSE:
			drawEllipse();
			break;
		case DRAW_MODE_FILLED_ELLIPSE:
			drawFilledEllipse();
			break;
		case DRAW_MODE_RECT:
			drawRect();
			break;
		case DRAW_MODE_FILLED_RECT:
			drawFilledRect();
			break;
		case DRAW_MODE_FILLED_XOR_RECT:
			drawFilledXORRect();
			break;
		case DRAW_MODE_XOR_HORIZ_LINE:
			drawXORHorizLine();
			break;
		case DRAW_MODE_XOR_VERT_LINE:
			drawXORVertLine();
			break;
		case DRAW_MODE_XOR_RECT:
			drawXORRect();
			break;
		case DRAW_MODE_XOR_PIXEL:
			drawXORPixel();
			break;
		case DRAW_MODE_PIXEL:
			drawPixel();
			break;
		case DRAW_MODE_TEXT:
			drawText();
			break;
		case DRAW_MODE_BITMAP:
			drawBitmap();
			break;
		case DRAW_MODE_BITMAP_TRANSPARENT:
			drawBitmapTransparent();
			break;
		case DRAW_MODE_DIM:
			drawDim();
			break;
		default:
			break;
	}
}

void GadgetDrawing::drawLine() {

	// Generate random start/end co-ords for the line
	s16 x1 = rand() % 256;
	s16 y1 = rand() % 192;
	s16 x2 = rand() % 256;
	s16 y2 = rand() % 192;
	
	GraphicsPort* port = _window->newGraphicsPort(false);
	port->drawLine(x1, y1, x2, y2, getRandomColour());
	delete port;
}

void GadgetDrawing::drawEllipse() {

	// Generate random co-ordinates and dimensions for the ellipse
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	s16 hRadius = rand() % 100;
	s16 vRadius = rand() % 100;
	
	GraphicsPort* port = _window->newGraphicsPort(false);
	port->drawEllipse(x, y, hRadius, vRadius, getRandomColour());
	delete port;
}

void GadgetDrawing::drawFilledEllipse() {

	// Generate random co-ordinates and dimensions for the ellipse
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	s16 hRadius = rand() % 100;
	s16 vRadius = rand() % 100;
	
	GraphicsPort* port = _window->newGraphicsPort(false);
	port->drawFilledEllipse(x, y, hRadius, vRadius, getRandomColour());
	delete port;
}

u16 GadgetDrawing::getRandomColour() {

	// Generate a random colour
	u8 r = rand() % 31;
	u8 g = rand() % 31;
	u8 b = rand() % 31;
	return woopsiRGB(r, g, b);
}

void GadgetDrawing::drawRect() {

	// Generate random start/end co-ords for the rect
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	s16 width = rand() % 256;
	s16 height = rand() % 192;
	
	GraphicsPort* port = _window->newGraphicsPort(false);
	port->drawRect(x, y, width, height, getRandomColour());
	delete port;
}

void GadgetDrawing::drawFilledRect() {

	// Generate random start/end co-ords for the rect
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	s16 width = rand() % 256;
	s16 height = rand() % 192;
	
	GraphicsPort* port = _window->newGraphicsPort(false);
	port->drawFilledRect(x, y, width, height, getRandomColour());
	delete port;
}

void GadgetDrawing::drawFilledXORRect() {

	// Generate random start/end co-ords for the rect
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	s16 width = rand() % 256;
	s16 height = rand() % 192;
	
	GraphicsPort* port = _window->newGraphicsPort(false);
	port->drawFilledXORRect(x, y, width, height);
	delete port;
}

void GadgetDrawing::drawXORHorizLine() {

	// Generate random co-ords for the line
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	s16 width = rand() % 256;
	
	GraphicsPort* port = _window->newGraphicsPort(false);
	port->drawXORHorizLine(x, y, width);
	delete port;
}

void GadgetDrawing::drawXORVertLine() {

	// Generate random co-ords for the line
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	s16 height = rand() % 192;
	
	GraphicsPort* port = _window->newGraphicsPort(false);
	port->drawXORVertLine(x, y, height);
	delete port;
}

void GadgetDrawing::drawXORRect() {

	// Generate random start/end co-ords for the rect
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	s16 width = rand() % 256;
	s16 height = rand() % 192;
	
	GraphicsPort* port = _window->newGraphicsPort(false);
	port->drawXORRect(x, y, width, height);
	delete port;
}

void GadgetDrawing::drawXORPixel() {

	// Generate random co-ords
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	
	GraphicsPort* port = _window->newGraphicsPort(false);
	port->drawXORPixel(x, y);
	delete port;
}

void GadgetDrawing::drawPixel() {

	// Generate random co-ords
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	
	GraphicsPort* port = _window->newGraphicsPort(false);
	port->drawPixel(x, y, getRandomColour());
	delete port;
}

void GadgetDrawing::drawText() {

	// Generate random co-ords
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	
	GraphicsPort* port = _window->newGraphicsPort(false);
	port->drawText(x, y, getFont(), "Text!", 0, 5, getRandomColour());
	delete port;
}

void GadgetDrawing::drawBitmap() {

	// Generate random co-ords
	s16 sx = rand() % 100;
	s16 sy = rand() % 20;
	s16 dx = rand() % 256;
	s16 dy = rand() % 192;
	u16 width = 100 - sx;
	u16 height = 20 - sy;
	
	GraphicsPort* port = _window->newGraphicsPort(false);
	port->drawBitmap(dx, dy, width, height, _bitmap, sx, sy);
	delete port;
}

void GadgetDrawing::drawBitmapTransparent() {

	// Generate random co-ords
	s16 sx = rand() % 100;
	s16 sy = rand() % 20;
	s16 dx = rand() % 256;
	s16 dy = rand() % 192;
	u16 width = 100 - sx;
	u16 height = 20 - sy;
	
	GraphicsPort* port = _window->newGraphicsPort(false);
	port->drawBitmap(dx, dy, width, height, _bitmap, sx, sy, woopsiRGB(31, 0, 0));
	delete port;
}

void GadgetDrawing::drawDim() {

	// Generate random start/end co-ords for the rect
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	s16 width = rand() % 256;
	s16 height = rand() % 192;
	
	GraphicsPort* port = _window->newGraphicsPort(false);
	port->dim(x, y, width, height);
	delete port;
}
