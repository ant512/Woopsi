#include "textbox.h"
#include "graphicsport.h"
#include "woopsitimer.h"
#include "stringiterator.h"

using namespace WoopsiUI;

TextBox::TextBox(s16 x, s16 y, u16 width, u16 height, const WoopsiString& text, GadgetStyle* style) : Label(x, y, width, height, text, style) {
	_cursorPos = 0;
	_firstCharIndex = 0;
	_showCursor = true;
	setOutlineType(OUTLINE_OUT_IN);
	moveCursorToPosition(_text.getLength());

	// Create the timer
	_initialRepeatTime = KEY_INITIAL_REPEAT_TIME;
	_secondaryRepeatTime = KEY_SECONDARY_REPEAT_TIME;
	_timer = new WoopsiTimer(_initialRepeatTime, true);
	_timer->addGadgetEventHandler(this);
	addGadget(_timer);

	_heldDirection = KEY_CODE_NONE;
}

void TextBox::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	port->drawFilledRect(0, 0, _width, _height, getBackColour());

	if (isEnabled()) {
		port->drawText(_textX, _textY, getFont(), _text, _firstCharIndex, _text.getLength() - _firstCharIndex);
	} else {
		port->drawText(_textX, _textY, getFont(), _text, _firstCharIndex, _text.getLength() - _firstCharIndex, getDarkColour());
	}

	// Draw cursor
	if (_showCursor) {
		if (_cursorPos < _text.getLength()) {

			// Cursor within the string - draw over the character
			port->drawFilledXORRect(getCursorXPos(), _textY, getFont()->getCharWidth(_text.getCharAt(_cursorPos)), getFont()->getHeight());
		} else {

			// Cursor past end of string - draw a space
			port->drawFilledXORRect(getCursorXPos(), _textY, getFont()->getCharWidth(' '), getFont()->getHeight());
		}
	}

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;
}

const u16 TextBox::getCursorXPos() const {

	// Calculate position of cursor
	u16 cursorX = _textX;

	StringIterator* iterator = _text.newStringIterator();
	
	if (iterator->moveTo(_firstCharIndex)) {
	
		for (u16 i = _firstCharIndex; i < _cursorPos; i++) {
			cursorX += getFont()->getCharWidth(iterator->getCodePoint());
			
			iterator->moveToNext();
		}
	}
	
	delete iterator;

	return cursorX;
}

void TextBox::setText(const WoopsiString& text) {
	_text.setText(text);
	moveCursorToPosition(_text.getLength());
	calculateTextPositionHorizontal();
	calculateTextPositionVertical();
	redraw();
	_gadgetEventHandlers->raiseValueChangeEvent();
}

void TextBox::appendText(const WoopsiString& text) {
	_text.append(text);
	moveCursorToPosition(_text.getLength());
	calculateTextPositionHorizontal();
	calculateTextPositionVertical();
	redraw();
	_gadgetEventHandlers->raiseValueChangeEvent();
}

void TextBox::insertText(const WoopsiString& text, const u32 index) {
	// Get current text length - use this later to quickly get the length
	// of the inserted string to shift the cursor around
	u32 oldLen = _text.getLength();

	_text.insert(text, index);
	
	// Get the new string length and use it to calculate the length
	// of the inserted string
	u32 insertLen = _text.getLength() - oldLen;

	moveCursorToPosition(index + insertLen);
	calculateTextPositionHorizontal();
	calculateTextPositionVertical();
	redraw();
	_gadgetEventHandlers->raiseValueChangeEvent();
}

void TextBox::insertTextAtCursor(const WoopsiString& text) {
	insertText(text, getCursorPosition());
}

void TextBox::moveCursorToPosition(const u32 position) {
	u32 len = _text.getLength();
	_cursorPos = len >= position ? position : len;
	
	calculateFirstCharIndex();

	redraw();
}

void TextBox::showCursor() {
	if (!_showCursor) {
		_showCursor = true;
		redraw();
	}
}

void TextBox::hideCursor() {
	if (_showCursor) {
		_showCursor = false;
		redraw();
	}
}

