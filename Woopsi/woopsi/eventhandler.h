#ifndef _EVENTHANDLER_H_
#define _EVENTHANDLER_H_

#include <nds.h>

class Gadget;

/**
 * Enum listing all possible events that can occur within the system.
 */
enum EventType {
	EVENT_NONE = 0,
	EVENT_CLICK = 1,
	EVENT_DRAG = 2,
	EVENT_RELEASE = 3,
	EVENT_KEY_PRESS = 4,
	EVENT_KEY_RELEASE = 5,
	EVENT_VBL = 6,
	EVENT_LID_OPENED = 7,
	EVENT_LID_CLOSED = 8,
	EVENT_FOCUS = 9,
	EVENT_BLUR = 10,
	EVENT_CLOSE = 11,
	EVENT_HIDE = 12,
	EVENT_SHOW = 13,
	EVENT_ENABLE = 14,
	EVENT_DISABLE = 15,
	EVENT_VALUE_CHANGE = 16,
	EVENT_RESIZE = 17,
	EVENT_MOVE = 18,
	EVENT_SCROLL = 19
};

/**
 * Enum listing all key codes.
 */
enum KeyCode {
	KEY_CODE_NONE = 0,
	KEY_CODE_UP = 1,
	KEY_CODE_DOWN = 2,
	KEY_CODE_LEFT = 3,
	KEY_CODE_RIGHT = 4,
	KEY_CODE_A = 5,
	KEY_CODE_B = 6,
	KEY_CODE_X = 7,
	KEY_CODE_Y = 8,
	KEY_CODE_L = 9,
	KEY_CODE_R = 10,
	KEY_CODE_START = 11,
	KEY_CODE_SELECT = 12
};

/**
 * EventArgs struct.  Passed as the argument for every event.
 */
typedef struct {
	EventType type;
	Gadget* gadget;
	s16 eventX;
	s16 eventY;
	KeyCode keyCode;
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
	EventHandler() { }
	
	/**
	 * Destructor.
	 */
	virtual ~EventHandler() { }
	
	/**
	 * Event handler function.  Each gadget that this class is the event handler for will
	 * call this function and pass it details of the event in the EventArgs parameter.
	 * @param e The event's argument data.
	 */
	virtual bool handleEvent(const EventArgs& e) = 0;
};

#endif
