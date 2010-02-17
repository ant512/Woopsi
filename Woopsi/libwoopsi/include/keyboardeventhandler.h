#ifndef _KEYBOARDEVENTHANDLER_H_
#define _KEYBOARDEVENTHANDLER_H_

#include <nds.h>
#include "keyboardeventargs.h"

namespace WoopsiUI {

	class Gadget;

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
		 * @param e The event data.
		 */
		virtual void handleKeyboardPressEvent(const KeyboardEventArgs& e) { };

		/**
		 * Handle a keyboard repeat event.
		 * @param e The event data.
		 */
		virtual void handleKeyboardRepeatEvent(const KeyboardEventArgs& e) { };

		/**
		 * Handle a keyboard release event.
		 * @param e The event data.
		 */
		virtual void handleKeyboardReleaseEvent(const KeyboardEventArgs& e) { };
	};
}

#endif
