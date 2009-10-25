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
		 * Get the length of the string in characters.
		 * @return The length of the string.
		 */
		virtual inline const u32 getLength() const { return _length; };

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
		 * Overload the [] operator to allow array-style access.
		 * @param index The index to retrieve.
		 * @return The char at the specified index.
		 */
		char& operator[](const u32 index) const {
			return _text[index];
		};
		
		/**
		 * Get the character at the specified index.
		 * @param index The index of the character to retrieve.
		 * @return The character at the specified index.
		 */
		virtual inline const char getCharAt(u32 index) { return _text[index]; }; 

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
		virtual inline u32 getAllocatedSize() { return _allocatedSize; };

	private:
		u32 _length;							/**< Cache length of string for fast access */
		u32 _allocatedSize;						/**< Number of bytes allocated for this string */
		u32 _growAmount;						/**< Number of chars that the string grows by
													 whenever it needs to get larger */
	};
}

#endif
