#ifndef _GADGETEVENTARGS_H_
#define _GADGETEVENTARGS_H_

#include "eventargs.h"

namespace WoopsiUI {

	class Gadget;

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
		 * @param x The x co-ord of the event.
		 * @param y The y co-ord of the event.
		 * @param vX The x distance of the event.
		 * @param vY The y distance of the event.
		 * @param keyCode The keycode of the event.
		 */
		GadgetEventArgs(Gadget* source, const s16 x, const s16 y, const s16 vX, const s16 vY, const KeyCode keyCode) : EventArgs<Gadget*>(source) {
			_x = x;
			_y = y;
			_vX = vX;
			_vY = vY;
			_keyCode = keyCode;
		}

		/**
		 * Get the x co-ord of the event.
		 * @return The x co-ord of the event.
		 */
		inline const s16 getX() const { return _x; }

		/**
		 * Get the y co-ord of the event.
		 * @return The y co-ord of the event.
		 */
		inline const s16 getY() const { return _y; }

		/**
		 * Get the x-axis stylus move distance of the event.
		 * @return The x-axis stylus move distance of the event.
		 */
		inline const s16 getVX() const { return _vX; }

		/**
		 * Get the y-axis stylus move distance of the event.
		 * @return The y-axis stylus move distance of the event.
		 */
		inline const s16 getVY() const { return _vY; }

		/**
		 * Get the keycode of the event.
		 * @return The keycode of the event.
		 */
		inline const KeyCode getKeyCode() const { return _keyCode; }

	private:
		s16 _x;							/**< X co-ordinate of the event */
		s16 _y;							/**< Y co-ordinate of the event */
		s16 _vX;						/**< X distance moved during event, for dragging */
		s16 _vY;						/**< Y distance moved during event, for dragging */
		KeyCode _keyCode;				/**< The key code that raised the event */
	};
}

#endif
