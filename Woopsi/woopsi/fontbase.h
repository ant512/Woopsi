#ifndef _FONT_BASE_H_
#define _FONT_BASE_H_

#include <nds.h>
#include <string.h>

#define GLYPH_MAP_SIZE 32

namespace WoopsiUI {

	/**
	 * Abstract class defining the basic properties of a font and providing some of the
	 * essential functionality.  Should be used as a base class for all fonts.
	 * This class does not currently allow for non-bitmap fonts.
	 */
	class FontBase {

	public:

		/**
		 * Constructor.
		 * @param height The height of the font in pixels.
		 * @param transparentColour The colour in the font bitmap used as the background
		 * colour.
		 */
		FontBase(const u8 height, const u16 transparentColour = 0) {
			_height = height;
			_drawColour = 0;
			_isMonochrome = false;
		}
		
		/**
		 * Destructor.
		 */
		virtual inline ~FontBase() { };

		/**
		 * Checks if supplied character is blank in the current font.
		 * @param letter The character to check.
		 * @return True if the glyph contains any pixels to be drawn.  False if the glyph is
		 * blank.
		 */
		virtual const bool isCharBlank(const char letter) const = 0;
		
		/**
		 * Sets the colour to use as the drawing colour.  If set, this overrides the colours
		 * present in a non-monochrome font.
		 * @param colour The new drawing colour.
		 */
		void setColour(const u16 colour) {
			_drawColour = colour;
			_isMonochrome = true;
		}
		
		/**
		 * Gets the colour currently being used as the drawing colour.  This should be used
		 * in conjunction with isMonochrome() to determine if this is really being used or not;
		 * isMonochrome() must be true for this colour to be used.
		 * @return The current drawing colour.
		 */
		inline const u16 getColour() const { return _drawColour; };
		
		/**
		 * Returns true if the current font is being drawn using a single colour.
		 * @return True if the current font is monochrome.
		 */
		inline const bool isMonochrome() const { return _isMonochrome; };
		
		/**
		 * Get the colour currently being used as the transparent background colour.
		 * @return The transparent background colour.
		 */
		inline const u16 getTransparentColour() const { return _transparentColour; };
		
		/**
		 * Sets the transparent background colour to a new value.
		 * @param colour The new background colour.
		 */
		inline void setTransparentColour(const u16 colour) { _transparentColour = colour; };

		/**
		 * Resets back to mulitcolour mode if the font supports it and is currently set to
		 * monochrome mode.
		 */
		inline void clearColour() {
			_isMonochrome = false;
			_drawColour = 0;	
		};

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
		virtual s16 drawChar(u16* bitmap, u16 bitmapWidth, u16 bitmapHeight, char letter, s16 x, s16 y, u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2) = 0;
		
		/**
		 * Get the width of a string in pixels when drawn with this font.
		 * @param text The string to check.
		 * @return The width of the string in pixels.
		 */
		virtual u16 getStringWidth(const char* text) const = 0;

		/**
		 * Get the width of a string with a specified length in pixels when drawn with this font.
		 * Useful if you want to determine the length of a string without a terminator, or
		 * the length of a section of a string.
		 * @param text The string to check.
		 * @param length The length of the string in chars.
		 * @return The width of the string in pixels.
		 */
		virtual u16 getStringWidth(const char* text, u16 length) const = 0;

		/**
		 * Get the width of an individual character.
		 * @param letter The character to get the width of.
		 * @return The width of the character in pixels.
		 */
		virtual u16 getCharWidth(char letter) const = 0;

		/**
		 * Gets the height of the font.
		 * @return The height of the font.
		 */
		inline const u8 getHeight() const { return _height; };

	private:
		u8 _height;								/**< Height of the font */
		u16 _drawColour;						/**< Colour to draw the font with when rendering */
		bool _isMonochrome;						/**< True if the font is not multicolour */
		u16 _transparentColour;					/**< Background colour that should not be rendered */
	};
}

#endif
