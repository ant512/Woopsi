#include <ctype.h>
#include "stringiterator.h"
#include "woopsistring.h"

using namespace WoopsiUI;

StringIterator::StringIterator(const WoopsiString* string) {
	_string = string;
	_currentChar = _string->getCharArray();
	_currentIndex = 0;
}

void StringIterator::moveToFirst() {
	_currentChar = _string->getCharArray();
	_currentIndex = 0;
}

u8 StringIterator::getCodePointSize() {
	
	// Return 0 if string has no data
	if (_string->getLength() == 0) return 0;
	
	char value=*_currentChar;
	if (value<0x80) return 1;  
	if (value<0xC2) return 0; // Can't be a leading char
	if (value<0xE0) return 2;
	if (value<0xF0) return 3;
	if (value<0xF4) return 4; // Doesn't have legal unicode leading char after that
	return 0;
}

void StringIterator::moveToLast() {
	
	if (_string->getLength() > 0) {
		_currentChar = _string->getCharArray() + _string->getByteCount() - 1; 

		while ((*_currentChar >= 0x80) && (*_currentChar < 0xC0)) _currentChar--;
		
		// String has been filtered before; no need to check if value >=0xF4
		_currentIndex = _string->getLength()-1;
	}
}

bool StringIterator::moveToNext() {
	
	if (_currentIndex < _string->getLength() - 1) {
		_currentChar += getCodePointSize();
		_currentIndex++;
		
		return true;
	}
	
	return false;
}

bool StringIterator::moveToPrevious() {
	
	// Abort if already at the start of the string
	if (_currentIndex == 0) return false;
	
	// Move back one char to ensure we're in the middle of a char sequence
	do {
		_currentChar--;
	} while ((*_currentChar >= 0x80) && (*_currentChar < 0xC0));   
	
	// Loop has ended, so we must have found a valid codepoint; we know
	// that we're looking at the previous character index
	_currentIndex--;
	
	return true;
}

void StringIterator::iterateForwardsTo(s32 index) {
	do {
		moveToNext();
	} while (index > _currentIndex);
}

void StringIterator::iterateBackwardsTo(s32 index) {
	do {
		moveToPrevious();
	} while (_currentIndex > index);
}

bool StringIterator::moveTo(s32 index) {

	// Abort if index makes no sense
	if (index < 0) return false;
	
	// Abort if index exceeds the size of the string
	if (index >= _string->getLength()) return false;

	// Abort if new index matches current index
	if (index == _currentIndex) return true;

	// Move to end if requested index is at end of string
	if (index == _string->getLength() - 1) {
		moveToLast();
		return true;
	}

	// Move to start if requested index is 0
	if (index == 0) {
		moveToFirst();
		return true;
	}

	// Decide if it is faster to iterate over the string from the current point
	// or from the front or back
	if (index > _currentIndex) {

		// Requested index is past current point
		// Calculate distance to the requested index from the current point
		u32 distanceFromHere = index - _currentIndex;
		u32 distanceFromEnd = _string->getLength() - index - 1;

		if (distanceFromHere <= distanceFromEnd) {

			// Shorter distance from current point to the requested index, so
			// scan through string from this point forwards
			iterateForwardsTo(index);
			return true;
		} else {

			// Shorter distance from end to the requested index, so
			// jump to end of string and scan through string backwards
			moveToLast();
			iterateBackwardsTo(index);
			return true;
		}
	} else {

		// Requested index is before current point
		// Calculate distance to the requested index from the current point
		u32 distanceFromHere = _currentIndex - index;
		u32 distanceFromStart = index;

		if (distanceFromHere <= distanceFromStart) {

			// Shorter distance from current point to the requested index, so
			// scan through string from this point backwards
			iterateBackwardsTo(index);
			return true;
		} else {

			// Shorter distance from start to the requested index, so
			// jump to start of string and scan through string forwards
			moveToFirst();
			iterateForwardsTo(index);
			return true;
		}
	}

	// Should never reach this
	return false;
}

u32 StringIterator::getCodePoint() const {
	return _string->getCodePoint(_currentChar, NULL);
}

u32 StringIterator::getInteger(u32* charCount) {

	// strtoul() will discard any white space we might currently be looking at
	// which isn't the desired behaviour.  We prevent this by checking that
	// we're looking at a digit before we start
	if (isdigit(getCodePoint())) {

		char *end = NULL;
		u32 digits = strtoul(_currentChar, &end, 10);

		if (charCount != NULL) *charCount = (u32)(end - _currentChar);

		return digits;
	} else {

		if (charCount != NULL) *charCount = 0;
		return 0;
	}
}
