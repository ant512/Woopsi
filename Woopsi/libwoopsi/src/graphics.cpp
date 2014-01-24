#include "graphics.h"
#include "woopsifuncs.h"
#include "stringiterator.h"
#include "fontbase.h"

using namespace WoopsiUI;

Graphics::Graphics(MutableBitmapBase* bitmap, const Rect& clipRect) {

	_bitmap = bitmap;
	_width = bitmap->getWidth();
	_height = bitmap->getHeight();
	
	setClipRect(clipRect);
}

void Graphics::setClipRect(const Rect& clipRect) {
	_clipRect.height = clipRect.height;
	_clipRect.width = clipRect.width;
	_clipRect.x = clipRect.x;
	_clipRect.y = clipRect.y;

	// Ensure clipRect does not exceed bitmap dimensions
	if (_clipRect.x < 0) {
		_clipRect.width += _clipRect.x;
		_clipRect.x = 0;
	}

	if (_clipRect.y < 0) {
		_clipRect.height += _clipRect.y;
		_clipRect.y = 0;
	}

	if (_clipRect.width + _clipRect.x > _width) _clipRect.width = _width - _clipRect.x;
	if (_clipRect.height + _clipRect.y > _height) _clipRect.height = _height - clipRect.y;
}

void Graphics::getClipRect(Rect& rect) const {
	rect.x = _clipRect.x;
	rect.y = _clipRect.y;
	rect.width = _clipRect.width;
	rect.height = _clipRect.height;
}

bool Graphics::clipCoordinates(s16* x1, s16* y1, s16* x2, s16* y2, const Rect& clipRect) {

	// Get co-ords of clipping rect
	s16 minX = clipRect.x;
	s16 minY = clipRect.y;
	s16 maxX = clipRect.x + clipRect.width - 1;
	s16 maxY = clipRect.y + clipRect.height - 1;

	// Choose larger start point values
	minX = minX > 0 ? minX : 0;
	minY = minY > 0 ? minY : 0;
	
	// Choose smaller end point values
	maxX = maxX < (_width - 1) ? maxX : (_width - 1);
	maxY = maxY < (_height - 1) ? maxY : (_height - 1);
	
	// Ensure values don't exceed clipping rectangle
	*x1 = *x1 > minX ? *x1 : minX;
	*y1 = *y1 > minY ? *y1 : minY;
	*x2 = *x2 < maxX ? *x2 : maxX;
	*y2 = *y2 < maxY ? *y2 : maxY;
	
	// Return false if no box to draw
	if ((*x2 < *x1) || (*y2 < *y1)) return false;
	
	// Return true as box can be drawn
	return true;
}

// Draw a single pixel to the bitmap
void Graphics::drawPixel(s16 x, s16 y, u16 colour) {

	s16 clipX1 = x;
	s16 clipY1 = y;
	s16 clipX2 = x;
	s16 clipY2 = y;
	
	// Attempt to clip and draw
	if (!clipCoordinates(&clipX1, &clipY1, &clipX2, &clipY2, _clipRect)) return;

	// Plot the pixel
	_bitmap->setPixel(x, y, colour);
}

// Get a single pixel from the bitmap
const u16 Graphics::getPixel(s16 x, s16 y) const {

	// Prevent overflows
	if ((x < 0) || (y < 0)) return 0;
	if ((x >= _width) || (y >= _height)) return 0;

	// Get the pixel
	return _bitmap->getPixel(x, y);
}

void Graphics::drawFilledRect(s16 x, s16 y, u16 width, u16 height, u16 colour) {

	// Get end point of rect to draw
	s16 x2 = x + width - 1;
	s16 y2 = y + height - 1;
	
	// Attempt to clip
	if (!clipCoordinates(&x, &y, &x2, &y2, _clipRect)) return;
		
	// Calculate new width/height
	width = x2 - x + 1;
	height = y2 - y + 1;
		
	// Draw the rectangle
	for (u16 i = 0; i < height; i++) {
		drawHorizLine(x, y + i, width, colour);
	}
}

void Graphics::drawHorizLine(s16 x, s16 y, u16 width, u16 colour) {

	// Get end point of rect to draw
	s16 x2 = x + width - 1;
	s16 y2 = y;
	
	// Attempt to clip
	if (!clipCoordinates(&x, &y, &x2, &y2, _clipRect)) return;
		
	// Calculate new width
	width = x2 - x + 1;
	
	// Draw the line
	_bitmap->blitFill(x, y, colour, width);
}

