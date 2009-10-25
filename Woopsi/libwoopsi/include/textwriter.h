#ifndef _TEXTWRITER_H_
#define _TEXTWRITER_H_

#include <nds.h>
#include "fontbase.h"
#include "defines.h"

namespace WoopsiUI {

	/**
	 * Class providing static methods for text output.
	 */
	class TextWriter {

	public:

		/**
		 * Output an entire string to one of the DS' screens.  The screen number is calculated
		 * based on the specified Y co-ordinate.  Text is clipped to fit within the clipping
		 * co-ordinates.
		 * @param font The font to use for the glyph data.
		 * @param string The string to output.
		 * @param length The length of the string to output.  This makes it possible to supply
		 * a length smaller than the actual length of the string if the whole string should not
		 * be printed or if it does not end in a terminator character.
		 * @param x The x co-ordinate of the text.
		 * @param y The y co-ordinate of the text.
		 * @param clipX1 The left edge of the clipping rectangle.
		 * @param clipY1 The top edge of the clipping rectangle.
		 * @param clipX2 The right edge of the clipping rectangle.
		 * @param clipY2 The bottom edge of the clipping rectangle.
		 */
		static void drawString(FontBase* font, const char* string, u8 length, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2);
		
		/**
		 * Output an entire string to the specified bitmap.
		 * @param bitmap The bitmap to draw to.
		 * @param bitmapWidth The width of the bitmap being drawn to.
		 * @param bitmapHeight The height of the bitmap being drawn to.
		 * @param font The font to use for the glyph data.
		 * @param string The string to output.
		 * @param length The length of the string to output.  This makes it possible to supply
		 * a length smaller than the actual length of the string if the whole string should not
		 * be printed or if it does not end in a terminator character.
		 * @param x The x co-ordinate of the text.
		 * @param y The y co-ordinate of the text.
		 * @param clipX1 The left edge of the clipping rectangle.
		 * @param clipY1 The top edge of the clipping rectangle.
		 * @param clipX2 The right edge of the clipping rectangle.
		 * @param clipY2 The bottom edge of the clipping rectangle.
		 */
		static void drawString(u16* bitmap, u16 bitmapWidth, u16 bitmapHeight, FontBase* font, const char* string, u8 length, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2);
		
		/**
		 * Output a single character to one of the DS' screens.  The screen number is
		 * calculated based on the specified Y co-ordinate.  Text is clipped to fit within the
		 * clipping co-ordinates.
		 * @param font The font to use for the glyph data.
		 * @param letter The character to output.
		 * @param x The x co-ordinate of the text.
		 * @param y The y co-ordinate of the text.
		 * @param clipX1 The left edge of the clipping rectangle.
		 * @param clipY1 The top edge of the clipping rectangle.
		 * @param clipX2 The right edge of the clipping rectangle.
		 * @param clipY2 The bottom edge of the clipping rectangle.
		 */
		static void drawChar(FontBase* font, char letter, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2);
		
		/**
		 * Output a single character to one of the DS' screens.  The screen number is
		 * calculated based on the specified Y co-ordinate.  Text is clipped to fit within the
		 * clipping co-ordinates.  This function is intended to be used to print letters that
		 * appear within a larger string, as the index of the character within that string
		 * must be supplied, and the x co-ordinate of the character is adjusted to take
		 * the character's index into account.
		 * @param bitmap The bitmap to draw to.
		 * @param bitmapWidth The width of the bitmap being drawn to.
		 * @param bitmapHeight The height of the bitmap being drawn to.
		 * @param font The font to use for the glyph data.
		 * @param letter The character to output.
		 * @param charIndex The position of this character within a larger string.
		 * @param x The x co-ordinate of the text.
		 * @param y The y co-ordinate of the text.
		 * @param clipX1 The left edge of the clipping rectangle.
		 * @param clipY1 The top edge of the clipping rectangle.
		 * @param clipX2 The right edge of the clipping rectangle.
		 * @param clipY2 The bottom edge of the clipping rectangle.
		 */
		static void drawChar(u16* bitmap, u16 bitmapWidth, u16 bitmapHeight, FontBase* font, char letter, s16 charIndex, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2);

	private:
		/**
		 * Constructor.  Private to prevent creation.
		 */
		inline TextWriter() { };
		
		/**
		 * Destructor.  Private to prevent deletion.
		 */
		inline ~TextWriter() { };
	};
}

#endif
