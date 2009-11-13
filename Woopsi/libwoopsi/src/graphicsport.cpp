#include "graphicsport.h"
#include "defines.h"
#include "fontbase.h"
#include "textwriter.h"
#include "woopsifuncs.h"
#include "framebuffer.h"
#include "bitmapbase.h"

using namespace WoopsiUI;

GraphicsPort::GraphicsPort(Gadget* const gadget, const s16 x, const s16 y, const u16 width, const u16 height, FrameBuffer* bitmap, const WoopsiArray<Rect>* clipRectList, const Rect* clipRect) : GraphicsUnclipped(bitmap) {
	_gadget = gadget;
	_rect.x = x;
	_rect.y = y;
	_rect.width = width;
	_rect.height = height;
	
	// Set up clip rect
	if (clipRect != NULL) {
		_clipRect = new Rect;
		_clipRect->height = clipRect->height;
		_clipRect->width = clipRect->width;
		_clipRect->x = clipRect->x;
		_clipRect->y = clipRect->y;
	} else {
		_clipRect = NULL;

		// Set up clip rect list
		if (clipRectList != NULL) {
			_clipRectList = clipRectList;
		} else {
			_clipRectList = NULL;
		}
	}
}

// Clip filled rectangle
void GraphicsPort::clipFilledRect(s16 x, s16 y, u16 width, u16 height, u16 colour, const Rect& clipRect) {
	
	// Get end point of rect to draw
	s16 x2 = x + width - 1;
	s16 y2 = y + height - 1;
	
	// Attempt to clip
	if (clipCoordinates(&x, &y, &x2, &y2, clipRect)) {
		
		// Calculate new width/height
		width = x2 - x + 1;
		height = y2 - y + 1;
		
		// Draw the rectangle
		GraphicsUnclipped::drawFilledRect(x, y, width, height, colour);
	}
}

