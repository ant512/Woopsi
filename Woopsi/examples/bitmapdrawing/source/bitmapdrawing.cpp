// Includes
#include "bitmapdrawing.h"
#include "amigascreen.h"
#include "amigawindow.h"
#include "woopsitimer.h"
#include "woopsifuncs.h"
#include "bitmap.h"
#include "graphics.h"
#include "button.h"
#include "bitmap.h"

void BitmapDrawing::startup() {

	// Call base startup method
	Woopsi::startup();

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
	
	Rect clipRect;
	clipRect.x = 10;
	clipRect.y = 10;
	clipRect.width = rect.width - 20;
	clipRect.height = rect.height - 20;
	_graphics->setClipRect(clipRect);
	
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
	
	// Create array containing labels for all buttons
	WoopsiArray<const char*> buttonText;
	buttonText.push_back("Off");
	buttonText.push_back("Lines");
	buttonText.push_back("Ellipses");
	buttonText.push_back("Filled Ellipses");
	buttonText.push_back("Rects");
	buttonText.push_back("Filled Rects");
	buttonText.push_back("Filled XOR Rects");
	buttonText.push_back("Horiz Lines");
	buttonText.push_back("Vert Lines");
	buttonText.push_back("Circles");
	buttonText.push_back("Filled Circles");
	buttonText.push_back("XOR Horiz Lines");
	buttonText.push_back("XOR Vert Lines");
	buttonText.push_back("XOR Rects");
	buttonText.push_back("XOR Pixels");
	buttonText.push_back("Pixels");
	buttonText.push_back("Text");
	buttonText.push_back("Bitmap");
	buttonText.push_back("Bitmap Transparency");
	buttonText.push_back("Floodfill");
	buttonText.push_back("Copy");
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
	gfx->drawFilledCircle(50, 10, 10, woopsiRGB(0, 31, 0));
	gfx->drawText(30, 5, getFont(), "Bitmap", 0, 6, woopsiRGB(0, 0, 31));
	delete gfx;
	
	// Set to line drawing mode
	_mode = DRAW_MODE_OFF;

	// Ensure Woopsi can draw itself
	enableDrawing();
	
	// Draw GUI
	redraw();
}

void BitmapDrawing::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}

void BitmapDrawing::handleActionEvent(const GadgetEventArgs& e) {

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
			_mode = DRAW_MODE_HORIZ_LINE;
			break;
		case 10:
			// Vert lines button
			_mode = DRAW_MODE_VERT_LINE;
			break;
		case 11:
			// Circles button
			_mode = DRAW_MODE_CIRCLE;
			break;
		case 12:
			// Filled circles button
			_mode = DRAW_MODE_FILLED_CIRCLE;
			break;
		case 13:
			// Horiz lines button
			_mode = DRAW_MODE_XOR_HORIZ_LINE;
			break;
		case 14:
			// Vert lines button
			_mode = DRAW_MODE_XOR_VERT_LINE;
			break;
		case 15:
			// XOR rects button
			_mode = DRAW_MODE_XOR_RECT;
			break;
		case 16:
			// XOR pixels button
			_mode = DRAW_MODE_XOR_PIXEL;
			break;
		case 17:
			// Pixels button
			_mode = DRAW_MODE_PIXEL;
			break;
		case 18:
			// Text button
			_mode = DRAW_MODE_TEXT;
			break;
		case 19:
			// Bitmap button
			_mode = DRAW_MODE_BITMAP;
			break;
		case 20:
			// Bitmap transparent button
			_mode = DRAW_MODE_BITMAP_TRANSPARENT;
			break;
		case 21:
			// Floodfill button
			_mode = DRAW_MODE_FLOODFILL;
			break;
		case 22:
			// Copy button
			_mode = DRAW_MODE_COPY;
			break;
		case 23:
			// Dim button
			_mode = DRAW_MODE_DIM;
			break;
	}
}

void BitmapDrawing::handleTimer() {

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
		case DRAW_MODE_HORIZ_LINE:
			drawHorizLine();
			break;
		case DRAW_MODE_VERT_LINE:
			drawVertLine();
			break;
		case DRAW_MODE_CIRCLE:
			drawCircle();
			break;
		case DRAW_MODE_FILLED_CIRCLE:
			drawFilledCircle();
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
		case DRAW_MODE_FLOODFILL:
			drawFloodFill();
			break;
		case DRAW_MODE_COPY:
			drawCopy();
			break;
		case DRAW_MODE_DIM:
			drawDim();
			break;
		default:
			break;
	}
	
	// Redraw the superbitmap to the window
	_superBitmap->redraw();
}

void BitmapDrawing::drawLine() {

	// Generate random start/end co-ords for the line
	s16 x1 = rand() % 256;
	s16 y1 = rand() % 192;
	s16 x2 = rand() % 256;
	s16 y2 = rand() % 192;
	
	_graphics->drawLine(x1, y1, x2, y2, getRandomColour());
}

void BitmapDrawing::drawHorizLine() {

	// Generate random co-ords for the line
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	s16 width = rand() % 256;
	
	_graphics->drawHorizLine(x, y, width, getRandomColour());
}

