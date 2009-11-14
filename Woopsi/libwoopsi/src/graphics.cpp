#include "graphics.h"
#include "textwriter.h"
#include "woopsifuncs.h"

using namespace WoopsiUI;

// Constructor - allocates mem for bitmap
Graphics::Graphics(MutableBitmapBase* bitmap) : GraphicsUnclipped(bitmap) { }

// Draw a single pixel to the bitmap
void Graphics::drawPixel(s16 x, s16 y, u16 colour) {

	// Prevent overflows
	if ((x < 0) || (y < 0)) return;
	if ((x >= _width) || (y >= _height)) return;

	// Plot the pixel
	GraphicsUnclipped::drawPixel(x, y, colour);
}

// Get a single pixel from the bitmap
const u16 Graphics::getPixel(s16 x, s16 y) const {

	// Prevent overflows
	if ((x < 0) || (y < 0)) return 0;
	if ((x >= _width) || (y >= _height)) return 0;

	// Get the pixel
	return GraphicsUnclipped::getPixel(x, y);
}

void Graphics::drawFilledRect(s16 x, s16 y, u16 width, u16 height, u16 colour) {

	if (clipBitmapCoordinates(&x, &y, &width, &height)) {
		GraphicsUnclipped::drawFilledRect(x, y, width, height, colour);
	}
}

void Graphics::drawHorizLine(s16 x, s16 y, u16 width, u16 colour) {

	u16 height = 1;

	if (clipBitmapCoordinates(&x, &y, &width, &height)) {
		GraphicsUnclipped::drawHorizLine(x, y, width, colour);
	}
}

void Graphics::drawVertLine(s16 x, s16 y, u16 height, u16 colour) {

	u16 width = 1;

	if (clipBitmapCoordinates(&x, &y, &width, &height)) {
		GraphicsUnclipped::drawVertLine(x, y, height, colour);
	}
}

void Graphics::drawXORPixel(s16 x, s16 y) {

	// Prevent overflows
	if ((x < 0) || (y < 0)) return;
	if ((x >= _width) || (y >= _height)) return;

	// Get the pixel at the specified co-ords and XOR against 0xffff
	u32 pos = (y * _width) + x;
	u16 colour = (_data[pos] ^ 0xffff) | (1 << 15);

	// Draw the XORed pixel colour to the bitmap
	_data[pos] = colour;
}


void Graphics::drawXORHorizLine(s16 x, s16 y, u16 width) {

	u16 height = 1;

	if (clipBitmapCoordinates(&x, &y, &width, &height)) {
		GraphicsUnclipped::drawXORHorizLine(x, y, width);
	}
}

void Graphics::drawXORVertLine(s16 x, s16 y, u16 height) {

	u16 width = 1;

	if (clipBitmapCoordinates(&x, &y, &width, &height)) {
		GraphicsUnclipped::drawXORVertLine(x, y, height);
	}
}

// Scanline floodfill algorithm
void Graphics::floodFill(s16 x, s16 y, u16 newColour) {

	// Clip to bitmap
	if ((x < 0) || (y < 0)) return;
	if ((x >= _width) || (y >= _height)) return;

	// Get current colour
	u16 oldColour = getPixel(x, y);

	// Exit if colours match
	if (oldColour == newColour) return;

	// Initalise stack
	WoopsiArray<s32>* stack = new WoopsiArray<s32>();

	s16 x1; 
	u8 spanUp, spanDown;
	s32 rowThis, rowUp, rowDown;
	s16 rowStart;
	u16 rowWidth;

	// Push initial value onto the stack
	pushStack(x, y, stack);

	// Continue until stack is empty
	while (popStack(&x, &y, stack)) {

		x1 = x;
		rowThis = y * _width;
		rowUp = (y - 1) * _width;
		rowDown = (y + 1) * _width;

		// Locate leftmost column on screen in this row containing old colour
		while ((x1 >= 0) && (_data[x1 + rowThis] == oldColour)) {
			x1--;
		}

		// Move to first column of old colour
		x1++;

		rowStart = x1;
		rowWidth = 0;

		spanUp = spanDown = 0;

		// Scan right, filling each column of old colour
		while ((x1 < _width) && (_data[x1 + rowThis] == oldColour)) {

			// Check pixel above
			if ((!spanUp) && (y > 0) && (_data[x1 + rowUp] == oldColour)) {
				pushStack(x1, y - 1, stack);
				spanUp = 1;
			} else if ((spanUp) && (y > 0) && (_data[x1 + rowUp] != oldColour)) {
				spanUp = 0;
			}

			// Check pixel below
			if ((!spanDown) && (y < _height - 1) && (_data[x1 + rowDown] == oldColour)) {
				pushStack(x1, y + 1, stack);
				spanDown = 1;
			} else if ((spanDown) && (y < _height - 1) && (_data[x1 + rowDown] != oldColour)) {
				spanDown = 0;
			}

			x1++;
			rowWidth++;
		}

		// Draw line
		drawHorizLine(rowStart, y, rowWidth, newColour);
	}

	delete stack;
}

// Floodfill stack functions
bool Graphics::popStack(s16* x, s16* y, WoopsiArray<s32>* stack) { 
	if (stack->size() > 0) { 
		s32 val = stack->at(stack->size() - 1);
		*y = val / _width;
		*x = val % _width;
		stack->pop_back();
		return true;
	}

	return false;  
}

void Graphics::pushStack(s16 x, s16 y, WoopsiArray<s32>* stack) {
	stack->push_back(x + (y * _width));
} 

