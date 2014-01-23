#ifndef _PAD_H_
#define _PAD_H_

#include <nds.h>

/**
 * Abstraction of the DS' pad state to aid porting.
 */
class Pad {
public:

	static const s32 PAD_REPEAT_TIME = 10;	/**< VBLs before buttons repeat. */

	/**
	 * Constructor.
	 */
	inline Pad() {
		_up = 0;
		_down = 0;
		_left = 0;
		_right = 0;
		_a = 0;
		_b = 0;
		_x = 0;
		_y = 0;
		_l = 0;
		_r = 0;
		_start = 0;
		_select = 0;
	};

	/**
	 * Enum listing all key codes.
	 */
	typedef enum {
		KEY_CODE_UP = 0,			/**< D-pad up button. */
		KEY_CODE_DOWN = 1,			/**< D-pad down button. */
		KEY_CODE_LEFT = 2,			/**< D-pad left button. */
		KEY_CODE_RIGHT = 3,			/**< D-pad right button. */
		KEY_CODE_A = 4,				/**< A button. */
		KEY_CODE_B = 5,				/**< B button. */
		KEY_CODE_X = 6,				/**< X button. */
		KEY_CODE_Y = 7,				/**< Y button. */
		KEY_CODE_L = 8,				/**< L button. */
		KEY_CODE_R = 9,				/**< R button. */
		KEY_CODE_START = 10,		/**< Start button. */
		KEY_CODE_SELECT = 11		/**< Select button. */
	} KeyCode;

	/**
	 * Destructor.
	 */
	inline ~Pad() { };

	inline s32 heldTimeForKey(const KeyCode keyCode) const {
		switch (keyCode) {
			case KEY_CODE_A:
				return _a;
				break;
			case KEY_CODE_B:
				return _b;
				break;
			case KEY_CODE_DOWN:
				return _down;
				break;
			case KEY_CODE_L:
				return _l;
				break;
			case KEY_CODE_LEFT:
				return _left;
				break;
			case KEY_CODE_R:
				return _r;
				break;
			case KEY_CODE_RIGHT:
				return _right;
				break;
			case KEY_CODE_SELECT:
				return _select;
				break;
			case KEY_CODE_START:
				return _start;
				break;
			case KEY_CODE_UP:
				return _up;
				break;
			case KEY_CODE_X:
				return _x;
				break;
			case KEY_CODE_Y:
				return _y;
				break;
		}

		// Included to silence dumb GCC warning
		return 0;
	}

	/**
	 * Check if the key is newly pressed since the last call to update().
	 * @param keyCode The key to test.
	 * @return True if newly pressed.
	 */
	inline bool isNewPress(const KeyCode keyCode) const { return heldTimeForKey(keyCode) == 1; };

	/**
	 * Check if the key is newly released since the last call to update().
	 * @param keyCode The key to test.
	 * @return True if newly released.
	 */
	inline bool isReleased(const KeyCode keyCode) const { return heldTimeForKey(keyCode) == -1; };

	/**
	 * Check if the key is held down.
	 * @param keyCode The key to test.
	 * @return True if held down.
	 */
	inline bool isHeld(const KeyCode keyCode) const { return heldTimeForKey(keyCode) > 0; };

	/**
	 * Check if the key is repeating.
	 * @param keyCode The key to test.
	 * @return True if repeating.
	 */
	inline bool isRepeat(const KeyCode keyCode) const {
		s32 value = heldTimeForKey(keyCode);
		return value > 0 && value % PAD_REPEAT_TIME == 0;
	};

	/**
	 * Check if the most recently pressed direction was vertical or horizontal.
	 * @return True if the most recently-pressed direction was vertical.  False
	 * if the most recently-pressed direction was horizontal.
	 */
	bool isMostRecentDirectionVertical() const {
		if (_up > 0 && (_up < _left || _up < _right)) return true;
		if (_down > 0 && (_down < _left || _down < _right)) return true;
		return false;
	};

