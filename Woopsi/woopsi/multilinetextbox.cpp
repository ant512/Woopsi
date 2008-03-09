#include "multilinetextbox.h"
#include "woopsi.h"

MultiLineTextBox::MultiLineTextBox(s16 x, s16 y, u16 width, u16 height, char* text, u32 flags, s16 maxRows, FontBase* font) : ScrollingPanel(x, y, width, height, flags, font) {
	
	_rawText = NULL;
	_text = new Text(_font, "", width);

	_outline = OUTLINE_IN;

	_hPos = TEXT_POSITION_HORIZ_CENTRE;
	_vPos = TEXT_POSITION_VERT_CENTRE;
	_padding = 2;
	_topRow = 0;
	_autoDrawing = true;

	_flags.draggable = true;

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

	// Get client rect
	Rect clientRect;
	getClientRect(clientRect);

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	// Clear
	port->drawFilledRect(0, 0, _width, _height, _backColour);

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;

	port = newGraphicsPort(clipRect);

	// Calculate the maximum number of visible rows
	u8 rowCount = clientRect.height / _text->getLineHeight();

	// Calculate the top line of text in this region
	s32 topRowRegion = ((-_canvasY + (clipRect.y - getY())) / _text->getLineHeight()) - 1;

	// Calculate the number of rows in this region
	u8 rowCountRegion = (clipRect.height / _text->getLineHeight()) + 3;

	// Draw lines of text
	s16 textX;
	s16 textY;
	s32 currentTextRow = topRowRegion;
	u8 rowPixelWidth = 0;
	u8 rowLength = 0;
	_canvasX = 0;
	u8 i = 0;

	while ((i < rowCountRegion) && (i < _text->getLineCount() - topRowRegion) && (currentTextRow < _text->getLineCount())) {

		currentTextRow = i + topRowRegion;
		
		if (currentTextRow > -1) {

			// Precalculate lengths in characters and pixels
			rowLength = _text->getLineTrimmedLength(currentTextRow);
			rowPixelWidth = _text->getFont()->getWidth() * rowLength;

			textX = getRowX(rowPixelWidth) + _canvasX;
			textY = getRowY(currentTextRow, rowCount) + _canvasY + 1;
			
			port->drawText(textX, textY, _text->getFont(), rowLength, _text->getLinePointer(currentTextRow));
		}

		i++;
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

s16 MultiLineTextBox::getRowY(u8 screenRow, u8 screenRows) {

	s16 textY = 0;
	s16 startPos = 0;

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

char* MultiLineTextBox::getRawText() {
	return _rawText;
}

const Text* MultiLineTextBox::getText() const {
	return _text;
}

void MultiLineTextBox::setText(char* text) {
	setText(text, true);
}

void MultiLineTextBox::setText(char* text, bool raiseEvent) {
	
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

	// Update canvas height
	if (_text->getLineCount() > _visibleRows) {
		_canvasHeight = _text->getPixelHeight() + (_padding << 1);

		// Scroll to bottom of new text
		scroll(0, _canvasHeight - _height);
	}

	if (raiseEvent) raiseValueChangeEvent();
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

			if (_autoDrawing) {
				draw();
			}

		} else {
			_text->setText(_rawText);
		}
		
		// Update max scroll value
		if (_text->getLineCount() > _visibleRows) {
			_canvasHeight = _text->getPixelHeight() + (_padding << 1);

			// Scroll to bottom of new text
			scroll(0, -_canvasHeight);
		}

		raiseValueChangeEvent();
	} else {
		// No text, so set it for the first time
		setText(text);
	}
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

const u16 MultiLineTextBox::getPageCount() const {

	// Get client rect
	Rect clientRect;
	getClientRect(clientRect);

	// Calculate the maximum number of visible rows
	u8 rowCount = clientRect.height / _text->getLineHeight();
	
	// Return number of screens of text
	if (rowCount > 0) {
		u16 pages = _text->getLineCount() / rowCount;
		
		return pages + 1;
	} else {
		return 1;
	}
}

const u16 MultiLineTextBox::getCurrentPage() const {

	// Get client rect
	Rect clientRect;
	getClientRect(clientRect);

	// Calculate the top line of text
	s32 topRow = (-_canvasY / _text->getLineHeight());
	
	// Calculate the maximum number of visible rows
	u8 rowCount = clientRect.height / _text->getLineHeight();
	
	// Return the page on which the top row falls
	return topRow / rowCount;
}
