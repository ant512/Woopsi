#include "woopsikey.h"
#include "graphicsport.h"

using namespace WoopsiUI;

WoopsiKey::WoopsiKey(s16 x, s16 y, u16 width, u16 height, const WoopsiString& text, const KeyType keyType, GadgetStyle* style) : StickyButton(x, y, width, height, text, style) {
	_keyType = keyType;
	_keyMode = KEY_MODE_NORMAL;

	// Create new memory for alternate strings
	_normalText = text;
	_shiftText = text;
	_controlText = text;
	_shiftControlText = text;
	_capsLockText = text;
	_controlCapsLockText = text;

	setText(text);
}

WoopsiKey::WoopsiKey(s16 x, s16 y, u16 width, u16 height, const WoopsiString& normalText, const WoopsiString& shiftText, const WoopsiString& controlText, const WoopsiString& shiftControlText, const WoopsiString& capsLockText, const WoopsiString& controlCapsLockText, const KeyType keyType, GadgetStyle* style) : StickyButton(x, y, width, height, normalText, style) {
	_keyType = keyType;
	_keyMode = KEY_MODE_NORMAL;

	// Create new memory for alternate strings
	_normalText = normalText;
	_shiftText = shiftText;
	_controlText = controlText;
	_shiftControlText = shiftControlText;
	_capsLockText = capsLockText;
	_controlCapsLockText = controlCapsLockText;

	setText(_normalText);
}

void WoopsiKey::setKeyMode(KeyMode keyMode) {
	_keyMode = keyMode;

	switch (_keyMode) {
		case KEY_MODE_NORMAL:
			setText(_normalText);
			break;
		case KEY_MODE_SHIFT:
			setText(_shiftText);
			break;
		case KEY_MODE_CONTROL:
			setText(_controlText);
			break;
		case KEY_MODE_SHIFT_CONTROL:
			setText(_shiftControlText);
			break;
		case KEY_MODE_CAPS_LOCK:
			setText(_capsLockText);
			break;
		case KEY_MODE_CONTROL_CAPS_LOCK:
			setText(_controlCapsLockText);
			break;
	}
}

const char WoopsiKey::getValue() const {
	switch (_keyType) {
		case KEY_SPACE:
			// Return a blank space for the spacebar
			return ' ';
		case KEY_ALPHA_NUMERIC_SYMBOL:
			// Return the correct char for the current key mode
			switch (_keyMode) {
				case KEY_MODE_NORMAL:
					// Return the first char in the string; we assume that
					// an alphanumeric or symbolic key has only one char displayed
					return _normalText.getCharAt(0);
				case KEY_MODE_SHIFT:
					return _shiftText.getCharAt(0);
				case KEY_MODE_CONTROL:
					return _controlText.getCharAt(0);
				case KEY_MODE_SHIFT_CONTROL:
					return _shiftControlText.getCharAt(0);
				case KEY_MODE_CAPS_LOCK:
					return _capsLockText.getCharAt(0);
				case KEY_MODE_CONTROL_CAPS_LOCK:
					return _controlCapsLockText.getCharAt(0);
			}
		case KEY_RETURN:
			// Return a line breaking character
			return 10;
		default:
			// Return null chars for modifier keys
			return '\0';
	}
}
