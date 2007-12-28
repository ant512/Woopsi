#include "textviewer.h"

// TODO: Fix clipping when scrolling
// TODO: Allow line spacing to be set in constructor only, or make const
// TODO: Check for other characters to split on
// TODO: Switch to GraphicsPort

TextViewer::TextViewer(s16 x, s16 y, u16 width, u16 height, s32 textY, char* text, FontBase* font) : Gadget(x, y, width, height, 0, font) {
	_textY = textY;

	// Recalculate width to match font - divide to get integer value
	_innerWidth = ((width / font->getWidth()) * font->getWidth()) - 2;
	_innerHeight = height - 2;

	_outline = OUTLINE_IN;

	_text = new Text(font, text, _innerWidth);
}

// Destructor
TextViewer::~TextViewer() {
	delete _text;
}

Text* TextViewer::getText() {
	return _text;
}

void TextViewer::draw(Rect clipRect) {
	clear(clipRect);

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);
	
	delete port;

	// TODO: Implement a tidier version of the screen drawing routine for
	// clipped rects
	drawScreen(clipRect);
}

void TextViewer::draw() {
	Gadget::draw();
}

bool TextViewer::resize(u16 width, u16 height) {

	if ((_width != width) || (_height != height)) {
		erase();
		_width = width;
		_height = height;

		// Recalculate width to match font
		_innerWidth = width - 2;
		_innerHeight = height - 2;

		_text->setWidth(_innerWidth);

		draw();

		raiseResizeEvent(width, height);

		return true;
	}

	return false;
}

// Increase/decrease the Y position of the text
void TextViewer::increaseTextY(s32 incY) {

	// Only scroll if necessary
	if (_text->getPixelHeight() > _innerHeight) {

		// Ensure that we don't scroll too far down
		if (_textY + incY > 0) {
			if (_textY < 0) {
				incY = -_textY;
			} else {
				incY = 0;
			}
		}

		// Ensure that we don't scroll too far up
		if (_textY + incY < -_text->getPixelHeight() + _innerHeight + (_text->getLineHeight() * 2)) {
			//incY = -_text->getPixelHeight() - _textY + _innerHeight + (_text->getLineHeight() * 2);
		}

		// Increase Y position
		_textY += incY;

		// Handle scrolling
		if (incY < 0) {
			// Scroll upwards
			scrollUp(incY);
		} else if (incY > 0) {
			// Scroll downwards
			scrollDown(incY);
		}
	}
}

// Set the Y position of the text
void TextViewer::setTextY(s32 textY) {
	_textY = textY;
}

// Set the text
void TextViewer::setText(char* text) {
	_text->setText(text);

	raiseValueChangeEvent();
}

// Return the current Y position of the text
u32 TextViewer::getTextY() {
	return _textY;
}

// Draw the lines scrolling into the bottom of the gadget
void TextViewer::drawLastLines(s32 incY) {

	_firstLineIndex = (0 - _textY) / _text->getLineHeight();
	_lastLineIndex = (((0 - _textY) + _innerHeight) / _text->getLineHeight() + 1);

	// Ensure first line not less than 0
	if (_firstLineIndex < 0) {
		_firstLineIndex = 0;
	}

	// Calculate the number of lines changed
	u8 linesChanged = ((0 - incY) / _text->getLineHeight()) + 2;

	// Clear the bottom lines
	GraphicsPort* port = newGraphicsPort();
	port->drawFilledRect(0, _innerHeight + incY, _innerWidth, -incY, _backColour);
	delete port;

	// Ensure last line does not exceed total lines
	if (_lastLineIndex > 0) {

		// Precalculate values
		s16 outputX = getX() + 1;
		u16 line0 = _textY + getY() + 1 + ((_lastLineIndex - 1) * _text->getLineHeight());
		u16 linei = line0;
		u8 clipX1 = getX() + 1;
		u8 clipY1 = getY() + 1;
		u8 clipX2 = getX() + _innerWidth;
		u8 clipY2 = getY() + _innerHeight;

		// Output the new lines
		for (u8 i = 0; i < linesChanged; i++) {
			if ((_lastLineIndex - i > 0) && (_lastLineIndex - i < _text->getLineCount())) {
			
				// Calculate string properties
				u32 lineNumber = _lastLineIndex - 1 - i;

				// Draw the string
				TextWriter::drawString(_text->getFont(), _text->getLinePointer(lineNumber), _text->getLineLength(lineNumber), outputX, linei, clipX1, clipY1, clipX2, clipY2);
			}

			linei -= _text->getLineHeight();
		}
	}
}

// Draw the new lines that have scrolled into the top of the gadget
void TextViewer::drawFirstLines(s32 incY) {

	_firstLineIndex = (0 - _textY) / _text->getLineHeight();
	_lastLineIndex = (((0 - _textY) + _innerHeight) / _text->getLineHeight() + 1);

	// Calculate the number of lines changed
	u8 linesChanged = (incY / _text->getLineHeight()) + 2;

	// Clear the top lines
	GraphicsPort* port = newGraphicsPort();
	port->drawFilledRect(0, 0, _innerWidth, incY, _backColour);
	delete port;

	// Ensure first line not less than 0
	if (_firstLineIndex < _text->getLineCount() - 1) {

		// Precalculate values
		s16 outputX = getX() + 1;
		u16 line0 = _textY + getY() + 1 + (_firstLineIndex * _text->getLineHeight());
		u16 linei = line0;
		u8 clipX1 = getX() + 1;
		u8 clipY1 = getY() + 1;
		u8 clipX2 = getX() + _innerWidth;
		u8 clipY2 = getY() + _innerHeight;

		// Output the new lines
		for (u8 i = 0; i < linesChanged; i++) {
			if ((_firstLineIndex + i >= 0) && (_firstLineIndex + i < _text->getLineCount() - 1)) {
			
				// Calculate string properties
				u32 lineNumber = _firstLineIndex + i;

				// Draw the string
				TextWriter::drawString(_text->getFont(), _text->getLinePointer(lineNumber), _text->getLineLength(lineNumber), outputX, linei, clipX1, clipY1, clipX2, clipY2);

				//textWriter->drawString(0, text, strlen(text), outputX, textY + getY() + 1 + ((firstLine + i) * lineHeight), _getX() + 1, _getY() + 1, _getX() + _width - 1, _getY() + _height - 1);
			}

			linei += _text->getLineHeight();
		}
	}
}

