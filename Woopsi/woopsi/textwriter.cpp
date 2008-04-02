#include "textwriter.h"
#include "woopsifuncs.h"

void TextWriter::drawChar(FontBase* font, char letter, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2) {
	u16* bitmap = NULL;

	if (y > TOP_SCREEN_Y_OFFSET - 1) {
		y -= TOP_SCREEN_Y_OFFSET;
		clipY2 -= TOP_SCREEN_Y_OFFSET;
		clipY1 -= TOP_SCREEN_Y_OFFSET;
		bitmap = DrawBg[1];
	} else {
		bitmap = DrawBg[0];
	}

	drawChar(bitmap, SCREEN_WIDTH, SCREEN_HEIGHT, font, letter, 0, x, y, clipX1, clipY1, clipX2, clipY2);
}

void TextWriter::drawString(FontBase* font, char* string, u8 length, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2) {
	u16* bitmap = NULL;

	if (y > TOP_SCREEN_Y_OFFSET - 1) {
		y -= TOP_SCREEN_Y_OFFSET;
		clipY2 -= TOP_SCREEN_Y_OFFSET;
		clipY1 -= TOP_SCREEN_Y_OFFSET;
		bitmap = DrawBg[1];
	} else {
		bitmap = DrawBg[0];
	}

	drawString(bitmap, SCREEN_WIDTH, SCREEN_HEIGHT, font, string, length, x, y, clipX1, clipY1, clipX2, clipY2);
}

// Output a whole string to the display
void TextWriter::drawString(u16* bitmap, u16 bitmapWidth, u16 bitmapHeight, FontBase* font, char* string, u8 length, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2) {

	for (u8 i = 0; i < length; i++) {

		// TODO: Preclip text output here
		drawChar(bitmap, bitmapWidth, bitmapHeight, font, string[i], i, x, y, clipX1, clipY1, clipX2, clipY2);
	}
}

// Draw a single character at a particular x pos
void TextWriter::drawChar(u16* bitmap, u16 bitmapWidth, u16 bitmapHeight, FontBase* font, char letter, s16 charIndex, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2) {
	
	// Check that the font has data for the glyph
	if (font->checkGlyphData(letter)) {
	
		u16 letterStart = letter * font->getWidth();
		u8 letterStartY = 0;
		u16 sourcePixel;

		// Extract data from font object for extra speed
		u16 transparentColour = font->getTransparentColour();
		u8 fontWidth = font->getWidth();
		u8 fontHeight = font->getHeight();
		u16 fontBitmapWidth = font->getBitmapWidth();
		
		// Calculate starting x position of this letter
		x += (charIndex * fontWidth);

		// Calculate the start position of the letter in the font bitmap
		letterStartY = letterStart / fontBitmapWidth;
		letterStart %= fontBitmapWidth;

		letterStart += fontHeight * (letterStartY * fontBitmapWidth);

		// TODO: Finish per-row/per-column clipping here
		// Calculate values for clipping
		s16 startX = x > clipX1 ? x : clipX1;
		s16 endX = x + fontWidth - 1 < clipX2 ? x + fontWidth - 1: clipX2;
		s16 startY = y > clipY1 ? y : clipY1;
		s16 endY = y + fontHeight - 1 < clipY2 ? y + fontHeight - 1: clipY2;

		// Draw letter to the screen in rows

		u16 sourceColour = 0;

		for (s16 destPixelY = startY; destPixelY <= endY; destPixelY++) {

			for (s16 destPixelX = startX; destPixelX <= endX; destPixelX++) {

				// Get the array index of the pixel to copy
				sourcePixel = letterStart + (destPixelX - x) + ((destPixelY - y) * fontBitmapWidth);

				sourceColour = font->getPixel(sourcePixel);

				// Is the pixel part of the font, not the background?
				if (sourceColour != transparentColour) {

					// Draw text pixel
					bitmap[destPixelX + (destPixelY * bitmapWidth)] = sourceColour;
				}
			}
		}
	}
}
