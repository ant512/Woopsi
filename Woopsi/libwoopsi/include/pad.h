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
	 * Destructor.
	 */
	inline ~Pad() { };

	/**
	 * Check if up is newly pressed since the last call to update().
	 * @return True if newly pressed.
	 */
	inline bool isUpNewPress() const { return _up == 1; };

	/**
	 * Check if down is newly pressed since the last call to update().
	 * @return True if newly pressed.
	 */
	inline bool isDownNewPress() const { return _down == 1; };

	/**
	 * Check if left is newly pressed since the last call to update().
	 * @return True if newly pressed.
	 */
	inline bool isLeftNewPress() const { return _left == 1; };

	/**
	 * Check if right is newly pressed since the last call to update().
	 * @return True if newly pressed.
	 */
	inline bool isRightNewPress() const { return _right == 1; };

	/**
	 * Check if A is newly pressed since the last call to update().
	 * @return True if newly pressed.
	 */
	inline bool isANewPress() const { return _a == 1; };

	/**
	 * Check if B is newly pressed since the last call to update().
	 * @return True if newly pressed.
	 */
	inline bool isBNewPress() const { return _b == 1; };

	/**
	 * Check if X is newly pressed since the last call to update().
	 * @return True if newly pressed.
	 */
	inline bool isXNewPress() const { return _x == 1; };

	/**
	 * Check if Y is newly pressed since the last call to update().
	 * @return True if newly pressed.
	 */
	inline bool isYNewPress() const { return _y == 1; };

	/**
	 * Check if L is newly pressed since the last call to update().
	 * @return True if newly pressed.
	 */
	inline bool isLNewPress() const { return _l == 1; };

	/**
	 * Check if R is newly pressed since the last call to update().
	 * @return True if newly pressed.
	 */
	inline bool isRNewPress() const { return _r == 1; };

	/**
	 * Check if start is newly pressed since the last call to update().
	 * @return True if newly pressed.
	 */
	inline bool isStartNewPress() const { return _start == 1; };

	/**
	 * Check if select is newly pressed since the last call to update().
	 * @return True if newly pressed.
	 */
	inline bool isSelectNewPress() const { return _select == 1; };

	/**
	 * Check if up is newly released since the last call to update().
	 * @return True if newly released.
	 */
	inline bool isUpReleased() const { return _up == -1; };

	/**
	 * Check if down is newly released since the last call to update().
	 * @return True if newly released.
	 */
	inline bool isDownReleased() const { return _down == -1; };

	/**
	 * Check if left is newly released since the last call to update().
	 * @return True if newly released.
	 */
	inline bool isLeftReleased() const { return _left == -1; };

	/**
	 * Check if right is newly released since the last call to update().
	 * @return True if newly released.
	 */
	inline bool isRightReleased() const { return _right == -1; };

	/**
	 * Check if A is newly released since the last call to update().
	 * @return True if newly released.
	 */
	inline bool isAReleased() const { return _a == -1; };

	/**
	 * Check if B is newly released since the last call to update().
	 * @return True if newly released.
	 */
	inline bool isBReleased() const { return _b == -1; };

	/**
	 * Check if X is newly released since the last call to update().
	 * @return True if newly released.
	 */
	inline bool isXReleased() const { return _x == -1; };

	/**
	 * Check if Y is newly released since the last call to update().
	 * @return True if newly released.
	 */
	inline bool isYReleased() const { return _y == -1; };

	/**
	 * Check if L is newly released since the last call to update().
	 * @return True if newly released.
	 */
	inline bool isLReleased() const { return _l == -1; };

	/**
	 * Check if R is newly released since the last call to update().
	 * @return True if newly released.
	 */
	inline bool isRReleased() const { return _r == -1; };

	/**
	 * Check if start is newly released since the last call to update().
	 * @return True if newly released.
	 */
	inline bool isStartReleased() const { return _start == -1; };

	/**
	 * Check if select is newly released since the last call to update().
	 * @return True if newly released.
	 */
	inline bool isSelectReleased() const { return _select == -1; };

	/**
	 * Check if up is held down.
	 * @return True if held down.
	 */
	inline bool isUpHeld() const { return _up > 0; };

	/**
	 * Check if down is held down.
	 * @return True if held down.
	 */
	inline bool isDownHeld() const { return _down > 0; };

	/**
	 * Check if left is held down.
	 * @return True if held down.
	 */
	inline bool isLeftHeld() const { return _left > 0; };

	/**
	 * Check if right is held down.
	 * @return True if held down.
	 */
	inline bool isRightHeld() const { return _right > 0; };

	/**
	 * Check if A is held down.
	 * @return True if held down.
	 */
	inline bool isAHeld() const { return _a > 0; };

	/**
	 * Check if B is held down.
	 * @return True if held down.
	 */
	inline bool isBHeld() const { return _b > 0; };

	/**
	 * Check if X is held down.
	 * @return True if held down.
	 */
	inline bool isXHeld() const { return _x > 0; };

	/**
	 * Check if Y is held down.
	 * @return True if held down.
	 */
	inline bool isYHeld() const { return _y > 0; };

	/**
	 * Check if L is held down.
	 * @return True if held down.
	 */
	inline bool isLHeld() const { return _l > 0; };

	/**
	 * Check if R is held down.
	 * @return True if held down.
	 */
	inline bool isRHeld() const { return _r > 0; };

	/**
	 * Check if start is held down.
	 * @return True if held down.
	 */
	inline bool isStartHeld() const { return _start > 0; };

	/**
	 * Check if select is held down.
	 * @return True if held down.
	 */
	inline bool isSelectHeld() const { return _select > 0; };

	/**
	 * Check if up is repeating.
	 * @return True if repeating.
	 */
	inline bool isUpRepeat() const { return _up > 0 && _up % PAD_REPEAT_TIME == 0; };

	/**
	 * Check if down is repeating.
	 * @return True if repeating.
	 */
	inline bool isDownRepeat() const { return _down > 0 && _down % PAD_REPEAT_TIME == 0; };

	/**
	 * Check if left is repeating.
	 * @return True if repeating.
	 */
	inline bool isLeftRepeat() const { return _left > 0 && _left % PAD_REPEAT_TIME == 0; };

	/**
	 * Check if right is repeating.
	 * @return True if repeating.
	 */
	inline bool isRightRepeat() const { return _right > 0 && _right % PAD_REPEAT_TIME == 0; };

	/**
	 * Check if A is repeating.
	 * @return True if repeating.
	 */
	inline bool isARepeat() const { return _a > 0 && _a % PAD_REPEAT_TIME == 0; };

	/**
	 * Check if B is repeating.
	 * @return True if repeating.
	 */
	inline bool isBRepeat() const { return _b > 0 && _b % PAD_REPEAT_TIME == 0; };

	/**
	 * Check if X is repeating.
	 * @return True if repeating.
	 */
	inline bool isXRepeat() const { return _x > 0 && _x % PAD_REPEAT_TIME == 0; };

	/**
	 * Check if Y is repeating.
	 * @return True if repeating.
	 */
	inline bool isYRepeat() const { return _y > 0 && _y % PAD_REPEAT_TIME == 0; };

	/**
	 * Check if L is repeating.
	 * @return True if repeating.
	 */
	inline bool isLRepeat() const { return _l > 0 && _l % PAD_REPEAT_TIME == 0; };

	/**
	 * Check if R is repeating.
	 * @return True if repeating.
	 */
	inline bool isRRepeat() const { return _r > 0 && _r % PAD_REPEAT_TIME == 0; };

	/**
	 * Check if start is repeating.
	 * @return True if repeating.
	 */
	inline bool isStartRepeat() const { return _start > 0 && _start % PAD_REPEAT_TIME == 0; };

	/**
	 * Check if select is repeating.
	 * @return True if repeating.
	 */
	inline bool isSelectRepeat() const { return _select > 0 && _select % PAD_REPEAT_TIME == 0; };

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