// Draw the screen in its entirety
void TextViewer::drawScreen() {

	_firstLineIndex = (0 - _textY) / _text->getLineHeight();
	_lastLineIndex = (((0 - _textY) + _innerHeight) / _text->getLineHeight());

	// Ensure first line not less than 0
	if (_firstLineIndex < 0) {
		_firstLineIndex = 0;
	}

	// Ensure last line does not exceed total lines
	if (_lastLineIndex > _text->getLineCount()) {
		_lastLineIndex = _text->getLineCount();
	}

	// Precalculate values
	s16 outputX = getX() + 1;
	u16 line0 = _textY + getY() + 1 + (_firstLineIndex * _text->getLineHeight());
	u16 linei = line0;
	u8 clipX1 = getX() + 1;
	u8 clipY1 = getY() + 1;
	u8 clipX2 = getX() + _innerWidth - 1;
	u8 clipY2 = getY() + _innerHeight - 1;

	for (s32 i = _firstLineIndex; i < _lastLineIndex; i++) {
		
		// Calculate string properties
		u32 lineNumber = i;

		// Draw the string
		TextWriter::drawString(_text->getFont(), _text->getLinePointer(lineNumber), _text->getLineLength(lineNumber), outputX, linei, clipX1, clipY1, clipX2, clipY2);

		linei += _text->getLineHeight();
	}
}

// Draw clipped version of the screen
void TextViewer::drawScreen(Rect clipRect) {

	_firstLineIndex = (0 - _textY) / _text->getLineHeight();
	_lastLineIndex = (((0 - _textY) + _innerHeight) / _text->getLineHeight());

	// Ensure first line not less than 0
	if (_firstLineIndex < 0) {
		_firstLineIndex = 0;
	}

	// Ensure last line does not exceed total lines
	if (_lastLineIndex > _text->getLineCount()) {
		_lastLineIndex = _text->getLineCount();
	}

	// Precalculate values
	s16 outputX = getX() + 1;
	u16 line0 = _textY + getY() + 1 + (_firstLineIndex * _text->getLineHeight());
	u16 linei = line0;

	s16 clipX1 = clipRect.x > getX() + 1 ? clipRect.x : getX() + 1;
	s16 clipY1 = clipRect.y > getY() + 1 ? clipRect.y : getY() + 1;
	s16 clipX2 = clipRect.width + clipRect.x - 1 < getX() + _innerWidth - 1? clipRect.width + clipRect.x - 1 : getX() + _innerWidth - 1;
	s16 clipY2 = clipRect.height + clipRect.y - 1 < getY() + _innerHeight + 1 ? clipRect.height + clipRect.y - 1 : getY() + _innerHeight + 1;

	for (s32 i = _firstLineIndex; i < _lastLineIndex; i++) {
	
		// Calculate string properties
		u32 lineNumber = i;

		// Draw the string
		TextWriter::drawString(_text->getFont(), _text->getLinePointer(lineNumber), _text->getLineLength(lineNumber), outputX, linei, clipX1, clipY1, clipX2, clipY2);

		linei += _text->getLineHeight();
	}
}

// Use DMA_Copy to scroll all rows upwards
void TextViewer::scrollUp(s32 incY) {

	// Precalculate values
	u16 maxLine = _innerHeight;
	u16* srcLinei = DrawBg[0] + getX() + 1 + ((-incY + getY() + 1) << 8);

	u16* destLinei = DrawBg[0] + getX() + 1 + ((getY() + 1) << 8);
	u16 lineInc = 1 << 8;

	// Copy all lines
	for (s16 i = 0 - incY; i < maxLine; i++) {
		// Perform copy
		DMA_Copy(srcLinei, destLinei, _innerWidth, DMA_16NOW);

		// Move to next set of lines
		srcLinei += lineInc;
		destLinei += lineInc;
	}

	// Draw new rows at bottom of bottom screen
	drawLastLines(incY);
}

// Use DMA_Copy to scroll all rows downwards
void TextViewer::scrollDown(s32 incY) {

	// Precalculate values
	u16* srcLinei = DrawBg[0] + getX() + 1 + (((_innerHeight - incY) + getY()) << 8);

	u16* destLinei = DrawBg[0] + getX() + 1 + (((_innerHeight - incY) + incY + getY()) << 8);
	u16 lineInc = 1 << 8;

	for (s16 i = _innerHeight - incY; i > -1; i--) {
		DMA_Copy(srcLinei, destLinei, _innerWidth, DMA_16NOW);

		// Move to next set of lines
		srcLinei -= lineInc;
		destLinei -= lineInc;
	}

	// Draw new rows at top of top screen
	drawFirstLines(incY);
}

bool TextViewer::drag(s16 x, s16 y, s16 vX, s16 vY) {
	if (_flags.enabled) {
		if (_flags.clicked) {
			increaseTextY(vY);

			raiseDragEvent(x, y, vX, vY);

			return true;
		}
	}

	return false;
}

void TextViewer::setFont(FontBase* font) {
	_font = font;
	_text->setFont(font);
}