// Print a single character in a specific colour
void GraphicsPort::drawText(s16 x, s16 y, FontBase* font, char letter, u16 colour) {
	
	// Ignore command if gadget deleted or invisible
	if (!_gadget->isDrawingEnabled()) return;
	
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

// Print a string in a specific colour
void GraphicsPort::drawText(s16 x, s16 y, FontBase* font, const char* string, u16 colour) {
	
	// Ignore command if gadget deleted or invisible
	if (!_gadget->isDrawingEnabled()) return;
	
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

// Print a single character
void GraphicsPort::drawText(s16 x, s16 y, FontBase* font, char letter) {
	
	// Ignore command if gadget deleted or invisible
	if (!_gadget->isDrawingEnabled()) return;
	
	// Convert char to char[]
	char text[2];
	text[0] = letter;
	text[1] = '\0';
	
	// Output as normal
	drawText(x, y, font, text);
}

void GraphicsPort::drawText(s16 x, s16 y, FontBase* font, const char* string) {
	drawText(x, y, font, strlen(string), string);
}

void GraphicsPort::drawText(s16 x, s16 y, FontBase* font, u16 length, const char* string) {
	
	// Ignore command if gadget deleted or invisible
	if (!_gadget->isDrawingEnabled()) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);
	
	if (_clipRect == NULL) {
		// Draw all visible rects
		for (s32 i = 0; i < _clipRectList->size(); i++) {
			clipText(x, y, font, length, string, _clipRectList->at(i));
		}
	} else {
		// Draw single rectangle
		clipText(x, y, font, length, string, *_clipRect);
	}
}

// Print a string in a specific colour
void GraphicsPort::drawText(s16 x, s16 y, FontBase* font, u16 length, const char* string, u16 colour) {
	
	// Ignore command if gadget deleted or invisible
	if (!_gadget->isDrawingEnabled()) return;
	
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

// Clip and draw text
void GraphicsPort::clipText(s16 x, s16 y, FontBase* font, u16 length, const char* string, const Rect& clipRect) {
	s16 clipX1 = clipRect.x;
	s16 clipY1 = clipRect.y;
	s16 clipX2 = clipRect.x + clipRect.width - 1;
	s16 clipY2 = clipRect.y + clipRect.height - 1;
	
	// Attempt to clip and draw
	if (clipCoordinates(&clipX1, &clipY1, &clipX2, &clipY2, clipRect)) {
		// Compensate for top screen offset
		if (y >= TOP_SCREEN_Y_OFFSET) {
			y -= TOP_SCREEN_Y_OFFSET;
		}
		
		TextWriter::drawString(_bitmap, font, string, length, x, y, clipX1, clipY1, clipX2, clipY2);
	}
}

// Clip vertical line
void GraphicsPort::clipVertLine(s16 x, s16 y, s16 height, u16 colour, const Rect& clipRect) {
	
	// Get end point of rect to draw
	s16 x2 = x;
	s16 y2 = y + height - 1;
	
	// Attempt to clip
	if (clipCoordinates(&x, &y, &x2, &y2, clipRect)) {
		
		// Calculate new height
		height = y2 - y + 1;
		
		// Draw the line
		GraphicsUnclipped::drawVertLine(x, y, height, colour);
	}
}

// Clip horizontal line
void GraphicsPort::clipHorizLine(s16 x, s16 y, s16 width, u16 colour, const Rect& clipRect) {
	
	// Get end point of rect to draw
	s16 x2 = x + width - 1;
	s16 y2 = y;
	
	// Attempt to clip
	if (clipCoordinates(&x, &y, &x2, &y2, clipRect)) {
		
		// Calculate new width
		width = x2 - x + 1;
		
		// Draw the line
		GraphicsUnclipped::drawHorizLine(x, y, width, colour);
	}
}

// Clip XORed horizontal line
void GraphicsPort::clipXORHorizLine(s16 x, s16 y, s16 width, const Rect& clipRect) {
	
	// Get end point of rect to draw
	s16 x2 = x + width - 1;
	s16 y2 = y;
	
	// Attempt to clip
	if (clipCoordinates(&x, &y, &x2, &y2, clipRect)) {
		
		// Calculate new width
		width = x2 - x + 1;
		
		// Draw the line
		for (u16 i = 0; i < width; i++) {
			GraphicsUnclipped::drawXORPixel(x + i, y);
		}
	}
}

// Clip XORed vertical line
void GraphicsPort::clipXORVertLine(s16 x, s16 y, s16 height, const Rect& clipRect) {
	
	// Get end point of rect to draw
	s16 x2 = x;
	s16 y2 = y + height - 1;
	
	// Attempt to clip
	if (clipCoordinates(&x, &y, &x2, &y2, clipRect)) {
		
		// Calculate new height
		height = y2 - y + 1;
		
		// Draw the line
		for (u16 i = 0; i < height; i++) {
			GraphicsUnclipped::drawXORPixel(x, y + i);
		}
	}
}

// Draw horizontal line - external function
void GraphicsPort::drawHorizLine(s16 x, s16 y, s16 width, u16 colour) {
	
	// Ignore command if gadget deleted or invisible
	if (!_gadget->isDrawingEnabled()) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);
	
	if (_clipRect == NULL) {
		
		// Draw all visible rectangles
		for (s32 i = 0; i < _clipRectList->size(); i++) {
			clipHorizLine(x, y, width, colour, _clipRectList->at(i));
		}
	} else {
		// Draw single rectangle
		clipHorizLine(x, y, width, colour, *_clipRect);
	}
}

// Draw vertical line - external function
void GraphicsPort::drawVertLine(s16 x, s16 y, s16 height, u16 colour) {
	
	// Ignore command if gadget deleted or invisible
	if (!_gadget->isDrawingEnabled()) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);
	
	if (_clipRect == NULL) {
		// Draw all visible rectangles
		for (s32 i = 0; i < _clipRectList->size(); i++) {
			clipVertLine(x, y, height, colour, _clipRectList->at(i));
		}
	} else {
		// Draw single rectangle
		clipVertLine(x, y, height, colour, *_clipRect);
	}
}

// Draw filled rectangle - external function
void GraphicsPort::drawFilledRect(s16 x, s16 y, u16 width, u16 height, u16 colour) {
	
	// Ignore command if gadget deleted or invisible
	if (!_gadget->isDrawingEnabled()) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);
	
	if (_clipRect == NULL) {
		// Draw all visible rectangles
		for (s32 i = 0; i < _clipRectList->size(); i++) {
			clipFilledRect(x, y, width, height, colour, _clipRectList->at(i));
		}
	} else {
		// Draw single rectangle
		clipFilledRect(x, y, width, height, colour, *_clipRect);
	}
}

void GraphicsPort::drawCircle(s16 x0, s16 y0, u16 radius, u16 colour) {
	// Ignore command if gadget deleted or invisible
	if (!_gadget->isDrawingEnabled()) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x0, &y0);
	
	if (_clipRect == NULL) {
		// Draw all visible rectangles
		for (s32 i = 0; i < _clipRectList->size(); i++) {
			clipCircle(x0, y0, radius, colour, _clipRectList->at(i));
		}
	} else {
		// Draw single rectangle
		clipCircle(x0, y0, radius, colour, *_clipRect);
	}
}

