#include "textbox.h"
#include "graphicsport.h"
#include "woopsitimer.h"
#include "stringiterator.h"
#include "woopsikey.h"
#include "woopsi.h"

using namespace WoopsiUI;

TextBox::TextBox(s16 x, s16 y, u16 width, u16 height, const WoopsiString& text, GadgetStyle* style) : Label(x, y, width, height, text, style) {
	_cursorPos = 0;
	_showCursor = true;
	_flags.doubleClickable = true;
	_opensKeyboard = true;

	_borderSize.top = 4;
	_borderSize.right = 4;
	_borderSize.bottom = 4;
	_borderSize.left = 4;

	setBorderless(false);

	moveCursorToPosition(_text.getLength());
}

void TextBox::drawContents(GraphicsPort* port) {

	u16 textColour = isEnabled() ? getTextColour() : getDarkColour();
	port->drawText(_textX, _textY, getFont(), _text, 0, _text.getLength(), textColour);

	// Draw cursor
	if (_showCursor && hasFocus()) {
		port->drawFilledXORRect(getCursorXPos() + _textX, _textY, getCursorWidth(), getFont()->getHeight());
	}
}

void TextBox::drawBorder(GraphicsPort* port) {
	port->drawFilledRect(0, 0, getWidth(), getHeight(), getBackColour());

	// Stop drawing if the gadget indicates it should not have an outline
	if (isBorderless()) return;

	port->drawBevelledRect(0, 0, getWidth(), getHeight(), getShineColour(), getShadowColour());
	port->drawBevelledRect(1, 1, getWidth() - 2, getHeight() - 2, getShadowColour(), getShineColour());
}

const u16 TextBox::getCursorWidth() const {
	if (_cursorPos < _text.getLength()) {

		// Cursor within the string - get the width of the character
		return getFont()->getCharWidth(_text.getCharAt(_cursorPos));
	} else {

		// Cursor past end of string - get the width of a space
		return getFont()->getCharWidth(' ');
	}
}

const u16 TextBox::getCursorXPos() const {

	// Calculate position of cursor
	u16 cursorX = 0;

	StringIterator* iterator = _text.newStringIterator();
	
	for (u16 i = 0; i < _cursorPos; i++) {
		cursorX += getFont()->getCharWidth(iterator->getCodePoint());
			
		iterator->moveToNext();
	}
	
	delete iterator;

	return cursorX;
}

void TextBox::setText(const WoopsiString& text) {

	u16 oldWidth = getFont()->getStringWidth(_text);
	u16 oldX = _textX;
	u16 oldY = _textY;

	_text.setText(text);
	repositionCursor(_text.getLength());

	calculateTextPositionHorizontal();

	u16 newWidth = getFont()->getStringWidth(_text);
	u16 newX = _textX;
	u16 newY = _textY;

	Rect rect;
	getClientRect(rect);

	rect.x += newX < oldX ? newX : oldX;
	rect.y += newY < oldY ? newY : oldY;
	rect.width = newWidth > oldWidth ? newWidth : oldWidth;
	rect.height = getFont()->getHeight();

	// Ensure that the cursor will always get redrawn regardless of where it is
	rect.width += getCursorWidth();

	markRectDamaged(rect);

	if (raisesEvents()) {
		_gadgetEventHandler->handleValueChangeEvent(*this);
	}
}

void TextBox::appendText(const WoopsiString& text) {

	u16 oldWidth = getFont()->getStringWidth(_text);
	u16 oldX = _textX;
	u16 oldY = _textY;

	_text.append(text);
	repositionCursor(_text.getLength());

	calculateTextPositionHorizontal();

	u16 newWidth = getFont()->getStringWidth(_text);
	u16 newX = _textX;
	u16 newY = _textY;

	Rect rect;
	getClientRect(rect);

	rect.x += newX < oldX ? newX : oldX;
	rect.y += newY < oldY ? newY : oldY;
	rect.width = newWidth > oldWidth ? newWidth : oldWidth;
	rect.height = getFont()->getHeight();

	// Ensure that the cursor will always get redrawn regardless of where it is
	rect.width += getCursorWidth();

	markRectDamaged(rect);

	if (raisesEvents()) {
		_gadgetEventHandler->handleValueChangeEvent(*this);
	}
}

