// TODO: Padding of 12px and above goes crazy

#include "multilinetextbox.h"
#include "fontbase.h"
#include "text.h"
#include "graphicsport.h"
#include "woopsifuncs.h"

MultiLineTextBox::MultiLineTextBox(s16 x, s16 y, u16 width, u16 height, const char* text, u32 flags, s16 maxRows, FontBase* font) : ScrollingPanel(x, y, width, height, flags, font) {

	_outline = OUTLINE_IN;

	_hAlignment = TEXT_ALIGNMENT_HORIZ_CENTRE;
	_vAlignment = TEXT_ALIGNMENT_VERT_CENTRE;
	_padding = 2;
	_topRow = 0;

	Rect rect;
	getClientRect(rect);
	_text = new Text(_font, "", rect.width - (_padding << 1));

	_flags.draggable = true;
	_maxRows = maxRows;

	calculateVisibleRows();

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

	// Create a graphics port to draw the border
 	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	// Clear
	port->drawFilledRect(0, 0, _width, _height, _backColour);

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;

	// Create a graphics port to draw the text - we use a non-internal port
	// to ensure that we clip within the border we've just drawn
	port = newGraphicsPort(clipRect);


/*
	s16 textX;
	s16 textY;
	u8 rowLength;

	for (s32 i = 0; i < _text->getLineCount(); ++i) {

		rowLength = _text->getLineTrimmedLength(i);

		textX = getRowX(i) + _canvasX;
		textY = getRowY(i) + _canvasY;

		port->drawText(textX, textY, _text->getFont(), rowLength, _text->getLinePointer(i));
	}

	*/

	// Calculate various values needed to output text for this cliprect
	u8 lineHeight = _text->getLineHeight();
	s16 offsetY = clipRect.y - getY();					// Translate the physical y co-ords back to gadget space
	s32 regionY = -_canvasY + offsetY;					// Y co-ord of the visible region of this canvas

	s32 topRow = (regionY / lineHeight) - 1;			// Calculate the top line of text in this region
	s32 bottomRow = ((regionY + clipRect.height) / lineHeight);	// Calculate bottom line of text

	// Early exit checks
	if ((topRow < 0) && (bottomRow < 0)) return;
	if ((bottomRow >= _text->getLineCount()) && (topRow >= _text->getLineCount())) return;

	// Prevent overflows
	if (topRow < 0) topRow = 0;
	if (bottomRow >= _text->getLineCount()) bottomRow = _text->getLineCount() - 1;

	// Draw lines of text
	s16 textX;
	s16 textY;
	s32 currentRow = topRow;
	u8 rowLength = 0;

	// Draw all rows in this region
	while (currentRow <= bottomRow) {

		rowLength = _text->getLineTrimmedLength(currentRow);

		textX = getRowX(currentRow) + _canvasX;
		textY = getRowY(currentRow) + _canvasY;

		port->drawText(textX, textY, _text->getFont(), rowLength, _text->getLinePointer(currentRow));

		currentRow++;
	}

	delete port;
}

// Calculate values for centralised text
u8 MultiLineTextBox::getRowX(s32 row) {

	Rect rect;
	getClientRect(rect);

	u8 rowLength = _text->getLineTrimmedLength(row);
	u8 rowPixelWidth = _text->getFont()->getStringWidth(_text->getLinePointer(row), rowLength);

	// Calculate horizontal position
	switch (_hAlignment) {
		case TEXT_ALIGNMENT_HORIZ_CENTRE:
			return ((rect.width - (_padding << 1)) - rowPixelWidth) >> 1;
		case TEXT_ALIGNMENT_HORIZ_LEFT:
			return _padding;
		case TEXT_ALIGNMENT_HORIZ_RIGHT:
			return rect.width - rowPixelWidth - _padding;
	}

	// Will never be reached
	return 0;
}

s16 MultiLineTextBox::getRowY(s32 row) {

	s16 textY = 0;
	s16 startPos = 0;

    s32 canvasRows = 0;
    s32 textRows = 0;

	Rect rect;
	getClientRect(rect);

	// Calculate vertical position
	switch (_vAlignment) {
		case TEXT_ALIGNMENT_VERT_CENTRE:

			// Calculate the maximum number of rows
            canvasRows = _canvasHeight / _text->getLineHeight();
			textY = row * _text->getLineHeight();

            // Get the number of rows of text
            textRows = _text->getLineCount();

            // Calculate the start position of the block of text
            startPos = ((canvasRows - textRows) >> 1) * _text->getLineHeight();

            // Calculate the row Y co-ordinate
			textY = startPos + textY;
			break;
		case TEXT_ALIGNMENT_VERT_TOP:
			textY = _padding + (row * _text->getLineHeight());
			break;
		case TEXT_ALIGNMENT_VERT_BOTTOM:

			// Calculate the maximum number of rows
            s32 maxRows = rect.height / _text->getLineHeight();

			textY = (rect.height - (_text->getLineHeight() * (maxRows - row))) - _padding;
			break;
	}

	return textY;
}

