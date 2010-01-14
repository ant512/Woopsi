#include "graphicsunclipped.h"
#include "woopsifuncs.h"
#include "stringiterator.h"

using namespace WoopsiUI;

// Constructor - allocates mem for bitmap
GraphicsUnclipped::GraphicsUnclipped(MutableBitmapBase* bitmap) {
	_bitmap = bitmap;
	_width = bitmap->getWidth();
	_height = bitmap->getHeight();
}

void GraphicsUnclipped::drawPixel(s16 x, s16 y, u16 colour) {
	_bitmap->setPixel(x, y, colour);
}

const u16 GraphicsUnclipped::getPixel(s16 x, s16 y) const {
	return _bitmap->getPixel(x, y);
}

void GraphicsUnclipped::drawFilledRect(s16 x, s16 y, u16 width, u16 height, u16 colour) {
	for (u16 i = 0; i < height; i++) {
		GraphicsUnclipped::drawHorizLine(x, y + i, width, colour);
	}
}

void GraphicsUnclipped::drawHorizLine(s16 x, s16 y, u16 width, u16 colour) {
	_bitmap->blitFill(x, y, colour, width);
}

void GraphicsUnclipped::drawVertLine(s16 x, s16 y, u16 height, u16 colour) {
	for (u16 i = 0; i < height; i++) {
		_bitmap->setPixel(x, y + i, colour);
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
			GraphicsUnclipped::drawPixel(x1, y1,colour);
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
			GraphicsUnclipped::drawPixel(x1, y1, colour);
			if (e >= 0) {
					x1 += inx;
					e -= dy;
			}
			e += dx; y1 += iny;
		}
	}
	GraphicsUnclipped::drawPixel(x1, y1, colour);
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

void GraphicsUnclipped::drawText(s16 x, s16 y, FontBase* font, const WoopsiString& string) {
	drawText(x, y, font, string, 0, string.getLength());
}

void GraphicsUnclipped::drawText(s16 x, s16 y, FontBase* font, const WoopsiString& string, u32 startIndex, u32 length) {

	// Draw the string char by char
	StringIterator* iterator = string.newStringIterator();
	
	if (iterator->moveTo(startIndex)) {
		do {
			x = font->drawChar(_bitmap, iterator->getCodePoint(), x, y, 0, 0, _width - 1, _height - 1);

			// Abort if x pos outside bitmap
			if (x > _width - 1) break;
		} while (iterator->moveToNext() && (iterator->getIndex() < startIndex + length));
	}

	delete iterator;
}

// Print a string in a specific colour
void GraphicsUnclipped::drawText(s16 x, s16 y, FontBase* font, const WoopsiString& string, u32 startIndex, u32 length, u16 colour) {

	// Store current font colour
	bool isMonochrome = font->isMonochrome();
	u16 oldColour = font->getColour();

	// Update font colour
	font->setColour(colour);

	// Output as normal
	drawText(x, y, font, string, startIndex, length);

	// Reset colour
	if (!isMonochrome) {
		font->clearColour();
	} else {
		font->setColour(oldColour);
	}
}

//Draw bitmap to the internal bitmap
void GraphicsUnclipped::drawBitmap(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16 bitmapY) {
	for (u16 i = 0; i < height; i++) {
		_bitmap->blit(x, y + i, bitmap->getData(bitmapX, bitmapY + i), width);
	}
}

//Draw bitmap with transparency
void GraphicsUnclipped::drawBitmap(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16 bitmapY, u16 transparentColour) {

	u16 source = 0;
	
	// Plot pixels one by one, ignoring transparent pixels
	for (s16 i = 0; i < width; i++) {
		for (s16 j = 0; j < height; j++) {

			source = bitmap->getPixel(bitmapX + i, bitmapY + j);
			
			// Plot ignoring transparency
			if (source != transparentColour) {
				_bitmap->setPixel(x + i, y + j, source);
			}
		}
	}
}

//Draw bitmap with transparency
void GraphicsUnclipped::drawBitmapGreyScale(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16 bitmapY) {

	u16 colour = 0;
	
	// Plot pixels one by ones
	for (s16 i = 0; i < width; i++) {
		for (s16 j = 0; j < height; j++) {

			colour = bitmap->getPixel(bitmapX + i, bitmapY + j);

			// Extract individual components
			colour >>= 1;
			u8 r = (colour >> 1) & 7;
			u8 g = (colour >> 5) & 15;
			u8 b = (colour >> 11) & 7;

			colour = r + g + b;
			colour = colour | (colour << 5) | (colour << 10) | (1 << 15);
			

			// Plot the pixel
			_bitmap->setPixel(x + i, y + j, colour);
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
	u16 colour = (_bitmap->getPixel(x, y) ^ 0xffff) | (1 << 15);

	// Draw the XORed pixel colour to the bitmap
	_bitmap->setPixel(x, y, colour);
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
			
			for (u16 y = 0; y < height; y++) {
				
				// Copy row to buffer
				woopsiDmaCopy(_bitmap->getData(sourceX, sourceY + y), buffer, width);
				
				// Copy row back to screen
				_bitmap->blit(destX, destY + y, buffer, width);
			}
			
			delete buffer;
			
			return;
		}
	}
	
	// Vertical movement or non overlap means we do not need to use an intermediate buffer
	// Copy from top to bottom if moving up; from bottom to top if moving down.
	// Ensures that rows to be copied are not overwritten
	if (sourceY > destY) {
		
		// Copy up
		for (s32 i = 0; i < height; ++i) {
			_bitmap->blit(destX, destY + i, _bitmap->getData(sourceX, sourceY + i), width);
		}
	} else {
		
		// Copy down
		for (s32 i = height - 1; i >= 0; --i) {
			_bitmap->blit(destX, destY + i, _bitmap->getData(sourceX, sourceY + i), width);
		}
	}
}

void GraphicsUnclipped::dim(s16 x, s16 y, u16 width, u16 height) {

	// Loop through all pixels within the region
	for (s16 i = 0; i < height; i++) {
		for (s16 j = 0; j < width; j++) {
	
			// Get pixel data directly from the framebuffer
			u16 colour = _bitmap->getPixel(x + j, y + i);
		
			// Halve the intensity of the colour (cheers Jeff)
			colour = ((colour >> 1) & (15 | (15 << 5) | (15 << 10)));

			// Write back to framebuffer
			_bitmap->setPixel(x + j, y + i, colour | 0x8000);
		}
	}
}

void GraphicsUnclipped::greyScale(s16 x, s16 y, u16 width, u16 height) {

	// Loop through all pixels within the region
	for (s16 i = 0; i < height; i++) {
		for (s16 j = 0; j < width; j++) {
	
			// Get pixel data directly from the framebuffer
			u16 colour = _bitmap->getPixel(x + j, y + i);

			// Extract individual components
			colour >>= 1;
			u8 r = (colour >> 1) & 7;
			u8 g = (colour >> 5) & 15;
			u8 b = (colour >> 11) & 7;

			colour = r + g + b;
			colour = colour | (colour << 5) | (colour << 10) | (1 << 15);

			// Write back to framebuffer
			_bitmap->setPixel(x + j, y + i, colour | 0x8000);
		}
	}
}
