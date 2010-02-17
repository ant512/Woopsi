#ifndef _FONT_H_
#define _FONT_H_

#include <nds.h>
#include "fixedwidthfontbase.h"

namespace WoopsiUI {

	class MutableBitmapBase;
	class BitmapBase;

	/**
	 * Fixed-width multicolour bitmap font that uses a 16-bit bitmap as its
	 * glyph data.
	 */
	class Font : public FixedWidthFontBase {

	public:

		/**
		 * Constructor.
		 * @param bitmap Bitmap containing the font's glyph data.
		 * @param width The width of a single glyph.
		 * @param height The height of a single glyph.
		 * @param transparentColour The colour of the font's background.
		 */
		Font(const BitmapBase* bitmap, const u8 width, const u8 height, const u16 transparentColour);
		
		/**
		 * Destructor.
		 */
		inline ~Font() { };

		/**
		 * Get the colour of the pixel at the specified offset from the start of
		 * the bitmap.
		 * @param x The x co-ordinate of the pixel.
		 * @param y The y co-ordinate of the pixel.
		 * @return The pixel colour.
		 */
		const u16 getPixel(const s16 x, const s16 y) const;

		/**
		 * Draw an individual character of the font to the specified bitmap.
		 * @param bitmap The bitmap to draw to.
		 * @param letter The character to output.
		 * @param x The x co-ordinate of the text.
		 * @param y The y co-ordinate of the text.
		 * @param clipX1 The left edge of the clipping rectangle.
		 * @param clipY1 The top edge of the clipping rectangle.
		 * @param clipX2 The right edge of the clipping rectangle.
		 * @param clipY2 The bottom edge of the clipping rectangle.
		 * @return The x co-ordinate for the next character to be drawn.
		 */
		s16 drawChar(MutableBitmapBase* bitmap, u32 letter, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2);

	private:
		const BitmapBase* _bitmap;				/**< Pointer to the bitmap containing the font glyphs. */
		
		/**
		 * Scans the glyph bitmap at the specified co-ordinates to see if it
		 * contains data or not.
		 * @param x The x co-ordinate of the start of the glyph.
		 * @param y The y co-ordinate of the start of the glyph.
		 * @return True if the glyph bitmap contains data for this glyph.
		 */
		const bool scanGlyph(const u16 x, const u16 y) const;
	};
}

#endif