void GraphicsPort::drawFilledCircle(s16 x0, s16 y0, u16 radius, u16 colour) {
	// Ignore command if gadget deleted or invisible
	if (!_gadget->isDrawingEnabled()) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x0, &y0);
	
	if (_clipRect == NULL) {
		// Draw all visible rectangles
		for (s32 i = 0; i < _clipRectList->size(); i++) {
			clipFilledCircle(x0, y0, radius, colour, _clipRectList->at(i));
		}
	} else {
		// Draw single rectangle
		clipFilledCircle(x0, y0, radius, colour, *_clipRect);
	}
}

void GraphicsPort::clipCircle(s16 x0, s16 y0, u16 radius, u16 colour, const Rect& clipRect) {
	s16 f = 1 - radius;
	s16 ddF_x = 0;
	s16 ddF_y = -2 * radius;
	s16 x = 0;
	s16 y = radius;

	clipPixel(x0, y0 + radius, colour, clipRect);
	clipPixel(x0, y0 - radius, colour, clipRect);
	clipPixel(x0 + radius, y0, colour, clipRect);
	clipPixel(x0 - radius, y0, colour, clipRect);

	while(x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x + 1;    
		clipPixel(x0 + x, y0 + y, colour, clipRect);
		clipPixel(x0 - x, y0 + y, colour, clipRect);
		clipPixel(x0 + x, y0 - y, colour, clipRect);
		clipPixel(x0 - x, y0 - y, colour, clipRect);
		clipPixel(x0 + y, y0 + x, colour, clipRect);
		clipPixel(x0 - y, y0 + x, colour, clipRect);
		clipPixel(x0 + y, y0 - x, colour, clipRect);
		clipPixel(x0 - y, y0 - x, colour, clipRect);
	}
}

void GraphicsPort::clipFilledCircle(s16 x0, s16 y0, u16 radius, u16 colour, const Rect& clipRect) {
	s16 f = 1 - radius;
	s16 ddF_x = 0;
	s16 ddF_y = -2 * radius;
	s16 x = 0;
	s16 y = radius;
	
	// Draw central line
	clipHorizLine(x0 - radius, y0, (radius << 1) + 1, colour, clipRect);
	
	while(x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x + 1;
		
		clipHorizLine(x0 - x, y0 + y, (x << 1) + 1, colour, clipRect);
		clipHorizLine(x0 - x, y0 - y, (x << 1) + 1, colour, clipRect);
		
		clipHorizLine(x0 - y, y0 + x, (y << 1) + 1, colour, clipRect);
		clipHorizLine(x0 - y, y0 - x, (y << 1) + 1, colour, clipRect);
	}
}

// Draw rectangle - external function
void GraphicsPort::drawRect(s16 x, s16 y, u16 width, u16 height, u16 colour) {
	
	// Ignore command if gadget deleted or invisible
	if (!_gadget->isDrawingEnabled()) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);
	
	if (_clipRect == NULL) {
		// Draw all visible rectangles
		for (s32 i = 0; i < _clipRectList->size(); i++) {
			clipHorizLine(x, y, width, colour, _clipRectList->at(i));						// Top
			clipHorizLine(x, y + height - 1, width, colour, _clipRectList->at(i));			// Bottom
			clipVertLine(x, y + 1, height - 2, colour, _clipRectList->at(i));				// Left
			clipVertLine(x + width - 1, y + 1, height - 2, colour, _clipRectList->at(i));	// Right
		}
	} else {
		// Draw single rectangle
		clipHorizLine(x, y, width, colour, *_clipRect);						// Top
		clipHorizLine(x, y + height - 1, width, colour, *_clipRect);		// Bottom
		clipVertLine(x, y + 1, height - 2, colour, *_clipRect);				// Left
		clipVertLine(x + width - 1, y + 1, height - 2, colour, *_clipRect);	// Right
	}
}

// Draw bevelled rectangle based on gadget border - external function
void GraphicsPort::drawBevelledRect(s16 x, s16 y, u16 width, u16 height) {
	
	// Ignore command if gadget deleted or invisible
	if (!_gadget->isDrawingEnabled()) return;
	
	// Ignore command if gadget is borderless
	if (_gadget->isBorderless()) return;
	
	// Work out which colours to use
	u16 col1;
	u16 col2;
	
	// Bevel in or out?
	if ((_gadget->isClicked() && (_gadget->getOutlineType() == Gadget::OUTLINE_CLICK_DEPENDENT)) || (_gadget->getOutlineType() == Gadget::OUTLINE_IN)) {
		// Bevelled into the screen
		col1 = _gadget->getShadowColour();
		col2 = _gadget->getShineColour();
	} else {
		// Bevelled out of the screen
		col1 = _gadget->getShineColour();
		col2 = _gadget->getShadowColour();
	}
	
	drawBevelledRect(x, y, width, height, col1, col2);
	
	// Draw the secondary border if the gadget is bevelled out and in
	if (_gadget->getOutlineType() == Gadget::OUTLINE_OUT_IN) {
		drawBevelledRect(x + 1, y + 1, width - 2, height - 2, col2, col1);
	}
}

