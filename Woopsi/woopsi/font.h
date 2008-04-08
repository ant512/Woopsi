#ifndef _FONT_H_
#define _FONT_H_

#include <nds.h>
#include "fontbase.h"

using namespace std;

/**
 * Fixed-width multicolour bitmap font that uses a 16-bit bitmap as its glyph data.
 */
class Font : public FontBase {

public:

	/**
	 * Constructor.
	 * @param bitmap Bitmap containing the font's glyph data.
	 * @param bitmapWidth The width of the glyph bitmap.
	 * @param bitmapHeight The height of the glyph bitmap.
	 * @param width The width of a single glyph.
	 * @param height The height of a single glyph.
	 * @param transparentColour The colour of the font's background.
	 */
	Font(const u16* bitmap, const u16 bitmapWidth, const u16 bitmapHeight, const u8 width, const u8 height, const u16 transparentColour);
	
	/**
	 * Destructor.
	 */
	inline ~Font() { };

	/**
	 * Get the colour of the pixel at the specified offset from the start of the bitmap.
	 * @param position The pixel's offset from the start of the bitmap.
	 * @return The pixel colour.
	 */
	const u16 getPixel(const u32 position) const;

private:
	const u16* _bitmap;
	
	/**
	 * Scans the glyph bitmap at the specified co-ordinates to see if it contains data or
	 * not.  Must be overridden.
	 * @param x The x co-ordinate of the start of the glyph.
	 * @param y The y co-ordinate of the start of the glyph.
	 * @return True if the glyph bitmap contains data for this glyph.
	 */
	const bool scanGlyph(const u16 x, const u16 y) const;
};

#endif
