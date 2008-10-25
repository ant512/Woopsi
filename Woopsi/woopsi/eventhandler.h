#ifndef _EVENTHANDLER_H_
#define _EVENTHANDLER_H_

#include <nds.h>

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
	 * EventArgs struct.  Passed as the argument for every event.
	 */
	typedef struct {
		EventType type;					/**< The type of event */
		Gadget* gadget;					/**< Pointer to the gadget that raised the event */
		s16 eventX;						/**< X co-ordinate of the event */
		s16 eventY;						/**< Y co-ordinate of the event */
		s16 eventVX;					/**< X distance moved during event, for dragging */
		s16 eventVY;					/**< Y distance moved during event, for dragging */
		KeyCode keyCode;				/**< The key code that raised the event */
	} EventArgs;

	/**
	 * Base EventHandler class, intended to be subclassed.  Any class that needs to listen for
	 * gadget events should inherit from this class.
	 */
	class EventHandler {
	public:
		/**
		 * Constructor.
		 */
		inline EventHandler() { }
		
		/**
		 * Destructor.
		 */
		virtual inline ~EventHandler() { }
		
		/**
		 * Event handler function.  Each gadget that this class is the event handler for will
		 * call this function and pass it details of the event in the EventArgs parameter.
		 * @param e The event's argument data.
		 */
		virtual bool handleEvent(const EventArgs& e) = 0;
	};
}

#endif
