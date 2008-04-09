#include "text.h"
#include "fixedwidthfontbase.h"

// TODO: Allow line spacing to be set in constructor only, or make const
// TODO: Check for other characters to split on
// TODO: Finish text wrapping by pixel widths rather than string lengths

Text::Text(FontBase* font, char* text, u16 width) {
	_font = font;

	// Recalculate width to match font
	_width = width;
	_lineSpacing = 1;
	
	calculateLineHeight();
	calculateMaxLineLength();
	
	setText(text);
}

FontBase* Text::getFont() {
	return _font;
}

// Calculates the pixel height of a text row
void Text::calculateLineHeight() {
	_lineHeight = _font->getHeight() + _lineSpacing;
}

// Calculates the max chars per text row
void Text::calculateMaxLineLength() {

	// TODO: Make this work with proportional fonts
	_maxLineLength = (_width / ((FixedWidthFontBase*)_font)->getWidth()) - 1;
}

// Calculates the total height in pixels of the text
void Text::calculateTextPixelHeight() {
	_textPixelHeight = _totalLines * _lineHeight;
}

// Calculate the length of an individual line
u8 Text::getLineLength(s32 lineNumber) {
	if (lineNumber < _totalLines - 1) {
		return _linePositions[lineNumber + 1] - _linePositions[lineNumber];
	}

	return strlen(_text) - _linePositions[lineNumber]; 
}

// Calculate the length of an individual line sans right-hand spaces
u8 Text::getLineTrimmedLength(s32 lineNumber) {
	s16 length = getLineLength(lineNumber);

	// Strip any trailing spaces, etc
	while ((length > 0) && (!_font->checkGlyphData(_text[_linePositions[lineNumber] + length - 1]))) {
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

// Returns a pointer to the start of a line of text
char* Text::getLinePointer(s32 lineNumber) {
	return _text + _linePositions[lineNumber];
}

// Set the text
void Text::setText(char* text) {
	_text = text;
	
	wrap();
}

// Set the line spacing
void Text::setLineSpacing(u8 lineSpacing) {
	_lineSpacing = lineSpacing;
	
	calculateLineHeight();
}

void Text::setWidth(u16 width) {
	_width = width;

	calculateMaxLineLength();

	wrap();
}

// Wrap text to fit onto DS screens
void Text::wrap() {

	u32 pos = 0;
	u32 remainingSize = strlen(_text);
	u32 totalSize = remainingSize;

	// Wipe the width variable
	_textPixelWidth = 0;

	// Empty existing line positions
	_linePositions.clear();

	// Push first line start into vector
	_linePositions.push_back(pos);
	
	// Pre-declare values for loop
	u32 currentPos = pos;
	u32 lastBreakPos = 0;
	u8 lineLength = 0;
	u16 linePixelWidth = 0;
	bool gotBreak = false;

	// Keep looping until we run out of lines
	while (remainingSize > 0) {

		currentPos = pos;
		lastBreakPos = 0;
		lineLength = 0;
		linePixelWidth = 0;
		gotBreak = false;

		// Search for a break point (from left to right)
		while ((linePixelWidth < _width) && (remainingSize > 0) && (currentPos < totalSize)) {
			lineLength++;
			linePixelWidth += _font->getCharWidth(_text[currentPos]);
		
			if (_text[currentPos] == '\n') {
				// Got a return

				// Recalculate positions
				if (remainingSize > lineLength) {
					remainingSize -= lineLength;
				} else {
					remainingSize = 0;
				}

				// Advance to next character and remember we got a break
				pos = currentPos + 1;
				gotBreak = true;
				
				// Push next line start into vector
				_linePositions.push_back(pos);
				
				break;
				
			} else if ((_text[currentPos] == ' ') ||
					   (_text[currentPos] == ',') ||
					   (_text[currentPos] == '.') ||
					   (_text[currentPos] == '-') ||
					   (_text[currentPos] == '_')) {
			
				// Remember the most recent breakpoint
				lastBreakPos = currentPos;
			}

			// Move forward to next character
			currentPos++;
		}

		// Did we hit a line return?
		if (!gotBreak) {
		
			// Did we find a breaking character?
			if (lastBreakPos > 0) {
				
				// Got a breaking character
				if (remainingSize > (lastBreakPos - pos)) {
					remainingSize -= (lastBreakPos - pos);
				} else {
					remainingSize = 0;
				}

				pos = lastBreakPos + 1;
					
				// Push next line start into vector if this line contains data
				if (lineLength > 0) {
					_linePositions.push_back(pos);
				}
			} else {
		
				// No break found
				
				if (remainingSize > _maxLineLength) {
					// Add a whole row
					remainingSize -= _maxLineLength;
					pos += _maxLineLength;
				} else {
					// Add remaining text
					pos += remainingSize;
					remainingSize = 0;
				}
			
				// Push next line start into vector if this line contains data
				if (lineLength > 0) {
					_linePositions.push_back(pos);
				}
			}
		}

		// Is this the longest line?
		if (linePixelWidth > _textPixelWidth) _textPixelWidth = linePixelWidth;
	}

	// Add final string
	if (remainingSize > 0) {
		_linePositions.push_back(pos + remainingSize);

		// Check for longest line
		if (_textPixelWidth < remainingSize) {
			_textPixelWidth = remainingSize;
		}
	}
	
	// Precalculate some values

	// Total lines is 1 less than the size of the position array because the first element
	// is the start of the first line
	_totalLines = _linePositions.size() - 1;

	calculateTextPixelHeight();
}

void Text::setFont(FontBase* font) {
	_font = font;

	calculateLineHeight();
	calculateMaxLineLength();
	
	wrap();
}