// Draw bevelled rectangle - external function
void GraphicsPort::drawBevelledRect(s16 x, s16 y, u16 width, u16 height, u16 shineColour, u16 shadowColour) {
	
	// Ignore command if gadget deleted or invisible
	if (!_gadget->isDrawingEnabled()) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);
	
	if (_clipRect == NULL) {
		// Draw all visible rectangles
		for (s32 i = 0; i < _clipRectList->size(); i++) {
			clipHorizLine(x, y, width, shineColour, _clipRectList->at(i));							// Top
			clipHorizLine(x, y + height - 1, width, shadowColour, _clipRectList->at(i));			// Bottom
			clipVertLine(x, y + 1, height - 2, shineColour, _clipRectList->at(i));					// Left
			clipVertLine(x + width - 1, y + 1, height - 2, shadowColour, _clipRectList->at(i));		// Right
		}
	} else {
		// Draw single rectangle
		clipHorizLine(x, y, width, shineColour, *_clipRect);						// Top
		clipHorizLine(x, y + height - 1, width, shadowColour, *_clipRect);			// Bottom
		clipVertLine(x, y + 1, height - 2, shineColour, *_clipRect);				// Left
		clipVertLine(x + width - 1, y + 1, height - 2, shadowColour, *_clipRect);	// Right
	}
}

// Draw filled XORed rectangle - external function
void GraphicsPort::drawFilledXORRect(s16 x, s16 y, u16 width, u16 height) {
	
	// Ignore command if gadget deleted or invisible
	if (!_gadget->isDrawingEnabled()) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);
	
	if (_clipRect == NULL) {
		// Draw all visible rectangles
		for (s32 i = 0; i < _clipRectList->size(); i++) {
			
			// Draw all rows of rectangle
			for (s32 j = 0; j < height; j++) {
				clipXORHorizLine(x, y + j, width, _clipRectList->at(i));
			}
		}
	} else {
		// Draw single rectangle row by row
		for (s32 j = 0; j < height; j++) {
			clipXORHorizLine(x, y + j, width, *_clipRect);	
		}
	}
}

// Draw XORed rectangle - external function
void GraphicsPort::drawXORRect(s16 x, s16 y, u16 width, u16 height) {
	
	// Ignore command if gadget deleted or invisible
	if (!_gadget->isDrawingEnabled()) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);
	
	if (_clipRect == NULL) {
		// Draw all visible rectangles
		for (s32 i = 0; i < _clipRectList->size(); i++) {
			clipXORHorizLine(x, y, width, _clipRectList->at(i));						// Top
			clipXORHorizLine(x, y + height - 1, width, _clipRectList->at(i));			// Bottom
			clipXORVertLine(x, y + 1, height - 2, _clipRectList->at(i));				// Left
			clipXORVertLine(x + width - 1, y + 1, height - 2, _clipRectList->at(i));	// Right
		}
	} else {
		// Draw single rectangle
		clipXORHorizLine(x, y, width, *_clipRect);						// Top
		clipXORHorizLine(x, y + height - 1, width, *_clipRect);			// Bottom
		clipXORVertLine(x, y + 1, height - 2, *_clipRect);				// Left
		clipXORVertLine(x + width - 1, y + 1, height - 2, *_clipRect);	// Right
	}
}

//Draw bitmap - external function
void GraphicsPort::drawBitmap(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16 bitmapY) {
	
	// Ignore command if gadget deleted or invisible
	if (!_gadget->isDrawingEnabled()) return;
	
	u16 bitmapWidth = bitmap->getWidth();
	u16 bitmapHeight = bitmap->getHeight();

	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);
	
	// Ensure width of rect being drawn into does not exceed size of bitmap
	if (bitmapWidth - bitmapX < width) {
		width = bitmapWidth - bitmapX;
	}
	
	if (bitmapHeight - bitmapY < height) {
		height = bitmapHeight - bitmapY;
	}
	
	if (_clipRect == NULL) {
		// Draw all visible rectangles
		for (s32 i = 0; i < _clipRectList->size(); i++) {
			clipBitmap(x, y, width, height, bitmap, bitmapX, bitmapY, _clipRectList->at(i));
		}
	} else {
		// Draw single rectangle
		clipBitmap(x, y, width, height, bitmap, bitmapX, bitmapY, *_clipRect);
	}
}

