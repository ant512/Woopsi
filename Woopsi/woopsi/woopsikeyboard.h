#ifndef _WOOPSI_KEYBOARD_H_
#define _WOOPSI_KEYBOARD_H_

#include <nds.h>
#include "amigawindow.h"

using namespace std;

class Button;

enum Key {
	KEY_NONE = 0,
	KEY_1 = 1,
	KEY_2 = 2,
	KEY_3 = 3,
	KEY_4 = 4,
	KEY_5 = 5,
	KEY_6 = 6,
	KEY_7 = 7,
	KEY_8 = 8,
	KEY_9 = 9,
	KEY_0 = 10
};

/**
 * Class providing a window containing a multitude of buttons arranged like a keyboard.
 * When any key is released the keyboard fires an EVENT_ACTION event.
 */
class WoopsiKeyboard : public AmigaWindow {
public:

	/**
	 * Constructor.
	 * @param x The x co-ordinate of the window.
	 * @param y The y co-ordinate of the window.
	 * @param width The width of the window.
	 * @param height The height of the window.
	 * @param title The title of the window.
	 * @param font Optional font to use for text output.
	 */
	WoopsiKeyboard(s16 x, s16 y, u16 width, u16 height, const char* title, FontBase* font = NULL);

	/**
	 * Handles events raised by its sub-gadgets.
	 */
	virtual bool handleEvent(const EventArgs& e);

protected:
	Button* _button;					/**< Pointer to the OK button */
	Button* _lastButtonReleased;		/**< Pointer to the last button released */


	/**
	 * Destructor.
	 */
	virtual inline ~WoopsiKeyboard() { };

	/**
	 * Copy constructor is protected to prevent usage.
	 */
	inline WoopsiKeyboard(const WoopsiKeyboard& keyboard) : AmigaWindow(keyboard) { };
};

#endif
