#include "woopsikey.h"

using namespace WoopsiUI;

WoopsiKey::WoopsiKey(s16 x, s16 y, u16 width, u16 height, const char* text, const KeyType keyType, FontBase* font) : Button(x, y, width, height, text, font) {
	_keyType = keyType;
	_keyMode = KEY_MODE_NORMAL;

	// Create new memory for alternate strings
	_normalText = new char[strlen(text) + 1];
	_shiftText = new char[strlen(text) + 1];
	_controlText = new char[strlen(text) + 1];
	_shiftControlText = new char[strlen(text) + 1];
	_capsLockText = new char[strlen(text) + 1];
	_controlCapsLockText = new char[strlen(text) + 1];

	// Copy text
	strcpy(_normalText, text);
	strcpy(_shiftText, text);
	strcpy(_controlText, text);
	strcpy(_shiftControlText, text);
	strcpy(_capsLockText, text);
	strcpy(_controlCapsLockText, text);
}

WoopsiKey::WoopsiKey(s16 x, s16 y, u16 width, u16 height, const char* normalText, const char* shiftText, const char* controlText, const char* shiftControlText, const char* capsLockText, const char* controlCapsLockText, const KeyType keyType, FontBase* font) : Button(x, y, width, height, normalText, font) {
	_keyType = keyType;
	_keyMode = KEY_MODE_NORMAL;

	// Create new memory for alternate strings
	_normalText = new char[strlen(normalText) + 1];
	_shiftText = new char[strlen(shiftText) + 1];
	_controlText = new char[strlen(controlText) + 1];
	_shiftControlText = new char[strlen(shiftControlText) + 1];
	_capsLockText = new char[strlen(capsLockText) + 1];
	_controlCapsLockText = new char[strlen(controlCapsLockText) + 1];

	// Copy text
	strcpy(_normalText, normalText);
	strcpy(_shiftText, shiftText);
	strcpy(_controlText, controlText);
	strcpy(_shiftControlText, shiftControlText);
	strcpy(_capsLockText, capsLockText);
	strcpy(_controlCapsLockText, controlCapsLockText);
}

WoopsiKey::WoopsiKey(s16 x, s16 y, u16 width, u16 height, const char glyph, const KeyType keyType, FontBase* font) : Button(x, y, width, height, glyph, font) {
	_keyType = keyType;
	_keyMode = KEY_MODE_NORMAL;

	_normalText = new char[2];
	_shiftText = new char[2];
	_controlText = new char[2];
	_shiftControlText = new char[2];
	_capsLockText = new char[2];
	_controlCapsLockText = new char[2];

	_normalText[0] = glyph;
	_normalText[1] = '\0';

	strcpy(_shiftText, _normalText);
	strcpy(_controlText, _normalText);
	strcpy(_shiftControlText, _normalText);
	strcpy(_capsLockText, _normalText);
	strcpy(_controlCapsLockText, _normalText);

	// Show the default text
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
					return _normalText[0];
				case KEY_MODE_SHIFT:
					return _shiftText[0];
				case KEY_MODE_CONTROL:
					return _controlText[0];
				case KEY_MODE_SHIFT_CONTROL:
					return _shiftControlText[0];
				case KEY_MODE_CAPS_LOCK:
					return _capsLockText[0];
				case KEY_MODE_CONTROL_CAPS_LOCK:
					return _controlCapsLockText[0];
			}
		case KEY_RETURN:
			// Return a line breaking character
			return 10;
		default:
			// Return null chars for modifier keys
			return '\0';
	}
}