//Draw bitmap to the internal bitmap
void Graphics::drawBitmap(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16  bitmapY) {
	
	// Early exit conditions
	if (x > _width) return;
	if (y > _height) return;

	u16 bitmapWidth = bitmap->getWidth();
	u16 bitmapHeight = bitmap->getHeight();

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

	if (x + width > _width) {
		width = _width - x;
	}

	if (y + height > _height) {
		height = _height - y;
	}

	// Ensure requested drawing dimensions do not exceed dimensions of bitmap
	if (width > bitmapWidth - bitmapX) {
		width = bitmapWidth - bitmapX;
	}

	if (height > bitmapHeight - bitmapY) {
		height = bitmapHeight - bitmapY;
	}

	if ((width > 0) && (height > 0)) {
		GraphicsUnclipped::drawBitmap(x, y, width, height, bitmap, bitmapX, bitmapY);
	}
}

//Draw bitmap to the internal bitmap
void Graphics::drawBitmap(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16  bitmapY, u16 transparentColour) {
	
	// Early exit conditions
	if (x > _width) return;
	if (y > _height) return;

	u16 bitmapWidth = bitmap->getWidth();
	u16 bitmapHeight = bitmap->getHeight();

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

	if (x + width > _width) {
		width = _width - x;
	}

	if (y + height > _height) {
		height = _height - y;
	}

	// Ensure requested drawing dimensions do not exceed dimensions of bitmap
	if (width > bitmapWidth - bitmapX) {
		width = bitmapWidth - bitmapX;
	}

	if (height > bitmapHeight - bitmapY) {
		height = bitmapHeight - bitmapY;
	}

	if ((width > 0) && (height > 0)) {
		GraphicsUnclipped::drawBitmap(x, y, width, height, bitmap, bitmapX, bitmapY, transparentColour);
	}
}

void Graphics::copy(s16 sourceX, s16 sourceY, s16 destX, s16 destY, u16 width, u16 height) {
	if (clipBitmapCoordinates(&sourceX, &sourceY, &width, &height)) {
		if (clipBitmapCoordinates(&destX, &destY, &width, &height)) {
			GraphicsUnclipped::copy(sourceX, sourceY, destX, destY, width, height);
		}
	}
}

void Graphics::dim(s16 x, s16 y, u16 width, u16 height) {
	if (clipBitmapCoordinates(&x, &y, &width, &height)) {
		GraphicsUnclipped::dim(x, y, width, height);
	}
}

// Clip co-ordinates
bool Graphics::clipBitmapCoordinates(s16* x, s16* y, u16* width, u16* height) {

	s32 x2 = *x + *width - 1;
	s32 y2 = *y + *height - 1;

	// Ensure values don't exceed bitmap dimensions
	if (*x < 0) *x = 0;
	if (*y < 0) *y = 0;
	if (x2 > _width - 1) x2 = _width - 1;
	if (y2 > _height - 1) y2 = _height - 1;

	// Return false if no box to draw
	if ((x2 < *x) || (y2 < *y)) return false;

	// Update pointer values
	*width = (x2 - *x) + 1;
	*height = (y2 - *y) + 1;

	// Return true as box can be drawn
	return true;
}

u8 Graphics::getClipLineOutCode(s16 x, s16 y, s16 xMin, s16 yMin, s16 xMax, s16 yMax) {
	u8 code = 0;
	
	if (y > yMax) code |= 1;
	if (y < yMin) code |= 2;
	if (x > xMax) code |= 4;
	if (x < xMin) code |= 8;
	
	return code;
}

void Graphics::drawLine(s16 x1, s16 y1, s16 x2, s16 y2, u16 colour) {
	
	// Extract data from cliprect
	s16 minX = 0;
	s16 minY = 0;
	s16 maxX = _width - 1;
	s16 maxY = _height - 1;
		
	// Get outcodes for each point
	u8 code1 = getClipLineOutCode(x1, y1, minX, minY, maxX, maxY);
	u8 code2 = getClipLineOutCode(x2, y2, minX, minY, maxX, maxY);
	
	// Clip
	while (1) {
		// Check for trivial cases
		if (!(code1 | code2)) {
			
			// Line entirely within visible region
			// Draw the line
			GraphicsUnclipped::drawLine(x1, y1, x2, y2, colour);
			return;
		} else if (code1 & code2) {
			
			// Both end points fall within the same off-screen region
			return;
		} else {
			
			// No trivial accept
			s16 x = 0;
			s16 y = 0;
			s32 t = 0;
			u8 codeout;
			
			// Choose one of the end points to manipulate (only choose
			// the end point that is still off screen)
			codeout = code1 ? code1 : code2;
			
			// Check each region
			if (codeout & 1) {
				
				// Check top value
				t = ((maxY - y1) << 8) / (y2 - y1);
				x = x1 + ((t * (x2 - x1)) >> 8);
				y = maxY;
			} else if (codeout & 2) {
				
				// Check bottom value
				t = ((minY - y1) << 8) / (y2 - y1);
				x = x1 + ((t * (x2 - x1)) >> 8);
				y = minY;
			} else if (codeout & 4) {
				
				// Check right value
				t = ((maxX - x1) << 8) / (x2 - x1);
				y = y1 + ((t * (y2 - y1)) >> 8);
				x = maxX;
			} else if (codeout & 8) {
				
				// Check left value
				t = ((minX - x1) << 8) / (x2 - x1);
				y = y1 + ((t * (y2 - y1)) >> 8);
				x = minX;
			}
			
			// Check to see which endpoint we clipped
			if (codeout == code1) {
				// First endpoint clipped; update first point
				x1 = x;
				y1 = y;
				
				// Clip again
				code1 = getClipLineOutCode(x1, y1, minX, minY, maxX, maxY);
			} else {
				// Second endpoint clipped; update second point
				x2 = x;
				y2 = y;
				
				// Clip again
				code2 = getClipLineOutCode(x2, y2, minX, minY, maxX, maxY);
			}
		}
	}
}
