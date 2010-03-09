#ifndef _FIXED_WIDTH_FONT_BASE_H_
#define _FIXED_WIDTH_FONT_BASE_H_

#include <nds.h>
#include "fontbase.h"

namespace WoopsiUI {

	/**
	 * Abstract class defining the basic properties of a fixed-width font and
	 * providing some of the essential functionality.  Should be used as a base
	 * class for all fixed-width fonts.
	 */
	class FixedWidthFontBase : public FontBase {

	public:

		/**
		 * Constructor.
		 * @param bitmapWidth The width of the bitmap containing the font's
		 * glyph data.
		 * @param bitmapHeight The height of the bitmap containing the font's
		 * glyph data.
		 * @param width The width of an individual glyph.
		 * @param height The height of the font.
		 * @param transparentColour The colour in the font bitmap used as the
		 * background colour.
		 */
		FixedWidthFontBase(const u16 bitmapWidth, const u16 bitmapHeight, const u8 width, const u8 height, const u16 transparentColour = 0);
		
		/**
		 * Destructor.
		 */
		virtual inline ~FixedWidthFontBase() { };

		/**
		 * Gets the width of the font's glyph bitmap.
		 * @return The width of the font's glyph bitmap.
		 */
		inline const u16 getBitmapWidth() const { return _bitmapWidth; };
		
		/**
		 * Gets the height of the font's glyph bitmap.
		 * @return The height of the font's glyph bitmap.
		 */
		inline const u16 getBitmapHeight() const { return _bitmapHeight; };

		/**
		 * Get the colour of the pixel at specified co-ordinates.
		 * @param x The x co-ordinate of the pixel.
		 * @param y The y co-ordinate of the pixel.
		 * @return The colour of the pixel.
		 */
		virtual const u16 getPixel(const s16 x, const s16 y) const = 0;

		/**
		 * Gets the width of an individual glyph.
		 * @return The width of an individual glyph.
		 */
		inline const u8 getWidth() const { return _width; };

		/**
		 * Get the width of a string in pixels when drawn with this font.
		 * @param text The string to check.
		 * @return The width of the string in pixels.
		 */
		u16 getStringWidth(const WoopsiString& text) const;

		/**
		 * Get the width of a portion of a string in pixels when drawn with this
		 * font.
		 * Useful if you want to determine the length of a string without a
		 * terminator, or
		 * the length of a section of a string.
		 * @param text The string to check.
		 * @param startIndex The start point of the substring within the string.
		 * @param length The length of the substring in chars.
		 * @return The width of the substring in pixels.
		 */
		inline u16 getStringWidth(const WoopsiString& text, s32 startIndex, s32 length) const { return length * getWidth(); };

		/**
		 * Get the width of an individual character.
		 * @param letter The character to get the width of.
		 * @return The width of the character in pixels.
		 */
		inline u16 getCharWidth(u32 letter) const { return getWidth(); };

		/**
		 * Checks if supplied character is blank in the current font.
		 * @param letter The character to check.
		 * @return True if the glyph contains any pixels to be drawn.  False if
		 * the glyph is blank.
		 */
		virtual inline const bool isCharBlank(const u32 letter) const { return !(_glyphMap[letter >> 3] & (1 << (letter % 8))); };
		
	protected: 
		/**
		 * Populates the previously created glyph map with the data on which
		 * glyphs have data and which do not.  Does this by scanning through
		 * each pixel in each glyph and adding true to the map if data is found
		 * or false if no data is found.
		 * @see initGlyphMap().
		 */
		virtual void createGlyphMap();

		/**
		 * Scans the glyph bitmap at the specified co-ordinates to see if it
		 * contains data or not.  Must be overridden.
		 * @param x The x co-ordinate of the start of the glyph.
		 * @param y The y co-ordinate of the start of the glyph.
		 * @return True if the glyph bitmap contains data for this glyph.
		 */
		virtual const bool scanGlyph(const u16 x, const u16 y) const = 0;

	protected:

		/**
		 * Creates a map of which glyphs have data and which do not.
		 * @see createGlyphMap().
		 */
		void initGlyphMap();

		u8 _glyphMap[GLYPH_MAP_SIZE];		/**< Map of which characters have glyphs in the current font. */

	private:
		u8 _width;							/**< The width of a single glyph. */
		u16 _bitmapWidth;					/**< Width of the bitmap containing glyph data. */
		u16 _bitmapHeight;					/**< Height of the bitmap containing glyph data. */
	};
}

#endif
