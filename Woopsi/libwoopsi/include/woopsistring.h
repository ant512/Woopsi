#ifndef _WOOPSI_STRING_H_
#define _WOOPSI_STRING_H_

#include <stdarg.h>
#include <nds.h>

namespace WoopsiUI {
	
	class StringIterator;

	/**
	 * Unicode string class.  Uses UTF-8 encoding.  For optimal performance, use
	 * the StringIterator class to iterate over a WoopsiString instance.
	 *
	 * Where possible, the string avoids allocating memory each time the string
	 * grows or shrinks.  This means that the string may consume more memory
	 * than the number of chars would seem to dictate if the object previously
	 * contained a large string that has subsequently been truncated.  It also
	 * means that increasing the length of such a string is a cheaper operation
	 * as memory does not need to allocated and copied.
	 *
	 * Additionally, the string increases its array size by _growAmount every
	 * time it needs to allocate extra memory, potentially reducing the number
	 * of reallocs needed.
	 *
	 * The string is not null-terminated.  Instead, it uses a _stringLength
	 * member that stores the number of characters in the string.  This saves a
	 * byte and makes calls to getLength() run in O(1) time instead of O(n).
	 */
	class WoopsiString {
	public:

		/**
		 * Constructor to create a blank object.
		 */
		WoopsiString();

		/**
		 * Constructor to create a string from a char array.
		 * @param text Pointer to a char array to use as the basis of the
		 * string.
		 */
		WoopsiString(const char* text);
		
		/**
		 * Constructor to create a string from a single character.
		 * @param letter Single character to use as the basis of the string.
		 */
		WoopsiString(const u32 letter);

		/**
		 * Copy constructor.
		 * @param string WoopsiString object to create a copy of.
		 */
		WoopsiString(const WoopsiString& string);

		/**
		 * Destructor.
		 */
		virtual inline ~WoopsiString() {
			delete[] _text;
			_text = NULL;
		};
		
		/**
		 * Creates and returns a new StringIterator object that will iterate
		 * over this string.  The object must be manually deleted once it is
		 * no longer needed.
		 * @return A new StringIterator object.
		 */
		StringIterator* newStringIterator() const;

		/**
		 * Copy the internal array to the supplied buffer.  The buffer must be
		 * large enough to contain the full text in the string.  The
		 * getByteCount() method can be used to obtain the length of the string.
		 * Unlike the WoopsiString class, the char array is null-terminated.
		 * The buffer must be (getByteCount() + 1) bytes long, in order to
		 * accommodate the terminator.
		 * @param buffer Buffer to copy the internal char array to.
		 */
		virtual void copyToCharArray(char* buffer) const;
		
		/**
		 * Set the text in the string.
		 * @param text WoopsiString containing the new data for this string.
		 */
		virtual void setText(const WoopsiString& text);

		/**
		 * Set the text in the string.
		 * @param text Char array to use as the new data for this string.
		 */
		virtual void setText(const char* text);

		/**
		 * Set the text in the string.
		 * @param text Character to to use as the new data for this string.
		 */
		virtual void setText(const u32 text);

		/**
		 * Append text to the end of the string.
		 * @param text String to append.
		 */
		virtual void append(const WoopsiString& text);

		/**
		 * Insert text at the specified character index.
		 * @param text The text to insert.
		 * @param index The index at which to insert the text.
		 */
		virtual void insert(const WoopsiString& text, const s32 index);

		/**
		 * Remove all characters from the string from the start index onwards.
		 * @param startIndex Index to remove from.
		 */
		virtual void remove(const s32 startIndex);

		/**
		 * Remove specified number of characters from the string from the
		 * start index onwards.
		 * @param startIndex Index to remove from.
		 * @param count Number of characters to remove.
		 */
		virtual void remove(const s32 startIndex, const s32 count);

		/**
		 * Get the of number of UTF-8 tokens (ie. the length) of the string.
		 * @return The length of the string.
		 */
		virtual const s32 getLength() const { return _stringLength; };

		/**
		 * Get the of number of bytes in the string.
		 * @return The number of bytes of the string.
		 */
		virtual const s32 getByteCount() const { return _dataLength; };

