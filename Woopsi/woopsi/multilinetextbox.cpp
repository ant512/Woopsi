// TODO: Padding of 12px and above goes crazy

#include "multilinetextbox.h"
#include "fontbase.h"
#include "text.h"
#include "graphicsport.h"

MultiLineTextBox::MultiLineTextBox(s16 x, s16 y, u16 width, u16 height, const char* text, u32 flags, s16 maxRows, FontBase* font) : ScrollingPanel(x, y, width, height, flags, font) {

	_outline = OUTLINE_IN;

	_hPos = TEXT_POSITION_HORIZ_CENTRE;
	_vPos = TEXT_POSITION_VERT_CENTRE;
	_padding = 2;
	_topRow = 0;

	Rect rect;
	getClientRect(rect);
	_text = new Text(_font, "", rect.width - (_padding << 1));

	_flags.draggable = true;

	_canvasWidth = rect.width;

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
}

void MultiLineTextBox::draw(Rect clipRect) {

 	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	// Clear
	port->drawFilledRect(0, 0, _width, _height, _backColour);

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;

	port = newGraphicsPort(clipRect);

	// Calculate the top line of text in this region
	s32 topRowRegion = ((-_canvasY + (clipRect.y - getY())) / _text->getLineHeight()) - 1;

	// Calculcate the bottom line of text in this region
	s32 bottomRowRegion = (((-_canvasY + (clipRect.y + clipRect.height - getY())) / _text->getLineHeight())) + 1;

	// Calculate the number of rows in this region
	u8 rowCountRegion = (bottomRowRegion - topRowRegion);

	// Draw lines of text
	s16 textX;
	s16 textY;
	s32 currentTextRow = topRowRegion;
	u8 rowPixelWidth = 0;
	u8 rowLength = 0;
	u8 i = 0;

	while ((i < rowCountRegion) && (i < _text->getLineCount() - topRowRegion) && (currentTextRow < _text->getLineCount() - 1)) {

		currentTextRow = i + topRowRegion;

		if (currentTextRow > -1) {

			// Precalculate lengths in characters and pixels
			rowLength = _text->getLineTrimmedLength(currentTextRow);
			rowPixelWidth = _text->getFont()->getStringWidth(_text->getLinePointer(currentTextRow), rowLength);

			textX = getRowX(rowPixelWidth) + _canvasX;
			textY = getRowY(currentTextRow) + _canvasY;

			port->drawText(textX, textY, _text->getFont(), rowLength, _text->getLinePointer(currentTextRow));
		}

		i++;
	}

	delete port;
}

// Calculate values for centralised text
u8 MultiLineTextBox::getRowX(u8 rowPixelWidth) {

	Rect rect;
	getClientRect(rect);

	// Calculate horizontal position
	switch (_hPos) {
		case TEXT_POSITION_HORIZ_CENTRE:
			return ((rect.width - (_padding << 1)) - rowPixelWidth) >> 1;
		case TEXT_POSITION_HORIZ_LEFT:
			return _padding;
		case TEXT_POSITION_HORIZ_RIGHT:
			return rect.width - rowPixelWidth - _padding;
	}

	// Will never be reached
	return 0;
}

s16 MultiLineTextBox::getRowY(u8 screenRow) {

	s16 textY = 0;
	s16 startPos = 0;

    s32 canvasRows = 0;
    s32 textRows = 0;

	Rect rect;
	getClientRect(rect);

	// Calculate vertical position
	switch (_vPos) {
		case TEXT_POSITION_VERT_CENTRE:

			// Calculate the maximum number of rows
            canvasRows = _canvasHeight / _text->getLineHeight();

            // Get the number of rows of text
            textRows = _text->getLineCount();

            // Calculate the start position of the block of text
            startPos = ((canvasRows - textRows) >> 1) * _text->getLineHeight();

            // Calculate the row Y co-ordinate
			textY = startPos + (screenRow * _text->getLineHeight());
			break;
		case TEXT_POSITION_VERT_TOP:
			textY = _padding + (screenRow * _text->getLineHeight());
			break;
		case TEXT_POSITION_VERT_BOTTOM:

			// Calculate the maximum number of rows
            s32 screenRows = rect.height / _text->getLineHeight();

			textY = (rect.height - (_text->getLineHeight() * (screenRows - screenRow))) - _padding;
			break;
	}

	return textY;
}

