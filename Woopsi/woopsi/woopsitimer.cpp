#include "woopsitimer.h"
#include "woopsi.h"

WoopsiTimer::WoopsiTimer(u32 timeout, bool repeat) : Gadget(0, 0, 0, 0, 0) {
	_timeout = timeout;
	_isRepeater = repeat;
	_frameCount = 0;
	_isRunning = false;

	woopsiApplication->registerForVBL(this);
}

bool WoopsiTimer::vbl() {
	if (_isRunning) {
		_frameCount++;

		if (_frameCount >= _timeout) {
			if (!_isRepeater) {
				stop();
			} else {
				_frameCount = 0;
			}

			raiseActionEvent(0, 0, 0, 0, KEY_CODE_NONE);

			return true;
		}
	}

	return false;
}
