#include "stringiterator.h"
#include "woopsistring.h"

using namespace WoopsiUI;

StringIterator::StringIterator(const WoopsiString* string) {
	_string = string;
	_currentChar = _string->getCharArray();
	_currentIndex = 0;
}

u8 StringIterator::getCodePointSize() {
	u8 size = 0;
	
	// Return 0 if string has no data
	if (_string->getLength() == 0) return 0;

	// Return char size
	_string->getCodePoint(_currentChar, &size);
	return size;
}

void StringIterator::moveToFirst() {
	_currentChar = _string->getCharArray();
	_currentIndex = 0;
}

void StringIterator::moveToLast() {
	_currentChar = _string->getCharArray() + _string->getByteCount();
	_currentIndex = _string->getByteCount();
	
	moveToPrevious();
}

bool StringIterator::moveToNext() {
	
	if (_currentIndex < _string->getLength()) {
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
	_currentChar--;
	
	while (!_string->getCodePoint(_currentChar, NULL)) {
		_currentChar--;
		
		// Stop looking if we've reached the start of the string
		if (_currentIndex == 0) return false;
	}
	
	// Loop has ended, so we must have found a valid codepoint; we know
	// that we're looking at the previous character index
	_currentIndex--;
	
	return true;
}

void StringIterator::iterateForwardsTo(u32 index) {
	do {
		moveToNext();
	} while (index > _currentIndex);
}

void StringIterator::iterateBackwardsTo(u32 index) {
	do {
		moveToPrevious();
	} while (_currentIndex > index);
}

bool StringIterator::moveTo(u32 index) {
	
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
