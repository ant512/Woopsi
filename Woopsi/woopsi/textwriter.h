#ifndef _TEXTWRITER_H_
#define _TEXTWRITER_H_

#include <nds.h>
#include "fontbase.h"
#include "defines.h"

using namespace std;

class TextWriter {

public:
	static void drawString(FontBase* font, char* string, u8 length, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2);
	static void drawString(u16* bitmap, u16 bitmapWidth, u16 bitmapHeight, FontBase* font, char* string, u8 length, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2);
	static void drawChar(FontBase* font, char letter, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2);
	static void drawChar(u16* bitmap, u16 bitmapWidth, u16 bitmapHeight, FontBase* font, char letter, s16 charIndex, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2);

private:
	TextWriter();
	~TextWriter();
};

#endif
