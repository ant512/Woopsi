#include "text.h"
#include "stringiterator.h"

using namespace WoopsiUI;

Text::Text(FontBase* font, const WoopsiString& text, u16 width) : WoopsiString(text) {
	_font = font;
	_width = width;
	_lineSpacing = 1;
	wrap();
}

FontBase* Text::getFont() const {
	return _font;
}

// Calculate the length of an individual line
const u8 Text::getLineLength(const s32 lineNumber) const {
	if (lineNumber < getLineCount() - 1) {
		return _linePositions[lineNumber + 1] - _linePositions[lineNumber];
	}
	
	return getLength() - _linePositions[lineNumber];
}

// Calculate the length of an individual line sans right-hand spaces
const s16 Text::getLineTrimmedLength(const s32 lineNumber) const {
	s16 length = getLineLength(lineNumber);
	
	// Loop through string until the end
	StringIterator* iterator = newStringIterator();
	
	// Get char at the end of the line
	if (iterator->moveTo(_linePositions[lineNumber] + length - 1)) {
		do{
			if (!_font->isCharBlank(iterator->getCodePoint())) break;
			length--;
		} while (iterator->moveToPrevious() && (length > 0));
		return length;
	}
	
	// May occur if data has been horribly corrupted somewhere
	return 0;
}

const s16 Text::getLinePixelLength(const s32 lineNumber) const {
	return _font->getStringWidth(*this, getLineStartIndex(lineNumber), getLineLength(lineNumber));
}

const s16 Text::getLineTrimmedPixelLength(const s32 lineNumber) const {
	return _font->getStringWidth(*this, getLineStartIndex(lineNumber), getLineTrimmedLength(lineNumber));
}

void Text::setText(const WoopsiString& text) {
	WoopsiString::setText(text);
	wrap();
}

void Text::setText(const char* text) {
	WoopsiString::setText(text);
	wrap();
}

void Text::setText(const u32 text) {
	WoopsiString::setText(text);
	wrap();
}

void Text::append(const WoopsiString& text) {
	WoopsiString::append(text);
	wrap(getLength() - 1);
}

void Text::insert(const WoopsiString& text, const u32 index) {
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
	u32 lineWidth;
	u32 breakIndex;
	bool endReached = false;
	
	if (_linePositions.size() == 0) charIndex = 0;
	
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
		while ((_linePositions.size() > 0) && (_linePositions.size() - 1 > (s32)lineIndex)) {
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
	
	// Loop through string until the end
	StringIterator* iterator = newStringIterator();
	
	while (!endReached) {
		breakIndex = 0;
		lineWidth = 0;
		
		if (iterator->moveTo(pos)) {
			
			// Search for line breaks and valid breakpoints until we exceed the width of the
			// text field or we run out of string to process
			while (lineWidth + _font->getCharWidth(iterator->getCodePoint()) <= _width) {
				lineWidth += _font->getCharWidth(iterator->getCodePoint());
				
				// Check for line return
				if (iterator->getCodePoint() == '\n') {
					
					// Remember this breakpoint
					breakIndex = iterator->getIndex();
					break;
				} else if ((iterator->getCodePoint() == ' ') ||
						   (iterator->getCodePoint() == ',') ||
						   (iterator->getCodePoint() == '.') ||
						   (iterator->getCodePoint() == '-') ||
						   (iterator->getCodePoint() == ':') ||
						   (iterator->getCodePoint() == ';') ||
						   (iterator->getCodePoint() == '?') ||
						   (iterator->getCodePoint() == '!') ||
						   (iterator->getCodePoint() == '+') ||
						   (iterator->getCodePoint() == '=') ||
						   (iterator->getCodePoint() == '/') ||
						   (iterator->getCodePoint() == '\0')) {
					
					// Remember the most recent breakpoint
					breakIndex = iterator->getIndex();
				}
				
				// Move to the next character
				if (!iterator->moveToNext()) {
					
					// No more text; abort loop
					endReached = true;
					break;
				}
			}
		} else {
			endReached = true;
		}
		
		if ((!endReached) && (iterator->getIndex() > pos)) {
			
			// Process any found data
			
			// If we didn't find a breakpoint split at the current position
			if (breakIndex == 0) breakIndex = iterator->getIndex() - 1;
			
			// Trim blank space from the start of the next line
			StringIterator* breakIterator = newStringIterator();
			
			if (breakIterator->moveTo(breakIndex + 1)) {
				while (breakIterator->getCodePoint() == ' ') {
					if (breakIterator->moveToNext()) {
						breakIndex++;
					} else {
						break;
					}
				}
			}
			
			delete breakIterator;
			
			// Add the start of the next line to the vector
			pos = breakIndex + 1;
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
		} else if (!endReached) {
			
			// Add a blank row if we're not at the end of the string
			pos++;
			_linePositions.push_back(pos);
		}
	}
	
	// Add marker indicating end of text
	// If we reached the end of the text, append the stopping point
	if (_linePositions[_linePositions.size() - 1] != getLength() + 1) {
		_linePositions.push_back(getLength());
	}
	
	delete iterator;
	
	// Calculate the total height of the text
	_textPixelHeight = getLineCount() * (_font->getHeight() + _lineSpacing);
	
	// Ensure height is always at least one row
	if (_textPixelHeight == 0) _textPixelHeight = _font->getHeight() + _lineSpacing;
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

const u32 Text::getLineContainingCharIndex(const u32 index) const {
	
	// Early exit if there is no existing line data
	if (_linePositions.size() == 0) return 0;
	
	// Early exit if the character is in the last row
	if (index >= _linePositions[_linePositions.size() - 2]) return _linePositions.size() - 2;
	
	// Binary search the line vector for the line containing the supplied index
	u32 bottom = 0;
	u32 top = _linePositions.size() - 1;
	u32 mid;
	
	while (bottom <= top) {
		
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
