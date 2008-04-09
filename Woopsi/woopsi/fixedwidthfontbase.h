#ifndef _FIXED_WIDTH_FONT_BASE_H_
#define _FIXED_WIDTH_FONT_BASE_H_

#include <nds.h>
#include <string.h>
#include "fontbase.h"

using namespace std;

/**
 * Abstract class defining the basic properties of a fixed-width font and providing some of the
 * essential functionality.  Should be used as a base class for all fixed-width fonts.
 */
class FixedWidthFontBase : public FontBase {

public:

	/**
	 * Constructor.
	 * @param bitmapWidth The width of the bitmap containing the font's glyph data.
	 * @param bitmapHeight The height of the bitmap containing the font's glyph data.
	 * @param width The width of an individual glyph.
	 * @param height The height of the font.
	 * @param transparentColour The colour in the font bitmap used as the background
	 * colour.
	 */
	FixedWidthFontBase(const u16 bitmapWidth, const u16 bitmapHeight, const u8 width, const u8 height, const u16 transparentColour = 0);
	
	/**
	 * Destructor.
	 */
	virtual inline ~FixedWidthFontBase() { };

	/**
	 * Get the colour of a pixel at a particular offset from the start of the bitmap.  Must
	 * be overridden in subclasses.
	 * @param position The offset of the pixel from the start of the bitmap.
	 * @return The colour of the pixel.
	 */
	virtual const u16 getPixel(const u32 position) const = 0;

	/**
	 * Get the colour of the pixel at specified co-ordinates.
	 * @param x The x co-ordinate of the pixel.
	 * @param y The y co-ordinate of the pixel.
	 * @return The colour of the pixel.
	 */
	inline const u16 getPixel(const u16 x, const u16 y) const { return getPixel(x + (y * _width)); };

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
	inline u16 getStringWidth(char* text) { return strlen(text) * getWidth(); };

	/**
	 * Get the width of a string with a specified length in pixels when drawn with this font.
	 * Useful if you want to determine the length of a string without a terminator, or
	 * the length of a section of a string.
	 * @param text The string to check.
	 * @param length The length of the string in chars.
	 * @return The width of the string in pixels.
	 */
	inline u16 getStringWidth(char* text, u16 length) { return length * getWidth(); };

	/**
	 * Get the width of an individual character.
	 * @param letter The character to get the width of.
	 * @return The width of the character in pixels.
	 */
	inline u16 getCharWidth(char letter) { return getWidth(); };
	
protected: 
	/**
	 * Populates the previously created glyph map with the data on which glyphs have data
	 * and which do not.  Does this by scanning through each pixel in each glyph and adding
	 * true to the map if data is found or false if no data is found.
	 * @see initGlyphMap().
	 */
	virtual void createGlyphMap();

private:
	u8 _width;
};

#endif
