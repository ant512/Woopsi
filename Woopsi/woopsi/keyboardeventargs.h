#include "eventargs.h"

#ifndef _KEYBOARDEVENTARGS_H_
#define _KEYBOARDEVENTARGS_H_

namespace WoopsiUI {

	class WoopsiKey;
	class WoopsiKeyboard;

	/**
	 * Event arguments passed to listeners when a WoopsiKeyboard object raises an event.
	 */
	class KeyboardEventArgs : public EventArgs<WoopsiKeyboard*> {
	public:

		/**
		 * Constructor.
		 * @param source Pointer to the WoopsiKeyboard object that raised the event.
		 * @param key Pointer to the key that caused the event.
		 */
		KeyboardEventArgs(WoopsiKeyboard* source, WoopsiKey* key) : EventArgs<WoopsiKeyboard*>(source) {
			_key = key;
		};

		/**
		 * Get the key that caused the event.
		 * @return Pointer to the key that caused the event.
		 */
		inline const WoopsiKey* getKey() const { return _key; };

	private:
		WoopsiKey* _key;					/**< Pointer to the key that caused the event */
	};
}

#endif
