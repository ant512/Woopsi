#include "packedfontbase.h"
#include "mutablebitmapbase.h"
#include "woopsistring.h"

using namespace WoopsiUI;

u16 PackedFontBase::getCharWidth(u32 letter) const
{
	if (_fontWidth) return _fontWidth;

	if (letter < _first || letter > _last) return _spWidth;
	return _glyphWidth[letter - _first] + 1;
}

const bool PackedFontBase::isCharBlank(const u32 letter) const
{
	if (letter >= _first && letter <= _last) return _glyphWidth[letter - _first] == 0;
	return false;
}

u16 PackedFontBase::getStringWidth(const WoopsiString& text) const
{
	if (_fontWidth) return _fontWidth * text.getLength();

	u16 total = 0;

	const char* currentChar = text.getCharArray();
	u8 bytes = 0;

	for (u32 i = 0; i < text.getLength(); ++i) {
		total += getCharWidth(text.getCodePoint(currentChar, &bytes));
		currentChar += bytes;
	}

	return total;
}

u16 PackedFontBase::getStringWidth(const WoopsiString& text, u32 startIndex, u32 length) const
{
	if (_fontWidth) return _fontWidth * length;

	u16 total = 0;

	const char* currentChar = text.getToken(startIndex);
	u8 bytes = 0;

	for (u32 i = startIndex; i < text.getLength(); ++i) {
		total += getCharWidth(text.getCodePoint(currentChar, &bytes));
		currentChar += bytes;
	}

	return total;
}

s16 PackedFontBase::drawChar(
	MutableBitmapBase* bitmap,
	u32 letter,
	s16 x, s16 y,
	u16 clipX1, u16 clipY1, u16 clipX2, u16 clipY2)
{
	// if there is no glyphdata for this letter, just advance by a space
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
