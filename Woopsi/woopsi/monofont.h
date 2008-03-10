#ifndef _MONO_FONT_H_
#define _MONO_FONT_H_

#include <nds.h>
#include "fontbase.h"

using namespace std;

/**
 * Fixed-width monochrome bitmap font that uses a bitpacked bitmap of u16s as its glyph
 * data.  1 represents a filled pixel; 0 represents an empty pixel.
 */
class MonoFont : public FontBase {

public:

	/**
	 * Constructor.
	 * @param bitmap Bitmap containing the font's glyph data.
	 * @param bitmapWidth The width of the glyph bitmap.
	 * @param bitmapHeight The height of the glyph bitmap.
	 * @param width The width of a single glyph.
	 * @param height The height of a single glyph.
	 * @param drawColour The colour to use for text output.
	 */
	MonoFont(const u16* bitmap, const u16 bitmapWidth, const u16 bitmapHeight, const u8 width, const u8 height, const u16 drawColour);
	
	/**
	 * Destructor.
	 */
	~MonoFont();

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