//Draw bitmap with transparency - external function
void GraphicsPort::drawBitmap(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16  bitmapY, u16 transparentColour) {
	
	// Ignore command if gadget deleted or invisible
	if (!_gadget->isDrawingEnabled()) return;

	u16 bitmapWidth = bitmap->getWidth();
	u16 bitmapHeight = bitmap->getHeight();
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);
	
	// Ensure width of rect being drawn into does not exceed size of bitmap
	if (bitmapWidth - bitmapX < width) {
		width = bitmapWidth - bitmapX;
	}
	
	if (bitmapHeight - bitmapY < height) {
		height = bitmapHeight - bitmapY;
	}
	
	if (_clipRect == NULL) {
		// Draw all visible rectangles
		for (s32 i = 0; i < _clipRectList->size(); i++) {
			clipBitmap(x, y, width, height, bitmap, bitmapX, bitmapY, transparentColour, _clipRectList->at(i));
		}
	} else {
		// Draw single rectangle
		clipBitmap(x, y, width, height, bitmap, bitmapX, bitmapY, transparentColour, *_clipRect);
	}
}

// Draw XORed horizontal line - external function
void GraphicsPort::drawXORHorizLine(s16 x, s16 y, s16 width) {
	
	// Ignore command if gadget deleted or invisible
	if (!_gadget->isDrawingEnabled()) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);
	
	if (_clipRect == NULL) {
		
		// Draw all visible rectangles
		for (s32 i = 0; i < _clipRectList->size(); i++) {
			clipXORHorizLine(x, y, width, _clipRectList->at(i));
		}
	} else {
		
		// Draw single rectangle
		clipXORHorizLine(x, y, width, *_clipRect);
	}
}

// Draw XORed vertical line - external function
void GraphicsPort::drawXORVertLine(s16 x, s16 y, s16 height) {
	
	// Ignore command if gadget deleted or invisible
	if (!_gadget->isDrawingEnabled()) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);
	
	if (_clipRect == NULL) {
		
		// Draw all visible rectangles
		for (s32 i = 0; i < _clipRectList->size(); i++) {
			clipXORVertLine(x, y, height, _clipRectList->at(i));
		}
	} else {
		
		// Draw single rectangle
		clipXORVertLine(x, y, height, *_clipRect);
	}
}

void GraphicsPort::clipBitmap(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16 bitmapY, const Rect& clipRect) {
	
	// Get co-ords of screen section we're drawing to
	s16 minX = x;
	s16 minY = y;
	s16 maxX = x + width - 1;
	s16 maxY = y + height - 1;
	
	// Attempt to clip
	if (clipCoordinates(&minX, &minY, &maxX, &maxY, clipRect)) {
		
		// Calculate new width and height
		width = maxX - minX + 1;
		height = maxY - minY + 1;
		
		//Adjust bitmap co-ordinates to allow for clipping changes to visible section
		if (minX > x) {
			bitmapX += minX - x;
		}
		if (y < TOP_SCREEN_Y_OFFSET) {
			if (minY > y) {
				bitmapY += minY - y;
			}
		} else {
			if (minY + TOP_SCREEN_Y_OFFSET > y) {
				bitmapY += (minY + TOP_SCREEN_Y_OFFSET) - y;
			}
		}
		
		// Draw the bitmap
		GraphicsUnclipped::drawBitmap(minX, minY, width, height, bitmap, bitmapX, bitmapY);
	}
}

void GraphicsPort::clipBitmap(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16 bitmapY, u16 transparentColour, const Rect& clipRect) {

	// Get co-ords of screen section we're drawing to
	s16 minX = x;
	s16 minY = y;
	s16 maxX = x + width - 1;
	s16 maxY = y + height - 1;
	
	// Attempt to clip
	if (clipCoordinates(&minX, &minY, &maxX, &maxY, clipRect)) {
		
		// Calculate new width and height
		width = maxX - minX + 1;
		height = maxY - minY + 1;
		
		//Adjust bitmap co-ordinates to allow for clipping changes to visible section
		if (minX > x) {
			bitmapX += minX - x;
		}
		if (y < TOP_SCREEN_Y_OFFSET) {
			if (minY > y) {
				bitmapY += minY - y;
			}
		} else {
			if (minY + TOP_SCREEN_Y_OFFSET > y) {
				bitmapY += (minY + TOP_SCREEN_Y_OFFSET) - y;
			}
		}
		
		// Draw the bitmap
		GraphicsUnclipped::drawBitmap(minX, minY, width, height, bitmap, bitmapX, bitmapY, transparentColour);
	}
}

// Erase by redrawing gadget
void GraphicsPort::clear() {
	
	// Ignore command if gadget deleted or invisible
	if (!_gadget->isDrawingEnabled()) return;
	
	_gadget->redraw();
}

