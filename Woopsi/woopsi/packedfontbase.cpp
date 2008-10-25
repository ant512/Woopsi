#include "packedfontbase.h"

using namespace WoopsiUI;

/**
 * Get the width of an individual character.
 * @param letter The character to get the width of.
 * @return The width of the character in pixels.
 */
u16 PackedFontBase::getCharWidth(char letter) const
{
	if (_fontWidth) return _fontWidth;

	if (letter < _first || letter > _last) return _spWidth;
	return _glyphWidth[letter - _first] + 1;
}

/**
 * Decide whether a given character is blank or not
 */
const bool PackedFontBase::isCharBlank(const char letter) const
{
	if (letter >= _first && letter <= _last) return _glyphWidth[letter - _first] != 0;
	return false;
}

/**
 * Get the width of a string in pixels when drawn with this font.
 * @param text The string to check.
 * @return The width of the string in pixels.
 */
u16 PackedFontBase::getStringWidth(const char* text) const
{
	u16 total = 0;
	while (*text) {
		total += getCharWidth(*text++);
	}
	return total;
}

/**
 * Get the width of a string with a specified length in pixels when drawn with this font.
 * Useful if you want to determine the width of a string without a terminator, or
 * the width of a section of a string.
 * @param text The string to check.
 * @param length The length of the string in chars.
 * @return The width of the string in pixels.
 */
u16 PackedFontBase::getStringWidth(const char* text, u16 length) const
{
	if (_fontWidth) return _fontWidth * length;

	u16 total = 0;
	while (length-- > 0) {
		total += getCharWidth(*text++);
	}
	return total;
}

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
s16 PackedFontBase::drawChar(
	u16* bitmap,
	u16 bitmapWidth, u16 bitmapHeight,
	char letter,
	s16 x, s16 y,
	u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2)
{
	// if there is no glyphdata for this letter, just advance by an 'm'
	if (letter < _first || letter > _last) {
		return x + _spWidth;
	}

	// check what its pixel width is - zero means no such character so
	// fall back on the width of a space
	u16 pixelWidth = _glyphWidth[letter - _first];
	if (pixelWidth == 0) {
		return x + _spWidth;
	}

	// pass off to a subclass for rendering
	renderChar(
		&_glyphData[_glyphOffset[letter - _first]],
		pixelWidth,
		&bitmap[x + (y * bitmapWidth)],		// location for top-left pixel of glyph
		bitmapWidth,
		x, y,
		clipX1, clipY1, clipX2, clipY2);

	return x + getCharWidth(letter);
}
