#include "superbitmap.h"

// Constructor - allocates mem for bitmap
SuperBitmap::SuperBitmap(s16 x, s16 y, u16 width, u16 height, u16 bitmapWidth, u16 bitmapHeight, bool isDecoration, FontBase* font) : Gadget(x, y, width, height, GADGET_BORDERLESS, font) {
	_bitmapWidth = bitmapWidth;
	_bitmapHeight = bitmapHeight;

	_bitmapX = 0;
	_bitmapY = 0;

	_outline = OUTLINE_OUT;

	_flags.decoration = isDecoration;

	if (isDecoration) {
		_flags.borderless = true;
		_allowStylusScroll = false;
	} else {
		_allowStylusScroll = true;
	}

	initBitmap();
}

SuperBitmap::~SuperBitmap() {
	delete[] _bitmap;
}

// Set up initial bitmap
void SuperBitmap::initBitmap() {
	// Allocate memory for bitmap
	_bitmap = new u16[_bitmapWidth * _bitmapHeight];

	clearBitmap();
}

void SuperBitmap::clearBitmap() {
	drawFilledRect(0, 0, _bitmapWidth, _bitmapHeight, _backColour);
}

void SuperBitmap::setAllowStylusScroll(bool allowStylusScroll) {
	_allowStylusScroll = allowStylusScroll;
}

// Copy bitmap to gadget
void SuperBitmap::draw() {
	Gadget::draw();
}

void SuperBitmap::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	s16 x = !_flags.borderless;
	s16 y = x;

	// Flush out the bitmap mem cache to ensure DMA can see correct data
	DC_FlushRange(_bitmap, _bitmapWidth * _bitmapHeight * sizeof(u16));

	port->drawBitmap(x, y, _width, _height, _bitmap, _bitmapX, _bitmapY, _bitmapWidth, _bitmapHeight);

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);
	
	delete port;
}

// Draw a single pixel to the bitmap
void SuperBitmap::drawPixel(s16 x, s16 y, u16 colour) {

	// Prevent overflows
	if ((x < 0) || (y < 0)) return;
	if ((x >= _bitmapWidth) || (y >= _bitmapHeight)) return;

	// Plot the pixel
	u32 pos = (y * _bitmapWidth) + x;
	_bitmap[pos] = colour;
}

// Get a single pixel from the bitmap
u16 SuperBitmap::getPixel(s16 x, s16 y) {

	// Prevent overflows
	if ((x < 0) || (y < 0)) return 0;
	if ((x >= _bitmapWidth) || (y >= _bitmapHeight)) return 0;

	u16 pos = x + (y * _bitmapWidth);
	return _bitmap[pos];
}

// Return the bitmap
u16* SuperBitmap::getBitmap() {
	return _bitmap;
}

// External filled rectangle function
void SuperBitmap::drawFilledRect(s16 x, s16 y, u16 width, u16 height, u16 colour) {

	if (clipBitmapCoordinates(&x, &y, &width, &height)) {

		// Draw initial pixel to heap
		u16* pos = new u16;
		*pos = colour;

		// Ensure DMA can see latest memory state
		DC_FlushRange(pos, sizeof(u16));
		
		// Target location to draw to
		u16* target = _bitmap + (y * _bitmapWidth) + x;

		for (u16 i = 0; i < height; i++) {
			// Wait until DMA channel is clear
			while (DMA_Active());

			// Duplicate pixel
			DMA_Force(*pos, target, width, DMA_16NOW);

			// Move to next row
			target += _bitmapWidth;
		}

		delete pos;
	}
}

void SuperBitmap::drawHorizLine(s16 x, s16 y, u16 width, u16 colour) {

	u16 height = 1;

	if (clipBitmapCoordinates(&x, &y, &width, &height)) {
		// Draw initial pixel
		u16* pos = _bitmap + (y * _bitmapWidth) + x;
		*pos = colour;

		// Ensure DMA can see latest memory state
		DC_FlushRange(pos, sizeof(u16));

		// Wait until DMA channel is clear
		while (DMA_Active());

		// Duplicate pixel
		DMA_Force(*pos, (pos + 1), width - 1, DMA_16NOW);
	}
}

void SuperBitmap::drawVertLine(s16 x, s16 y, u16 height, u16 colour) {

	u16 width = 1;

	if (clipBitmapCoordinates(&x, &y, &width, &height)) {

		u32 pos = x + (y * _bitmapWidth);

		for (u16 i = 0; i < height; i++) {
			_bitmap[pos] = colour;
			pos += _bitmapWidth;
		}
	}
}

void SuperBitmap::drawRect(s16 x, s16 y, u16 width, u16 height, u16 colour) {

	drawHorizLine(x, y + height - 1, width, colour);	// Bottom
	drawVertLine(x + width - 1, y, height, colour);		// Right

	drawHorizLine(x, y, width, colour);					// Top
	drawVertLine(x, y, height, colour);					// Left
}

