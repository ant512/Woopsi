#ifndef _FONT_BASE_H_
#define _FONT_BASE_H_

#include <nds.h>
#include <string.h>

#define GLYPH_MAP_SIZE 32

using namespace std;

/**
 * Abstract class defining the basic properties of a font and providing some of the
 * essential functionality.  Should be used as a base class for all fonts.
 * This class does not currently allow for proportional fonts or non-bitmap fonts.  All
 * fonts must be fixed-width.
 */
class FontBase {

public:

	/**
	 * Constructor.
	 * @param bitmapWidth The width of the bitmap containing the font's glyph data.
	 * @param bitmapHeight The height of the bitmap containing the font's glyph data.
	 * @param width The width of an individual glyph.
	 * @param height The height of an individual glyph.
	 * @param transparentColour The colour in the font bitmap used as the background
	 * colour.
	 */
	FontBase(const u16 bitmapWidth, const u16 bitmapHeight, const u8 width, const u8 height, const u16 transparentColour = 0);
	
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
	const u16 getPixel(const u16 x, const u16 y) const;
	
	/**
	 * Gets the width of the font's glyph bitmap.
	 * @return The width of the font's glyph bitmap.
	 */
	const u16 getBitmapWidth() const;
	
	/**
	 * Gets the height of the font's glyph bitmap.
	 * @return The height of the font's glyph bitmap.
	 */
	const u16 getBitmapHeight() const;
	
	/**
	 * Gets the width of an individual glyph.
	 * @return The width of an individual glyph.
	 */
	const u8 getWidth() const;
	
	/**
	 * Gets the height of an individual glyph.
	 * @return The height of an individual glyph.
	 */
	const u8 getHeight() const;
	
	/**
	 * Checks if the glyph map to see if the glyph representing the specified character
	 * contains any data.
	 * @param glyph The character to check.
	 * @return True if the glyph contains any pixels to be drawn.  False if the glyph is
	 * blank.
	 */
	const bool checkGlyphData(const char glyph) const;
	
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
	const u16 getColour() const;
	
	/**
	 * Returns true if the current font is being drawn using a single colour.
	 * @return True if the current font is monochrome.
	 */
	const bool isMonochrome() const;
	
	/**
	 * Get the width in pixels of the supplied string if printed using this font.
	 * @return The pixel width of the string.
	 */
	const u8 getTextPixelWidth(const char* text) const;
	
	/**
	 * Get the colour currently being used as the transparent background colour.
	 * @return The transparent background colour.
	 */
	const u16 getTransparentColour() const;
	
	/**
	 * Sets the transparent background colour to a new value.
	 * @param colour The new background colour.
	 */
	void setTransparentColour(const u16 colour);

	/**
	 * Resets back to mulitcolour mode if the font supports it and is currently set to
	 * monochrome mode.
	 */
	void clearColour();

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
	void createGlyphMap();

	/**
	 * Scans the glyph bitmap at the specified co-ordinates to see if it contains data or
	 * not.  Must be overridden.
	 * @param x The x co-ordinate of the start of the glyph.
	 * @param y The y co-ordinate of the start of the glyph.
	 * @return True if the glyph bitmap contains data for this glyph.
	 */
	virtual const bool scanGlyph(const u16 x, const u16 y) const = 0;

private:
	u8 _width;
	u8 _height;
	u16 _bitmapWidth;
	u16 _bitmapHeight;
	u8 _glyphMap[GLYPH_MAP_SIZE];
	u16 _drawColour;
	bool _isMonochrome;
	u16 _transparentColour;
};

#endif
