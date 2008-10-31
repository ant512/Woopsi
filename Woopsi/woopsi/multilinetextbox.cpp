#include "multilinetextbox.h"
#include "fontbase.h"
#include "text.h"
#include "graphicsport.h"
#include "woopsifuncs.h"

using namespace WoopsiUI;

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

	_cursorPos = 0;
	_showCursor = false;

	setText(text);
}

MultiLineTextBox::~MultiLineTextBox() {
	delete _text;
	_text = NULL;
}

void MultiLineTextBox::drawText(Rect clipRect, s32 topRow, s32 bottomRow) {

	// Early exit checks
	if ((topRow < 0) && (bottomRow < 0)) return;
	if ((bottomRow >= _text->getLineCount()) && (topRow >= _text->getLineCount())) return;

	// Prevent overflows
	if (topRow < 0) topRow = 0;
	if (bottomRow >= _text->getLineCount()) bottomRow = _text->getLineCount() - 1;

	// Create a graphics port to draw the text - we use a non-internal port
	// to ensure that we clip within the border we've just drawn
	GraphicsPort* port = newGraphicsPort(clipRect);

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

void MultiLineTextBox::drawTextTop(Rect clipRect) {

	// Early exit if there is no text to display
	if (_text->getLineCount() == 0) return;

	// Calculate various values needed to output text for this cliprect
	u8 lineHeight = _text->getLineHeight();
	s16 offsetY = clipRect.y - getY();					// Translate the physical y co-ords back to gadget space
	s32 regionY = -_canvasY + offsetY;					// Y co-ord of the visible region of this canvas

	s32 topRow = (regionY / lineHeight) - 1;			// Calculate the top line of text in this region
	s32 bottomRow = ((regionY + clipRect.height) / lineHeight);	// Calculate bottom line of text

	// Draw lines of text
	drawText(clipRect, topRow, bottomRow);	
}

void MultiLineTextBox::draw(Rect clipRect) {

	// Create a graphics port to draw the border
 	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	// Clear
	port->drawFilledRect(0, 0, _width, _height, _backColour);

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;

	// Always use top alignment if the number of rows of text exceeds or is
	// equal to the number of visible rows
	if (_visibleRows <= _text->getLineCount()) {
		drawTextTop(clipRect);
	} else {
		drawText(clipRect, 0, _text->getLineCount());
	}

	// Draw the cursor
	drawCursor(clipRect);
}

void MultiLineTextBox::drawCursor(Rect clipRect) {

	GraphicsPort* port = newGraphicsPort(clipRect);

	// Get the cursor co-ords
	if (_showCursor) {
		u32 cursorRow = 0;

		u16 cursorX = 0;
		s16 cursorY = 0;

		// Only calculate the cursor position if the cursor isn't at the start of the text
		if (_cursorPos > 0) {

			// Calculate the row in which the cursor appears
			cursorRow = _text->getLineContainingCharIndex(_cursorPos);

			// Cursor line offset gives us the distance of the cursor from the start of the line
			u8 cursorLineOffset = _cursorPos - _text->getLineStartIndex(cursorRow);
			
			// Grab a pointer to the start of the current line of text for ease of
			// processing later
			const char* lineData = _text->getLinePointer(cursorRow);
			
			// Sum the width of each char in the row to find the x co-ord 
			for (s32 i = 0; i < cursorLineOffset; i++) {
				cursorX += _font->getCharWidth(lineData[i]);
			}
		}

		// Add offset of row (taking into account canvas co-ord and text alignment)
		// to calculated value
		cursorX += getRowX(cursorRow) + _canvasX;

		// Calculate y co-ord of the cursor
		cursorY = getRowY(cursorRow) + _canvasY;

		// Draw cursor
		port->drawFilledXORRect(cursorX, cursorY, _font->getCharWidth(_text->getCharArray()[_cursorPos]), _font->getHeight());
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
            startPos = ((canvasRows - textRows) * _text->getLineHeight()) >> 1;

            // Calculate the row Y co-ordinate
			textY = startPos + textY;
			break;
		case TEXT_ALIGNMENT_VERT_TOP:
			textY = _padding + (row * _text->getLineHeight());
			break;
		case TEXT_ALIGNMENT_VERT_BOTTOM:
			textY = rect.height - (((_text->getLineCount() - row) * _text->getLineHeight())) - _padding;
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

	moveCursorToPosition(startIndex);

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

void MultiLineTextBox::showCursor() {
	if (!_showCursor) {
		_showCursor = true;
		draw();
	}
}

void MultiLineTextBox::hideCursor() {
	if (_showCursor) {
		_showCursor = false;
		draw();
	}
}

void MultiLineTextBox::insertTextAtCursor(const char* text) {
	insertText(text, getCursorPosition());
}

void MultiLineTextBox::insertTextAtCursor(const char text) {
	insertText(text, getCursorPosition());
}

void MultiLineTextBox::moveCursorToPosition(const s32 position) {

	// Force position to within confines of string
	if (position < 0) {
		_cursorPos = 0;
	} else {
		s32 len = (s32)_text->getLength();
		_cursorPos = len > position ? position : len;
	}

	draw();
}

void MultiLineTextBox::insertText(const char* text, const u32 index) {
	// Get current text length - use this later to quickly get the length
	// of the inserted string to shift the cursor around
	u32 oldLen = _text->getLength();

	_text->insert(text, index);
	
	// Get the new string length and use it to calculate the length
	// of the inserted string
	u32 insertLen = _text->getLength() - oldLen;

	moveCursorToPosition(index + insertLen);

	// Update max scroll value
	if (_text->getLineCount() > _visibleRows) {
		_canvasHeight = _text->getPixelHeight() + (_padding << 1);

		// Scroll to bottom of new text
		jump(0, -(_canvasHeight - _height));
	}

	Gadget::draw();

	raiseValueChangeEvent();
}

void MultiLineTextBox::insertText(const char text, const u32 index) {
	_text->insert(text, index);

	// Cursor position just increases by one as we're inserting a single char
	moveCursorToPosition(getCursorPosition() + 1);

	// Update max scroll value
	if (_text->getLineCount() > _visibleRows) {
		_canvasHeight = _text->getPixelHeight() + (_padding << 1);

		// Scroll to bottom of new text
		jump(0, -(_canvasHeight - _height));
	}

	Gadget::draw();

	raiseValueChangeEvent();
}
