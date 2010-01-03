#ifndef _WOOPSI_STRING_H_
#define _WOOPSI_STRING_H_

#include <nds.h>

namespace WoopsiUI {

	/**
	 * Yet another string class.  Where possible, the string avoids allocating memory
	 * each time the string grows or shrinks.  This means that the string may consume
	 * more memory than the number of chars would seem to dictate if the object previously
	 * contained a large string that has subsequently been truncated.  It also means
	 * that increasing the length of such a string is a cheaper operation as memory does
	 * not need to allocated and copied.
	 *
	 * Additionally, the string increases its array size by _growAmount every time it
	 * needs to allocate extra memory, potentially reducing the number of reallocs needed.
	 */
	class WoopsiString {
	public:

		/**
		 * Constructor to create a blank object.
		 */
		WoopsiString();

		/**
		 * Constructor to create a string from a char array.
		 * @param text Pointer to a char array to use as the basis of the string.
		 */
		WoopsiString(const char* text);
		
		/**
		 * Constructor to create a string from a single character.
		 * @param letter Single character to use as the basis of the string.
		 */
		WoopsiString(const char letter);
		
		/**
		 * Returns a pointer to the raw char array data.
		 * @return Pointer to the char array.
		 */
		virtual inline const char* getCharArray() const { return _text; };

		/**
		 * Copy the internal array to the supplied buffer.  The buffer must be
		 * large enough to contain the full text in the string.  The
		 * getLength() method can be used to obtain the length of the string.
		 * The buffer will be (getLength() + 1), to accommodate the terminator.
		 * @param buffer Buffer to copy the internal char array to.
		 */
		virtual void copyToCharArray(char* buffer) const;
		
		/**
		 * Set the text in the string.
		 * @param text Char array to use as the new data for this string.
		 */
		virtual void setText(const char* text);

		/**
		 * Set the text in the string.
		 * @param text Character to to use as the new data for this string.
		 */
		virtual void setText(const char text);
		
		/**
		 * Append text to the end of the string.
		 * @param text String to append.
		 */
		virtual void append(const char* text);

		/**
		 * Append text to the end of the string.
		 * @param text Char to append.
		 */
		virtual void append(const char text);

		/**
		 * Insert text at the specified character index.
		 * @param text The text to insert.
		 * @param index The index at which to insert the text.
		 */
		virtual void insert(const char* text, const u32 index);

		/**
		 * Insert text at the specified character index.
		 * @param text Char to insert
		 * @param index The index at which to insert the char.
		 */
		virtual void insert(const char text, const u32 index);

		/**
		 * Remove all characters from the string from the start index onwards.
		 * @param startIndex Index to remove from.
		 */
		virtual void remove(const u32 startIndex);

		/**
		 * Remove specified number of characters from the string from the
		 * start index onwards.
		 * @param startIndex Index to remove from.
		 * @param count Number of characters to remove.
		 */
		virtual void remove(const u32 startIndex, const u32 count);

		/**
		 * Get the of number of UTF-8 tokens (ie. the length) of the string.
		 * @return The length of the string.
		 */
		virtual const u32 getLength() const;

		/**
		 * Copy constructor.
		 * @param string WoopsiString object to create a copy of.
		 */
		inline WoopsiString(const WoopsiString& string) {
			setText(string.getCharArray());
		};

		/**
		 * Destructor.
		 */
		virtual inline ~WoopsiString() {
			delete[] _text;
			_text = NULL;
		};
		
		/**
		 * Get the character at the specified index.
		 * @param index The index of the character to retrieve.
		 * @return The character at the specified index.
		 */
		virtual const u32 getCharAt(u32 index) const;

		/**
		 * Return a pointer to the specified UTF-8 token.
		 * @param index Index of the UTF-8 token to retrieve.
		 */
		virtual char* getToken(const u32 index) const;

	protected:
		char* _text;							/**< Raw char array data */

		/**
		 * Allocate memory for the string.
		 * @param chars Number of chars to allocate.
		 * @param preserve If true, the data in the existing memory will be preserved if new
		 * memory must be allocated
		 */
		virtual void allocateMemory(u32 chars, bool preserve);

		/**
		 * Check if we've got any string data stored or not.
		 * @return True if the string contains any data; false if no data has yet been supplied.
		 */
		virtual inline bool hasData() const { return (_text != NULL); };

		/**
		 * Get the amount of allocated memory.
		 * @return The number of chars allocated in RAM.
		 */
		virtual inline u32 getAllocatedSize() const { return _allocatedSize; };

		/**
		 * Get the UTF-8 token at the specified index.
		 * @param string String containing the token.
		 * @param index Index of the token.
		 * @return Pointer to the UTF-8 token.
		 */
		char* getUTF8Token(const char* string, u32 index) const;

		/**
		 * Get the number of chars read in the UTF-8 token and its codepoint.  In the case of
		 * an invalid codepoint, numChars will be 0.
		 * @param string String to analyse.
		 * @param numChars Pointer to a u8 that will hold the number of chars in the codepoint once
		 * the method ends.  Will contain 0 if the codepoint is invalid.
		 * @return The codepoint.
		 */
		u32 getCodePoint(const char* string, u8* numChars) const;

		/**
		 * Get the unicode char at the specified index.
		 * @param string String to retrieve the char from.
		 * @param index The index of the character to retrieve.
		 * @return The unicode char at the specified index.
		 */
		u32 getUnicodeCharAt(const char* string, u32 index) const;

		/**
		 * Copies the valid utf-8 tokens of the string src into string dest 
		 * and returns the number of chars in the filtered string.
		 * @param dest Destination string.
		 * @param stc Source string.
		 * @return The number of chars in the filtered string.
		 */
		u32 filterString(char* dest, const char* src) const;

	private:
		u32 _length;							/**< Cache length of string for fast access */
		u32 _allocatedSize;						/**< Number of bytes allocated for this string */
		u32 _growAmount;						/**< Number of chars that the string grows by
													 whenever it needs to get larger */
	};
}

#endif
