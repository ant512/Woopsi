#include "multilinetextbox.h"
#include "woopsi.h"

// TODO: Make addText() routine strip lines of text as new lines of text
// are added, *if* the number of lines of text exceeds the maximum value

// TODO: Switch to GraphicsPort

MultiLineTextBox::MultiLineTextBox(s16 x, s16 y, u16 width, u16 height, char* text, s16 maxRows, FontBase* font) : Gadget(x, y, width, height, 0, font) {
	
	_rawText = NULL;
	_text = new Text(_font, "", width);

	_outline = OUTLINE_IN;

	_hPos = TEXT_POSITION_HORIZ_CENTRE;
	_vPos = TEXT_POSITION_VERT_CENTRE;
	_padding = 3;
	_topRow = 0;
	_autoDrawing = true;

	_maxRows = maxRows;

	calculateTotalVisibleRows();

	// Set maximum rows if value not set
	if (_maxRows == 0) {
		_maxRows = _visibleRows + 1;
	}

	setText(text);
}

MultiLineTextBox::~MultiLineTextBox() {
	delete _text;
	_text = NULL;

	delete [] _rawText;
	_rawText = NULL;
}

void MultiLineTextBox::setAutomaticDrawing(bool autoDrawing) {
	_autoDrawing = autoDrawing;
}

void MultiLineTextBox::draw(Rect clipRect) {
	clear(clipRect);

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	// Draw lines of text
	u8 textX;
	u8 textY;

	u8 rowCount = _visibleRows > _text->getLineCount() ? _text->getLineCount() : _visibleRows;
	u32 currentTextRow = 0;
	u8 rowPixelWidth = 0;
	u8 rowLength = 0;

	for (u8 i = 0; i < rowCount; i += 1) {

		currentTextRow = i + _topRow;

		// Precalculate lengths in characters and pixels
		rowLength = _text->getLineTrimmedLength(currentTextRow);
		rowPixelWidth = _text->getFont()->getWidth() * rowLength;

		textX = getRowX(rowPixelWidth);
		textY = getRowY(i, rowCount);
		
		port->drawText(textX, textY, _text->getFont(), rowLength, _text->getLinePointer(currentTextRow));
	}

	delete port;
}

void MultiLineTextBox::draw() {
	Gadget::draw();
}

// Calculate values for centralised text
u8 MultiLineTextBox::getRowX(u8 rowPixelWidth) {

	// Calculate horizontal position
	switch (_hPos) {
		case TEXT_POSITION_HORIZ_CENTRE:
			return ((_width - (_padding << 1)) >> 1) - (rowPixelWidth >> 1);
		case TEXT_POSITION_HORIZ_LEFT:
			return _padding;
		case TEXT_POSITION_HORIZ_RIGHT:
			return _width - rowPixelWidth - _padding;
	}

	// Will never be reached
	return 0;
}

u8 MultiLineTextBox::getRowY(u8 screenRow, u8 screenRows) {

	u8 textY = 0;
	u8 startPos = 0;

	// Calculate vertical position
	switch (_vPos) {
		case TEXT_POSITION_VERT_CENTRE:
			startPos = ((_height - (_padding << 1)) >> 1) - ((_text->getLineHeight() * screenRows) >> 1);
			textY = startPos + (screenRow * _text->getLineHeight());
			break;
		case TEXT_POSITION_VERT_TOP:
			textY = _padding + (screenRow * _text->getLineHeight());
			break;
		case TEXT_POSITION_VERT_BOTTOM:
			textY = (_height - (_text->getLineHeight() * (screenRows - screenRow))) - _padding;
			break;
	}

	return textY;
}

void MultiLineTextBox::calculateTotalVisibleRows() {
	_visibleRows = (_height - (_padding << 1)) / _text->getLineHeight();
}

void MultiLineTextBox::setTextPositionHoriz(TextPositionHoriz position) {
	_hPos = position;

	if (_autoDrawing) {
		draw();
	}
}

void MultiLineTextBox::setTextPositionVert(TextPositionVert position) {
	_vPos = position;

	if (_autoDrawing) {
		draw();
	}
}

char* MultiLineTextBox::getText() {
	return _rawText;
}

void MultiLineTextBox::setText(char* text) {
	
	// Have we already created a block of memory that we need to free?
	if (_rawText != NULL) {
		// Free the memory
		delete [] _rawText;
	}

	// Create new memory for string
	_rawText = new char[strlen(text) + 1];

	// Copy text
	strcpy(_rawText, text);

	_text->setText(_rawText);

	// Ensure that we have the correct number of rows
	if (_text->getLineCount() > _maxRows) {
		stripTopLines(_text->getLineCount() - _maxRows);
	}

	if (_autoDrawing) {
		draw();
	}

	raiseValueChangeEvent();
}

void MultiLineTextBox::addText(char* text) {

	if (_rawText != NULL) {
		// Reserve memory for concatenated string
		char* newText = new char[strlen(_rawText) + strlen(text) + 1];

		// Copy old text into new text
		strcpy(newText, _rawText);

		// Concatenate strings
		strcat(newText, text);

		// Free existing memory
		delete [] _rawText;

		// Update pointer
		_rawText = newText;

		// Ensure that we have the correct number of rows
		if (_text->getLineCount() > _maxRows) {
			stripTopLines(_text->getLineCount() - _maxRows);
		} else {
			_text->setText(_rawText);
		}

		if (_autoDrawing) {
			draw();
		}

		raiseValueChangeEvent();
	} else {
		// No text, so set it for the first time
		setText(text);

		// Ensure that we have the correct number of rows
		if (_text->getLineCount() > _maxRows) {
			stripTopLines(_text->getLineCount() - _maxRows);
		}

		if (_autoDrawing) {
			draw();
		}

		raiseValueChangeEvent();
	}
}

bool MultiLineTextBox::click(s16 x, s16 y) {

	if (_flags.enabled) {
		if (checkCollision(x, y)) {
			Gadget::click(x, y);

			return true;
		}
	}

	return false;
}

bool MultiLineTextBox::release(s16 x, s16 y) {

	if (_flags.clicked) {
		Gadget::release(x, y);

		return true;
	}

	return false;
}

void MultiLineTextBox::setFont(FontBase* font) {
	_font = font;
	_text->setFont(font);
}

void MultiLineTextBox::stripTopLines(const u32 lines) {

	// Get the start point of the text we want to keep
	u16 textStart = 0;
	
	for (u32 i = 0; i < lines; i++) {
		textStart += _text->getLineLength(i);
	}

	// Reserve memory for shortened string
	u16 newLength = strlen(_rawText) - textStart;
	char* newText = new char[newLength + 1];

	// Copy old text into new text
	strcpy(newText, (_rawText + textStart));

	// Free existing memory
	delete [] _rawText;

	// Update pointer
	_rawText = newText;

	_text->setText(_rawText);
}
