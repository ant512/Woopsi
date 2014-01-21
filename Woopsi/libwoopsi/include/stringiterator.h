#ifndef _STRING_ITERATOR_H_
#define _STRING_ITERATOR_H_

#include <nds.h>

namespace WoopsiUI {
	
	class WoopsiString;
	
	/**
	 * Class used to efficiently iterate over the characters in a WoopsiString
	 * object.  The WoopsiString is a Unicode string that uses the
	 * variable-width UTF-8 encoding to represent its characters.  This means
	 * that it is not possible to iterate over a WoopsiString in the same way
	 * that you would iterate over a char array.
	 */
	class StringIterator {
	public:

		/**
		 * Constructor.  Moves the iterator to the first character in the
		 * string.
		 * @param string Pointer to the string that will be iterated over.
		 */
		StringIterator(const WoopsiString* string);

		/**
		 * Destructor.
		 */
		inline ~StringIterator() { };
		
		/**
		 * Moves the iterator to the first character in the string.
		 */
		void moveToFirst();

		/**
		 * Moves the iterator to the last character in the string.
		 */
		void moveToLast();

		/**
		 * Move the iterator to the next character in the string.
		 * @return True if the iterator moved; false if not (indicates end of
		 * string).
		 */
		bool moveToNext();

		/**
		 * Move the iterator to the previous character in the string.
		 * @return True if the iterator moved; false if not (indicates start of
		 * string).
		 */
		bool moveToPrevious();

		/**
		 * Move the iterator to the specified index.
		 * @param index The index to move to.
		 * @return True if the iterator moved; false if not (indicates end of
		 * string).
		 */
		bool moveTo(s32 index);
		
		/**
		 * Get the current position of the iterator within the string.
		 * @return The current character index of the iterator.
		 */
		inline s32 getIndex() const { return _currentIndex; };

		/**
		 * Get the codepoint in the string at the iterator's current point.
		 * @return The current character in the string.
		 */
		u32 getCodePoint() const;

		/**
		 * Get the integer value represented by the sequence of digits at the
		 * current point.
		 * @param charCount If set to anything other than NULL, the pointer will
		 * be populated with the number of characters in the integer.  A value
		 * of 0 indicates that no digits were found at the current point.
		 * @return The integer value represented by the current string position.
		 */
		u32 getInteger(u32* charCount = NULL);
		
	protected:

		/**
		 * Get the size of the current codepoint in bytes.
		 * @return The size of the current codepoint.
		 */
		u8 getCodePointSize();

		/**
		 * Move forwards from the current index to the specified index.
		 * @param index The index to move to.
		 */
		void iterateForwardsTo(s32 index);

		/**
		 * Move backwards from the current index to the specified index.
		 * @param index The index to move to.
		 */
		void iterateBackwardsTo(s32 index);
		
	private:
		const WoopsiString* _string;		/**< String being iterated over. */
		const char* _currentChar;	/**< Pointer to the current position of the iterator. */
		s32 _currentIndex;					/**< Iterator's current index within the string. */
	};
}

#endif