		/**
		 * Get the character at the specified index.  This function is useful
		 * for finding the occasional character at an index, but for iterating
		 * over strings it is exceptionally slow.  The newStringIterator()
		 * method should be used to retrieve an iterator object that can iterate
		 * over the string efficiently.
		 * @param index The index of the character to retrieve.
		 * @return The character at the specified index.
		 */
		virtual const u32 getCharAt(s32 index) const;

		/**
		 * Returns the first index of the specified letter within the string.
		 * Will return -1 if the letter is not found.
		 * @param letter Letter to find.
		 * @return The index of the letter.
		 */
		const s32 indexOf(u32 letter) const;

		/**
		 * Returns the first index of the specified letter within the string.
		 * Will return -1 if the letter is not found.  Scans through the string
		 * from "startIndex" until it has examined all subsequent letters.
		 * @param letter Letter to find.
		 * @param startIndex The index to start searching from.
		 * @return The index of the letter.
		 */
		const s32 indexOf(u32 letter, s32 startIndex) const;

		/**
		 * Returns the first index of the specified letter within the string.
		 * Will return -1 if the letter is not found.  Scans through the string
		 * from "startIndex" until it has examined all letters within the
		 * range "count".
		 * @param letter Letter to find.
		 * @param startIndex The index to start searching from.
		 * @param count The number of characters to examine.
		 * @return The index of the letter.
		 */
		virtual const s32 indexOf(u32 letter, s32 startIndex, s32 count) const;

		/**
		 * Returns the last index of the specified letter within the string.
		 * Will return -1 if the letter is not found.
		 * @param letter Letter to find.
		 * @return The index of the letter.
		 */
		const s32 lastIndexOf(u32 letter) const;

		/**
		 * Returns the last index of the specified letter within the string.
		 * Will return -1 if the letter is not found.  Scans through the string
		 * backwards from "startIndex" until it has examined all preceding
		 * letters within the string.
		 * @param letter Letter to find.
		 * @param startIndex The index to start searching from.
		 * @return The index of the letter.
		 */
		const s32 lastIndexOf(u32 letter, s32 startIndex) const;

		/**
		 * Returns the last index of the specified letter within the string.
		 * Will return -1 if the letter is not found.  Scans through the string
		 * backwards from "startIndex" until it has examined all letters within
		 * the range "count".
		 * @param letter Letter to find.
		 * @param startIndex The index to start searching from.
		 * @param count The number of characters to examine.
		 * @return The index of the letter.
		 */
		virtual const s32 lastIndexOf(u32 letter, s32 startIndex, s32 count) const;

		/**
		 * Get a substring from this string.  It is the responsibility of the
		 * caller to delete the substring when it is no longer required.
		 * @param startIndex The starting point of the substring.
		 * @return A pointer to a new WoopsiString object containing the
		 * substring.
		 */
		WoopsiString* subString(s32 startIndex) const;

		/**
		 * Get a substring from this string.  It is the responsibility of the
		 * caller to delete the substring when it is no longer required.
		 * @param startIndex The starting point of the substring.
		 * @param length The length of the substring.
		 * @return A pointer to a new WoopsiString object containing the
		 * substring.
		 */
		WoopsiString* subString(s32 startIndex, s32 length) const;
		
		/**
		 * Overloaded assignment operator.  Copies the data within the argument
		 * string to this string.
		 * @param string The string to copy.
		 * @return This string.
		 */
		WoopsiString& operator=(const WoopsiString& string);

		/**
		 * Overloaded assignment operator.  Copies the data within the argument
		 * char array to this string.
		 * @param string The string to copy.
		 * @return This string.
		 */
		WoopsiString& operator=(const char* string);

		/**
		 * Overloaded assignment operator.  Copies the data from the argument
		 * char to this string.
		 * @param letter The char to copy.
		 * @return This string.
		 */
		WoopsiString& operator=(u32 letter);

		/**
		 * Overloaded addition operator.  Creates a copy of the current string,
		 * appends the parameter string, and returns the updated copy.
		 * @param string The string to append.
		 * @return A copy of this string with the parameter string appended.
		 */
		WoopsiString operator+(const WoopsiString& string);

		/**
		 * Overloaded compound addition/assignment operator.  Appends the
		 * parameter string to the current string.
		 * @param string The string to append.
		 * @return This string.
		 */
		WoopsiString& operator+=(const WoopsiString& string);