	/**
	 * Update the pad's state to match the latest DS state.
	 */
	void update() {

		// If we released any keys on the previous iteration we need to reset
		// them
		if (_up == -1) ++_up;
		if (_down == -1) ++_down;
		if (_left == -1) ++_left;
		if (_right == -1) ++_right;
		if (_l == -1) ++_l;
		if (_r == -1) ++_r;
		if (_a == -1) ++_a;
		if (_b == -1) ++_b;
		if (_x == -1) ++_x;
		if (_y == -1) ++_y;
		if (_start == -1) ++_start;
		if (_select == -1) ++_select;

#ifndef USING_SDL

		scanKeys();

		s32 pressed = keysDown();	// Buttons pressed this loop
		s32 held = keysHeld();		// Buttons currently held
		s32 allKeys = pressed | held;

		if (allKeys & KEY_UP) {
			++_up;
		} else if (_up > 0) {
			_up = -1;
		}

		if (allKeys & KEY_DOWN) {
			++_down;
		} else if (_down > 0) {
			_down = -1;
		}

		if (allKeys & KEY_LEFT) {
			++_left;
		} else if (_left > 0) {
			_left = -1;
		}

		if (allKeys & KEY_RIGHT) {
			++_right;
		} else if (_right > 0) {
			_right = -1;
		}

		if (allKeys & KEY_L) {
			++_l;
		} else if (_l > 0) {
			_l = -1;
		}

		if (allKeys & KEY_R) {
			++_r;
		} else if (_r > 0) {
			_r = -1;
		}

		if (allKeys & KEY_A) {
			++_a;
		} else if (_a > 0) {
			_a = -1;
		}

		if (allKeys & KEY_B) {
			++_b;
		} else if (_b > 0) {
			_b = -1;
		}

		if (allKeys & KEY_X) {
			++_x;
		} else if (_x > 0) {
			_x = -1;
		}

		if (allKeys & KEY_Y) {
			++_y;
		} else if (_y > 0) {
			_y = -1;
		}

		if (allKeys & KEY_START) {
			++_start;
		} else if (_start > 0) {
			_start = -1;
		}

		if (allKeys & KEY_SELECT) {
			++_select;
		} else if (_select > 0) {
			_select = -1;
		}

#else

		const Uint8* keyState = SDL_GetKeyboardState(NULL);

		if (keyState[SDL_SCANCODE_UP]) {
			++_up;
		} else if (_up > 0) {
			_up = -1;
		}

		if (keyState[SDL_SCANCODE_DOWN]) {
			++_down;
		} else if (_down > 0) {
			_down = -1;
		}

		if (keyState[SDL_SCANCODE_LEFT]) {
			++_left;
		} else if (_left > 0) {
			_left = -1;
		}

		if (keyState[SDL_SCANCODE_RIGHT]) {
			++_right;
		} else if (_right > 0) {
			_right = -1;
		}

		if (keyState[SDL_SCANCODE_A]) {
			++_l;
		} else if (_l > 0) {
			_l = -1;
		}

		if (keyState[SDL_SCANCODE_S]) {
			++_r;
		} else if (_r > 0) {
			_r = -1;
		}

		if (keyState[SDL_SCANCODE_Z]) {
			++_a;
		} else if (_a > 0) {
			_a = -1;
		}

		if (keyState[SDL_SCANCODE_X]) {
			++_b;
		} else if (_b > 0) {
			_b = -1;
		}

		if (keyState[SDL_SCANCODE_C]) {
			++_x;
		} else if (_x > 0) {
			_x = -1;
		}

		if (keyState[SDL_SCANCODE_V]) {
			++_y;
		} else if (_y > 0) {
			_y = -1;
		}

		if (keyState[SDL_SCANCODE_D]) {
			++_start;
		} else if (_start > 0) {
			_start = -1;
		}

		if (keyState[SDL_SCANCODE_F]) {
			++_select;
		} else if (_select > 0) {
			_select = -1;
		}

#endif
	};

private:

	/**
	 * Each value represents the amount of time (in VBLs) that the button has
	 * been held.  Therefore:
	 *  -1 = released;
	 *   0 = not held;
	 *   1 = newly held;
	 *  >1 = held for n frames.
	 */

	s32 _up;		/**< Is up held? */
	s32 _down;		/**< Is down held? */
	s32 _left;		/**< Is left held? */
	s32 _right;		/**< Is right held? */
	s32 _a;			/**< Is a held? */
	s32 _b;			/**< Is b held? */
	s32 _x;			/**< Is x held? */
	s32 _y;			/**< Is y held? */
	s32 _l;			/**< Is l held? */
	s32 _r;			/**< Is r held? */
	s32 _start;		/**< Is start held? */
	s32 _select;	/**< Is select held? */
};

#endif
