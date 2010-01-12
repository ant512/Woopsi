#ifndef _STRING_ITERATOR_H_
#define _STRING_ITERATOR_H_

#include <nds.h>

namespace WoopsiUI {
	
	class WoopsiString;
	
	class StringIterator {
	public:
		StringIterator(const WoopsiString* string);
		inline ~StringIterator() { };
		
		void moveToFirst();
		void moveToLast();
		bool moveToNext();
		bool moveToPrevious();
		bool moveTo(u32 index);
		
		inline u32 getIndex() const { return _currentIndex; };
		u32 getCodePoint() const;
		
	protected:
		u8 getCodePointSize();

		void iterateForwardsTo(u32 index);
		void iterateBackwardsTo(u32 index);
		
	private:
		const WoopsiString* _string;
		const char* _currentChar;
		u32 _currentIndex;
	};
}

#endif
