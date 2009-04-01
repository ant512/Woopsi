#include "eventargs.h"

#ifndef _GADGETEVENTARGS_H_
#define _GADGETEVENTARGS_H_

namespace WoopsiUI {

	class Gadget;

	/**
	 * Enum listing all possible events that can occur within the system.
	 */
	enum EventType {
		EVENT_NONE = 0,						/**< No event to process; only included for completeness */
		EVENT_CLICK = 1,					/**< Gadget has been clicked */
		EVENT_DRAG = 2,						/**< Gadget has been dragged */
		EVENT_RELEASE = 3,					/**< Gadget has been released (ie. stylus removed from screen) */
		EVENT_RELEASE_OUTSIDE = 4,			/**< Gadget has been released with the stylus outside the gadget */
		EVENT_KEY_PRESS = 5,				/**< DS button or d-pad has been pressed */
		EVENT_KEY_RELEASE = 6,				/**< DS button or d-pad has been released */
		EVENT_LID_OPEN = 7,					/**< DS' lid has been opened */
		EVENT_LID_CLOSE = 8,				/**< DS' lid has been closed */
		EVENT_FOCUS = 9,					/**< Gadget has received focus */
		EVENT_BLUR = 10,					/**< Gadget has lost focus */
		EVENT_CLOSE = 11,					/**< Gadget has been closed */
		EVENT_HIDE = 12,					/**< Gadget has been hidden */
		EVENT_SHOW = 13,					/**< Hidden gadget has been made visible */
		EVENT_ENABLE = 14,					/**< Disabled gadget has been enabled */
		EVENT_DISABLE = 15,					/**< Enabled gadget has been disabled */
		EVENT_VALUE_CHANGE = 16,			/**< Gadget has changed value */
		EVENT_RESIZE = 17,					/**< Gadget has been resized */
		EVENT_MOVE = 18,					/**< Gadget has been moved */
		EVENT_SCROLL = 19,					/**< Gadget has been scrolled */
		EVENT_SHIFT_CLICK = 20,				/**< Gadget has been shift-clicked */
		EVENT_CONTEXT_MENU_SELECTION = 21,	/**< Item selected in gadget's context menu */
		EVENT_DOUBLE_CLICK = 22,			/**< Gadget has been double-clicked */
		EVENT_SHELVE = 23,					/**< Gadget has been shelved */
		EVENT_UNSHELVE = 24,				/**< Shelved gadget has been unshelved */
		EVENT_ACTION = 25,					/**< Gadget has performed its action (ie. button released) */
		EVENT_MOVE_FORWARD = 26,			/**< Gadget has been raised up its parent's gadget stack */
		EVENT_MOVE_BACKWARD = 27			/**< Gadget has been lowered down its parent's gadget stack */
	};

	/**
	 * Enum listing all key codes.
	 */
	enum KeyCode {
		KEY_CODE_NONE = 0,				/**< No key.  Only included for completeness */
		KEY_CODE_UP = 1,				/**< D-pad up button */
		KEY_CODE_DOWN = 2,				/**< D-pad down button */
		KEY_CODE_LEFT = 3,				/**< D-pad left button */
		KEY_CODE_RIGHT = 4,				/**< D-pad right button */
		KEY_CODE_A = 5,					/**< A button */
		KEY_CODE_B = 6,					/**< B button */
		KEY_CODE_X = 7,					/**< X button */
		KEY_CODE_Y = 8,					/**< Y button */
		KEY_CODE_L = 9,					/**< L button */
		KEY_CODE_R = 10,				/**< R button */
		KEY_CODE_START = 11,			/**< Start button */
		KEY_CODE_SELECT = 12			/**< Select button */
	};

	/**
	 * Event arguments passed to listeners when a Gadget object raises an event.
	 */
	class GadgetEventArgs : public EventArgs<Gadget*> {
	public:

		/**
		 * Constructor.
		 * @param source Pointer to the Gadget object that raised the event.
		 */
		GadgetEventArgs(Gadget* source, const EventType type, const s16 x, const s16 y, const s16 vX, const s16 vY, const KeyCode keyCode) : EventArgs<Gadget*>(source) {
			_type = type;
			_x = x;
			_y = y;
			_vX = vX;
			_vY = vY;
			_keyCode = keyCode;
		}

		inline const s16 getX() const { return _x; }

		inline const s16 getY() const { return _y; }

		inline const s16 getVX() const { return _vX; }

		inline const s16 getVY() const { return _vY; }

		inline const KeyCode getKeyCode() const { return _keyCode; }

		inline const EventType getType() const { return _type; }

	private:
		EventType _type;				/**< The type of event */
		s16 _x;							/**< X co-ordinate of the event */
		s16 _y;							/**< Y co-ordinate of the event */
		s16 _vX;						/**< X distance moved during event, for dragging */
		s16 _vY;						/**< Y distance moved during event, for dragging */
		KeyCode _keyCode;				/**< The key code that raised the event */
	};
}

#endif