void SuperBitmap::drawLine(s16 x1, s16 y1, u16 x2, u16 y2, u16 colour) {
	int slope;
	int dx, dy, incE, incNE, d, x, y;

	// Reverse lines where x1 > x2
	if (x1 > x2) {
		drawLine(x2, y2, x1, y1, colour);
		return;
	}

	dx = x2 - x1;
	dy = y2 - y1;

	// Adjust y-increment for negatively sloped lines
	if (dy < 0) {
		slope = -1;
		dy = -dy;
	} else {
		slope = 1;
	}

	// Bresenham constants
	incE = 2 * dy;
	incNE = 2 * dy - 2 * dx;
	d = 2 * dy - dx;
	y = y1;

	// Blit
	for (x = x1; x <= x2; x++) {
		drawPixel(x, y, colour);
		if (d <= 0) {
			d += incE;
		} else {
			d += incNE;
			y += slope;
		}
	}
}

void SuperBitmap::drawCircle(s16 x0, s16 y0, u16 radius, u16 colour) {
	s16 f = 1 - radius;
	s16 ddF_x = 0;
	s16 ddF_y = -2 * radius;
	s16 x = 0;
	s16 y = radius;

	drawPixel(x0, y0 + radius, colour);
	drawPixel(x0, y0 - radius, colour);
	drawPixel(x0 + radius, y0, colour);
	drawPixel(x0 - radius, y0, colour);

	while(x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x + 1;    
		drawPixel(x0 + x, y0 + y, colour);
		drawPixel(x0 - x, y0 + y, colour);
		drawPixel(x0 + x, y0 - y, colour);
		drawPixel(x0 - x, y0 - y, colour);
		drawPixel(x0 + y, y0 + x, colour);
		drawPixel(x0 - y, y0 + x, colour);
		drawPixel(x0 + y, y0 - x, colour);
		drawPixel(x0 - y, y0 - x, colour);
	}
}

void SuperBitmap::drawFilledCircle(s16 x0, s16 y0, u16 radius, u16 colour) {
	s16 f = 1 - radius;
	s16 ddF_x = 0;
	s16 ddF_y = -2 * radius;
	s16 x = 0;
	s16 y = radius;

	// Draw central line
	drawHorizLine(x0 - radius, y0, (radius << 1) + 1, colour);

	while(x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x + 1;

		drawHorizLine(x0 - x, y0 + y, (x << 1) + 1, colour);
		drawHorizLine(x0 - x, y0 - y, (x << 1) + 1, colour);

		drawHorizLine(x0 - y, y0 + x, (y << 1) + 1, colour);
		drawHorizLine(x0 - y, y0 - x, (y << 1) + 1, colour);
	}
}

void SuperBitmap::drawText(s16 x, s16 y, FontBase* font, char* string) {
	TextWriter::drawString(_bitmap, _bitmapWidth, _bitmapHeight, font, string, strlen(string), x, y, 0, 0, _bitmapWidth - 1, _bitmapHeight - 1);
}

// Print a string in a specific colour
void SuperBitmap::drawText(s16 x, s16 y, FontBase* font, char* string, u16 colour) {

	// Store current font colour
	bool isMonochrome = font->isMonochrome();
	u16 oldColour = font->getColour();

	// Update font colour
	font->setColour(colour);

	// Output as normal
	drawText(x, y, font, string);

	// Reset colour
	if (!isMonochrome) {
		font->clearColour();
	} else {
		font->setColour(oldColour);
	}
}

bool SuperBitmap::drag(s16 x, s16 y, s16 vX, s16 vY) {

	if (_flags.enabled) {
		if (_flags.clicked && _allowStylusScroll) {
			_bitmapX -= vX;
			_bitmapY -= vY;

			// Prevent scrolling outside boundaries of bitmap
			if (_bitmapX < 0) {
				_bitmapX = 0;
			} else if (_bitmapX > _bitmapWidth - _width) {
				_bitmapX = _bitmapWidth - _width;
			}

			if (_bitmapY < 0) {
				_bitmapY = 0;
			} else if (_bitmapY > _bitmapHeight - _height) {
				_bitmapY = _bitmapHeight - _height;
			}

			// Redraw the gadget
			draw();

			raiseDragEvent(x, y, vX, vY);

			return true;
		}
	}

	return false;
}

