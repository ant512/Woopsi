#include "graphicsunclipped.h"
#include "textwriter.h"
#include "woopsifuncs.h"

using namespace WoopsiUI;

// Constructor - allocates mem for bitmap
GraphicsUnclipped::GraphicsUnclipped(MutableBitmapBase* bitmap) {
	_bitmap = bitmap;
	_data = bitmap->getEditableData();
	_width = bitmap->getWidth();
	_height = bitmap->getHeight();
}

// Draw a single pixel to the bitmap
void GraphicsUnclipped::drawPixel(s16 x, s16 y, u16 colour) {

	// Plot the pixel
	u32 pos = (y * _width) + x;
	_data[pos] = colour;
}

// Get a single pixel from the bitmap
const u16 GraphicsUnclipped::getPixel(s16 x, s16 y) const {

	// Get the pixel
	u32 pos = (y * _width) + x;
	return _data[pos];
}

// External filled rectangle function
void GraphicsUnclipped::drawFilledRect(s16 x, s16 y, u16 width, u16 height, u16 colour) {
	
	// Target location to draw to
	u16* target = _data + (y * _width) + x;

	for (u16 i = 0; i < height; i++) {

		// Duplicate pixel
		woopsiDmaFill(colour, target, width);

		// Move to next row
		target += _width;
	}
}

void GraphicsUnclipped::drawHorizLine(s16 x, s16 y, u16 width, u16 colour) {

	u16* pos = _data + (y * _width) + x;

	woopsiDmaFill(colour, pos, width);
}

void GraphicsUnclipped::drawVertLine(s16 x, s16 y, u16 height, u16 colour) {

	u32 pos = x + (y * _width);

	for (u16 i = 0; i < height; i++) {
		_data[pos] = colour;
		pos += _width;
	}
}

void GraphicsUnclipped::drawRect(s16 x, s16 y, u16 width, u16 height, u16 colour) {

	drawHorizLine(x, y + height - 1, width, colour);	// Bottom
	drawVertLine(x + width - 1, y, height, colour);		// Right

	drawHorizLine(x, y, width, colour);					// Top
	drawVertLine(x, y, height, colour);					// Left
}

void GraphicsUnclipped::drawLine(s16 x1, s16 y1, s16 x2, s16 y2, u16 colour) {
	int dx, dy, inx, iny, e;
	
	dx = x2 - x1;
	dy = y2 - y1;
	inx = dx > 0 ? 1 : -1;
	iny = dy > 0 ? 1 : -1;

	if (dx < 0) dx = -dx;
	if (dy < 0) dy = -dy;
	
	if (dx >= dy) {
		dy <<= 1;
		e = dy - dx;
		dx <<= 1;
		while (x1 != x2) {
			drawPixel(x1, y1,colour);
			if (e >= 0) {
					y1 += iny;
					e-= dx;
			}
			e += dy; x1 += inx;
		}
	} else {
		dx <<= 1;
		e = dx - dy;
		dy <<= 1;
		while (y1 != y2) {
			drawPixel(x1, y1, colour);
			if (e >= 0) {
					x1 += inx;
					e -= dy;
			}
			e += dx; y1 += iny;
		}
	}
	drawPixel(x1, y1, colour);
}

