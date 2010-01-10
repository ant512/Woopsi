#include "stringiterator.h"
#include "woopsistring.h"

using namespace WoopsiUI;

StringIterator::StringIterator(const WoopsiString* string) {
	_string = string;
	_currentChar = _string->getCharArray();
	_currentIndex = 0;
}

u8 StringIterator::getCurrentCharSize() {
	u8 size = 0;
	
	// Return 0 if string has no data
	if (_string->getLength() == 0) return 0;

	// Return char size
	_string->getCodePoint(_currentChar, &size);
	return size;
}

void StringIterator::moveToFirst() {
	moveTo(0);
}

void StringIterator::moveToLast() {
	_currentChar = _string->getCharArray() + _string->getByteCount();
	_currentIndex = _string->getByteCount();
	
	moveToPrevious();
}

bool StringIterator::moveToNext() {
	
	if (_currentIndex < _string->getLength()) {
		_currentChar += getCurrentCharSize();
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

bool StringIterator::moveTo(u32 index) {
	
	// This could probably be optimised by deciding whether to start at the
	// frnt or the end depending on whether the index is greater than
	// half of the string size
	
	// Early exit if the index exceeds the size of the string
	if (index > _string->getLength()) return false;
	
	_currentChar = _string->getToken(index);
	_currentIndex = index;

	return true;
}

u32 StringIterator::getCodePoint(u8* numBytes) const {
	return _string->getCodePoint(_currentChar, numBytes);
}
