#include "multilinetextbox.h"
#include "fontbase.h"
#include "text.h"
#include "graphicsport.h"
#include "woopsifuncs.h"
#include "stringiterator.h"
#include "woopsitimer.h"
#include "woopsikey.h"
#include "woopsi.h"

using namespace WoopsiUI;

MultiLineTextBox::MultiLineTextBox(s16 x, s16 y, u16 width, u16 height, const WoopsiString& text, u32 flags, s16 maxRows, GadgetStyle* style) : ScrollingPanel(x, y, width, height, flags, style) {

	_hAlignment = TEXT_ALIGNMENT_HORIZ_CENTRE;
	_vAlignment = TEXT_ALIGNMENT_VERT_CENTRE;
	_topRow = 0;

	_borderSize.top = 3;
	_borderSize.right = 3;
	_borderSize.bottom = 3;
	_borderSize.left = 3;

	Rect rect;
	getClientRect(rect);
	_text = new Text(getFont(), "", rect.width);
	_canvasWidth = rect.width;

	_flags.draggable = true;
	_flags.doubleClickable = true;
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

void MultiLineTextBox::drawText(GraphicsPort* port) {

	// Early exit if there is no text to display
	if (_text->getLineCount() == 0) return;

	// Determine the top and bottom rows within the graphicsport's clip rect.
	// We only draw these rows in order to increase the speed of the routine.
	Rect rect;
	port->getClipRect(rect);

	s32 regionY = -_canvasY + rect.y;						// Y co-ord of the visible region of this canvas
	s32 topRow = getRowContainingCoordinate(regionY);
	s32 bottomRow = getRowContainingCoordinate(regionY + rect.height);

	// Early exit checks
	if ((topRow < 0) && (bottomRow < 0)) return;
	if ((bottomRow >= _text->getLineCount()) && (topRow >= _text->getLineCount())) return;

	// Prevent overflows
	if (topRow < 0) topRow = 0;
	if (bottomRow >= _text->getLineCount()) bottomRow = _text->getLineCount() - 1;

	// Draw lines of text
	s32 currentRow = topRow;

	// Draw all rows in this region
	while (currentRow <= bottomRow) {
		drawRow(port, currentRow);
		currentRow++;
	}
}

void MultiLineTextBox::drawRow(GraphicsPort* port, s32 row) {

	u8 rowLength = _text->getLineTrimmedLength(row);
	s16 textX = getRowX(row) + _canvasX;
	s16 textY = getRowY(row) + _canvasY;
	
	if (isEnabled()) {
		port->drawText(textX, textY, _text->getFont(), *_text, _text->getLineStartIndex(row), rowLength);
	} else {
		port->drawText(textX, textY, _text->getFont(), *_text, _text->getLineStartIndex(row), rowLength, getDarkColour());
	}
}

void MultiLineTextBox::drawContents(GraphicsPort* port) {

	drawText(port);

	// Draw the cursor
	drawCursor(port);
}

void MultiLineTextBox::drawBorder(GraphicsPort* port) {

	port->drawFilledRect(0, 0, _width, _height, getBackColour());

	// Stop drawing if the gadget indicates it should not have an outline
	if (isBorderless()) return;

	port->drawBevelledRect(0, 0, _width, _height, getShadowColour(), getShineColour());
}

void MultiLineTextBox::getCursorCoordinates(s16& x, s16& y) const {

	u32 cursorRow = 0;

	x = 0;
	y = 0;

	// Only calculate the cursor position if the cursor isn't at the start of the text
	if (_cursorPos > 0) {

		// Calculate the row in which the cursor appears
		cursorRow = _text->getLineContainingCharIndex(_cursorPos);

		// Cursor line offset gives us the distance of the cursor from the start of the line
		u8 cursorLineOffset = _cursorPos - _text->getLineStartIndex(cursorRow);
			
		StringIterator* iterator = _text->newStringIterator();
		iterator->moveTo(_text->getLineStartIndex(cursorRow));
			
		// Sum the width of each char in the row to find the x co-ord
		for (s32 i = 0; i < cursorLineOffset; ++i) {
			x += getFont()->getCharWidth(iterator->getCodePoint());
			iterator->moveToNext();
		}
			
		delete iterator;
	}

	// Add offset of row to calculated value
	x += getRowX(cursorRow);

	// Calculate y co-ord of the cursor
	y = getRowY(cursorRow);
}

void MultiLineTextBox::drawCursor(GraphicsPort* port) {

	// Get the cursor co-ords
	if (_showCursor) {

		s16 cursorX = 0;
		s16 cursorY = 0;

		getCursorCoordinates(cursorX, cursorY);

		// Adjust for canvas offsets
		cursorX += _canvasX;
		cursorY += _canvasY;

		// Draw cursor
		port->drawFilledXORRect(cursorX, cursorY, _text->getFont()->getCharWidth(getCursorCodePoint()), _text->getFont()->getHeight());
	}
}

u32 MultiLineTextBox::getCursorCodePoint() const {
	if ((u32)_cursorPos < _text->getLength()) {
		return _text->getCharAt(_cursorPos);
	} else {
		return ' ';
	}
}

// Calculate values for centralised text
u8 MultiLineTextBox::getRowX(s32 row) const {

	Rect rect;
	getClientRect(rect);

	u8 rowLength = _text->getLineTrimmedLength(row);
	u8 rowPixelWidth = _text->getFont()->getStringWidth(*_text, _text->getLineStartIndex(row), rowLength);

	// Calculate horizontal position
	switch (_hAlignment) {
		case TEXT_ALIGNMENT_HORIZ_CENTRE:
			return (rect.width - rowPixelWidth) >> 1;
		case TEXT_ALIGNMENT_HORIZ_LEFT:
			return 0;
		case TEXT_ALIGNMENT_HORIZ_RIGHT:
			return rect.width - rowPixelWidth;
	}

	// Will never be reached
	return 0;
}

s16 MultiLineTextBox::getRowY(s32 row) const {

	// If the amount of text exceeds the size of the gadget, force
	// the text to be top-aligned
	if (_visibleRows <= _text->getLineCount()) {
		return row * _text->getLineHeight();
	}

	// All text falls within the textbox, so obey the alignment
	// options

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

			// Ensure there's always one row
			if (textRows == 0) textRows = 1;

            // Calculate the start position of the block of text
            startPos = ((canvasRows - textRows) * _text->getLineHeight()) >> 1;

            // Calculate the row Y co-ordinate
			textY = startPos + textY;
			break;
		case TEXT_ALIGNMENT_VERT_TOP:
			textY = row * _text->getLineHeight();
			break;
		case TEXT_ALIGNMENT_VERT_BOTTOM:
			textY = rect.height - (((_text->getLineCount() - row) * _text->getLineHeight()));
			break;
	}

	return textY;
}