void MultiLineTextBox::calculateVisibleRows() {

	Rect rect;
	getClientRect(rect);

	_visibleRows = (rect.height - (_padding << 1)) / _text->getLineHeight();
}

void MultiLineTextBox::setTextAlignmentHoriz(TextAlignmentHoriz alignment) {
	_hAlignment = alignment;
	Gadget::draw();
}

void MultiLineTextBox::setTextAlignmentVert(TextAlignmentVert alignment) {
	_vAlignment = alignment;
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

	// Update max scroll value
	if (_text->getLineCount() > _visibleRows) {
		_canvasHeight = _text->getPixelHeight() + (_padding << 1);

		// Scroll to bottom of new text
		jump(0, -(_canvasHeight - _height));
	}

	Gadget::draw();

	raiseValueChangeEvent();
}

void MultiLineTextBox::setText(const char text) {

	char* newText = new char[2];
	newText[0] = text;
	newText[1] = '\0';

	setText(newText);

	delete[] newText;
}

void MultiLineTextBox::appendText(const char* text) {

	_text->append(text);

	// Ensure that we have the correct number of rows
	if (_text->getLineCount() > _maxRows) {
		_text->stripTopLines(_text->getLineCount() - _maxRows);

		_canvasHeight = _text->getPixelHeight() + (_padding << 1);
	}

	// Update max scroll value
	if (_text->getLineCount() > _visibleRows) {
		_canvasHeight = _text->getPixelHeight() + (_padding << 1);

		// Scroll to bottom of new text
		jump(0, -(_canvasHeight - _height));
	}

	Gadget::draw();

	raiseValueChangeEvent();
}

void MultiLineTextBox::appendText(const char text) {

	char* newText = new char[2];
	newText[0] = text;
	newText[1] = '\0';

	appendText(newText);

	delete[] newText;
}

void MultiLineTextBox::removeText(const u32 startIndex) {
	_text->remove(startIndex);

	// Update max scroll value
	if (_text->getLineCount() > _visibleRows) {
		_canvasHeight = _text->getPixelHeight() + (_padding << 1);

		// Scroll to bottom of new text
		jump(0, -(_canvasHeight - _height));
	}

	Gadget::draw();

	raiseValueChangeEvent();
}

void MultiLineTextBox::removeText(const u32 startIndex, const u32 count) {
	_text->remove(startIndex, count);

	// Update max scroll value
	if (_text->getLineCount() > _visibleRows) {
		_canvasHeight = _text->getPixelHeight() + (_padding << 1);

		// Scroll to bottom of new text
		jump(0, -(_canvasHeight - _height));
	}

	Gadget::draw();

	raiseValueChangeEvent();
}


void MultiLineTextBox::setFont(FontBase* font) {
	_font = font;
	_text->setFont(font);
}

const u16 MultiLineTextBox::getPageCount() const {

	// Get client rect
	Rect clientRect;
	getClientRect(clientRect);

	// Return number of screens of text
	if (_visibleRows > 0) {
		u16 pages = _text->getLineCount() / _visibleRows;

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

	// Return the page on which the top row falls
	if (_visibleRows > 0) {
		return topRow / _visibleRows;
	} else {
		return 1;
	}
}

bool MultiLineTextBox::resize(u16 width, u16 height) {

	// Ensure drawing is disabled before we start
	bool drawing = _flags.drawingEnabled;
	_flags.drawingEnabled = false;

	bool raiseEvent = false;

	// Resize the gadget
	Gadget::resize(width, height);

	// Resize the canvas' width
	Rect rect;
	getClientRect(rect);
	_canvasWidth = rect.width;

	calculateVisibleRows();

	// Restore the gadget's drawing flag to its original state
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

	// Update canvas height
	if (_text->getLineCount() > _visibleRows) {
		_canvasHeight = _text->getPixelHeight() + (_padding << 1);

		// Jump to bottom of new text
		jump(0, -(_canvasHeight - _height));
	}

	Gadget::draw();

	if (raiseEvent) raiseValueChangeEvent();

	return true;
}

const u32 MultiLineTextBox::getTextLength() const {
	return _text->getLength();
}
