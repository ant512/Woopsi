#include "textwriter.h"
#include "woopsifuncs.h"
#include "mutablebitmapbase.h"
#include "framebuffer.h"

using namespace WoopsiUI;

void TextWriter::drawChar(FontBase* font, char letter, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2) {
	FrameBuffer* bitmap = NULL;

	if (y > TOP_SCREEN_Y_OFFSET - 1) {
		y -= TOP_SCREEN_Y_OFFSET;
		clipY2 -= TOP_SCREEN_Y_OFFSET;
		clipY1 -= TOP_SCREEN_Y_OFFSET;
		bitmap = frameBuffer[1];
	} else {
		bitmap = frameBuffer[0];
	}

	font->drawChar(bitmap, letter, x, y, clipX1, clipY1, clipX2, clipY2);
}

void TextWriter::drawString(FontBase* font, const char* string, u8 length, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2) {
	FrameBuffer* bitmap = NULL;

	if (y > TOP_SCREEN_Y_OFFSET - 1) {
		y -= TOP_SCREEN_Y_OFFSET;
		clipY2 -= TOP_SCREEN_Y_OFFSET;
		clipY1 -= TOP_SCREEN_Y_OFFSET;
		bitmap = frameBuffer[1];
	} else {
		bitmap = frameBuffer[0];
	}

	drawString(bitmap, font, string, length, x, y, clipX1, clipY1, clipX2, clipY2);
}

// Output a whole string to the display
void TextWriter::drawString(MutableBitmapBase* bitmap, FontBase* font, const char* string, u8 length, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2) {
	for (u32 i = 0; i < length; i++) {
		x = font->drawChar(bitmap, string[i], x, y, clipX1, clipY1, clipX2, clipY2);
	}
}
