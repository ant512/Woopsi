#ifndef _KEYBOARDEVENTHANDLER_H_
#define _KEYBOARDEVENTHANDLER_H_

#include <nds.h>

namespace WoopsiUI {

	class Gadget;
	class WoopsiKeyboard;
	class WoopsiKey;

	/**
	 * Base keyboard event handler, intended to be subclassed.  The
	 * WoopsiKeyboard class raises events to instances of this class for
	 * processing.
	 */
	class KeyboardEventHandler {
	public:
		/**
		 * Constructor.
		 */
		inline KeyboardEventHandler() { };
		
		/**
		 * Destructor.
		 */
		virtual inline ~KeyboardEventHandler() { };
		
		/**
		 * Handle a keyboard press event.
		 * @param source The keyboard that was pressed.
		 * @param key The key that was pressed.
		 */
		virtual void handleKeyboardPressEvent(WoopsiKeyboard& source, const WoopsiKey& key) { };

		/**
		 * Handle a keyboard repeat event.
		 * @param source The keyboard that was pressed.
		 * @param key The key that was pressed.
		 */
		virtual void handleKeyboardRepeatEvent(WoopsiKeyboard& source, const WoopsiKey& key) { };

		/**
		 * Handle a keyboard release event.
		 * @param source The keyboard that was released.
		 * @param key The key that was released.
		 */
		virtual void handleKeyboardReleaseEvent(WoopsiKeyboard& source, const WoopsiKey& key) { };
	};
}

#endif
