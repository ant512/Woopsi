#ifndef _FONT_H_
#define _FONT_H_

#include <nds.h>
#include "fixedwidthfontbase.h"

namespace WoopsiUI {

	/**
	 * Fixed-width multicolour bitmap font that uses a 16-bit bitmap as its glyph data.
	 */
	class Font : public FixedWidthFontBase {

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

		/**
		 * Draw an individual character of the font to the specified bitmap.
		 * @param bitmap The bitmap to draw to.
		 * @param bitmapWidth The width of the bitmap being drawn to.
		 * @param bitmapHeight The height of the bitmap being drawn to.
		 * @param letter The character to output.
		 * @param x The x co-ordinate of the text.
		 * @param y The y co-ordinate of the text.
		 * @param clipX1 The left edge of the clipping rectangle.
		 * @param clipY1 The top edge of the clipping rectangle.
		 * @param clipX2 The right edge of the clipping rectangle.
		 * @param clipY2 The bottom edge of the clipping rectangle.
		 * @return The x co-ordinate for the next character to be drawn.
		 */
		s16 drawChar(u16* bitmap, u16 bitmapWidth, u16 bitmapHeight, char letter, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2);

	private:
		const u16* _bitmap;				/**< Pointer to the bitmap containing the font glyphs */
		
		/**
		 * Scans the glyph bitmap at the specified co-ordinates to see if it contains data or
		 * not.  Must be overridden.
		 * @param x The x co-ordinate of the start of the glyph.
		 * @param y The y co-ordinate of the start of the glyph.
		 * @return True if the glyph bitmap contains data for this glyph.
		 */
		const bool scanGlyph(const u16 x, const u16 y) const;
	};
}

#endif