		/**
		 * Overloaded equality operator.  Checks if this string and the supplied
		 * argument are equal.
		 * @param string The string to compare with.
		 * @return True if the strings are equal; false if not.
		 */
		bool operator==(const WoopsiString& string) { return compareTo(string) == 0; };

		/**
		 * Overloaded inequality operator.  Checks if this string and the
		 * supplied argument are not equal.
		 * @param string The string to compare with.
		 * @return False if the strings are equal; true if not.
		 */
		bool operator!=(const WoopsiString& string) { return compareTo(string) != 0; };

		/**
		 * Compares this string to the argument.
		 * @param string String to compare to.
		 * @param caseSensitive Performs a case-sensitive compare if this is set
		 * to true.
		 * @return Zero if both strings are equal.  A value greater than zero
		 * indicates that this string is greater than the argument string.  A
		 * value less than zero indicates the opposite.  Note that the return
		 * value indicates the *byte* that does not match, not the *character*.
		 */
		virtual s8 compareTo(const WoopsiString& string, bool caseSensitive = false) const;

		/**
		 * Gets the character index of the first occurence of the supplied char
		 * array within the string.  If the char array is not found, the method
		 * will return -1.
		 * @param text The char array to find.
		 * @return The index of the char array within the current string if it
		 * exists, or -1 if it does not.
		 */
		s32 indexOf(const char* text) const;

		/**
		 * Gets the character index of the first occurence after startIndex of
		 * the supplied char array within the string.  If the char array is not
		 * found, the method will return -1.
		 * @param text The char array to find.
		 * @param startIndex The index to start searching from.
		 * @return The index of the char array within the current string if it
		 * exists, or -1 if it does not.
		 */
		s32 indexOf(const char* text, s32 startIndex) const;

		/**
		 * Gets the character index of the first occurence of the supplied
		 * string within the bounds of the current string between startIndex and
		 * startIndex + count.  If the string is not found, the method will
		 * return -1.
		 * @param text The char array to find.
		 * @param startIndex The index to start searching from.
		 * @param count The number of characters to examine.
		 * @return The index of the char array within the current string if it
		 * exists, or -1 if it does not.
		 */
		s32 indexOf(const WoopsiString& string, s32 startIndex, s32 count) const;

		/**
		 * Gets the character index of the last occurence of the supplied char
		 * array within the string.  If the char array is not found, the method
		 * will return -1.
		 * @param text The char array to find.
		 * @return The index of the char array within the current string if it
		 * exists, or -1 if it does not.
		 */
		s32 lastIndexOf(const char* text) const;

		/**
		 * Gets the character index of the last occurence after startIndex of
		 * the supplied char array within the string.  If the char array is not
		 * found, the method will return -1.
		 * @param text The char array to find.
		 * @param startIndex The index to start searching from.
		 * @return The index of the char array within the current string if it
		 * exists, or -1 if it does not.
		 */
		s32 lastIndexOf(const char* text, s32 startIndex) const;

		/**
		 * Gets the character index of the last occurence of the supplied
		 * string within the bounds of the current string between startIndex and
		 * startIndex - count.  If the string is not found, the method will
		 * return -1.
		 * @param text The char array to find.
		 * @param startIndex The index to start searching from.
		 * @param count The number of characters to examine.
		 * @return The index of the char array within the current string if it
		 * exists, or -1 if it does not.
		 */
		s32 lastIndexOf(const WoopsiString& string, s32 startIndex, s32 count) const;

		/**
		 * Formats the supplied char array using standard printf() syntax.  The
		 * formatted text replaces the contents of the current string object.
		 * @param format The format to use.
		 * @param ... The formatting arguments.
		 */
		void format(const char *format, ...);

		/**
		 * Formats the supplied char array using standard printf() syntax.  The
		 * formatted text replaces the contents of the current string object.
		 * Note that va_start() must be called before this method is used, and
		 * va_end() must be called when it finishes.
		 *
		 * Note that the compiler message regarding va_list mangling changes
		 * in GCC 4.4 are not problems with Woopsi's code.  They are the result
		 * of the GCC team bizarrely deciding to include news items in compiler
		 * messages:
		 * http://gcc.gnu.org/bugzilla/show_bug.cgi?id=42748
		 *
		 * @param format The format to use.
		 * @param args The formatting arguments.
		 */
		void format(const char *format, va_list args);