void MultiLineTextBox::calculateVisibleRows() {

	Rect rect;
	getClientRect(rect);

	_visibleRows = rect.height / _text->getLineHeight();
}

void MultiLineTextBox::setTextAlignmentHoriz(TextAlignmentHoriz alignment) {
	_hAlignment = alignment;
	redraw();
}

void MultiLineTextBox::setTextAlignmentVert(TextAlignmentVert alignment) {
	_vAlignment = alignment;
	redraw();
}

bool MultiLineTextBox::cullTopLines() {

	// Ensure that we have the correct number of rows
	if (_text->getLineCount() > _maxRows) {
		_text->stripTopLines(_text->getLineCount() - _maxRows);
		return true;
	}

	return false;
}

void MultiLineTextBox::limitCanvasHeight() {

	_canvasHeight = _text->getPixelHeight();

	Rect rect;
	getClientRect(rect);
	if (_canvasHeight < rect.height) _canvasHeight = rect.height;
}

void MultiLineTextBox::limitCanvasY() {
	Rect rect;
	getClientRect(rect);

	// Ensure that the visible portion of the canvas is not less than the
	// height of the viewer window
	if (_canvasY + _canvasHeight < rect.height) {
		jumpToTextBottom();
	}
}

void MultiLineTextBox::jumpToTextBottom() {
	Rect rect;
	getClientRect(rect);
	jump(0, -(_canvasHeight - rect.height));
}