void TextBox::removeText(const u32 startIndex) {

	u16 oldWidth = getFont()->getStringWidth(_text);
	u16 oldX = _textX;
	u16 oldY = _textY;

	_text.remove(startIndex);
	repositionCursor(startIndex);

	calculateTextPositionHorizontal();

	u16 newWidth = getFont()->getStringWidth(_text);
	u16 newX = _textX;
	u16 newY = _textY;

	Rect rect;
	getClientRect(rect);

	rect.x += newX < oldX ? newX : oldX;
	rect.y += newY < oldY ? newY : oldY;
	rect.width = newWidth > oldWidth ? newWidth : oldWidth;
	rect.height = getFont()->getHeight();

	// Ensure that the cursor will always get redrawn regardless of where it is
	rect.width += getCursorWidth();

	markRectDamaged(rect);

	if (raisesEvents()) {
		_gadgetEventHandler->handleValueChangeEvent(*this);
	}
}

void TextBox::removeText(const u32 startIndex, const u32 count) {

	u16 oldWidth = getFont()->getStringWidth(_text);
	u16 oldX = _textX;
	u16 oldY = _textY;

	_text.remove(startIndex, count);
	repositionCursor(startIndex);

	calculateTextPositionHorizontal();

	u16 newWidth = getFont()->getStringWidth(_text);
	u16 newX = _textX;
	u16 newY = _textY;

	Rect rect;
	getClientRect(rect);

	rect.x += newX < oldX ? newX : oldX;
	rect.y += newY < oldY ? newY : oldY;
	rect.width = newWidth > oldWidth ? newWidth : oldWidth;
	rect.height = getFont()->getHeight();

	// Ensure that the cursor will always get redrawn regardless of where it is
	rect.width += getCursorWidth();

	markRectDamaged(rect);

	if (raisesEvents()) {
		_gadgetEventHandler->handleValueChangeEvent(*this);
	}
}

void TextBox::insertText(const WoopsiString& text, const u32 index) {

	u16 oldWidth = getFont()->getStringWidth(_text);
	u16 oldX = _textX;
	u16 oldY = _textY;

	_text.insert(text, index);
	repositionCursor(index + text.getLength());

	calculateTextPositionHorizontal();

	u16 newWidth = getFont()->getStringWidth(_text);
	u16 newX = _textX;
	u16 newY = _textY;

	Rect rect;
	getClientRect(rect);

	rect.x += newX < oldX ? newX : oldX;
	rect.y += newY < oldY ? newY : oldY;
	rect.width = newWidth > oldWidth ? newWidth : oldWidth;
	rect.height = getFont()->getHeight();

	// Ensure that the cursor will always get redrawn regardless of where it is
	rect.width += getCursorWidth();

	markRectDamaged(rect);

	if (raisesEvents()) {
		_gadgetEventHandler->handleValueChangeEvent(*this);
	}
}

void TextBox::insertTextAtCursor(const WoopsiString& text) {
	insertText(text, getCursorPosition());
}

void TextBox::repositionCursor(const s32 position) {
	s32 len = _text.getLength();
	_cursorPos = len >= position ? position : len;
}

void TextBox::moveCursorToPosition(const s32 position) {
	repositionCursor(position);
	calculateTextPositionHorizontal();
	markTextRectDamaged();
}

void TextBox::showCursor() {
	if (!_showCursor) {
		_showCursor = true;

		markCursorRectDamaged();
	}
}

void TextBox::hideCursor() {
	if (_showCursor) {
		_showCursor = false;

		markCursorRectDamaged();
	}
}

void TextBox::markCursorRectDamaged() {
	Rect rect;
	getClientRect(rect);

	rect.x = getCursorXPos();
	rect.y += _textY;
	rect.width = getCursorWidth();
	rect.height = getFont()->getHeight();

	markRectDamaged(rect);
}

void TextBox::moveCursorToClickLocation(s16 x, s16 y) {

	// Work out where in the string the click co-ordinates represent
	// and move the cursor to that location

	if (_text.getLength() > 0) {

		// Transform click co-ordinates to gadget-space co-ordinates
		s16 clickX = x - getX() - _borderSize.left;

		s16 charX = _textX;

		// Locate the first character that comes after the clicked character
		StringIterator* iterator = _text.newStringIterator();

		while (charX < clickX) {
			charX += getFont()->getCharWidth(iterator->getCodePoint());
			
			if (!iterator->moveToNext()) break;
		}
		
		u32 index = iterator->getIndex();

		// Move back to the clicked character if we've moved past it
		if (charX > clickX) {
			iterator->moveToPrevious();
			index = iterator->getIndex();
		} else if (charX < clickX) { 
			
			// Move past end of string if click is after the text
			index++;
		}

		moveCursorToPosition(index);

		delete iterator;
	}
}

void TextBox::onClick(s16 x, s16 y) {
	moveCursorToClickLocation(x, y);
}

void TextBox::onDoubleClick(s16 x, s16 y) {
	if (_opensKeyboard) woopsiApplication->showKeyboard(this);
}

