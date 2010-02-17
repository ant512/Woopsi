#ifndef _WOOPSI_KEYBOARD_H_
#define _WOOPSI_KEYBOARD_H_

#include <nds.h>
#include "woopsiarray.h"
#include "gadgetstyle.h"
#include "gadget.h"
#include "gadgeteventhandler.h"
#include "graphicsport.h"

namespace WoopsiUI {

	class WoopsiKey;
	class WoopsiTimer;
	class KeyboardEventHandler;

	/**
	 * Class providing a window containing a multitude of buttons arranged like a keyboard.
	 * When any key is released the keyboard fires an EVENT_ACTION event.
	 */
	class WoopsiKeyboard : public Gadget, public GadgetEventHandler {
	public:

		/**
		 * Constructor.
		 * @param x The x co-ordinate of the window.
		 * @param y The y co-ordinate of the window.
		 * @param flags Standard flags.  Setting GADGET_BORDERLESS hides the Amiga borders.
		 * @param style The style that the gadget should use.  If this is not
		 * specified, the gadget will use the values stored in the global
		 * defaultGadgetStyle object.  The gadget will copy the properties of
		 * the style into its own internal style object.
		 */
		WoopsiKeyboard(s16 x, s16 y, u32 flags, GadgetStyle* style = NULL);

		/**
		 * Handles events raised by its sub-gadgets.
		 * @param e Event data to process.
		 */
		virtual void handleReleaseEvent(const GadgetEventArgs& e);

		/**
		 * Handles events raised by its sub-gadgets.
		 * @param e Event data to process.
		 */
		virtual void handleReleaseOutsideEvent(const GadgetEventArgs& e);

		/**
		 * Handles events raised by its sub-gadgets.
		 * @param e Event data to process.
		 */
		virtual void handleClickEvent(const GadgetEventArgs& e);

		/**
		 * Handles events raised by its sub-gadgets.
		 * @param e Event data to process.
		 */
		virtual void handleActionEvent(const GadgetEventArgs& e);

		/**
		 * Process key releases and tidy up the keyboard state.
		 * @param key Key that was released.
		 */
		virtual void processKeyRelease(WoopsiKey* key);

		/**
		 * Check if the shift key is held down.
		 * @return True if shift is down.
		 */
		inline const bool isShiftDown() const { return _isShiftDown; };

		/**
		 * Check if the control key is held down.
		 * @return True if control is down.
		 */
		inline const bool isControlDown() const { return _isControlDown; };

		/**
		 * Check if the caps lock key is held down.
		 * @return True if caps lock is down.
		 */
		inline const bool isCapsLockDown() const { return _isCapsLockDown; };

		/**
		 * Adds a keyboard event handler.  The event handler will receive
		 * all keyboard events raised by this gadget.
		 * @param eventHandler A pointer to the event handler.
		 */
		inline void addKeyboardEventHandler(KeyboardEventHandler* eventHandler) { _keyboardEventHandlers.push_back(eventHandler); };

		/**
		 * Remove a keyboard event handler.
		 * @param eventHandler A pointer to the event handler to remove.
		 */
		void removeKeyboardEventHandler(KeyboardEventHandler* eventHandler);

	protected:
		WoopsiKey* _shiftKey;			/**< Pointer to the shift key */
		WoopsiKey* _controlKey;			/**< Pointer to the control key */
		WoopsiKey* _capsLockKey;		/**< Pointer to the caps lock key */
		bool _isShiftDown;				/**< True if shift key is currently in down position */
		bool _isControlDown;			/**< True if control key is currently in down position */
		bool _isCapsLockDown;			/**< True if caps lock key is currently in down position */
		WoopsiTimer* _timer;			/**< Timer for handling key repeats */
		u32 _initialRepeatTime;			/**< Time until held key starts to repeat */
		u32 _secondaryRepeatTime;		/**< Time until a key already repeating repeats again */
		WoopsiArray<KeyboardEventHandler*> _keyboardEventHandlers;	/**< List of keyboard event handlers */

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the redraw() function to draw all visible regions.
		 * @param port The GraphicsPort to draw to.
		 * @see redraw()
		 */
		virtual void drawBorder(GraphicsPort* port);

		/**
		 * Swap the keyboard layout to the correct display based on current modifier keys.
		 */
		void showCorrectKeys();

		/**
		 * Swap the keyboard layout to the standard display.
		 */
		void showNormalKeys();

		/**
		 * Swap the keyboard layout to the shifted display.
		 */
		void showShiftKeys();

		/**
		 * Swap the keyboard layout to the controlled display.
		 */
		void showControlKeys();

		/**
		 * Swap the keyboard layout to the shifted and controlled display.
		 */
		void showShiftControlKeys();

		/**
		 * Swap the keyboard layout to the caps locked display.
		 */
		void showCapsLockKeys();

		/**
		 * Swap the keyboard layout to the controlled and caps locked display.
		 */
		void showControlCapsLockKeys();

		/**
		 * Destructor.
		 */
		virtual inline ~WoopsiKeyboard() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline WoopsiKeyboard(const WoopsiKeyboard& keyboard) : Gadget(keyboard) { };

		/**
		 * Raise a press event.  Raised when a key is pressed.
		 * @param key The key that was pressed.
		 */
		void raiseKeyboardPressEvent(WoopsiKey* key);

		/**
		 * Raise a repeat event.  Raised when a key is held and repeats.
		 * @param key The key that was pressed.
		 */
		void raiseKeyboardRepeatEvent(WoopsiKey* key);

		/**
		 * Raise a release event.  Raised when a key is released.
		 * @param key The key that was released.
		 */
		void raiseKeyboardReleaseEvent(WoopsiKey* key);
	};
}

#endif