void BitmapDrawing::drawVertLine() {

	// Generate random co-ords for the line
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	s16 height = rand() % 192;
	
	_graphics->drawVertLine(x, y, height, getRandomColour());
}

void BitmapDrawing::drawEllipse() {

	// Generate random co-ordinates and dimensions for the ellipse
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	s16 hRadius = rand() % 100;
	s16 vRadius = rand() % 100;
	
	_graphics->drawEllipse(x, y, hRadius, vRadius, getRandomColour());
}

void BitmapDrawing::drawFilledEllipse() {

	// Generate random co-ordinates and dimensions for the ellipse
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	s16 hRadius = rand() % 100;
	s16 vRadius = rand() % 100;
	
	_graphics->drawFilledEllipse(x, y, hRadius, vRadius, getRandomColour());
}

void BitmapDrawing::drawCircle() {

	// Generate random co-ordinates and radius for the circle
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	s16 radius = rand() % 100;
	
	_graphics->drawCircle(x, y, radius, getRandomColour());
}

void BitmapDrawing::drawFilledCircle() {

	// Generate random co-ordinates and radius for the circle
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	s16 radius = rand() % 100;
	
	_graphics->drawFilledCircle(x, y, radius, getRandomColour());
}

u16 BitmapDrawing::getRandomColour() {

	// Generate a random colour
	u8 r = rand() % 31;
	u8 g = rand() % 31;
	u8 b = rand() % 31;
	return woopsiRGB(r, g, b);
}

void BitmapDrawing::drawRect() {

	// Generate random start/end co-ords for the rect
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	s16 width = rand() % 256;
	s16 height = rand() % 192;
	
	_graphics->drawRect(x, y, width, height, getRandomColour());
}

void BitmapDrawing::drawFilledRect() {

	// Generate random start/end co-ords for the rect
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	s16 width = rand() % 256;
	s16 height = rand() % 192;
	
	_graphics->drawFilledRect(x, y, width, height, getRandomColour());
}

void BitmapDrawing::drawFilledXORRect() {

	// Generate random start/end co-ords for the rect
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	s16 width = rand() % 256;
	s16 height = rand() % 192;
	
	_graphics->drawFilledXORRect(x, y, width, height);
}

void BitmapDrawing::drawXORHorizLine() {

	// Generate random co-ords for the line
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	s16 width = rand() % 256;
	
	_graphics->drawXORHorizLine(x, y, width);
}

void BitmapDrawing::drawXORVertLine() {

	// Generate random co-ords for the line
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	s16 height = rand() % 192;
	
	_graphics->drawXORVertLine(x, y, height);
}

void BitmapDrawing::drawXORRect() {

	// Generate random start/end co-ords for the rect
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	s16 width = rand() % 256;
	s16 height = rand() % 192;
	
	_graphics->drawXORRect(x, y, width, height);
}

void BitmapDrawing::drawXORPixel() {

	// Generate random co-ords
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	
	_graphics->drawXORPixel(x, y);
}

void BitmapDrawing::drawPixel() {

	// Generate random co-ords
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	
	_graphics->drawPixel(x, y, getRandomColour());
}

void BitmapDrawing::drawText() {

	// Generate random co-ords
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	
	_graphics->drawText(x, y, getFont(), "Text!", 0, 5, getRandomColour());
}

void BitmapDrawing::drawBitmap() {

	// Generate random co-ords
	s16 sx = rand() % 100;
	s16 sy = rand() % 20;
	s16 dx = rand() % 256;
	s16 dy = rand() % 192;
	u16 width = 100 - sx;
	u16 height = 20 - sy;
	
	_graphics->drawBitmap(dx, dy, width, height, _bitmap, sx, sy);
}

void BitmapDrawing::drawBitmapTransparent() {

	// Generate random co-ords
	s16 sx = rand() % 100;
	s16 sy = rand() % 20;
	s16 dx = rand() % 256;
	s16 dy = rand() % 192;
	u16 width = 100 - sx;
	u16 height = 20 - sy;
	
	_graphics->drawBitmap(dx, dy, width, height, _bitmap, sx, sy, woopsiRGB(31, 0, 0));
}

void BitmapDrawing::drawFloodFill() {

	// Generate random co-ords
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	
	_graphics->floodFill(x, y, getRandomColour());
}

void BitmapDrawing::drawCopy() {

	// Generate random start/end co-ords for the rect
	s16 sx = rand() % 256;
	s16 sy = rand() % 192;
	s16 dx = rand() % 256;
	s16 dy = rand() % 192;
	s16 width = rand() % 256;
	s16 height = rand() % 192;
	
	_graphics->copy(sx, sy, dx, dy, width, height);
}

void BitmapDrawing::drawDim() {

	// Generate random start/end co-ords for the rect
	s16 x = rand() % 256;
	s16 y = rand() % 192;
	s16 width = rand() % 256;
	s16 height = rand() % 192;
	
	_graphics->dim(x, y, width, height);
}
