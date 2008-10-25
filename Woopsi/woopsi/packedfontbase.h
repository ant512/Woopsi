/* Contributed by Jeff Laing
 * 20080414
 */

#ifndef _PACKED_FONT_BASE_
#define _PACKED_FONT_BASE_

#include <nds.h>
#include "fontbase.h"

namespace WoopsiUI {

	/**
	 * PackedFont is a base class defining a font whose data is packed into a
	 * more efficient data format.
	 */
	class PackedFontBase : public FontBase
	{
	public:
		/**
		 * Constructor.
		 * @param first Ascii index of first character in glphyDdata.
		 * @param last Ascii index of last character in glyphData.
		 * @param glyphData Packed array representing font.
		 * @param glyphOffset Offset into glyphData[] of character[i].
		 * @param glyphWidth Pixel width of character[i].
		 * @param height The height of the font.
		 * @param spWidth The width of a space character
		 * @param fixedWidth Character width (fixed), or 0 for proportional.
		 */
		PackedFontBase(
			u8 first, u8 last,
			const u16 *glyphData,
			const u16 *glyphOffset,
			const u8 *glyphWidth,
			const u8 height,
			const u8 spWidth,
			const u8 fixedWidth = 0)
			:
			  FontBase(height, 0),
			  _first(first), _last(last),
			  _glyphData(glyphData), _glyphOffset(glyphOffset), _glyphWidth(glyphWidth),
			  _fontWidth(0),	// Thanks, John
			  _spWidth(spWidth), _widMax(fixedWidth) { }

		/**
		 * Makes this font fixed-width, though doesn't allow the spacing to be less
		 * than the font's known maximum character width.  If you want fixed spacing but
		 * don't know the required size, pass 1.
		 * @param fontWidth Font width to use (0 for proportional, 1 for wide-enough)
		 */
		inline void setFontWidth(u8 fontWidth) { _fontWidth = (fontWidth && (fontWidth < _widMax)) ? _widMax : fontWidth; }

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
		virtual s16 drawChar(
			u16* bitmap,
			u16 bitmapWidth, u16 bitmapHeight,
			char letter,
			s16 x, s16 y,
			u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2);

		/**
		 * Get the width of a string in pixels when drawn with this font.
		 * @param text The string to check.
		 * @return The width of the string in pixels.
		 */
		virtual u16 getStringWidth(const char* text) const;

		/**
		 * Get the width of a string with a specified length in pixels when drawn with this font.
		 * Useful if you want to determine the width of a string without a terminator, or
		 * the width of a section of a string.
		 * @param text The string to check.
		 * @param length The length of the string in chars.
		 * @return The width of the string in pixels.
		 */
		virtual u16 getStringWidth(const char* text, u16 length) const;

		/**
		 * Get the width of an individual character.
		 * @param letter The character to get the width of.
		 * @return The width of the character in pixels.
		 */
		virtual u16 getCharWidth(char letter) const;

		/**
		 * Text uses this to decide whether a given character is blank or not
		 */
		const bool isCharBlank(const char letter) const;

		/**
		 * Render an individual character of the font to the specified bitmap.
		 * @param pixelData The font-specific pixel data
		 * @param pixelsPerRow The number of pixels to render per row (for this character)
		 * @param bitmap Top-left corner of output space to draw to.
		 * @param bitmapWidth The width of the bitmap being drawn to.
		 * @param x The x co-ordinate of the text.
		 * @param y The y co-ordinate of the text.
		 * @param clipX1 The left edge of the clipping rectangle.
		 * @param clipY1 The top edge of the clipping rectangle.
		 * @param clipX2 The right edge of the clipping rectangle.
		 * @param clipY2 The bottom edge of the clipping rectangle.
		 */
		virtual void renderChar(
			const u16* pixelData, u16 pixelsPerRow,
			u16* bitmap, u16 bitmapWidth,
			s16 x, s16 y,
			u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2) = 0;

	protected:
		u8 _first, _last;
		const u16 *_glyphData;	
		const u16 *_glyphOffset;
		const u8 *_glyphWidth;
		u8 _fontWidth;
		u8 _spWidth;
		u8 _widMax;
	};
}

#endif
