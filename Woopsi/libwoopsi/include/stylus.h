#ifndef _STYLUS_H_
#define _STYLUS_H_

#include <nds.h>

/**
 * Abstraction of the DS' stylus state to aid porting.
 */
class Stylus {
public:

	static const s32 STYLUS_REPEAT_TIME = 10;	/**< VBLs before the stylus repeats. */

	/**
	 * Number of VBLs within which the user can click again to fire a
	 * double-click.
	 */
	static const s32 DOUBLE_CLICK_TIME = 8000;

	/**
	 * Constructor.
	 */
	inline Stylus() {
		_touchedTime = 0;
		_x = 0;
		_y = 0;
		_oldX = 0;
		_oldY = 0;
		_isDoubleClick = false;
		_doubleClickTimeout = 0;
	};

	/**
	 * Destructor.
	 */
	inline ~Stylus() { };

	/**
	 * Check if the stylus has been newly pressed down.
	 * @return True if the stylus has been newly pressed down in this VBL.
	 */
	inline bool isNewPress() const { return _touchedTime == 1; };

	/**
	 * Check if the stylus is held down.
	 * @return True if the stylus is held down.
	 */
	inline bool isHeld() const { return _touchedTime > 0; };

	/**
	 * Check if the stylus is newly released.
	 * @return True if the stylus is newly released.
	 */
	inline bool isReleased() const { return _touchedTime == -1; };

	/**
	 * Check if the stylus is repeating.  If the stylus has been held for a
	 * while it is occasionally useful to treat that as a repeat press.  If this
	 * method returns true, then a repeat action can be triggered.
	 * @return True if the stylus is triggering a repeat.
	 */
	inline bool isRepeat() const { return _touchedTime > 0 && _touchedTime % STYLUS_REPEAT_TIME == 0; };

	/**
	 * Check if the current click was a double-click.
	 * @return True if the last click was a double-click.
	 */
	inline bool isDoubleClick() const { return _isDoubleClick; };

	/**
	 * Get the x co-ordinate of the stylus.
	 * @return The x co-ordinate of the stylus.
	 */
	inline s16 getX() const { return _x; };

	/**
	 * Get the y co-ordinate of the stylus.
	 * @return The y co-ordinate of the stylus.
	 */
	inline s16 getY() const { return _y; };

	/**
	 * Get the x distance moved.
	 * @return The x distance moved.
	 */
	inline s16 getVX() const { return _vX; };

	/**
	 * Get the y distance moved.
	 * @return The y distance moved.
	 */
	inline s16 getVY() const { return _vY; };

	/**
	 * Update the stylus' state to match the latest DS state.  The libnds
	 * function scanKeys() must be called before this method.
	 */
	void update() {

		// If we released on the previous iteration, we need to reset to 0
		if (_touchedTime == -1) ++_touchedTime;

		if (_doubleClickTimeout > 0) --_doubleClickTimeout;

#ifndef USING_SDL

		s32 pressed = keysDown();	// buttons pressed this loop
		s32 held = keysHeld();		// buttons currently held
		s32 allKeys = pressed | held;

		if (allKeys & KEY_TOUCH) {

			// Stylus is held
			++_touchedTime;

			if (_touchedTime == 1) {
				_isDoubleClick = _doubleClickTimeout > 0;
				_doubleClickTimeout = DOUBLE_CLICK_TIME;
			}
		} else if (_touchedTime > 0) {

			// Stylus is released
			_touchedTime = -1;
			_doubleClickTimeout = 0;
		}

		touchPosition touch;
		touchRead(&touch);

		if (_touchedTime > 0) {
			_oldX = _x;
			_oldY = _y;

			_x = touch.px;
			_y = touch.py;

			_vX = _x - _oldX;
			_vY = _y - _oldY;
		}

#else

		// Read mouse state
		int mouseX;
		int mouseY;
		
		int mouseState = SDL_GetMouseState(&mouseX, &mouseY);
		
		// Check buttons
		if (mouseState & SDL_BUTTON_LEFT) {

			// Stylus is held
			++_touchedTime;

			if (_touchedTime == 1) {
				_isDoubleClick = _doubleClickTimeout > 0;
				_doubleClickTimeout = DOUBLE_CLICK_TIME;
			}
		} else if (_touchedTime > 0) {

			// Stylus is released
			_touchedTime = -1;
			_doubleClickTimeout = 0;
		}
		
		if (_touchedTime > 0) {
			_oldX = _x;
			_oldY = _y;

			_x = mouseX;
			_y = mouseY - SCREEN_HEIGHT;

			_vX = _x - _oldX;
			_vY = _y - _oldY;
		}

#endif
	};

private:
	s32 _touchedTime;			/**< Time that stylus has been pressed */
	s16 _x;						/**< X co-ord of the stylus */
	s16 _y;						/**< Y co-ord of the stylus */
	s16 _oldX;					/**< X co-ord of the previous stylus press */
	s16 _oldY;					/**< Y co-ord of the previous stylus press */
	s16 _vX;					/**< X distance moved */
	s16 _vY;					/**< Y distance moved */
	s32 _doubleClickTimeout;	/**< Time until double-clicks aren't detected */
	s32 _isDoubleClick;			/**< Set if double-click is detected */
};

#endif