void TextBox::calculateFirstCharIndex() {
	
	// Ensure firstCharIndex is not greater than the cursor pos.
	// Can exit early if this occurs, as it means that the cursor has moved to
	// the left
	if (_cursorPos < _firstCharIndex) {
		_firstCharIndex = _cursorPos;
		return;
	}
	
	// Calculate the available client space
	Rect rect;
	getClientRect(rect);
	
	// Calculate the width of the cursor
	u16 cursorWidth = 0;
	
	if (_cursorPos >= _text.getLength()) {
		cursorWidth = getFont()->getCharWidth(' ');
	} else {
		cursorWidth = getFont()->getCharWidth(_text.getCharAt(_cursorPos));
	}
	
	// Calculate the width of the visible string up to the cursor
	u16 stringToCursorWidth = getFont()->getStringWidth(_text, _firstCharIndex, _cursorPos - _firstCharIndex);
	
	// Early exit if cursor is on screen
	if (stringToCursorWidth + cursorWidth < rect.width) return;
	
	StringIterator* iterator = _text.newStringIterator();
	iterator->moveTo(_firstCharIndex);
	
	// Cursor is off right side of text box, so increase start character until 
	// the cursor is back on screen
	while (stringToCursorWidth + cursorWidth > rect.width) {
		stringToCursorWidth -= getFont()->getCharWidth(iterator->getCodePoint());
		_firstCharIndex++;
		
		if (!iterator->moveToNext()) break;
	}
		
	delete iterator;
}

// Client rect is 1px smaller than usual as the border is 2 pixels thick
void TextBox::getClientRect(Rect& rect) const {
	if (!_flags.borderless) {
		rect.x = 2;
		rect.y = 2;
		rect.width = _width - 4;
		rect.height = _height - 4;
	} else {
		rect.x = 0;
		rect.y = 0;
		rect.width = _width;
		rect.height = _height;
	}
}

bool TextBox::click(s16 x, s16 y) {
	if (Gadget::click(x, y)) {

		if (isEnabled()) {
			// Work out where in the string the textbox was clicked and move the cursor to that
			// location

			if (_text.getLength() > 0) {
				s16 clickX = x - getX();
				s16 charX = _textX;

				// Locate the first character that comes after the clicked character
				StringIterator* iterator = _text.newStringIterator();
				iterator->moveTo(_firstCharIndex);

				while (charX < clickX) {
					charX += getFont()->getCharWidth(iterator->getCodePoint());
					
					if (!iterator->moveToNext()) break;
				}

				// Move back to the clicked character if we've moved past it
				if (charX > clickX) {
					iterator->moveToPrevious();
				}

				moveCursorToPosition(iterator->getIndex());

				delete iterator;
			}
		}

		return true;
	}

	return false;
}

bool TextBox::keyPress(KeyCode keyCode) {
	if (Gadget::keyPress(keyCode)) {
		if (keyCode == KEY_CODE_LEFT) {
			if (_cursorPos > 0) {
				moveCursorToPosition(_cursorPos - 1);
				
				_heldDirection = KEY_CODE_LEFT;

				// Start the timer
				_timer->setTimeout(_initialRepeatTime);
				_timer->start();
			}
		} else if (keyCode == KEY_CODE_RIGHT) {
			if (_cursorPos < _text.getLength()) {
				moveCursorToPosition(_cursorPos + 1);
				
				_heldDirection = KEY_CODE_RIGHT;

				// Start the timer
				_timer->setTimeout(_initialRepeatTime);
				_timer->start();
			}
		}

		return true;
	}

	return false;
}

bool TextBox::keyRelease(KeyCode keyCode) {
	if (Gadget::keyRelease(keyCode)) {
		if (_heldDirection == keyCode) {

			// Forget the previously-held key
			_heldDirection = KEY_CODE_NONE;
			_timer->stop();
		}

		return true;
	}

	return false;
}

void TextBox::handleActionEvent(const GadgetEventArgs& e) {

	if (e.getSource() != NULL) {

		// Check if the event was fired by the timer (key repeat)
		if (e.getSource() == _timer) {

			// Event is a key repeat - move the cursor
			if (_heldDirection == KEY_CODE_LEFT) {
				if (_cursorPos > 0) {
					moveCursorToPosition(_cursorPos - 1);
				}
			} else if (_heldDirection == KEY_CODE_RIGHT) {
				if (_cursorPos < _text.getLength()) {
					moveCursorToPosition(_cursorPos + 1);
				}
			}
			
			// Ensure that subsequent repeats are faster
			_timer->setTimeout(_secondaryRepeatTime);

			return;
		}
	}
}

void TextBox::calculateTextPositionHorizontal() {
	
	// Calculate the string width - if the width is longer than the box,
	// ignore alignment and align left
	u16 stringWidth = getFont()->getStringWidth(_text);
	
	Rect rect;
	getClientRect(rect);
	
	if (stringWidth > rect.width) {
		_textX = _padding;
		return;
	}
	
	// Text not wider than box, so apply alignment options
	switch (_hAlignment) {
		case TEXT_ALIGNMENT_HORIZ_CENTRE:
			_textX = (_width - stringWidth) >> 1;
			break;
		case TEXT_ALIGNMENT_HORIZ_LEFT:
			_textX = _padding;
			break;
		case TEXT_ALIGNMENT_HORIZ_RIGHT:
			_textX = _width - stringWidth - _padding;
			break;
	}
}
