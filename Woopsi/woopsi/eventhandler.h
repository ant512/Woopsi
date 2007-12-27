#ifndef _EVENTHANDLER_H_
#define _EVENTHANDLER_H_

#include <nds.h>

class Gadget;

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
	EVENT_MOVE = 18
};

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

typedef struct {
	EventType type;
	Gadget* gadget;
	s16 eventX;
	s16 eventY;
	KeyCode keyCode;
} EventArgs;

class EventHandler {
public:
	EventHandler() { }
	virtual ~EventHandler() { }
	virtual bool handleEvent(const EventArgs& e) { return false; }
};

#endif