void MultiLineTextBox::jumpToCursor() {

	// Get the co-odinates of the cursor
	s16 cursorX;
	s16 cursorY;

	getCursorCoordinates(cursorX, cursorY);

	// Work out which row the cursor falls within
	s32 cursorRow = _text->getLineContainingCharIndex(_cursorPos);
	s16 rowY = getRowY(cursorRow);

	// If the cursor is outside the visible portion of the canvas, jump to it
	Rect rect;
	getClientRect(rect);

	if (rowY + _text->getLineHeight() + _canvasY > rect.height) {

		// Cursor is below the visible portion of the canvas, so
		// jump down so that the cursor's row is the bottom row of
		// text
		jump(0, -(rowY + _text->getLineHeight() - rect.height));
	} else if (rowY + _canvasY < 0) {

		// Cursor is above the visible portion of the canvas, so
		// jump up so that the cursor's row is the top row of text
		jump(0, -cursorY);
	}
}

void MultiLineTextBox::setText(const WoopsiString& text) {

	bool drawingEnabled = isDrawingEnabled();
	disableDrawing();

	_text->setText(text);

	cullTopLines();
	limitCanvasHeight();
	jumpToTextBottom();

	if (drawingEnabled) enableDrawing();

	redraw();

	_gadgetEventHandlers->raiseValueChangeEvent();
}

void MultiLineTextBox::appendText(const WoopsiString& text) {

	bool drawingEnabled = isDrawingEnabled();
	disableDrawing();

	_text->append(text);

	cullTopLines();
	limitCanvasHeight();
	jumpToTextBottom();

	if (drawingEnabled) enableDrawing();

	redraw();

	_gadgetEventHandlers->raiseValueChangeEvent();
}

void MultiLineTextBox::removeText(const u32 startIndex) {
	removeText(startIndex, _text->getLength() - startIndex);
}

void MultiLineTextBox::removeText(const u32 startIndex, const u32 count) {

	bool drawingEnabled = isDrawingEnabled();
	disableDrawing();

	_text->remove(startIndex, count);

	limitCanvasHeight();
	limitCanvasY();

	moveCursorToPosition(startIndex);

	if (drawingEnabled) enableDrawing();

	redraw();

	_gadgetEventHandlers->raiseValueChangeEvent();
}

void MultiLineTextBox::insertText(const WoopsiString& text, const u32 index) {

	bool drawingEnabled = isDrawingEnabled();
	disableDrawing();

	_text->insert(text, index);

	cullTopLines();
	limitCanvasHeight();

	moveCursorToPosition(index + text.getLength());

	if (drawingEnabled) enableDrawing();

	redraw();

	_gadgetEventHandlers->raiseValueChangeEvent();
}

void MultiLineTextBox::setFont(FontBase* font) {

	bool drawingEnabled = isDrawingEnabled();
	disableDrawing();

	_style.font = font;
	_text->setFont(font);

	cullTopLines();
	limitCanvasHeight();
	limitCanvasY();

	if (drawingEnabled) enableDrawing();

	redraw();

	_gadgetEventHandlers->raiseValueChangeEvent();
}

const u16 MultiLineTextBox::getPageCount() const {
	if (_visibleRows > 0) {
		return (_text->getLineCount() / _visibleRows) + 1;
	} else {
		return 1;
	}
}

const u16 MultiLineTextBox::getCurrentPage() const {

	// Calculate the top line of text
	s32 topRow = -_canvasY / _text->getLineHeight();

	// Return the page on which the top row falls
	if (_visibleRows > 0) {
		return topRow / _visibleRows;
	} else {
		return 1;
	}
}