// Adjust co-ords from port-space to screen-space
void GraphicsPort::convertPortToScreenSpace(s16* x, s16* y) {
	*x += _rect.x + _gadget->getX();
	*y += _rect.y + _gadget->getY();
}

// Clip co-ordinates
bool GraphicsPort::clipCoordinates(s16* x1, s16* y1, s16* x2, s16* y2, const Rect& clipRect) {
	// Get co-ords of clipping rect
	s16 minX = clipRect.x;
	s16 minY = clipRect.y;
	s16 maxX = clipRect.x + clipRect.width - 1;
	s16 maxY = clipRect.y + clipRect.height - 1;
	
	// Get start and point co-ords of graphics port
	s16 portX1 = getX();
	s16 portY1 = getY();
	s16 portX2 = getX() + _rect.width - 1;
	s16 portY2 = getY() + _rect.height - 1;
	
	// Choose larger start point values
	minX = minX > portX1 ? minX : portX1;
	minY = minY > portY1 ? minY : portY1;
	
	// Choose smaller end point values
	maxX = maxX < portX2 ? maxX : portX2;
	maxY = maxY < portY2 ? maxY : portY2;
	
	// Ensure values don't exceed clipping rectangle
	*x1 = *x1 > minX ? *x1 : minX;
	*y1 = *y1 > minY ? *y1 : minY;
	*x2 = *x2 < maxX ? *x2 : maxX;
	*y2 = *y2 < maxY ? *y2 : maxY;
	
	// Compensate for top screen offset
	if (*y1 >= TOP_SCREEN_Y_OFFSET) {
		*y1 -= TOP_SCREEN_Y_OFFSET;
		*y2 -= TOP_SCREEN_Y_OFFSET;
	}
	
	// Ensure Y values don't exceed screen dimensions
	if (*y1 > SCREEN_HEIGHT) {
		return false;
	}
	if (*y2 >= SCREEN_HEIGHT) {
		*y2 = SCREEN_HEIGHT - 1;
	}
	
	// Return false if no box to draw
	if ((*x2 < *x1) || (*y2 < *y1)) {
		return false;
	}
	
	// Return true as box can be drawn
	return true;
}

void GraphicsPort::drawPixel(s16 x, s16 y, u16 colour) {
	
	// Ignore command if gadget deleted or invisible
	if (!_gadget->isDrawingEnabled()) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);
	
	if (_clipRect == NULL) {
		// Draw all visible rects
		for (s32 i = 0; i < _clipRectList->size(); i++) {
			clipPixel(x, y, colour, _clipRectList->at(i));
		}
	} else {
		// Draw single rectangle
		clipPixel(x, y, colour, *_clipRect);
	}
}

// Clip and draw pixel
void GraphicsPort::clipPixel(s16 x, s16 y, u16 colour, const Rect& clipRect) {
	s16 clipX1 = x;
	s16 clipY1 = y;
	s16 clipX2 = x;
	s16 clipY2 = y;
	
	// Attempt to clip and draw
	if (clipCoordinates(&clipX1, &clipY1, &clipX2, &clipY2, clipRect)) {
		GraphicsUnclipped::drawPixel(clipX1, clipY1, colour);
	}
}

void GraphicsPort::drawXORPixel(s16 x, s16 y) {
	
	// Ignore command if gadget deleted or invisible
	if (!_gadget->isDrawingEnabled()) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);
	
	if (_clipRect == NULL) {
		// Draw all visible rects
		for (s32 i = 0; i < _clipRectList->size(); i++) {
			clipXORPixel(x, y, _clipRectList->at(i));
		}
	} else {
		// Draw single rectangle
		clipXORPixel(x, y, *_clipRect);
	}
}

void GraphicsPort::clipXORPixel(s16 x, s16 y, const Rect& clipRect) {
	s16 clipX1 = x;
	s16 clipY1 = y;
	s16 clipX2 = x;
	s16 clipY2 = y;
	
	// Attempt to clip and draw
	if (clipCoordinates(&clipX1, &clipY1, &clipX2, &clipY2, clipRect)) {
		GraphicsUnclipped::drawXORPixel(clipX1, clipY1);
	}
}

void GraphicsPort::drawLine(s16 x1, s16 y1, s16 x2, s16 y2, u16 colour) {
	
	// Ignore command if gadget deleted or invisible
	if (!_gadget->isDrawingEnabled()) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x1, &y1);
	convertPortToScreenSpace(&x2, &y2);
	
	if (_clipRect == NULL) {
		// Draw all visible rects
		for (s32 i = 0; i < _clipRectList->size(); i++) {
			clipLine(x1, y1, x2, y2, colour, _clipRectList->at(i));
		}
	} else {
		// Draw single rectangle
		clipLine(x1, y1, x2, y2, colour, *_clipRect);
	}
}

