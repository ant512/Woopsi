#ifndef _WOOPSI_KEYBOARD_SCREEN_H_
#define _WOOPSI_KEYBOARD_SCREEN_H_

#include "keyboardeventhandler.h"
#include "amigascreen.h"

namespace WoopsiUI {

	/**
	 * AmigaScreen that contains a keyboard.  Used by the Woopsi class
	 * as a means of automatically showing a keyboard when a textbox
	 * is double-clicked.
	 */
	class WoopsiKeyboardScreen : public AmigaScreen {
	public:

		/**
		 * Constructor.
		 * @param opener The keyboard event handler that created the gadget.
		 */
		WoopsiKeyboardScreen(KeyboardEventHandler* opener);
		
	protected:

		/**
		 * Destructor.
		 */
		inline virtual ~WoopsiKeyboardScreen() { };

		/**
		 * Handle events fired by child gadgets.
		 * @param e Event arguments to process.
		 */
		virtual void handleClickEvent(const GadgetEventArgs& e);
		
		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline WoopsiKeyboardScreen(const WoopsiKeyboardScreen& keyboard) : AmigaScreen(keyboard) { };
	};
}

#endif
