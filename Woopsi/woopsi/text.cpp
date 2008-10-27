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
	wrap(getLength() - 1);
}

void Text::append(const char text) {
	WoopsiString::append(text);
	wrap(getLength() - 1);
}

void Text::insert(const char* text, const u32 index) {
	WoopsiString::insert(text, index);
	wrap(index);
}

void Text::insert(const char text, const u32 index) {
	WoopsiString::insert(text, index);
	wrap(index);
}

void Text::remove(const u32 startIndex) {
	WoopsiString::remove(startIndex);
	wrap(startIndex);
}

void Text::remove(const u32 startIndex, const u32 count) {
	WoopsiString::remove(startIndex, count);
	wrap(startIndex);
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
	wrap(0);
}

void Text::wrap(u32 charIndex) {

	// Declare vars in advance of loop
	u32 pos = 0;
	u32 scanPos;
	u32 lineWidth;
	u32 breakPos;
	bool endReached = false;

	// If we're wrapping from an offset in the text, ensure that any existing data
	// after the offset gets removed
	if (charIndex > 0) {

		// Remove wrapping data past this point

		// Get the index of the line in which the char index appears
		u32 lineIndex = getLineContainingCharIndex(charIndex);

		// Remove any longest line records that occur from the line index onwards
		while ((_longestLines.size() > 0) && (_longestLines[_longestLines.size() - 1].index >= lineIndex)) {
			_longestLines.pop_back();
		}

		// If there are any longest line records remaining, update the text pixel width
		// The last longest line record will always be the last valid longest line as
		// the vector is sorted by length
		if (_longestLines.size() > 0) {
			_textPixelWidth = _longestLines[_longestLines.size() - 1].width;
		} else {
			_textPixelWidth = 0;
		}

		// Remove any wrapping data from after this line index onwards
		while ((_linePositions.size() > 0) && (_linePositions[_linePositions.size() - 1] > lineIndex)) {
			_linePositions.pop_back();
		}

		// Adjust start position of wrapping loop so that it starts with the current line index
		if (_linePositions.size() > 0) {
			pos = _linePositions[_linePositions.size() - 1];
		}
	} else {

		// Remove all wrapping data

		// Wipe the width variable
		_textPixelWidth = 0;

		// Empty existing longest lines
		_longestLines.clear();

		// Empty existing line positions
		_linePositions.clear();

		// Push first line start into vector
		_linePositions.push_back(0);
	}

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

			// Is this the longest line observed so far?
			if (lineWidth > _textPixelWidth) {
				_textPixelWidth = lineWidth;

				// Push the description of the line into the longest lines
				// vector (note that we store the index in _linePositions that
				// refers to the start of the line, *not* the position of the
				// line in the char array)
				LongestLine line;
				line.index = _linePositions.size() - 2;
				line.width = lineWidth;
				_longestLines.push_back(line);
			}
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

u32 Text::getLineContainingCharIndex(u32 index) {

	// Binary search the line vector for the line containing the supplied index
	u32 bottom = 0;
	u32 top = _linePositions.size() - 1;
	u32 mid;

	while (bottom < top) {

		// Standard binary search
		mid = (bottom + top) >> 1;

		if (index < _linePositions[mid]) {

			// Index is somewhere in the lower search space
			top = mid - 1;
		} else if (index > _linePositions[mid]) {

			// Index is somewhere in the upper search space
			bottom = mid + 1;
		} else if (index == _linePositions[mid]) {

			// Located the index
			return mid;
		}


		// Check to see if we've moved past the line that contains the index
		// We have to do this because the index we're looking for can be within
		// a line; it isn't necessarily the start of a line (which is what is
		// stored in the _linePositions vector)
		if (index > _linePositions[top]) {
			
			// Search index falls within the line represented by the top position
			return top;
		} else if (index < _linePositions[bottom]) {
	
			// Search index falls within the line represented by the bottom position
			return bottom - 1;
		}

	}

	// Line cannot be found
	return 0;
}