u8 GraphicsPort::getClipLineOutCode(s16 x, s16 y, s16 xMin, s16 yMin, s16 xMax, s16 yMax) {
	u8 code = 0;
	
	if (y > yMax) code |= 1;
	if (y < yMin) code |= 2;
	if (x > xMax) code |= 4;
	if (x < xMin) code |= 8;
	
	return code;
}

void GraphicsPort::clipLine(s16 x1, s16 y1, s16 x2, s16 y2, u16 colour, const Rect& clipRect) {
	
	// Extract data from cliprect
	s16 minX = clipRect.x;
	s16 minY = clipRect.y;
	s16 maxX = clipRect.x + clipRect.width - 1;
	s16 maxY = clipRect.y + clipRect.height - 1;
	
	if (clipCoordinates(&minX, &minY, &maxX, &maxY, clipRect)) {
	
		// Compensate for top screen offset
		if (y1 >= TOP_SCREEN_Y_OFFSET) {
			y1 -= TOP_SCREEN_Y_OFFSET;
			y2 -= TOP_SCREEN_Y_OFFSET;
		}
		
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
}

void GraphicsPort::copy(s16 sourceX, s16 sourceY, s16 destX, s16 destY, u16 width, u16 height) {
	
	// Ignore command if gadget deleted or invisible
	if (!_gadget->isDrawingEnabled()) return;

	GraphicsUnclipped::copy(sourceX, sourceY, destX, destY, width, height);
}

void GraphicsPort::scroll(s16 x, s16 y, s16 xDistance, s16 yDistance, u16 width, u16 height, WoopsiArray<Rect>* revealedRects) {
	
	// Ignore command if gadget deleted or invisible
	if (!_gadget->isDrawingEnabled()) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);
	
	// Scroll the region
	if (_clipRect == NULL) {
			
		// Scroll all visible rects
		for (s32 i = 0; i < _clipRectList->size(); i++) {
			clipScroll(x, y, xDistance, yDistance, width, height, _clipRectList->at(i), revealedRects);
		}
	} else {

		// Scroll single rectangle
		clipScroll(x, y, xDistance, yDistance, width, height, *_clipRect, revealedRects);
	}
}