void MultiLineTextBox::onResize(u16 width, u16 height) {

	// Ensure the base class resize method is called
	ScrollingPanel::onResize(width, height);

	// Resize the canvas' width
	Rect rect;
	getClientRect(rect);
	_canvasWidth = rect.width;
	_canvasHeight = rect.height;
	_canvasX = 0;
	_canvasY = 0;

	calculateVisibleRows();

	// Re-wrap the text
	_text->setWidth(_width);
	_text->wrap();

	bool raiseEvent = cullTopLines();
	limitCanvasHeight();
	limitCanvasY();

	if (raiseEvent) _gadgetEventHandlers->raiseValueChangeEvent();
}

const u32 MultiLineTextBox::getTextLength() const {
	return _text->getLength();
}

void MultiLineTextBox::showCursor() {
	if (!_showCursor) {
		_showCursor = true;
		redraw();
	}
}

void MultiLineTextBox::hideCursor() {
	if (_showCursor) {
		_showCursor = false;
		redraw();
	}
}

void MultiLineTextBox::insertTextAtCursor(const WoopsiString& text) {
	insertText(text, getCursorPosition());
}

void MultiLineTextBox::moveCursorToPosition(const s32 position) {

	GraphicsPort* port = newGraphicsPort(false);

	// Erase existing cursor
	drawCursor(port);

	// Force position to within confines of string
	if (position < 0) {
		_cursorPos = 0;
	} else {
		s32 len = (s32)_text->getLength();
		_cursorPos = len > position ? position : len;
	}

	// Draw cursor in new position
	drawCursor(port);
}

void MultiLineTextBox::onClick(s16 x, s16 y) {
	startDragging(x, y);

	// Move cursor to clicked co-ords
	Rect rect;
	getClientRect(rect);

	// Adjust x and y from screen co-ords to canvas co-ords
	s16 canvasRelativeX = x - getX() - rect.x - _canvasX;
	s16 canvasRelativeY = y - getY() - rect.y - _canvasY;

	moveCursorToPosition(getCharIndexAtCoordinates(canvasRelativeX, canvasRelativeY));
}

void MultiLineTextBox::onDoubleClick(s16 x, s16 y) {
	woopsiApplication->showKeyboard(this);
}

void MultiLineTextBox::onKeyPress(KeyCode keyCode) {
	processPhysicalKey(keyCode);
}

void MultiLineTextBox::onKeyRepeat(KeyCode keyCode) {
	processPhysicalKey(keyCode);
}

void MultiLineTextBox::moveCursorUp() {
	s16 cursorX = 0;
	s16 cursorY = 0;

	getCursorCoordinates(cursorX, cursorY);

	// Get the midpoint of the cursor.  We use the midpoint to ensure that
	// the cursor does not drift off to the left as it moves up the text, which
	// is a problem when we use the left edge as the reference point when the
	// font is proportional
	cursorX += _text->getFont()->getCharWidth(_text->getCharAt(_cursorPos)) >> 1;

	// Locate the character above the midpoint
	s32 index = getCharIndexAtCoordinates(cursorX, cursorY + _text->getLineHeight());

	moveCursorToPosition(index);
	jumpToCursor();
}

void MultiLineTextBox::moveCursorDown() {
	s16 cursorX = 0;
	s16 cursorY = 0;

	getCursorCoordinates(cursorX, cursorY);

	// Get the midpoint of the cursor.  We use the midpoint to ensure that
	// the cursor does not drift off to the left as it moves up the text, which
	// is a problem when we use the left edge as the reference point when the
	// font is proportional
	cursorX += _text->getFont()->getCharWidth(_text->getCharAt(_cursorPos)) >> 1;

	// Locate the character above the midpoint
	s32 index = getCharIndexAtCoordinates(cursorX, cursorY - _text->getLineHeight());

	moveCursorToPosition(index);
	jumpToCursor();
}

