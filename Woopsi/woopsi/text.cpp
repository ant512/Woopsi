#include "text.h"

using namespace WoopsiUI;

Text::Text(FontBase* font, const char* text, u16 width) : WoopsiString(text) {
	_font = font;
	_width = width;
	_lineSpacing = 1;
}

FontBase* Text::getFont() {
	return _font;
}

// Calculate the length of an individual line
u8 Text::getLineLength(s32 lineNumber) {
	if (lineNumber < getLineCount() - 1) {
		return _linePositions[lineNumber + 1] - _linePositions[lineNumber];
	}

	return getLength() - _linePositions[lineNumber];
}

// Calculate the length of an individual line sans right-hand spaces
u8 Text::getLineTrimmedLength(s32 lineNumber) {
	s16 length = getLineLength(lineNumber);

	// Strip any trailing spaces, etc
	while ((length > 0) && (_font->isCharBlank(_text[_linePositions[lineNumber] + length - 1]))) {
		length--;
	}

	return length;
}

u8 Text::getLinePixelLength(s32 lineNumber) {
	return _font->getStringWidth(getLinePointer(lineNumber), getLineLength(lineNumber));
}

u8 Text::getLineTrimmedPixelLength(s32 lineNumber) {
	return _font->getStringWidth(getLinePointer(lineNumber), getLineTrimmedLength(lineNumber));
}

void Text::setText(const char* text) {
	WoopsiString::setText(text);
	wrap();
}

void Text::setText(const char text) {
	WoopsiString::setText(text);
	wrap();
}

void Text::append(const char* text) {
	WoopsiString::append(text);
	wrap();
}

void Text::append(const char text) {
	WoopsiString::append(text);
	wrap();
}

void Text::insert(const char* text, const u32 index) {
	WoopsiString::insert(text, index);
	wrap();
}

void Text::insert(const char text, const u32 index) {
	WoopsiString::insert(text, index);
	wrap();
}

void Text::remove(const u32 startIndex) {
	WoopsiString::remove(startIndex);
	wrap();
}

void Text::remove(const u32 startIndex, const u32 count) {
	WoopsiString::remove(startIndex, count);
	wrap();
}

void Text::setLineSpacing(u8 lineSpacing) {
	_lineSpacing = lineSpacing;
	wrap();
}

void Text::setWidth(u16 width) {
	_width = width;
	wrap();
}

void Text::wrap() {

	// Wipe the width variable
	_textPixelWidth = 0;

	// Empty existing line positions
	_linePositions.clear();

	// Push first line start into vector
	_linePositions.push_back(0);

	// Declare vars in advance of loop
	u32 pos = 0;
	u32 scanPos;
	u32 lineWidth;
	u32 breakPos;
	bool endReached = false;

	// Keep looping through text until we hit the terminator
	while (!endReached) {
		scanPos = pos;
		breakPos = 0;
		lineWidth = 0;

		// Search for line breaks and valid breakpoints until we exceed the width of the
		// text field or we run out of string to process
		while (lineWidth + _font->getCharWidth(_text[scanPos]) < _width) {
			lineWidth += _font->getCharWidth(_text[scanPos]);

			// Check for line return
			if (_text[scanPos] == '\n') {

				// Remember this breakpoint
				breakPos = scanPos;
				break;
			} else if ((_text[scanPos] == ' ') ||
					   (_text[scanPos] == ',') ||
					   (_text[scanPos] == '.') ||
					   (_text[scanPos] == '-') ||
					   (_text[scanPos] == ':') ||
					   (_text[scanPos] == ';') ||
					   (_text[scanPos] == '?') ||
					   (_text[scanPos] == '!') ||
					   (_text[scanPos] == '+') ||
					   (_text[scanPos] == '=') ||
					   (_text[scanPos] == '/') ||
					   (_text[scanPos] == '\0')) {

				// Remember the most recent breakpoint
				breakPos = scanPos;

				// Terminate?
				if (_text[scanPos] == '\0') {
				    endReached = true;
                    break;
				}
			}

			// Move to the next character
			scanPos++;
		}

		// Process any found data
		if (scanPos > pos) {

			// If we didn't find a breakpoint split at the current position
			if (breakPos == 0) breakPos = scanPos - 1;

			// Trim preceeding spaces from the start of the next line
			while (_text[breakPos + 1] == ' ') {
				breakPos++;
			}

			// Add the start of the next line to the vector
			pos = breakPos + 1;
			_linePositions.push_back(pos);

			// Is this the longest line?
			if (lineWidth > _textPixelWidth) _textPixelWidth = lineWidth;
		} else {

			// Add a blank row
			pos++;
			_linePositions.push_back(pos);
		}
	}

	// Calculate the total height of the text
	_textPixelHeight = getLineCount() * (_font->getHeight() + _lineSpacing);
}

void Text::setFont(FontBase* font) {
	_font = font;
	wrap();
}

void Text::stripTopLines(const s32 lines) {
	// Get the start point of the text we want to keep
	u16 textStart = 0;

	for (s32 i = 0; i < lines; i++) {
		textStart += getLineLength(i);
	}

	// Remove the characters from the start of the string to the found location
	remove(0, textStart);

	// Rewrap the text
	wrap();
}
