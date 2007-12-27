#ifndef _MONO_FONT_H_
#define _MONO_FONT_H_

#include <nds.h>
#include "fontbase.h"

using namespace std;

class MonoFont : public FontBase {

public:
	MonoFont(const u16* bitmap, const u16 bitmapWidth, const u16 bitmapHeight, const u8 width, const u8 height, const u16 drawColour);
	~MonoFont();

	const u16 getPixel(const u32 position) const;

private:
	const u16* _bitmap;
	
	const bool scanGlyph(const u16 x, const u16 y) const;
};

#endif