void MultiLineTextBox::calculateTotalVisibleRows() {

	Rect rect;
	getClientRect(rect);

	_visibleRows = (rect.height - (_padding << 1)) / _text->getLineHeight();
}

void MultiLineTextBox::setTextPositionHoriz(TextPositionHoriz position) {
	_hPos = position;
	Gadget::draw();
}

void MultiLineTextBox::setTextPositionVert(TextPositionVert position) {
	_vPos = position;
	Gadget::draw();
}

const Text* MultiLineTextBox::getText() const {
	return _text;
}

void MultiLineTextBox::setText(const char* text) {

	_text->setText(text);

	// Ensure that we have the correct number of rows
	if (_text->getLineCount() > _maxRows) {
		_text->stripTopLines(_text->getLineCount() - _maxRows);

		_canvasHeight = _text->getPixelHeight() + (_padding << 1);
	}

	Gadget::draw();

	// Update canvas height
	if (_text->getLineCount() > _visibleRows) {
		_canvasHeight = _text->getPixelHeight() + (_padding << 1);

		// Scroll to bottom of new text
		scroll(0, _canvasHeight - _height);
	}

	raiseValueChangeEvent();
}

void MultiLineTextBox::setText(const char text) {

	char* newText = new char[2];
	newText[0] = text;
	newText[1] = '\0';

	setText(newText);
}

void MultiLineTextBox::addText(const char* text) {

	_text->appendText(text);

	// Ensure that we have the correct number of rows
	if (_text->getLineCount() > _maxRows) {
		_text->stripTopLines(_text->getLineCount() - _maxRows);

		_canvasHeight = _text->getPixelHeight() + (_padding << 1);
	}

	Gadget::draw();

	// Update max scroll value
	if (_text->getLineCount() > _visibleRows) {
		_canvasHeight = _text->getPixelHeight() + (_padding << 1);

		// Scroll to bottom of new text
		jump(0, -_canvasHeight - _height);
	}

	raiseValueChangeEvent();
}

void MultiLineTextBox::addText(const char text) {

	char* newText = new char[2];
	newText[0] = text;
	newText[1] = '\0';

	addText(newText);
}

void MultiLineTextBox::setFont(FontBase* font) {
	_font = font;
	_text->setFont(font);
}

const u16 MultiLineTextBox::getPageCount() const {

	// Get client rect
	Rect clientRect;
	getClientRect(clientRect);

	// Calculate the maximum number of visible rows
	u8 rowCount = (clientRect.height - (_padding << 1)) / _text->getLineHeight();

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
	u8 rowCount = (clientRect.height - (_padding << 1)) / _text->getLineHeight();

	// Return the page on which the top row falls
	return topRow / rowCount;
}

bool MultiLineTextBox::resize(u16 width, u16 height) {
	bool drawing = _flags.drawingEnabled;
	_flags.drawingEnabled = false;

	bool raiseEvent = false;

	// Resize the gadget
	Gadget::resize(width, height);

	// Resize the canvas' width
	Rect rect;
	getClientRect(rect);
	_canvasWidth = rect.width;

	_flags.drawingEnabled = drawing;

	// Re-wrap the text
	_text->setWidth(_width);
	_text->wrap();

	// Ensure that we have the correct number of rows
	if (_text->getLineCount() > _maxRows) {
		_text->stripTopLines(_text->getLineCount() - _maxRows);

		_canvasHeight = _text->getPixelHeight() + (_padding << 1);
		raiseEvent = true;
	}

	Gadget::draw();

	// Update canvas height
	if (_text->getLineCount() > _visibleRows) {
		_canvasHeight = _text->getPixelHeight() + (_padding << 1);

		// Scroll to bottom of new text
		scroll(0, _canvasHeight - _height);
	}

	if (raiseEvent) raiseValueChangeEvent();

	return true;
}