void TextBox::onKeyPress(KeyCode keyCode) {
	if (keyCode == KEY_CODE_LEFT) {
		if (_cursorPos > 0) {
			moveCursorToPosition(_cursorPos - 1);
		}
	} else if (keyCode == KEY_CODE_RIGHT) {
		if (_cursorPos < _text.getLength()) {
			moveCursorToPosition(_cursorPos + 1);
		}
	}
}

void TextBox::onKeyRepeat(KeyCode keyCode) {
	if (keyCode == KEY_CODE_LEFT) {
		if (_cursorPos > 0) {
			moveCursorToPosition(_cursorPos - 1);
		}
	} else if (keyCode == KEY_CODE_RIGHT) {
		if (_cursorPos < _text.getLength()) {
			moveCursorToPosition(_cursorPos + 1);
		}
	}
}

void TextBox::handleKeyPressEvent(Gadget& source, const KeyCode keyCode) {
	onKeyPress(keyCode);
}

void TextBox::handleKeyRepeatEvent(Gadget& source, const KeyCode keyCode) {
	onKeyRepeat(keyCode);
}

void TextBox::calculateTextPositionHorizontal() {
	
	// Calculate the string width - if the width is longer than the box,
	// ignore alignment and align left
	u16 stringWidth = getFont()->getStringWidth(_text);
	
	// Add the width of a blank space to the width to ensure that we can
	// see the cursor
	if (_showCursor) {
		stringWidth += getFont()->getCharWidth(' ');
	}
	
	Rect rect;
	getClientRect(rect);
	
	// Use alignment options if cursor is hidden or string is smaller
	// than textbox
	if ((stringWidth < rect.width) || (!_showCursor)) {

		// Text not wider than box, so apply alignment options
		switch (_hAlignment) {
			case TEXT_ALIGNMENT_HORIZ_CENTRE:
				_textX = (rect.width - stringWidth) >> 1;
				break;
			case TEXT_ALIGNMENT_HORIZ_LEFT:
				_textX = 0;
				break;
			case TEXT_ALIGNMENT_HORIZ_RIGHT:
				_textX = rect.width - stringWidth;
				break;
		}

		return;
	}

	// Text is wider than box - view needs to follow the cursor

	// If cursor is at the end of the text, we can just right-align
	if (_cursorPos == _text.getLength()) {
		_textX = rect.width - stringWidth;
		return;
	}

	// Work out the co-ordinates of the left edge of the cursor
	s32 cursorX1 = getCursorXPos();

	// Work out the co-ordinates of the right edge of the cursor
	s32 cursorX2 = cursorX1 + getCursorWidth();

	// Ensure that the cursor is on-screen
	if (cursorX1 + _textX < 0) {

		// Cursor is off left side of screen, so adjust _textX
		_textX = 0 - cursorX1;
	} else if (cursorX2 + _textX > rect.width) {

		// Cursor is off right side of screen, so adjust _textX
		_textX = rect.width - cursorX2;
	}

	// We need to ensure that the text cannot be positioned in
	// such a way that there is a gap between the end of the
	// text and the right edge of the textbox
	if (stringWidth + _textX < rect.width) {
		_textX = rect.width - stringWidth;
	}
}

void TextBox::handleKeyboardPressEvent(WoopsiKeyboard* source, const WoopsiKey& key) {
	processKey(key);
}

void TextBox::handleKeyboardRepeatEvent(WoopsiKeyboard* source, const WoopsiKey& key) {
	processKey(key);
}

void TextBox::processKey(const WoopsiKey& key) {

	if (key.getKeyType() == WoopsiKey::KEY_BACKSPACE) {

		if (_cursorPos == 0) return;

		// Delete the character in front of the cursor
		removeText(_cursorPos - 1, 1);
	} else if (key.getKeyType() == WoopsiKey::KEY_RETURN) {

		// Close the keyboard
		woopsiApplication->hideKeyboard();

		// Fire an action event
		if (raisesEvents()) {
			_gadgetEventHandler->handleActionEvent(*this);
		}
	} else if (key.getValue() != '\0') {

		// Not modifier; append value
		insertTextAtCursor(key.getValue());
	} 
}

void TextBox::onBlur() {
	markRectsDamaged();
}

// Get the preferred dimensions of the gadget
void TextBox::getPreferredDimensions(Rect& rect) const {
	Label::getPreferredDimensions(rect);

	// Add the width of a blank space to the width to ensure that we can
	// see the cursor
	if (_showCursor) {
		rect.width += getFont()->getCharWidth(' ');
	}
}