void GraphicsUnclipped::drawCircle(s16 x0, s16 y0, u16 radius, u16 colour) {
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

void GraphicsUnclipped::drawFilledCircle(s16 x0, s16 y0, u16 radius, u16 colour) {
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

void GraphicsUnclipped::drawText(s16 x, s16 y, FontBase* font, const char* string) {
	TextWriter::drawString(_bitmap, font, string, strlen(string), x, y, 0, 0, _width - 1, _height - 1);
}

void GraphicsUnclipped::drawText(s16 x, s16 y, FontBase* font, u16 length, const char* string) {
	TextWriter::drawString(_bitmap, font, string, length, x, y, 0, 0, _width - 1, _height - 1);
}

// Print a string in a specific colour
void GraphicsUnclipped::drawText(s16 x, s16 y, FontBase* font, const char* string, u16 colour) {

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

// Print a single character in a specific colour
void GraphicsUnclipped::drawText(s16 x, s16 y, FontBase* font, char letter, u16 colour) {
	
	// Store current font colour
	bool isMonochrome = font->isMonochrome();
	u16 oldColour = font->getColour();
	
	// Update font colour
	font->setColour(colour);
	
	// Output as normal
	drawText(x, y, font, letter);
	
	// Reset colour
	if (!isMonochrome) {
		font->clearColour();
	} else {
		font->setColour(oldColour);
	}
}

// Print a single character
void GraphicsUnclipped::drawText(s16 x, s16 y, FontBase* font, char letter) {
	
	// Convert char to char[]
	char text[2];
	text[0] = letter;
	text[1] = '\0';
	
	// Output as normal
	drawText(x, y, font, text);
}

// Print a string in a specific colour
void GraphicsUnclipped::drawText(s16 x, s16 y, FontBase* font, u16 length, const char* string, u16 colour) {
	
	// Store current font colour
	bool isMonochrome = font->isMonochrome();
	u16 oldColour = font->getColour();
	
	// Update font colour
	font->setColour(colour);
	
	// Output as normal
	drawText(x, y, font, length, string);
	
	// Reset colour
	if (!isMonochrome) {
		font->clearColour();
	} else {
		font->setColour(oldColour);
	}
}

//Draw bitmap to the internal bitmap
void GraphicsUnclipped::drawBitmap(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16 bitmapY) {
	
	u16 bitmapWidth = bitmap->getWidth();
	const u16* bitmapData = bitmap->getData();

	// Precalculate line values for loop
	u16* srcLine0 = (u16*)bitmapData + bitmapX + (bitmapY * bitmapWidth);

	u16* srcLinei = srcLine0;

	u16* destLine0 = _data + x + (y * _width);
	u16* destLinei = destLine0;

	u16 lastLine = y + height;

	for (u16 i = y; i < lastLine; i++) {

		woopsiDmaCopy(srcLinei, destLinei, width);

		srcLinei += bitmapWidth;
		destLinei += _width;
	}
}

//Draw bitmap with transparency
void GraphicsUnclipped::drawBitmap(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16 bitmapY, u16 transparentColour) {

	u16 source = 0;
	const u16* bitmapData = bitmap->getData();
	u16 bitmapWidth = bitmap->getWidth();
	
	// Plot pixels one by one, ignoring transparent pixels
	for (s16 i = 0; i < width; i++) {
		for (s16 j = 0; j < height; j++) {
			
			// Get the source colour from the supplied bitmap
			source = bitmapData[((bitmapY + j) * bitmapWidth) + (bitmapX + i)];
			
			// Plot ignoring transparency
			if (source != transparentColour) {
				_data[((y + j) * _width) + (x + i)] = source;
			}
		}
	}
}

// Code borrowed from http://enchantia.com/software/graphapp/doc/tech/ellipses.html
// and partially rendered readable.  This is L. Patrick's implementation of Doug
// McIlroy's ellipse algorithm.
void GraphicsUnclipped::drawEllipse(s16 xCentre, s16 yCentre, s16 horizRadius, s16 vertRadius, u16 colour) {

	s16 x = 0;
	s16 y = vertRadius;

	// Precalculate squares of axes for speed
	s32 horizSquare = horizRadius * horizRadius;
	s32 vertSquare = vertRadius * vertRadius;

	s32 crit1 = -((horizSquare >> 2) + (horizRadius % 2) + vertSquare);
	s32 crit2 = -((vertSquare >> 2) + (vertRadius % 2) + horizSquare);
	s32 crit3 = -((vertSquare >> 2) + (vertRadius % 2));

	s32 t = -horizSquare * y;
	s32 dxt = vertSquare * x << 1;
	s32 dyt = -horizSquare * y << 1;
	s32 d2xt = vertSquare << 1;
	s32 d2yt = horizSquare << 1;

	// Loop until the ellipse is drawn - each iteration draws a point in every
	// quarter of the ellipse
	while ((y >= 0) && (x <= horizRadius)) {

		// Draw this point
		drawPixel(xCentre + x, yCentre + y, colour);

		// Draw the opposite point only if it will not overlap the first
		if ((x != 0) || (y != 0)) {
			drawPixel(xCentre - x, yCentre - y, colour);
		}

		// Draw the next two points if they will not overlap the previous
		// two points
		if ((x != 0) && (y != 0)) {
			drawPixel(xCentre + x, yCentre - y, colour);
			drawPixel(xCentre - x, yCentre + y, colour);
		}

		if ((t + (vertSquare * x) <= crit1) || (t + (horizSquare * y) <= crit3)) {

			// Inc x
			x++;
			dxt += d2xt;
			t += dxt;
		} else if (t - (horizSquare * y) > crit2) {

			// Inc y
			y--;
			dyt += d2yt;
			t += dyt;
		} else {

			// Inc x
			x++;
			dxt += d2xt;
			t += dxt;

			// Inc y
			y--;
			dyt += d2yt;
			t += dyt;
		}
	}
}

// Code borrowed from http://enchantia.com/software/graphapp/doc/tech/ellipses.html
// This is L. Patrick's implementation of a filled ellipse algorithm that uses
// filled rects to draw the ellipse. 
void GraphicsUnclipped::drawFilledEllipse(s16 xCentre, s16 yCentre, s16 horizRadius, s16 vertRadius, u16 colour) {

	/* e(x,y) = b^2*x^2 + a^2*y^2 - a^2*b^2 */
	s16 x = 0;
	s16 y = vertRadius;
	
	s16 rx = x;
	s16 ry = y;
	u16 width = 1;
	u16 height = 1;
	
	s32 horizSquare = horizRadius * horizRadius;
	s32 vertSquare = vertRadius * vertRadius;
	
	s32 crit1 = -((horizSquare >> 2) + (horizRadius % 2) + vertSquare);
	s32 crit2 = -((vertSquare >> 2) + (vertRadius % 2) + horizSquare);
	s32 crit3 = -((vertSquare >> 2) + (vertRadius % 2));
	
	s32 t = -horizSquare * y;
	s32 dxt = vertSquare * x << 1;
	s32 dyt = -horizSquare * y << 1;
	s32 d2xt = vertSquare << 1;
	s32 d2yt = horizSquare << 1;
		
	if (vertRadius == 0) {
		drawFilledRect(xCentre - horizRadius, yCentre, (horizRadius << 1) + 1, 1, colour);
		return;
	}
	
	while ((y >= 0) && (x <= horizRadius)) {
		
		if ((t + (vertSquare * x) <= crit1) || (t + (horizSquare * y) <= crit3)) {
			
			if (height == 1) {
				// Draw nothing
			} else if (ry * 2 + 1 > (height - 1) * 2) {
				drawFilledRect(xCentre - rx, yCentre - ry, width, height - 1, colour);
				drawFilledRect(xCentre - rx, yCentre + ry + 1 - height, width, height - 1, colour);
				ry -= height-1;
				height = 1;
			} else {
				drawFilledRect(xCentre - rx, yCentre - ry, width, ry * 2 + 1, colour);
				ry -= ry;
				height = 1;
			}
				
			// Inc x
			x++;
			dxt += d2xt;
			t += dxt;
			
			rx++;
			width += 2;
		} else if (t - (horizSquare * y) > crit2) { /* e(x+1/2,y-1) > 0 */

			// Inc y
			y--;
			dyt += d2yt;
			t += dyt;
			
			height++;
		} else {
			if (ry*2+1 > height*2) {
				drawFilledRect(xCentre - rx, yCentre - ry, width, height, colour);
				drawFilledRect(xCentre - rx, yCentre + ry + 1 - height, width, height, colour);
			} else {
				drawFilledRect(xCentre - rx, yCentre - ry, width, ry * 2 + 1, colour);
			}
			
			// Inc x
			x++;
			dxt += d2xt;
			t += dxt;
			
			// Inc y
			y--;
			dyt += d2yt;
			t += dyt;
			
			rx++;
			width += 2;
			ry -= height;
			height = 1;
		}
	}
		
	if (ry > height) {
		drawFilledRect(xCentre - rx, yCentre - ry, width, height, colour);
		drawFilledRect(xCentre - rx, yCentre + ry + 1 - height, width, height, colour);
	} else {
		drawFilledRect(xCentre - rx, yCentre - ry, width, ry * 2 + 1, colour);
	}
}

void GraphicsUnclipped::drawXORPixel(s16 x, s16 y) {

	// Get the pixel at the specified co-ords and XOR against 0xffff
	u32 pos = (y * _width) + x;
	u16 colour = (_data[pos] ^ 0xffff) | (1 << 15);

	// Draw the XORed pixel colour to the bitmap
	_data[pos] = colour;
}

void GraphicsUnclipped::drawXORHorizLine(s16 x, s16 y, u16 width) {
	for (u16 i = 0; i < width; i++) {
		drawXORPixel(x + i, y);
	}
}

void GraphicsUnclipped::drawXORVertLine(s16 x, s16 y, u16 height) {
	for (u16 i = 0; i < height; i++) {
		drawXORPixel(x, y + i);
	}
}

void GraphicsUnclipped::drawXORRect(s16 x, s16 y, u16 width, u16 height) {
	
	// Performs bounds checks so that individual rect lines are omitted if
	// they fall outside the bounds of the bitmap

	// Top
	if ((y >= 0) && (y < _height)) {
		drawXORHorizLine(x, y, width);
	}
	
	// Bottom
	if (y + height < _height) {
		drawXORHorizLine(x, y + height - 1, width);
	}
	
	// Left
	if ((x >= 0) && (x < _width)) {
		drawXORVertLine(x, y + 1, height - 2);
	}
	
	// Right
	if (x + width < _width) {
		drawXORVertLine(x + width - 1, y + 1, height - 2);
	}
}

void GraphicsUnclipped::drawFilledXORRect(s16 x, s16 y, u16 width, u16 height) {
	for (s32 j = 0; j < height; j++) {
		drawXORHorizLine(x, y + j, width);
	}
}

void GraphicsUnclipped::copy(s16 sourceX, s16 sourceY, s16 destX, s16 destY, u16 width, u16 height) {
	
	// Do nothing if no copying involved
	if ((sourceX == destX) && (sourceY == destY)) return;
	
	// If there is no vertical movement and the source and destination overlap,
	// we need to use an offscreen buffer to copy
	if (sourceY == destY) {
		if ((destX >= sourceX) && (destX <= sourceX + width)) {
			u16* buffer = new u16[width];
			u16* copySource = _data + sourceX + (sourceY * _width);
			u16* copyDest = _data + destX + (destY * _width);
			
			for (u16 y = 0; y < height; y++) {
				
				// Copy row to buffer
				woopsiDmaCopy(copySource, buffer, width);
				
				// Copy row back to screen
				woopsiDmaCopy(buffer, copyDest, width);
				
				copySource += _width;
				copyDest += _width;
			}
			
			delete buffer;
			
			return;
		}
	}
	
	// Vertical movement or non overlap means we do not need to use an intermediate buffer
	// Copy from top to bottom if moving up; from bottom to top if moving down.
	// Ensures that rows to be copied are not overwritten
	s16 delta;
	u16* copySource;
	u16* copyDest;
	
	if (sourceY > destY) {
		
		// Copy up
		delta = _width;
		copySource = _data + sourceX + (sourceY * _width);
		copyDest = _data + destX + (destY * _width);
	} else {
		
		// Copy down
		delta = -_width;
		copySource = _data + sourceX + ((sourceY + height - 1) * _width);
		copyDest = _data + destX + ((destY + height - 1) * _width);
	}
	
	// Perform copy	
	for (u16 i = 0; i < height; ++i) {
		
		// Copy row
		woopsiDmaCopy(copySource, copyDest, width);
		
		copySource += delta;
		copyDest += delta;
	}
}

void GraphicsUnclipped::dim(s16 x, s16 y, u16 width, u16 height) {
	
	s16 x2 = x + width - 1;
	s16 y2 = y + height - 1;

	u16* rowStart = _data + (y * _width);
	u16 colStart = x;
	u16 colPos;

	// Loop through all pixels within the region
	for (s16 i = y; i <= y2; i++) {

		// Reset to start pixel row
		colPos = colStart;

		for (s16 j = x; j <= x2; j++) {
	
			// Get pixel data directly from the framebuffer
			u16 colour = *(rowStart + colPos);
		
			// Halve the intensity of the colour (cheers Jeff)
			colour = ((colour  >> 1) & (15 | (15 << 5) | (15 << 10)));

			// Write back to framebuffer
			*(rowStart + colPos) = 0x8000 | colour;

			// Move to next pixel column
			colPos++;
		}

		// Move to next pixel row
		rowStart += _width;
	}
}
		