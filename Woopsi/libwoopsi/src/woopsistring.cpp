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

	// Ensure we've got enough memory available
	allocateMemory(strlen(text) + 1, false);

	// Copy/filter the valid UTF-8 tokens into _text and cache the length
	_length = filterString(_text, text);
}

void WoopsiString::setText(const char text) {

	char newText[2];
	newText[0] = text;
	newText[1] = '\0';

	setText(newText);
}

void WoopsiString::append(const char* text) {

	// Ensure we've got enough memory available
	allocateMemory(_length + strlen(text) + 1, true);

	// Append/filter the valid utf-8 tokens to _text
	_length += filterString(_text + _length, text);
}

void WoopsiString::append(const char text) {

	char newText[2];
	newText[0] = text;
	newText[1] = '\0';

	append(newText);
}

char* WoopsiString::getToken(u32 index) const {
        
	// Early exit if the string is empty
	if (!hasData()) return NULL;

	// Early exit if we want the entire string
	if (index == 0) return _text;

	char token;
	char* pos = _text;

	while ((index > 0) && (*pos !='\0')) {

		pos++;
		token = *pos;

		if ((token < 0x80) || ((token > 0xC2) && (token < 0xFE))) {
			if (index <= 1) return pos;
			index--;
		}
	}

	// No corresponding token found; just return the entire string
	return _text;
}

const u32 WoopsiString::getLength() const {
        
	// Early exit if the string is empty
	if (!hasData()) return 0;

	char token;
	char* pos = _text;
	u32 numTokens = 0;

	while (*pos !='\0') {
		token = *pos;

		if ((token < 0x80) || ((token > 0xC2) && (token < 0xFE))) numTokens++;

		pos++;
	}

	return numTokens; 
}

void WoopsiString::insert(const char* text, u32 index) { 

	// Early exit if the string is empty
	if (!hasData()) {
		setText(text);
		return;
	}

	// Locate the point at which we can cut the existing string 
	u32 insertPoint = getToken(index) - _text;

	// Get the total size of the string that we need
	u32 newSize = _length + strlen(text) + 1;

	// Reallocate memory if the existing memory isn't large enough
	if (_allocatedSize < newSize) {

		// Allocate new string large enough to contain additional data
		char* newText = new char[newSize];

		// Copy the start of the existing text to the newly allocated string
		if (insertPoint > 0) memcpy(newText, _text, insertPoint);

		// Insert the additional text into the new string
		u32 size = filterString(newText + insertPoint, text);

		// Copy the end of the existing text the the newly allocated string
		if (_length > insertPoint) strcpy(newText + insertPoint + size, _text + insertPoint);

		_allocatedSize = newSize;

		// Delete existing string
		delete[] _text;

		// Swap pointers 
		_text = newText;				
		_length = _length + size;
	} else {

		// Existing size large enough, so make space in string for insert
		for (u32 i = 0; i < insertPoint; ++i) {
			_text[newSize - i] = _text[_length - i];
		}

		// Insert the additional text into the new string
		filterString(_text + insertPoint, text);
	}
}

void WoopsiString::insert(const char text, u32 index) {
	char newText[2];
	newText[0] = text;
	newText[1] = '\0';

	insert(newText, index);
}

void WoopsiString::remove(const u32 startIndex) {

	// Reject if requested operation makes no sense
	if (!hasData()) return;
	if (startIndex > _length) return;
	if (_length == 0) return;

	// Find the UTF-8 token corresponding to startIndex        
	char* pos = getToken(startIndex);
        
	// Removing characters from the end of the string is trivial - simply
	// move the terminator and decrease the length
	*pos = '\0';
	_length = pos - _text;
}

void WoopsiString::remove(const u32 startIndex, const u32 count) {

	// Reject if requested operation makes no sense
	if (!hasData()) return;
	if (startIndex > _length) return;
	if (_length == 0) return;

	// Find the utf-8 token corresponding to startIndex
	char* startPos = getToken(startIndex);

	// Find the utf-8 token corresponding to startIndex + count
	char* endPos = getToken(startIndex + count);

	// Copy characters from a point after the area to be deleted into the space created
	// by the deletion
	if (startPos < endPos) memmove(startPos, endPos, _length - u32(endPos - _text));
	
	// Decrease length
	_length -= endPos - startPos;

	// Append terminator to the string since we didn't copy it initially
	_text[_length] = '\0';
}

const u32 WoopsiString::getCharAt(u32 index) const {
	const char* token = getToken(index);
	return getCodePoint(token, NULL);
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

		// Swap pointer to new memory
		_text = newText;

		// Remember how much memory we've allocated
		_allocatedSize = chars + _growAmount;
	}
}

