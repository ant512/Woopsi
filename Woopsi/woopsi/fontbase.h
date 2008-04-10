#ifndef _FONT_BASE_H_
#define _FONT_BASE_H_

#include <nds.h>
#include <string.h>

#define GLYPH_MAP_SIZE 32

using namespace std;

/**
 * Abstract class defining the basic properties of a font and providing some of the
 * essential functionality.  Should be used as a base class for all fonts.
 * This class does not currently allow for non-bitmap fonts.
 */
class FontBase {

public:

	/**
	 * Constructor.
	 * @param bitmapWidth The width of the bitmap containing the font's glyph data.
	 * @param bitmapHeight The height of the bitmap containing the font's glyph data.
	 * @param height The height of the font in pixels.
	 * @param transparentColour The colour in the font bitmap used as the background
	 * colour.
	 */
	FontBase(const u16 bitmapWidth, const u16 bitmapHeight, const u8 height, const u16 transparentColour = 0);
	
	/**
	 * Destructor.
	 */
	virtual inline ~FontBase() { };

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
	virtual const u16 getPixel(const u16 x, const u16 y) const = 0;
	
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
	 * Checks if the glyph map to see if the glyph representing the specified character
	 * contains any data.
	 * @param glyph The character to check.
	 * @return True if the glyph contains any pixels to be drawn.  False if the glyph is
	 * blank.
	 */
	inline const bool checkGlyphData(const char glyph) const { return (_glyphMap[glyph >> 3] & (1 << (glyph % 8))); };
	
	/**
	 * Sets the colour to use as the drawing colour.  If set, this overrides the colours
	 * present in a non-monochrome font.
	 * @param colour The new drawing colour.
	 */
	void setColour(const u16 colour);
	
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
	inline void clearColour() { _isMonochrome = false; };

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
	virtual u16 getStringWidth(char* text) = 0;

	/**
	 * Get the width of a string with a specified length in pixels when drawn with this font.
	 * Useful if you want to determine the length of a string without a terminator, or
	 * the length of a section of a string.
	 * @param text The string to check.
	 * @param length The length of the string in chars.
	 * @return The width of the string in pixels.
	 */
	virtual u16 getStringWidth(char* text, u16 length) = 0;

	/**
	 * Get the width of an individual character.
	 * @param letter The character to get the width of.
	 * @return The width of the character in pixels.
	 */
	virtual u16 getCharWidth(char letter) = 0;

	/**
	 * Gets the height of the font.
	 * @return The height of the font.
	 */
	inline const u8 getHeight() const { return _height; };

protected:

	/**
	 * Creates a map of which glyphs have data and which do not.
	 * @see createGlyphMap().
	 */
	void initGlyphMap();
	
	/**
	 * Populates the previously created glyph map with the data on which glyphs have data
	 * and which do not.  Does this by scanning through each pixel in each glyph and adding
	 * true to the map if data is found or false if no data is found.
	 * @see initGlyphMap().
	 */
	virtual void createGlyphMap() = 0;

	/**
	 * Scans the glyph bitmap at the specified co-ordinates to see if it contains data or
	 * not.  Must be overridden.
	 * @param x The x co-ordinate of the start of the glyph.
	 * @param y The y co-ordinate of the start of the glyph.
	 * @return True if the glyph bitmap contains data for this glyph.
	 */
	virtual const bool scanGlyph(const u16 x, const u16 y) const = 0;

protected:
	u8 _glyphMap[GLYPH_MAP_SIZE];

private:
	u8 _height;								/**< Height of the font */
	u16 _bitmapWidth;						/**< Width of the bitmap containing glyph data */
	u16 _bitmapHeight;						/**< Height of the bitmap containing glyph data */
	u16 _drawColour;						/**< Colour to draw the font with when rendering */
	bool _isMonochrome;						/**< True if the font is not multicolour */
	u16 _transparentColour;					/**< Background colour that should not be rendered */
};

#endif
