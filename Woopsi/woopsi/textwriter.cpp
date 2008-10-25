#include "textwriter.h"
#include "woopsifuncs.h"

using namespace WoopsiUI;

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

	font->drawChar(bitmap, SCREEN_WIDTH, SCREEN_HEIGHT, letter, x, y, clipX1, clipY1, clipX2, clipY2);
}

void TextWriter::drawString(FontBase* font, const char* string, u8 length, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2) {
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
void TextWriter::drawString(u16* bitmap, u16 bitmapWidth, u16 bitmapHeight, FontBase* font, const char* string, u8 length, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2) {
	for (u8 i = 0; i < length; i++) {
		x = font->drawChar(bitmap, bitmapWidth, bitmapHeight, string[i], x, y, clipX1, clipY1, clipX2, clipY2);
	}
}
