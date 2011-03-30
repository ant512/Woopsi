#ifndef _FONT_BASE_H_
#define _FONT_BASE_H_

#include <nds.h>

const s32 GLYPH_MAP_SIZE = 32;

namespace WoopsiUI {

	class MutableBitmapBase;
	class WoopsiString;

	/**
	 * Abstract class defining the basic properties of a font and providing some
	 * of the essential functionality.  Should be used as a base class for all
	 * fonts.
	 */
	class FontBase {

	public:

		/**
		 * Constructor.
		 */
		FontBase() { }
		
		/**
		 * Destructor.
		 */
		virtual inline ~FontBase() { };

		/**
		 * Checks if supplied character is blank in the current font.
		 * @param letter The character to check.
		 * @return True if the glyph contains any pixels to be drawn.  False if
		 * the glyph is blank.
		 */
		virtual const bool isCharBlank(const u32 letter) const = 0;

		/**
		 * Draw an individual character of the font to the specified bitmap.
		 * @param bitmap The bitmap to draw to.
		 * @param letter The character to output.
		 * @param colour The colour to draw with.
		 * @param x The x co-ordinate of the text.
		 * @param y The y co-ordinate of the text.
		 * @param clipX1 The left edge of the clipping rectangle.
		 * @param clipY1 The top edge of the clipping rectangle.
		 * @param clipX2 The right edge of the clipping rectangle.
		 * @param clipY2 The bottom edge of the clipping rectangle.
		 * @return The x co-ordinate for the next character to be drawn.
		 */
		virtual s16 drawChar(MutableBitmapBase* bitmap, u32 letter, u16 colour, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2) = 0;

		/**
		 * Draw an individual character of the font to the specified bitmap on a baseline.
		 * @param bitmap The bitmap to draw to.
		 * @param letter The character to output.
		 * @param colour The colour to draw with.
		 * @param x The x co-ordinate of the pen.
		 * @param y The y co-ordinate of the pen.
		 * @param clipX1 The left edge of the clipping rectangle.
		 * @param clipY1 The top edge of the clipping rectangle.
		 * @param clipX2 The right edge of the clipping rectangle.
		 * @param clipY2 The bottom edge of the clipping rectangle.
		 * @return The x co-ordinate for the next character to be drawn.
		 */
		virtual s16 drawBaselineChar(MutableBitmapBase* bitmap, u32 letter, u16 colour, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2) = 0;
		
		/**
		 * Get the width of a string in pixels when drawn with this font.
		 * @param text The string to check.
		 * @return The width of the string in pixels.
		 */
		virtual u16 getStringWidth(const WoopsiString& text) const = 0;

		/**
		 * Get the width of a portion of a string in pixels when drawn with this
		 * font.
		 * @param text The string to check.
		 * @param startIndex The start point of the substring within the string.
		 * @param length The length of the substring in chars.
		 * @return The width of the substring in pixels.
		 */
		virtual u16 getStringWidth(const WoopsiString& text, s32 startIndex, s32 length) const = 0;

		/**
		 * Get the width of an individual character.
		 * @param letter The character to get the width of.
		 * @return The width of the character in pixels.
		 */
		virtual u8 getCharWidth(u32 letter) const = 0;

		/**
		 * Get the height of an individual character.
		 * @param letter The letter to get the height of.
		 * @return The height of the character in pixels.
		 */
		virtual u8 getCharHeight(u32 letter) const = 0;
		
		/**
		 * Get the algebric distance from baseline to top of an individual character.
		 * @param letter The character to get the "top" of.
		 * @return The top of the character in pixels.
		 */
		virtual s8 getCharTop(u32 letter) const = 0;		

		/**
		 * Gets the height of the font.
		 * @return The height of the font.
		 */
		virtual const u8 getHeight() const = 0;
	};
}

#endif
