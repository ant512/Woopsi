#ifndef _WOOPSI_KEYBOARD_H_
#define _WOOPSI_KEYBOARD_H_

#include <nds.h>
#include "amigawindow.h"

using namespace std;

class WoopsiKey;

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

	/**
	 * Get a pointer to the last key that was released.
	 */
	inline const WoopsiKey* getLastKeyReleased() { return _lastKeyReleased; };

protected:
	WoopsiKey* _lastKeyReleased;		/**< Pointer to the last key released */
	WoopsiKey* _shiftKey;				/**< Pointer to the shift key */
	WoopsiKey* _controlKey;				/**< Pointer to the control key */
	WoopsiKey* _capsLockKey;			/**< Pointer to the caps lock key */
	bool _isShiftDown;
	bool _isControlDown;
	bool _isCapsLockDown;

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
	inline WoopsiKeyboard(const WoopsiKeyboard& keyboard) : AmigaWindow(keyboard) { };
};

#endif