		/**
		 * Replace all instances of oldText within the current string with
		 * newText.
		 * @param oldText The text to search for and replace.
		 * @param newText The new text that will appear in place of the old.
		 */
		void replace(const WoopsiString& oldText, const WoopsiString& newText);

		/**
		 * Replace all instances of oldText within the current string from
		 * startIndex onwards with newText.
		 * @param oldText The text to search for and replace.
		 * @param startIndex The index to start replacing from.
		 * @param newText The new text that will appear in place of the old.
		 */
		void replace(const WoopsiString& oldText, const s32 startIndex, const WoopsiString& newText);

		/**
		 * Replace the text between the bounds defined by startIndex and 
		 * startIndex + count with newText.
		 * @param startIndex The index to start replacing from.
		 * @param count The number of characters to replace.
		 * @param newText The new text that will appear in place of the old.
		 */
		void replace(const s32 startIndex, const s32 count, const WoopsiString& newText);

	protected:
		char* _text;							/**< Raw char array data */

		/**
		 * Initialise the string's default state; called by constructors.
		 */
		virtual void init();

		/**
		 * Allocate memory for the string.
		 * @param chars Number of chars to allocate.
		 * @param preserve If true, the data in the existing memory will be
		 * preserved if new memory must be allocated
		 */
		virtual void allocateMemory(s32 chars, bool preserve);

		/**
		 * Check if we've got any string data stored or not.
		 * @return True if the string contains any data; false if no data has
		 * yet been supplied.
		 */
		virtual inline bool hasData() const { return _dataLength > 0; };

		/**
		 * Get the amount of allocated memory.
		 * @return The number of chars allocated in RAM.
		 */
		virtual inline s32 getAllocatedSize() const { return _allocatedSize; };

		/**
		 * Copies the valid utf-8 tokens of the string src into string dest 
		 * and returns the number of bytes in the filtered string.
		 * @param dest Destination string.
		 * @param src Source string.
		 * @param sourceBytes Number of bytes in the source string.
		 * @param totalUnicodeChars Output parameter populated with the total
		 * number of unicode characters in the filtered string.
		 * @return The number of bytes in the filtered string.
		 */
		s32 filterString(char* dest, const char* src, s32 sourceBytes, s32* totalUnicodeChars) const;

		/**
		 * Returns a pointer to the raw char array data.
		 * @return Pointer to the char array.
		 */
		virtual inline const char* getCharArray() const { return _text; };

		/**
		 * Return a pointer to the specified UTF-8 token.
		 * @param index Index of the UTF-8 token to retrieve.
		 */
		virtual char* getToken(const s32 index) const;

		/**
		 * Get the number of chars read in the UTF-8 token and its codepoint.
		 * In the case of an invalid codepoint, the value returned will be 0.
		 * @param string String to analyse.
		 * @param numChars Pointer to a u8 that will hold the number of chars in
		 * the codepoint once the method ends.
		 * @return The codepoint.  Returns 0 if the codepoint is invalid.
		 */
		u32 getCodePoint(const char* string, u8* numChars = NULL) const;

	private:
		friend class StringIterator;
		
		s32 _dataLength;		/**< Length of char data in the string */
		s32 _stringLength;		/**< Number of unicode tokens in the string */
		s32 _allocatedSize;		/**< Number of bytes allocated for this string */
		s32 _growAmount;		/**< Number of chars that the string grows by
									 whenever it needs to get larger */
									 
		/**
		 * Encodes a codepoint into its UTF-8 representation.  Will allocate
		 * a block of memory if the encode is successful containing the encoded
		 * data.  This memory must be deleted with delete[] when it is no longer
		 * needed to avoid a memory leak.  If the codepoint is not valid, the
		 * method will return NULL.
		 * @param codePoint Codepoint to encode.
		 * @param numBytes The number of bytes in the encoded data.
		 * @return The encoded codepoint.
		 */
		 const char* encodeCodePoint(u32 codePoint, u8* numBytes = NULL) const;
	};
}

#endif