void Graphics::drawVertLine(s16 x, s16 y, u16 height, u16 colour) {

	// Get end point of rect to draw
	s16 x2 = x;
	s16 y2 = y + height - 1;
	
	// Attempt to clip
	if (!clipCoordinates(&x, &y, &x2, &y2, _clipRect)) return;
		
	// Calculate new height
	height = y2 - y + 1;
		
	// Draw the line
	for (u16 i = 0; i < height; i++) {
		_bitmap->setPixel(x, y + i, colour);
	}
}

void Graphics::drawRect(s16 x, s16 y, u16 width, u16 height, u16 colour) {

	drawHorizLine(x, y + height - 1, width, colour);	// Bottom
	drawVertLine(x + width - 1, y, height, colour);		// Right

	drawHorizLine(x, y, width, colour);					// Top
	drawVertLine(x, y, height, colour);					// Left
}

void Graphics::drawCircle(s16 x0, s16 y0, u16 radius, u16 colour) {
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

void Graphics::drawFilledCircle(s16 x0, s16 y0, u16 radius, u16 colour) {
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

void Graphics::drawText(s16 x, s16 y, FontBase* font, const WoopsiString& string) {
	drawText(x, y, font, string, 0, string.getLength());
}

void Graphics::drawText(s16 x, s16 y, FontBase* font, const WoopsiString& string, s32 startIndex, s32 length, u16 colour) {

	s16 clipX1 = _clipRect.x;
	s16 clipY1 = _clipRect.y;
	s16 clipX2 = _clipRect.x + _clipRect.width - 1;
	s16 clipY2 = _clipRect.y + _clipRect.height - 1;

	// Early exit checks before we reach the real clipping routine
	if (x > clipX2) return;
	if (y > clipY2) return;
	if (y < clipY1 - font->getHeight()) return;

	s16 textX1 = x;
	s16 textY1 = y;
	s16 textX2 = x + font->getStringWidth(string, startIndex, length) - 1;
	s16 textY2 = y + font->getHeight();
	
	// Attempt to clip
	if (!clipCoordinates(&textX1, &textY1, &textX2, &textY2, _clipRect)) return;
		
	// Draw the string char by char
	StringIterator* iterator = string.newStringIterator();
		
	if (iterator->moveTo(startIndex)) {
		do {
			x = font->drawChar(_bitmap, iterator->getCodePoint(), colour, x, y, clipX1, clipY1, clipX2, clipY2);

			// Abort if x pos outside clipping region
			if (x > clipX2) break;
		} while (iterator->moveToNext() && (iterator->getIndex() < startIndex + length));
	}

	delete iterator;
}

void Graphics::drawBaselineText(s16 x, s16 y, FontBase* font, const WoopsiString& string, s32 startIndex, s32 length, u16 colour) {

	s16 clipX1 = _clipRect.x;
	s16 clipY1 = _clipRect.y;
	s16 clipX2 = _clipRect.x + _clipRect.width - 1;
	s16 clipY2 = _clipRect.y + _clipRect.height - 1;
		
	// Draw the string char by char
	StringIterator* iterator = string.newStringIterator();
		
	// We can't do the same exit checks as we have no idea of the height, width, top of the string
	// We would need lineHeight, lineTop, lineWidth and that wouldn't tell us 
	// where to stop rendering anyway clipping will be done in the font, on a char basis 
	if (iterator->moveTo(startIndex)) {
		do {
		        x = font->drawBaselineChar(_bitmap, iterator->getCodePoint(), colour, x, y, clipX1, clipY1, clipX2, clipY2);
		} while (iterator->moveToNext() && (iterator->getIndex() < startIndex + length));
	}
	delete iterator;
}

void Graphics::drawXORPixel(s16 x, s16 y) {
	drawXORPixel(x, y, 0xffff);
}

void Graphics::drawXORPixel(s16 x, s16 y, u16 colour) {

	s16 clipX1 = x;
	s16 clipY1 = y;
	s16 clipX2 = x;
	s16 clipY2 = y;
	
	// Attempt to clip
	if (!clipCoordinates(&clipX1, &clipY1, &clipX2, &clipY2, _clipRect)) return;

	// Get the pixel at the specified co-ords and XOR against supplied colour
	u16 newColour = (_bitmap->getPixel(x, y) ^ colour) | (1 << 15);

	// Draw the XORed pixel colour to the bitmap
	_bitmap->setPixel(x, y, newColour);
}

void Graphics::drawXORHorizLine(s16 x, s16 y, u16 width, u16 colour) {

	// Get end point of rect to draw
	s16 x2 = x + width - 1;
	s16 y2 = y;
	
	// Attempt to clip
	if (!clipCoordinates(&x, &y, &x2, &y2, _clipRect)) return;
		
	// Calculate new width
	width = x2 - x + 1;
	
	// Draw the line
	for (u16 i = 0; i < width; i++) {
		drawXORPixel(x + i, y, colour);
	}
}

void Graphics::drawXORVertLine(s16 x, s16 y, u16 height, u16 colour) {

	// Get end point of rect to draw
	s16 x2 = x;
	s16 y2 = y + height - 1;
	
	// Attempt to clip
	if (!clipCoordinates(&x, &y, &x2, &y2, _clipRect)) return;
		
	// Calculate new height
	height = y2 - y + 1;
	
	// Draw the line
	for (u16 i = 0; i < height; i++) {
		drawXORPixel(x, y + i, colour);
	}
}

void Graphics::drawXORHorizLine(s16 x, s16 y, u16 width) {
	drawXORHorizLine(x, y, width, 0xffff);
}

void Graphics::drawXORVertLine(s16 x, s16 y, u16 height) {
	drawXORVertLine(x, y, height, 0xffff);
}

void Graphics::drawXORRect(s16 x, s16 y, u16 width, u16 height, u16 colour) {
	drawXORHorizLine(x, y, width, colour);						// Top
	drawXORHorizLine(x, y + height - 1, width, colour);			// Bottom
	drawXORVertLine(x, y + 1, height - 2, colour);				// Left
	drawXORVertLine(x + width - 1, y + 1, height - 2, colour);	// Right
}

void Graphics::drawFilledXORRect(s16 x, s16 y, u16 width, u16 height, u16 colour) {
	for (s32 j = 0; j < height; j++) {
		drawXORHorizLine(x, y + j, width, colour);
	}
}

void Graphics::drawXORRect(s16 x, s16 y, u16 width, u16 height) {
	drawXORRect(x, y, width, height, 0xffff);
}

void Graphics::drawFilledXORRect(s16 x, s16 y, u16 width, u16 height) {
	drawFilledXORRect(x, y, width, height, 0xffff);
}

// Scanline floodfill algorithm
void Graphics::floodFill(s16 x, s16 y, u16 newColour) {

	// Attempt to clip
	if (!clipCoordinates(&x, &y, &x, &y, _clipRect)) return;

	// Get current colour
	u16 oldColour = getPixel(x, y);

	// Exit if colours match
	if (oldColour == newColour) return;

	// Initalise stack
	WoopsiArray<s32> stack;

	s16 x1;
	u8 spanUp, spanDown;
	s16 rowStart;
	u16 rowWidth;

	// Push initial value onto the stack
	pushStack(x, y, stack);

	// Continue until stack is empty
	while (popStack(&x, &y, stack)) {

		x1 = x;

		// Locate leftmost column on screen in this row containing old colour
		while ((x1 >= _clipRect.x) && (_bitmap->getPixel(x1, y) == oldColour)) {
			x1--;
		}

		// Move to first column of old colour
		x1++;

		rowStart = x1;
		rowWidth = 0;

		spanUp = spanDown = 0;

		// Scan right, filling each column of old colour
		while ((x1 < _clipRect.width) && (_bitmap->getPixel(x1, y) == oldColour)) {

			// Check pixel above
			if ((!spanUp) && (y > _clipRect.y) && (_bitmap->getPixel(x1, y - 1) == oldColour)) {
				pushStack(x1, y - 1, stack);
				spanUp = 1;
			} else if ((spanUp) && (y > _clipRect.y) && (_bitmap->getPixel(x1, y - 1) != oldColour)) {
				spanUp = 0;
			}

			// Check pixel below
			if ((!spanDown) && (y < _clipRect.height - 1) && (_bitmap->getPixel(x1, y + 1) == oldColour)) {
				pushStack(x1, y + 1, stack);
				spanDown = 1;
			} else if ((spanDown) && (y < _clipRect.height - 1) && (_bitmap->getPixel(x1, y + 1) != oldColour)) {
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
bool Graphics::popStack(s16* x, s16* y, WoopsiArray<s32>& stack) { 
	if (stack.size() > 0) { 
		s32 val = stack.at(stack.size() - 1);
		*y = val / _width;
		*x = val % _width;
		stack.pop_back();
		return true;
	}

	return false;  
}

void Graphics::pushStack(s16 x, s16 y, WoopsiArray<s32>& stack) {
	stack.push_back(x + (y * _width));
}

//Draw bitmap to the internal bitmap
void Graphics::drawBitmap(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16  bitmapY) {
	
	// Get co-ords of screen section we're drawing to
	s16 minX = x;
	s16 minY = y;
	s16 maxX = x + width - 1;
	s16 maxY = y + height - 1;
	
	// Attempt to clip
	if (!clipCoordinates(&minX, &minY, &maxX, &maxY, _clipRect)) return;
		
	// Calculate new width and height
	width = maxX - minX + 1;
	height = maxY - minY + 1;
		
	//Adjust bitmap co-ordinates to allow for clipping changes to visible section
	if (minX > x) {
		bitmapX += minX - x;
	}

	if (minY > y) {
		bitmapY += minY - y;
	}

	x = minX;
	y = minY;

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

	// Stop if there is nothing to draw
	if ((width <= 0) || (height <= 0)) return;

	// Draw the bitmap
	for (u16 i = 0; i < height; i++) {
		_bitmap->blit(x, y + i, bitmap->getData(bitmapX, bitmapY + i), width);
	}
}

//Draw bitmap to the internal bitmap
void Graphics::drawBitmap(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16  bitmapY, u16 transparentColour) {
	
	// Get co-ords of screen section we're drawing to
	s16 minX = x;
	s16 minY = y;
	s16 maxX = x + width - 1;
	s16 maxY = y + height - 1;
	
	// Attempt to clip
	if (!clipCoordinates(&minX, &minY, &maxX, &maxY, _clipRect)) return;
		
	// Calculate new width and height
	width = maxX - minX + 1;
	height = maxY - minY + 1;
		
	//Adjust bitmap co-ordinates to allow for clipping changes to visible section
	if (minX > x) {
		bitmapX += minX - x;
	}

	if (minY > y) {
		bitmapY += minY - y;
	}

	x = minX;
	y = minY;

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

	// Stop if there is nothing to draw
	if ((width <= 0) || (height <= 0)) return;

	// Draw the bitmap
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

void Graphics::drawBitmapGreyScale(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16  bitmapY) {
	
	// Get co-ords of screen section we're drawing to
	s16 minX = x;
	s16 minY = y;
	s16 maxX = x + width - 1;
	s16 maxY = y + height - 1;
	
	// Attempt to clip
	if (!clipCoordinates(&minX, &minY, &maxX, &maxY, _clipRect)) return;
		
	// Calculate new width and height
	width = maxX - minX + 1;
	height = maxY - minY + 1;
		
	//Adjust bitmap co-ordinates to allow for clipping changes to visible section
	if (minX > x) {
		bitmapX += minX - x;
	}

	if (minY > y) {
		bitmapY += minY - y;
	}

	x = minX;
	y = minY;
	
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

	if ((width <= 0) || (height <= 0)) return;

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
void Graphics::drawEllipse(s16 xCentre, s16 yCentre, s16 horizRadius, s16 vertRadius, u16 colour) {

	// Use faster circle drawing routine if possible
	if (horizRadius == vertRadius) {
		drawCircle(xCentre, yCentre, horizRadius, colour);
		return;
	}

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
void Graphics::drawFilledEllipse(s16 xCentre, s16 yCentre, s16 horizRadius, s16 vertRadius, u16 colour) {

	// Use faster circle drawing routine if possible
	if (horizRadius == vertRadius) {
		drawFilledCircle(xCentre, yCentre, horizRadius, colour);
		return;
	}

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

void Graphics::copy(s16 sourceX, s16 sourceY, s16 destX, s16 destY, u16 width, u16 height) {
	
	// Do nothing if no copying involved
	if ((sourceX == destX) && (sourceY == destY)) return;

	// Get end point of source
	s16 x2 = sourceX + width - 1;
	s16 y2 = sourceY + height - 1;

	if (!clipCoordinates(&sourceX, &sourceY, &x2, &y2, _clipRect)) return;

	// Convert width and height back so that we can calculate the dimesions of the dest
	width = x2 + 1 - sourceX;
	height = y2 + 1 - sourceY;

	// Get end point of dest
	x2 = destX + width - 1;
	y2 = destY + height - 1;

	if (!clipCoordinates(&destX, &destY, &x2, &y2, _clipRect)) return;

	// Convert width and height back again for use in the rest of the function
	width = x2 + 1 - destX;
	height = y2 + 1 - destY;
	
	// If there is no vertical movement and the source and destination overlap,
	// we need to use an offscreen buffer to copy
	if (sourceY == destY) {
		if ((destX >= sourceX) && (destX <= sourceX + width)) {
			u16* buffer = new u16[width];
			
			for (u16 y = 0; y < height; y++) {
				
				// Copy row to buffer
				_bitmap->copy(sourceX, sourceY + y, width, buffer);
				
				// Copy row back to screen
				_bitmap->blit(destX, destY + y, buffer, width);
			}
			
			delete[] buffer;
			
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

void Graphics::dim(s16 x, s16 y, u16 width, u16 height) {

	s16 x2 = x + width - 1;
	s16 y2 = y + height - 1;

	if (!clipCoordinates(&x, &y, &x2, &y2, _clipRect)) return;

	width = (x2 - x) + 1;
	height = (y2 - y) + 1;

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

void Graphics::greyScale(s16 x, s16 y, u16 width, u16 height) {

	s16 x2 = x + width - 1;
	s16 y2 = y + height - 1;

	if (!clipCoordinates(&x, &y, &x2, &y2, _clipRect)) return;

	width = (x2 - x) + 1;
	height = (y2 - y) + 1;

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

u8 Graphics::getClipLineOutCode(s16 x, s16 y, s16 xMin, s16 yMin, s16 xMax, s16 yMax) {
	u8 code = 0;
	
	if (y > yMax) code |= 1;
	if (y < yMin) code |= 2;
	if (x > xMax) code |= 4;
	if (x < xMin) code |= 8;
	
	return code;
}

void Graphics::drawClippedLine(s16 x1, s16 y1, s16 x2, s16 y2, u16 colour) {
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

void Graphics::drawLine(s16 x1, s16 y1, s16 x2, s16 y2, u16 colour) {

	// Choose shortcut for horizontal or vertical lines
	if (x1 == x2) {
		
		if (y1 < y2) {
			drawVertLine(x1, y1, (y2 - y1) + 1, colour);
		} else {
			drawVertLine(x1, y2, (y1 - y2) + 1, colour);
		}
		return;
	} else if (y1 == y2) {
		
		if (x1 < x2) {
			drawHorizLine(x1, y1, (x2 - x1) + 1, colour);
		} else {
			drawHorizLine(x2, y1, (x1 - x2) + 1, colour);
		}
		return;
	}

	// Extract data from cliprect
	s16 minX = _clipRect.x;
	s16 minY = _clipRect.y;
	s16 maxX = _clipRect.x + _clipRect.width - 1;
	s16 maxY = _clipRect.y + _clipRect.height - 1;
	
	if (!clipCoordinates(&minX, &minY, &maxX, &maxY, _clipRect)) return;
		
	// Get outcodes for each point
	u8 code1 = getClipLineOutCode(x1, y1, minX, minY, maxX, maxY);
	u8 code2 = getClipLineOutCode(x2, y2, minX, minY, maxX, maxY);
	
	// Clip
	while (1) {
		// Check for trivial cases
		if (!(code1 | code2)) {
			
			// Line entirely within visible region
			// Draw the line
			drawClippedLine(x1, y1, x2, y2, colour);
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
				if (x2 - x1 != 0) {
					t = ((maxX - x1) << 8) / (x2 - x1);
					y = y1 + ((t * (y2 - y1)) >> 8);
					x = maxX;
				}
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

void Graphics::scroll(s16 x, s16 y, s16 xDistance, s16 yDistance, u16 width, u16 height, WoopsiArray<Rect>* revealedRects) {
	
	// Calculate the dimensions of the area being scrolled
	s16 regionX1 = x;
	s16 regionY1 = y;
	s16 regionX2 = x + width - 1;
	s16 regionY2 = y + height - 1;
	
	// Clip the dimensions so that they fit within the clip rect
	if (!clipCoordinates(&regionX1, &regionY1, &regionX2, &regionY2, _clipRect)) return;
	
	// Calculate co-ordinates of source rect
	s16 sourceX1 = regionX1;
	s16 sourceY1 = regionY1;
	s16 sourceX2 = regionX2;
	s16 sourceY2 = regionY2;
	
	// Adjust source to compensate for the fact that we are scrolling, not
	// copying - we are overwriting part of the source with itself
	if (xDistance < 0) sourceX1 -= xDistance;
	if (xDistance > 0) sourceX2 -= xDistance;
	if (yDistance < 0) sourceY1 -= yDistance;
	if (yDistance > 0) sourceY2 -= yDistance;
	
	// Check if the destination does not overlap the source - in that case,
	// we're scrolling the visible portion totally off-screen.  We just
	// need to push the entire region (clipped) to the redraw array
	// so that the current display is overwritten with totally new content.
	//
	// If the source rect has a negative width or height, the two rects
	// do not overlap
	if ((sourceX2 < sourceX1) || (sourceY2 < sourceY1)) {
		Rect rect;
		rect.x = regionX1;
		rect.y = regionY1;
		rect.width = regionX2 - regionX1 + 1;
		rect.height = regionY2 - regionY1 + 1;
		revealedRects->push_back(rect);
		
		return;
	}
	
	// Get co-ords of destination rectangle
	s16 destX1 = sourceX1 + xDistance;
	s16 destY1 = sourceY1 + yDistance;
	s16 destX2 = sourceX2 + xDistance;
	s16 destY2 = sourceY2 + yDistance;
	
	// Get the dimensions of the source and destination regions
	s16 widthSource = (sourceX2 - sourceX1) + 1;
	s16 heightSource = (sourceY2 - sourceY1) + 1;
	
	// Post-clipping regions overlap and are the same size, so copy the source to the dest
	copy(sourceX1, sourceY1, destX1, destY1, widthSource, heightSource);
	
	// Work out the dimensions of the non-overlapped areas
	s16 clippedWidth = (regionX2 - regionX1) + 1;
	s16 clippedHeight = (regionY2 - regionY1) + 1;
	
	// Horizontal area
	if (sourceX1 < destX1) {
		
		// Revealed area on the left of the destination
		Rect rect;
		rect.x = sourceX1;
		rect.y = sourceY1 < destY1 ? sourceY1 : destY1;
		rect.width = (destX1 - sourceX1);
		rect.height = clippedHeight;
		
		if ((rect.height > 0) && (rect.width > 0)) {
			revealedRects->push_back(rect);
		}
		
	} else if (sourceX1 > destX1) {
		
		// Revealed area on the right of the destination
		Rect rect;
		rect.x = destX2 + 1;
		rect.y = sourceY1 < destY1 ? sourceY1 : destY1;
		rect.width = (sourceX1 - destX1);
		rect.height = clippedHeight;
		
		if ((rect.height > 0) && (rect.width > 0)) {
			revealedRects->push_back(rect);
		}
	}
	
	if (sourceY1 < destY1) {
		
		// Vertical movement only - revealed area above the destination
		Rect rect;
		rect.x = sourceX1 < destX1 ? sourceX1 : destX1;
		rect.y = sourceY1;
		rect.width = clippedWidth;
		rect.height = (destY1 - sourceY1);
		
		if ((rect.height > 0) && (rect.width > 0)) {
			revealedRects->push_back(rect);
		}
		
	} else if (sourceY1 > destY1) {
		
		// Vertical movement only - revealed area below the destination
		Rect rect;
		rect.x = sourceX1 < destX1 ? sourceX1 : destX1;
		rect.y = destY2 + 1;
		rect.width = clippedWidth;
		rect.height = (sourceY1 - destY1);
		
		if ((rect.height > 0) && (rect.width > 0)) {
			revealedRects->push_back(rect);
		}
	}
}

void Graphics::drawBevelledRect(s16 x, s16 y, u16 width, u16 height, u16 shineColour, u16 shadowColour) {
	drawHorizLine(x, y, width, shineColour);						// Top
	drawHorizLine(x, y + height - 1, width, shadowColour);			// Bottom
	drawVertLine(x, y + 1, height - 2, shineColour);				// Left
	drawVertLine(x + width - 1, y + 1, height - 2, shadowColour);	// Right
}
