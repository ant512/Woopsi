#include <string.h>
#include "woopsistring.h"

using namespace WoopsiUI;

WoopsiString::WoopsiString() {
	_text = NULL;
	_length = 0;
	_allocatedSize = 0;
	_growAmount = 32;
}

WoopsiString::WoopsiString(const char* text) {
	_text = NULL;
	_length = 0;
	_allocatedSize = 0;
	_growAmount = 32;

	setText(text);
}

WoopsiString::WoopsiString(const char text) {
	_text = NULL;
	_length = 0;
	_allocatedSize = 0;
	_growAmount = 32;

	setText(text);
}

void WoopsiString::setText(const char* text) {

	u32 newLen = strlen(text);
	
	// Ensure we've got enough memory available
	allocateMemory(newLen + 1, false);

	// Cache the string length
	_length = newLen;

	// Copy text
	strcpy(_text, text);
}

void WoopsiString::setText(const char text) {

	char* newText = new char[2];
	newText[0] = text;
	newText[1] = '\0';

	setText(newText);

	delete[] newText;
}

void WoopsiString::append(const char* text) {

	if (hasData()) {

		// Cache string length
		_length += strlen(text);

		// Ensure we've got enough storage for the concatenated string
		allocateMemory(_length + 1, true);

		// Concatenate strings
		strcat(_text, text);
	} else {
		// No text, so set it for the first time
		setText(text);
	}
}

void WoopsiString::append(const char text) {

	char* newText = new char[2];
	newText[0] = text;
	newText[1] = '\0';

	append(newText);

	delete[] newText;
}

void WoopsiString::insert(const char* text, u32 index) {

	// Bounds check index
	if (index > _length) index = _length;

	if (hasData()) {

		u32 oldLen = _length;
		u32 insertLen = strlen(text);

		_length = oldLen + insertLen;

		// Attempt to use the existing memory without reallocating.
		// Can't use the standard allocateMemory() function efficiently
		// because we're modifying, not just preserving, the existing string
		if (_length + 1 > _allocatedSize) {

			// Remember new allocated size
			_allocatedSize = _length + 1 + _growAmount;

			// Reserve memory for new string
			char* newText = new char[_allocatedSize];

			// Copy start of existing string into new string
			strncpy(newText, _text, index);

			// Copy insert into new string
			strncpy(newText + index, text, insertLen);

			// Copy end of existing string into new string
			strcpy(newText + index + insertLen, _text + index);

			// Delete existing string
			delete[] _text;

			// Swap pointers
			_text = newText;
		} else {

			// Allocated memory is sufficiently large enough to store
			// existing and new text

			if (_length > 0) {
			
				// Copy end section of string to new location in reverse,
				// so no data is overwritten before it is copied
				char* dest = _text + _length + 1;
				char* src = _text + oldLen + 1;
				for (s32 i = _length; i >= (s32)index; i--) {
					 *dest-- = *src--;
				}
			}

			// Copy insert into string
			strncpy(_text + index, text, insertLen);
		}

	} else {
		// No text, so set it for the first time
		setText(text);
	}
}

void WoopsiString::insert(const char text, u32 index) {
	char* newText = new char[2];
	newText[0] = text;
	newText[1] = '\0';

	insert(newText, index);

	delete[] newText;
}

void WoopsiString::remove(const u32 startIndex) {

	// Reject if requested operation makes no sense
	if (!hasData()) return;
	if (startIndex > _length) return;
	if (_length == 0) return;

	// Removing characters from the end of the string is trivial - simply
	// move the terminator and decrease the length
	_length = startIndex;
	_text[startIndex] = '\0';
}

void WoopsiString::remove(const u32 startIndex, const u32 count) {

	// Reject if requested operation makes no sense
	if (!hasData()) return;
	if (startIndex > _length) return;
	if (_length == 0) return;

	// Bounds check the count
	u32 boundsCheckedCount = count;
	if (startIndex + count > _length) boundsCheckedCount = _length - startIndex;
	if (boundsCheckedCount == 0) return;

	// Copy characters from a point after the area to be deleted into the space created
	// by the deletion
	char* src = _text + startIndex + boundsCheckedCount;
	char* dest = _text + startIndex;

	while (*src != '\0') {
		*dest++ = *src++;
	}

	// Decrease length
	_length -= boundsCheckedCount;

	// Append terminator to the string since we didn't copy it initially
	_text[_length] = '\0';
}

void WoopsiString::allocateMemory(u32 chars, bool preserve) {

	// Do we already have enough memory allocated to contain this new size?
	// If so, we can avoid deallocating and allocating new memory by re-using the old
	
	if (chars > _allocatedSize) {

		// Not enough space in existing memory; allocate new memory
		char* newText = new char[chars + _growAmount];

		// Free old memory if necessary
		if (_text != NULL) {

			// Preserve existing data if required
			if (preserve) strcpy(newText, _text);

			delete[] _text;
		}

		// swap pointer to new memory
		_text = newText;

		// Remember how much memory we've allocated
		_allocatedSize = chars + _growAmount;
	}
}