void GraphicsPort::clipScroll(s16 x, s16 y, s16 xDistance, s16 yDistance, u16 width, u16 height, const Rect& clipRect, WoopsiArray<Rect>* revealedRects) {
	
	// Convert height/width to co-ordinates
	s16 sourceX1 = x;
	s16 sourceY1 = y;
	s16 sourceX2 = x + width - 1;
	s16 sourceY2 = y + height - 1;
	
	// Adjust source to compensate for the fact that we are scrolling, not copying - 
	// we are overwriting part of the source with itself
	if (xDistance < 0) sourceX1 -= xDistance;
	if (xDistance > 0) sourceX2 -= xDistance;
	if (yDistance < 0) sourceY1 -= yDistance;
	if (yDistance > 0) sourceY2 -= yDistance;
	
	// Get co-ords of destination rectangle
	s16 destX1 = sourceX1 + xDistance;
	s16 destY1 = sourceY1 + yDistance;
	s16 destX2 = sourceX2 + xDistance;
	s16 destY2 = sourceY2 + yDistance;
	
	// Check if the destination does not overlap the source - if so, we just
	// need to push the source region (clipped) to the redraw array
	if ((sourceX2 < sourceX1) || (sourceY2 < sourceY1)) {
		s16 newX1 = x;
		s16 newY1 = y;
		s16 newX2 = x + width - 1;
		s16 newY2 = y + height - 1;
		
		if (clipCoordinates(&newX1, &newY1, &newX2, &newY2, clipRect)) {
			Rect rect;
			rect.x = newX1;
			rect.y = newY1;
			rect.width = (newX2 - newX1) + 1;
			rect.height = (newY2 - newY1) + 1;
			revealedRects->push_back(rect);
		}
		
		return;
	}
		
	// Destination does overlap source, so attempt to use copy routine to scroll
	// the region
	if (clipCoordinates(&sourceX1, &sourceY1, &sourceX2, &sourceY2, clipRect)) {
		if (clipCoordinates(&destX1, &destY1, &destX2, &destY2, clipRect)) {
			
			s16 widthSource = (sourceX2 - sourceX1) + 1;
			s16 widthDest = (destX2 - destX1) + 1;
			s16 heightSource = (sourceY2 - sourceY1) + 1;
			s16 heightDest = (destY2 - destY1) + 1;
			
			// Get smallest dimensions - cannot copy into a space too small to accommodate it
			s16 minWidth = widthSource > widthDest ? widthDest : widthSource;
			s16 minHeight = heightSource > heightDest ? heightDest : heightSource;
			
			// Check if the post-clipping regions still overlap
			if ((sourceX1 + minWidth <= destX1) || (sourceX1 >= destX1 + minWidth) || (sourceY1 + minHeight <= destY1) || (sourceY1 >= destY1 + minHeight)) {
				
				// Post-clipping regions do not overlap - redraw the clipped source region
				s16 newX1 = x;
				s16 newY1 = y;
				s16 newX2 = x + width - 1;
				s16 newY2 = y + height - 1;
				
				if (clipCoordinates(&newX1, &newY1, &newX2, &newY2, clipRect)) {
					Rect rect;
					rect.x = newX1;
					rect.y = newY1;
					rect.width = (newX2 - newX1) + 1;
					rect.height = (newY2 - newY1) + 1;
					revealedRects->push_back(rect);
				}
				return;
			}
			
			// Post-clipping regions overlap, so copy the source to the dest
			GraphicsUnclipped::copy(sourceX1, sourceY1, destX1, destY1, minWidth, minHeight);
		
			// Work out the dimensions of the non-overlapped areas
			
			// Horizontal area
			if (sourceX1 < destX1) {
				
				// Revealed area on the left of the destination
				Rect rect;
				rect.x = sourceX1;
				rect.y = sourceY1;
				rect.width = (destX1 - sourceX1) + 1;
				rect.height = minHeight;
				
				revealedRects->push_back(rect);
				
				// Vertical area
				if (sourceY1 != destY1) {
					rect.x = destX1;
					rect.width = minWidth - rect.width;
					
					if (sourceY1 < destY1) {
						
						// Revealed area above the destination
						rect.height = (destY1 - sourceY1) + 1;
					} else if (sourceY1 > destY1) {
						
						// Revealed area below the destination
						rect.y = (destY1 + minHeight) - 1;
						rect.height = (sourceY1 - destY1) + 1;
					}
					
					revealedRects->push_back(rect);
				}
				
			} else if (sourceX1 > destX1) {
				
				// Revealed area on the right of the destination
				Rect rect;
				rect.x = (destX1 + width) - 1;
				rect.y = sourceY1;
				rect.width = (sourceX1 - destX1) + 1;
				rect.height = minHeight;
				revealedRects->push_back(rect);
				
				// Vertical area
				if (sourceY1 != destY1) {
					rect.x = sourceX1;
					rect.width = minWidth - rect.width;
					
					if (sourceY1 < destY1) {
						
						// Revealed area above the destination
						rect.height = (destY1 - sourceY1) + 1;
					} else if (sourceY1 > destY1) {
						
						// Revealed area below the destination
						rect.y = (destY1 + height) - 1;
						rect.height = (sourceY1 - destY1) + 1;
					}

					revealedRects->push_back(rect);
				}

			} else if (sourceY1 < destY1) {

				// Vertical movement only - revealed area above the destination
				Rect rect;
				rect.x = sourceX1;
				rect.y = sourceY1;
				rect.width = minWidth;
				rect.height = (destY1 - sourceY1) + 1;

				revealedRects->push_back(rect);

			} else if (sourceY1 > destY1) {

				// Vertical movement only - revealed area below the destination
				Rect rect;
				rect.x = sourceX1;
				rect.y = (destY1 + minHeight) - 1;
				rect.width = minWidth;
				rect.height = (sourceY1 - destY1) + 1;

				revealedRects->push_back(rect);
			}
		}
	}
}

void GraphicsPort::dim(s16 x, s16 y, u16 width, u16 height) {

	// Ignore command if gadget deleted or invisible
	if (!_gadget->isDrawingEnabled()) return;
	
	// Adjust from port-space to screen-space
	convertPortToScreenSpace(&x, &y);
	
	// Dim the region
	if (_clipRect == NULL) {
			
		// Dim all visible rects
		for (s32 i = 0; i < _clipRectList->size(); i++) {
			clipDim(x, y, width, height, _clipRectList->at(i));
		}
	} else {

		// Dim single rectangle
		clipDim(x, y, width, height, *_clipRect);
	}
}

void GraphicsPort::clipDim(s16 x, s16 y, u16 width, u16 height, const Rect& clipRect) {
	
	s16 x2 = x + width - 1;
	s16 y2 = y + height - 1;

	if (clipCoordinates(&x, &y, &x2, &y2, clipRect)) {

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
}