void MultiLineTextBox::moveCursorLeft() {
	if (_cursorPos > 0) {
		moveCursorToPosition(_cursorPos - 1);
	}

	jumpToCursor();
}

void MultiLineTextBox::moveCursorRight() {
	if (_cursorPos < (s32)_text->getLength()) {
		moveCursorToPosition(_cursorPos + 1);
	}

	jumpToCursor();
}

void MultiLineTextBox::processPhysicalKey(KeyCode keyCode) {
	switch (keyCode) {
		case KEY_CODE_LEFT:
			moveCursorLeft();
			break;
		case KEY_CODE_RIGHT:
			moveCursorRight();
			break;
		case KEY_CODE_UP:
			moveCursorDown();
			break;
		case KEY_CODE_DOWN:
			moveCursorUp();
			break;
		default:
			// Not interested in other keys
			break;
	}
}

void MultiLineTextBox::handleKeyboardPressEvent(const KeyboardEventArgs& e) {
	processKey(e.getKey());
}

void MultiLineTextBox::handleKeyboardRepeatEvent(const KeyboardEventArgs& e) {
	processKey(e.getKey());
}

void MultiLineTextBox::processKey(const WoopsiKey* key) {

	if (key->getKeyType() == WoopsiKey::KEY_BACKSPACE) {

		// Delete character in front of cursor
		if (_cursorPos > 0) removeText(_cursorPos - 1, 1);
	} else if (key->getValue() != '\0') {

		// Not modifier; append value
		insertTextAtCursor(key->getValue());
	} 
}

s32 MultiLineTextBox::getRowContainingCoordinate(s16 y) const {

	s32 row = -1;

	// Locate the row containing the character
	for (s32 i = 0; i < _text->getLineCount(); ++i) {

		// Abort search if we've found the row below the y co-ordinate
		if (getRowY(i) > y) {

			if (i == 0) {

				// If the co-ordinate is above the text, we return the top
				// row
				row = 0;
			} else {

				// Row within the text, so return the previous row - this is
				// the row that contains the co-ordinate.
				row = i - 1;
			}

			break;
		}
	}

	// If the co-ordinate is below the text, row will still be -1.
	// We need to set it to the last row
	if (row == -1) row = _text->getLineCount() - 1;

	return row;
}

u32 MultiLineTextBox::getCharIndexAtCoordinate(s16 x, s32 rowIndex) const {

	// Locate the character within the row
	s32 startIndex = _text->getLineStartIndex(rowIndex);
	s32 stopIndex = _text->getLineLength(rowIndex);
	s32 width = getRowX(rowIndex);
	s32 index = -1;

	StringIterator* iterator = _text->newStringIterator();
	iterator->moveTo(startIndex);

	width += _text->getFont()->getCharWidth(iterator->getCodePoint());

	for (s32 i = 0; i < stopIndex; ++i) {
		if (width > x) {

			if (i == 0) {

				// If the co-ordinate is on the left of the text, we add nothing
				// to the index
				index = startIndex;
			} else {

				// Character within the row.
				// This is the character that contains the co-ordinate.
				index = startIndex + i;
			}

			break;
		}

		iterator->moveToNext();

		width += _text->getFont()->getCharWidth(iterator->getCodePoint());
	}

	delete iterator;

	// If the co-ordinate is past the last character, index will still be -1.
	// We need to set it to the last character
	if (index == -1) {
		
		if (rowIndex == _text->getLineCount() - 1) {

			// Index past the end point of the text, so return an index
			// just past the text
			index = startIndex + stopIndex;
		} else {

			// Index at the end of a row, so return the last index of the
			// row
			index = startIndex + stopIndex - 1;
		}

	}

	return index;
}

u32 MultiLineTextBox::getCharIndexAtCoordinates(s16 x, s16 y) const {
	s32 rowIndex = getRowContainingCoordinate(y);
	return getCharIndexAtCoordinate(x, rowIndex);
}
