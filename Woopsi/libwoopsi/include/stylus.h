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
	 * Constructor.
	 */
	inline Stylus() {
		_touch = 0;
		_x = 0;
		_y = 0;
	};

	/**
	 * Destructor.
	 */
	inline ~Stylus() { };

	/**
	 * Check if the stylus has been newly pressed down.
	 * @return True if the stylus has been newly pressed down in this VBL.
	 */
	inline bool isNewPress() const { return _touch == 1; };

	/**
	 * Check if the stylus is held down.
	 * @return True if the stylus is held down.
	 */
	inline bool isHeld() const { return _touch > 0; };

	/**
	 * Check if the stylus is newly released.
	 * @return True if the stylus is newly released.
	 */
	inline bool isReleased() const { return _touch == -1; };

	/**
	 * Check if the stylus is repeating.  If the stylus has been held for a
	 * while it is occasionally useful to treat that as a repeat press.  If this
	 * method returns true, then a repeat action can be triggered.
	 * @return True if the stylus is triggering a repeat.
	 */
	inline bool isRepeat() const { return _touch > 0 && _touch % STYLUS_REPEAT_TIME == 0; };

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
	 * Update the stylus' state to match the latest DS state.  The libnds
	 * function scanKeys() must be called before this method.
	 */
	void update() {

		// If we released _touch on the previous iteration, we need to reset it
		// to 0
		if (_touch == -1) ++_touch;

#ifndef USING_SDL

		s32 pressed = keysDown();	// buttons pressed this loop
		s32 held = keysHeld();		// buttons currently held
		s32 allKeys = pressed | held;

		if (allKeys & KEY_TOUCH) {

			// Stylus is held
			++_touch;
		} else if (_touch > 0) {

			// Stylus is released
			_touch = -1;
		}

		touchPosition touch;
		touchRead(&touch);

		if (_touch > 0) {
			_x = touch.px;
			_y = touch.py;
		}

#else

		// Read mouse state
		int mouseX;
		int mouseY;
		
		int mouseState = SDL_GetMouseState(&mouseX, &mouseY);
		
		// Check buttons
		if (mouseState & SDL_BUTTON_LEFT) {

			// Stylus is held
			++_touch;
		} else if (_touch > 0) {

			// Stylus is released
			_touch = -1;
		}
		
		if (_touch > 0) {
			_x = mouseX;
			_y = mouseY - SCREEN_HEIGHT;
		}

#endif
	};

private:
	s32 _touch;			/**< Stylus has been newly pressed */
	s16 _x;				/**< X co-ord of the stylus */
	s16 _y;				/**< Y co-ord of the stylus */
};

#endif
