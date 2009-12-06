#include "packedfontbase.h"
#include "mutablebitmapbase.h"

using namespace WoopsiUI;

u16 PackedFontBase::getCharWidth(char letter) const
{
	if (_fontWidth) return _fontWidth;

	if (letter < _first || letter > _last) return _spWidth;
	return _glyphWidth[letter - _first] + 1;
}

const bool PackedFontBase::isCharBlank(const char letter) const
{
	if (letter >= _first && letter <= _last) return _glyphWidth[letter - _first] != 0;
	return false;
}

u16 PackedFontBase::getStringWidth(const char* text) const
{
	u16 total = 0;
	while (*text) {
		total += getCharWidth(*text++);
	}
	return total;
}

u16 PackedFontBase::getStringWidth(const char* text, u16 length) const
{
	if (_fontWidth) return _fontWidth * length;

	u16 total = 0;
	while (length-- > 0) {
		total += getCharWidth(*text++);
	}
	return total;
}

s16 PackedFontBase::drawChar(
	MutableBitmapBase* bitmap,
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
		bitmap,
		x, y,
		clipX1, clipY1, clipX2, clipY2);

	return x + getCharWidth(letter);
}