void WoopsiString::copyToCharArray(char* buffer) const {
	strcpy(buffer, _text);
}

char* WoopsiString::getUTF8Token(const char* string, u32 index) const {

	char token;
	char* pos = (char*)string;

	while ((index > 0) && (*pos !='\0')) {
		pos++;
		token = *pos;

		if ((token < 0x80) || ((token > 0xC2) && (token < 0xFE))) {
			if (index <= 1) return pos;
	
			index--;
		}
	}
	
	// No corresponding token found; just return the entire string
	return (char*)string;
}

u32 WoopsiString::getUnicodeCharAt(const char* string, u32 index) const {
	char* pos = getUTF8Token(string, index);
	return getCodePoint(pos, NULL);
}

u32 WoopsiString::filterString(char* dest, const char* src) const {
	u8 bytes;
	u32 length = 0;

	while (*src != '\0') {
		getCodePoint(src, &bytes);

		if (bytes == 0) {

			// This utf-8 token is corrupt; ignore the first char and find a new utf-8 token
			src++;
		} else {

			// Copy the valid utf-8 tokens
			memcpy(dest, src, bytes);
			length += bytes;
			src += bytes;
			dest += bytes;
		}
	}

	// Terminate the string
	*dest =  '\0';

	return length;
}

u32 WoopsiString::getCodePoint(const char* string, u8* numChars) const {
	char char0 = *string;

	// 0xxxxxxx ASCII char
	if (char0 < 0x80) {
		if (numChars) *numChars = 1;
		return char0;
	}

	// 1xxxxxxx 10xxxxxx
	if ((string[1] <0x80) || (string[1] >= 0xC0)) return 0; 


	// 110xxxxx 10xxxxxx
	if (char0 < 0xE0) {
		if (char0 < 0xC2) return 0; // 10xxxxxx (invalid leading char) or  1100000x 10xxxxxx (invalid representation : should have been coded with just 1 char)
		if (numChars) *numChars = 2;
		return ((char0 - 0xC0) << 6) | (string[1] - 0x80); 
	}

	// 111yyyyy 10xxxxxx 10xxxxxx
	if ((string[2]<0x80) || (string[2] >= 0xC0)) return 0; 
	
	// 1110xxxx 10xxxxxx 10xxxxxx
	if (char0 < 0xF0) {
		if ((char0 == 0xE0) && (string[1] < 0xA0)) return 0; // 11100000 100xxxxx 10xxxxxx (invalid representation : should have been coded with at most 2 chars)
		if (numChars) *numChars = 3;
		return ((char0 - 0xE0) << 12) | ((string[1] - 0x80) << 6) | (string[2] - 0x80); 
	}

	// There shouldn't be many utf-8 tokens beyond this point

	// 1111yyyy 10xxxxxx 10xxxxxx 10xxxxxx
	if ((string[3] < 0x80) || (string[3] >= 0xC0)) return 0; 

	// 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	if (char0 < 0xF8) {
		if ((char0 == 0xF0) && (string[1]<0x90)) return 0; // 11110000 1000xxxx 10xxxxxx 10xxxxxx  (invalid representation : should have been coded with at most 3 chars)
		if (numChars) *numChars = 4;
		return ((char0 - 0xF0) << 18) | ((string[1] - 0x80) << 12) | ((string[2] - 0x80) << 6) | (string[3] - 0x80);
	}            

	// 11111yyy 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	if ((string[4] < 0x80) || (string[4] >= 0xC0)) return 0; 

	// 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	if (char0 < 0xFC) {
		if ((char0 == 0xF8) && (string[1]<0x88)) return 0; // 11111000 10000xxx 10xxxxxx 10xxxxxx 10xxxxxx (invalid representation : should have been coded with at most 4 chars)
		if (numChars) *numChars = 5;
		return ((char0-0xF8)<<24) | ((string[1]-0x80)<<18) | ((string[2]-0x80)<<12) | ((string[3]-0x80)<<6) | (string[4] -0x80);
	}   

	// 111111yy 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	if ((string[5] < 0x80) || (string[5] >= 0xC0)) return 0; 

	// 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	if (char0 < 0xFE) {
		if ((char0 == 0xFC) && (string[1]<0x84)) return 0; // 11111100 100000xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx (invalid representation : should have been coded with at most 5 chars)
		if (numChars) *numChars = 6;
		return ((char0-0xFC)<<30) | ((string[1]-0x80)<<24) | ((string[2]-0x80)<<18) | ((string[3]-0x80)<<12) | ((string[4]-0x80)<<6) | (string[5] -0x80);
	}   

	// 11111110 and 11111111 are invalid
	return 0;
}
