#include "woopsikeyboard.h"
#include "button.h"
#include "woopsikey.h"
#include "woopsi.h"
#include "woopsitimer.h"
#include "keyboardeventhandler.h"
#include "keyboardeventargs.h"

using namespace WoopsiUI;

WoopsiKeyboard::WoopsiKeyboard(s16 x, s16 y, u16 width, u16 height, const char* title, u32 flags, u32 windowFlags, GadgetStyle* style) : AmigaWindow(x, y, width, height, title, flags, windowFlags, style) {

	// Get available window region
	Rect rect;
	getClientRect(rect);

	// Create buttons
	u8 buttonWidth = 19;
	u8 buttonHeight = 20;
	u8 buttonX = rect.x + 4;
	u8 buttonY = rect.y + 1;

	// 1234567890-=
	addGadget(new WoopsiKey(buttonX, buttonY, buttonWidth, buttonHeight, "1", "!", "1", "!", "1", "1"));
	addGadget(new WoopsiKey(buttonX + (1 + buttonWidth), buttonY, buttonWidth, buttonHeight, "2", "@", "2", "@", "2", "2"));
	addGadget(new WoopsiKey(buttonX + (2 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "3", "#", "3", "#", "3", "3"));
	addGadget(new WoopsiKey(buttonX + (3 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "4", "$", "4", "$", "4", "4"));
	addGadget(new WoopsiKey(buttonX + (4 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "5", "%", "5", "%", "5", "5"));
	addGadget(new WoopsiKey(buttonX + (5 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "6", "^", "6", "^", "6", "6"));
	addGadget(new WoopsiKey(buttonX + (6 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "7", "&", "7", "&", "7", "7"));
	addGadget(new WoopsiKey(buttonX + (7 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "8", "*", "8", "*", "8", "8"));
	addGadget(new WoopsiKey(buttonX + (8 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "9", "(", "9", "(", "9", "9"));
	addGadget(new WoopsiKey(buttonX + (9 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "0", ")", "0", ")", "0", "0"));
	addGadget(new WoopsiKey(buttonX + (10 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "-", "_", "-", "_", "-", "-"));
	addGadget(new WoopsiKey(buttonX + (11 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "=", "+", "=", "+", "=", "="));
	
	buttonY += buttonHeight + 1;

	// QWERTYUIOP Space
	addGadget(new WoopsiKey(buttonX + (buttonWidth / 2), buttonY, buttonWidth, buttonHeight, "q", "Q", "q", "Q", "Q", "Q"));
	addGadget(new WoopsiKey(buttonX + (buttonWidth / 2) + (1 + buttonWidth), buttonY, buttonWidth, buttonHeight, "w", "W", "w", "W", "W", "W"));
	addGadget(new WoopsiKey(buttonX + (buttonWidth / 2) + (2 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "e", "E", "e", "E", "E", "E"));
	addGadget(new WoopsiKey(buttonX + (buttonWidth / 2) + (3 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "r", "R", "r", "R", "R", "R"));
	addGadget(new WoopsiKey(buttonX + (buttonWidth / 2) + (4 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "t", "T", "t", "T", "T", "T"));
	addGadget(new WoopsiKey(buttonX + (buttonWidth / 2) + (5 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "y", "Y", "y", "Y", "Y", "Y"));
	addGadget(new WoopsiKey(buttonX + (buttonWidth / 2) + (6 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "u", "U", "u", "U", "U", "U"));
	addGadget(new WoopsiKey(buttonX + (buttonWidth / 2) + (7 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "i", "I", "i", "I", "I", "I"));
	addGadget(new WoopsiKey(buttonX + (buttonWidth / 2) + (8 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "o", "O", "o", "O", "O", "O"));
	addGadget(new WoopsiKey(buttonX + (buttonWidth / 2) + (9 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "p", "P", "p", "P", "P", "P"));
	addGadget(new WoopsiKey(buttonX + (buttonWidth / 2) + (10 * (1 + buttonWidth)), buttonY, 1 + (buttonWidth * 2) - (buttonWidth / 2), buttonHeight, GLYPH_BACKSPACE, WoopsiKey::KEY_BACKSPACE));

	buttonY += buttonHeight + 1;

	// Caps ASDFGHJKL Return
	_capsLockKey = new WoopsiKey(buttonX, buttonY, buttonWidth, buttonHeight, GLYPH_CAPS_LOCK, WoopsiKey::KEY_CAPS_LOCK);
	addGadget(_capsLockKey);
	addGadget(new WoopsiKey(buttonX + (1 + buttonWidth), buttonY, buttonWidth, buttonHeight, "a", "A", "a", "A", "A", "A"));
	addGadget(new WoopsiKey(buttonX + (2 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "s", "S", "s", "S", "S", "S"));
	addGadget(new WoopsiKey(buttonX + (3 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "d", "D", "d", "D", "D", "D"));
	addGadget(new WoopsiKey(buttonX + (4 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "f", "F", "f", "F", "F", "F"));
	addGadget(new WoopsiKey(buttonX + (5 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "g", "G", "g", "G", "G", "G"));
	addGadget(new WoopsiKey(buttonX + (6 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "h", "H", "h", "H", "H", "H"));
	addGadget(new WoopsiKey(buttonX + (7 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "j", "J", "j", "J", "J", "J"));
	addGadget(new WoopsiKey(buttonX + (8 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "k", "K", "k", "K", "K", "K"));
	addGadget(new WoopsiKey(buttonX + (9 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "l", "L", "l", "L", "L", "L"));
	addGadget(new WoopsiKey(buttonX + (10 * (1 + buttonWidth)), buttonY, 1 + (buttonWidth * 2), buttonHeight, GLYPH_RETURN, WoopsiKey::KEY_RETURN));

	buttonY += buttonHeight + 1;

	// Shift ZXCVBNM,./
	_shiftKey = new WoopsiKey(buttonX, buttonY, (buttonWidth * 2) - (buttonWidth / 2) - 1, buttonHeight, GLYPH_SHIFT, WoopsiKey::KEY_SHIFT);
	addGadget(_shiftKey);
	addGadget(new WoopsiKey(buttonX + (buttonWidth / 2) + (1 + buttonWidth), buttonY, buttonWidth, buttonHeight, "z", "Z", "z", "Z", "Z", "Z"));
	addGadget(new WoopsiKey(buttonX + (buttonWidth / 2) + (2 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "x", "X", "x", "X", "X", "X"));
	addGadget(new WoopsiKey(buttonX + (buttonWidth / 2) + (3 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "c", "C", "c", "C", "C", "C"));
	addGadget(new WoopsiKey(buttonX + (buttonWidth / 2) + (4 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "v", "V", "v", "V", "V", "V"));
	addGadget(new WoopsiKey(buttonX + (buttonWidth / 2) + (5 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "b", "B", "b", "B", "B", "B"));
	addGadget(new WoopsiKey(buttonX + (buttonWidth / 2) + (6 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "n", "N", "n", "N", "N", "N"));
	addGadget(new WoopsiKey(buttonX + (buttonWidth / 2) + (7 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "m", "M", "m", "M", "M", "M"));
	addGadget(new WoopsiKey(buttonX + (buttonWidth / 2) + (8 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, ",", "<", ",", "<", ",", ","));
	addGadget(new WoopsiKey(buttonX + (buttonWidth / 2) + (9 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, ".", ">", ".", ">", ".", "."));
	addGadget(new WoopsiKey(buttonX + (buttonWidth / 2) + (10 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "/", "?", "/", "?", "/", "/"));

	buttonY += buttonHeight + 1;

	// Ctrl ;'# Space [] backslash
	_controlKey = new WoopsiKey(buttonX, buttonY, buttonWidth, buttonHeight, GLYPH_CONTROL, WoopsiKey::KEY_CONTROL);
	addGadget(_controlKey);
	addGadget(new WoopsiKey(buttonX + (1 + buttonWidth), buttonY, buttonWidth, buttonHeight, "`", "~", "`", "~", "`", "`"));
	addGadget(new WoopsiKey(buttonX + (2 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, ";", ":", ";", ":", ";", ";"));
	addGadget(new WoopsiKey(buttonX + (3 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "'", "\"", "'", "\"", "'", "'"));
	addGadget(new WoopsiKey(buttonX + (4 * (1 + buttonWidth)), buttonY, 4 + (buttonWidth * 5), buttonHeight, "Space", WoopsiKey::KEY_SPACE));
	addGadget(new WoopsiKey(buttonX + (9 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "[", "{", "[", "{", "[", "["));
	addGadget(new WoopsiKey(buttonX + (10 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "]", "}", "]", "}", "]", "]"));
	addGadget(new WoopsiKey(buttonX + (11 * (1 + buttonWidth)), buttonY, buttonWidth, buttonHeight, "\\", "|", "\\", "|", "\\", "\\"));

	// Create the timer
	_initialRepeatTime = 25;
	_secondaryRepeatTime = 5;
	_timer = new WoopsiTimer(_initialRepeatTime, true);
	addGadget(_timer);

	// Set event handlers
	for (s32 i = getDecorationCount(); i < _gadgets.size(); i++) {
		_gadgets[i]->addGadgetEventHandler(this);
	}

	// Set initial modifier state
	_isShiftDown = false;
	_isCapsLockDown = false;
	_isControlDown = false;
}

void WoopsiKeyboard::handleActionEvent(const GadgetEventArgs& e) {

	if (e.getSource() != NULL) {

		// Check if the event was fired by the timer (key repeat)
		if (e.getSource() == _timer) {

			// Event is a key repeat, so raise repeat event
			raiseKeyboardRepeatEvent((WoopsiKey*)getFocusedGadget());

			// Ensure that subsequent repeats are faster
			_timer->setTimeout(_secondaryRepeatTime);

			return;
		}
	}

	AmigaWindow::handleActionEvent(e);
}

void WoopsiKeyboard::processKeyRelease(WoopsiKey* key) {

	// When a key is released, we need to restore the shift and
	// control keys back to their released state if they are
	// currently held.  Can't do this as part of the click event
	// because key repeats won't work in that situation.

	switch (key->getKeyType()) {
		case WoopsiKey::KEY_ALPHA_NUMERIC_SYMBOL:
		case WoopsiKey::KEY_BACKSPACE:
		case WoopsiKey::KEY_RETURN:
		case WoopsiKey::KEY_NONE:
		case WoopsiKey::KEY_SPACE:

			// Swap key modes
			if (_isShiftDown || _isControlDown) {
				// Reset shift key
				if (_isShiftDown) {
					_isShiftDown = false;
					_shiftKey->setOutlineType(Gadget::OUTLINE_CLICK_DEPENDENT);
					_shiftKey->redraw();
				}

				// Reset control key
				if (_isControlDown) {
					_isControlDown = false;
					_controlKey->setOutlineType(Gadget::OUTLINE_CLICK_DEPENDENT);
					_controlKey->redraw();
				}

				// Update the keyboard
				showCorrectKeys();
			}
			break;
		default:
			// Do nothing if other keys are released
			break;
	}
}

void WoopsiKeyboard::handleReleaseEvent(const GadgetEventArgs& e) {

	if (e.getSource() != NULL) {
		if (!e.getSource()->isDecoration()) {

			// Gadget not a decoration and not a timer, so must be a key
			WoopsiKey* key = (WoopsiKey*)e.getSource();

			raiseKeyboardReleaseEvent(key);

			processKeyRelease(key);

			// Stop the timer
			_timer->stop();

			return;
		}
	}

	AmigaWindow::handleReleaseEvent(e);
}

void WoopsiKeyboard::handleReleaseOutsideEvent(const GadgetEventArgs& e) {

	if (e.getSource() != NULL) {
		if (!e.getSource()->isDecoration()) {

			// Gadget not a decoration and not a timer, so must be a key
			WoopsiKey* key = (WoopsiKey*)e.getSource();

			raiseKeyboardReleaseEvent(key);

			processKeyRelease(key);

			// Stop the timer
			_timer->stop();

			return;
		}
	}

	AmigaWindow::handleReleaseOutsideEvent(e);
}

void WoopsiKeyboard::handleClickEvent(const GadgetEventArgs& e) {
		
	if (e.getSource() != NULL) {
		if (!e.getSource()->isDecoration()) {

			// Gadget not a decoration and not a timer, so must be a key
			WoopsiKey* key = (WoopsiKey*)e.getSource();

			// Inform the keyboard's keyboard event handlers that a key has been pressed
			raiseKeyboardPressEvent(key);

			// Process the key after the handler has dealt with it and update
			// the keyboard accordingly.  We do this after the handler because
			// we want to ensure that the keyboard state (ie. text on the buttons)
			// doesn't change before the handler has used this info.
			switch (key->getKeyType()) {
				case WoopsiKey::KEY_CAPS_LOCK:

					// Set the outline type so the key is obviously stuck down,
					// or reset it if the key is being clicked for the second time
					if (_isCapsLockDown) {
						_capsLockKey->setOutlineType(Gadget::OUTLINE_CLICK_DEPENDENT);
					} else {
						_capsLockKey->setOutlineType(Gadget::OUTLINE_IN);
					}

					// Remember the key's state
					_isCapsLockDown = !_isCapsLockDown;

					// Update the keyboard
					showCorrectKeys();
					break;
				case WoopsiKey::KEY_CONTROL:

					// Set the outline type so the key is obviously stuck down,
					// or reset it if the key is being clicked for the second time
					if (_isControlDown) {
						_controlKey->setOutlineType(Gadget::OUTLINE_CLICK_DEPENDENT);
					} else {
						_controlKey->setOutlineType(Gadget::OUTLINE_IN);
					}

					// Remember the key's state
					_isControlDown = !_isControlDown;

					// Update the keyboard
					showCorrectKeys();
					break;
				case WoopsiKey::KEY_SHIFT:

					// Set the outline type so the key is obviously stuck down,
					// or reset it if the key is being clicked for the second time
					if (_isShiftDown) {
						_shiftKey->setOutlineType(Gadget::OUTLINE_CLICK_DEPENDENT);
					} else {
						_shiftKey->setOutlineType(Gadget::OUTLINE_IN);
					}

					// Remember the key's state
					_isShiftDown = !_isShiftDown;

					// Update the keyboard
					showCorrectKeys();
					break;
				default:

					// Start the timer
					_timer->setTimeout(_initialRepeatTime);
					_timer->start();
					break;
			}

			return;
		}
	}

	AmigaWindow::handleClickEvent(e);
}

void WoopsiKeyboard::showCorrectKeys() {
	if (_isShiftDown && _isControlDown) {
		showShiftControlKeys();
	} else if (_isCapsLockDown && _isControlDown) {
		showControlCapsLockKeys();
	} else if (_isShiftDown) {
		showShiftKeys();
	} else if (_isCapsLockDown) {
		showCapsLockKeys();
	} else if (_isControlDown) {
		showControlKeys();
	} else {
		showNormalKeys();
	}
}

void WoopsiKeyboard::showNormalKeys() {
	for (s32 i = _decorationCount; i < _gadgets.size(); i++) {
		if (_gadgets[i] != _timer) {
			((WoopsiKey*)_gadgets[i])->setKeyMode(WoopsiKey::KEY_MODE_NORMAL);
		}
	}
}

void WoopsiKeyboard::showShiftKeys() {
	for (s32 i = _decorationCount; i < _gadgets.size(); i++) {
		if (_gadgets[i] != _timer) {
			((WoopsiKey*)_gadgets[i])->setKeyMode(WoopsiKey::KEY_MODE_SHIFT);
		}
	}
}

void WoopsiKeyboard::showControlKeys() {
	for (s32 i = _decorationCount; i < _gadgets.size(); i++) {
		if (_gadgets[i] != _timer) {
			((WoopsiKey*)_gadgets[i])->setKeyMode(WoopsiKey::KEY_MODE_CONTROL);
		}
	}
}

void WoopsiKeyboard::showShiftControlKeys() {
	for (s32 i = _decorationCount; i < _gadgets.size(); i++) {
		if (_gadgets[i] != _timer) {
			((WoopsiKey*)_gadgets[i])->setKeyMode(WoopsiKey::KEY_MODE_SHIFT_CONTROL);
		}
	}
}

void WoopsiKeyboard::showCapsLockKeys() {
	for (s32 i = _decorationCount; i < _gadgets.size(); i++) {
		if (_gadgets[i] != _timer) {
			((WoopsiKey*)_gadgets[i])->setKeyMode(WoopsiKey::KEY_MODE_CAPS_LOCK);
		}
	}
}

void WoopsiKeyboard::showControlCapsLockKeys() {
	for (s32 i = _decorationCount; i < _gadgets.size(); i++) {
		if (_gadgets[i] != _timer) {
			((WoopsiKey*)_gadgets[i])->setKeyMode(WoopsiKey::KEY_MODE_CONTROL_CAPS_LOCK);
		}
	}
}

void WoopsiKeyboard::removeKeyboardEventHandler(KeyboardEventHandler* eventHandler) {
	for (s32 i = 0; i < _keyboardEventHandlers.size(); ++i) {
		if (_keyboardEventHandlers.at(i) == eventHandler) {
			_keyboardEventHandlers.erase(i);
			return;
		}
	}
}

void WoopsiKeyboard::raiseKeyboardPressEvent(WoopsiKey* key) {
	if (raisesEvents()) {
		KeyboardEventArgs e(this, key);

		for (int i = 0; i < _keyboardEventHandlers.size(); ++i) {
			_keyboardEventHandlers.at(i)->handleKeyboardPressEvent(e);
		}
	}
}

void WoopsiKeyboard::raiseKeyboardRepeatEvent(WoopsiKey* key) {
	if (raisesEvents()) {
		KeyboardEventArgs e(this, key);

		for (int i = 0; i < _keyboardEventHandlers.size(); ++i) {
			_keyboardEventHandlers.at(i)->handleKeyboardRepeatEvent(e);
		}
	}
}

void WoopsiKeyboard::raiseKeyboardReleaseEvent(WoopsiKey* key) {
	if (raisesEvents()) {
		KeyboardEventArgs e(this, key);

		for (int i = 0; i < _keyboardEventHandlers.size(); ++i) {
			_keyboardEventHandlers.at(i)->handleKeyboardReleaseEvent(e);
		}
	}
}