// Scanline floodfill algorithm
void SuperBitmap::floodFill(s16 x, s16 y, u16 newColour) {

	// Clip to bitmap
	if ((x < 0) || (y < 0)) return;
	if ((x >= _bitmapWidth) || (y >= _bitmapHeight)) return;

	// Get current colour
	u16 oldColour = getPixel(x, y);

	// Exit if colours match
	if (oldColour == newColour) return;

	// Initalise stack
	vector<u16> stack;

	s16 x1; 
	u8 spanUp, spanDown;
	u16 rowThis, rowUp, rowDown;
	s16 rowStart;
	s16 rowWidth;

	// Push initial value onto the stack
	pushStack(x, y, &stack);

	// Continue until stack is empty
	while (popStack(&x, &y, &stack)) {

		x1 = x;
		rowThis = y * _bitmapWidth;
		rowUp = (y - 1) * _bitmapHeight;
		rowDown = (y + 1) * _bitmapWidth;

		// Locate leftmost column on screen in this row containing old colour
		while ((x1 >= 0) && (_bitmap[x1 + rowThis] == oldColour)) {
			x1--;
		}

		// Move to first column of old colour
		x1++;

		rowStart = x1;
		rowWidth = 0;

		spanUp = spanDown = 0;

		// Scan right, filling each column of old colour
		while ((x1 < _bitmapWidth) && (_bitmap[x1 + rowThis] == oldColour)) {

			// Check pixel above
			if ((!spanUp) && (y > 0) && (_bitmap[x1 + rowUp] == oldColour)) {
				pushStack(x1, y - 1, &stack);
				spanUp = 1;
			} else if ((spanUp) && (y > 0) && (_bitmap[x1 + rowUp] != oldColour)) {
				spanUp = 0;
			}

			// Check pixel below
			if ((!spanDown) && (y < _bitmapHeight - 1) && (_bitmap[x1 + rowDown] == oldColour)) {
				pushStack(x1, y + 1, &stack);
				spanDown = 1;
			} else if (spanDown && (y < _bitmapHeight - 1) && (_bitmap[x1 + rowDown] != oldColour)) {
				spanDown = 0;
			}

			x1++;
			rowWidth++;
		}

		// Draw line
		drawHorizLine(rowStart, y, rowWidth, newColour);
	}
}

// Floodfill stack functions
bool SuperBitmap::popStack(s16* x, s16* y, vector<u16>* stack) { 
	if (stack->size() > 0) { 
		u16 val = stack->at(stack->size() - 1);
		*y = val / _bitmapWidth;
		*x = val % _bitmapWidth;
		stack->pop_back();
		return true;
	} else {
		return false;
	}    
}

void SuperBitmap::pushStack(s16 x, s16 y, vector<u16>* stack) {
	stack->push_back(x + (y << 8));
}     

//Draw bitmap to the internal bitmap
void SuperBitmap::drawBitmap(s16 x, s16 y, u16 width, u16 height, const u16* bitmap, s16 bitmapX, s16  bitmapY, u16 bitmapWidth, u16 bitmapHeight) {
	
	// Ensure bitmap co-ordinates make sense
	if (bitmapX < 0) {
		bitmapX = 0;
	}

	if (bitmapY < 0) {
		bitmapY = 0;
	}

	// Ensure dimensions of bitmap being drawn do not exceed size of bitmap RAM
	if (x < 0) {
		bitmapX -= x;
		width += x;
		x = 0;
	}

	if (y < 0) {
		bitmapY -= y;
		height += y;
		y = 0;
	}

	if (x + width > _bitmapWidth) {
		width = _bitmapWidth - x;
	}

	if (y + height > _bitmapHeight) {
		height = _bitmapHeight - y;
	}

	// Ensure requested drawing dimensions do not exceed dimensions of bitmap
	if (width > bitmapWidth - bitmapX) {
		width = bitmapWidth - bitmapX;
	}

	if (height > bitmapHeight - bitmapY) {
		height = bitmapHeight - bitmapY;
	}

	if ((width > 0) && (height > 0)) {

		// Precalculate line values for loop
		u16* srcLine0 = (u16*)bitmap + bitmapX + (bitmapY * bitmapWidth);

		u16* srcLinei = srcLine0;

		u16* destLine0 = _bitmap + x + (y * _bitmapWidth);
		u16* destLinei = destLine0;

		u16 lastLine = y + height;

		for (u16 i = y; i < lastLine; i++) {

			// Wait until DMA channel is clear
			while (DMA_Active());

			DMA_Copy(srcLinei, destLinei, width, DMA_16NOW);

			srcLinei += bitmapWidth;
			destLinei += _bitmapWidth;
		}
	}
}

// Clip co-ordinates
bool SuperBitmap::clipBitmapCoordinates(s16* x, s16* y, u16* width, u16* height) {

	s32 x2 = *x + *width - 1;
	s32 y2 = *y + *height - 1;

	// Ensure values don't exceed bitmap dimensions
	if (*x < 0) *x = 0;
	if (*y < 0) *y = 0;
	if (x2 > _bitmapWidth - 1) x2 = _bitmapWidth - 1;
	if (y2 > _bitmapHeight - 1) y2 = _bitmapHeight - 1;

	// Return false if no box to draw
	if ((x2 < *x) || (y2 < *y)) return false;

	// Update pointer values
	*width = (x2 - *x) + 1;
	*height = (y2 - *y) + 1;

	// Return true as box can be drawn
	return true;
}
