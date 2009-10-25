#include "animation.h"

using namespace WoopsiUI;

// Constructor
// loops = 0 means the animation loops infinitely
Animation::Animation(const u8 speed, const LoopType loopType, const u16 loops) {
	_speed = speed;
	_frameTimer = speed;
	_currentFrame = 0;
	_status = ANIMATION_STATUS_STOPPED;
	_loopType = loopType;
	_frameInc = 1;
	_requestedLoops = loops;
	_loopCount = 0;
}

void Animation::addFrame(const u16* bitmap, const u16 width, const u16 height, const u8 delay) {
	
	// Create new animation frame
	AnimFrame newFrame;
	newFrame.bitmap = bitmap;
	newFrame.width = width;
	newFrame.height = height;
	newFrame.delay = delay;

	// Append frame to end of frames vector
	_frames.push_back(newFrame);
}

// Main function - should be called every VBL for animation to work properly
void Animation::run() {
	
	// Is the animation playing?
	if (_status == ANIMATION_STATUS_PLAYING) {

		// Need to show a new frame?
		if (_frameTimer > 0) {
			_frameTimer--;
		} else {
			// New frame due
			if (((_currentFrame == _frames.size() - 1) && (_frameInc > 0)) || ((_currentFrame == 0) && (_frameInc < 0))) {

				// Hit the end of the animation; try to loop
				loop();
			} else {

				// Advance to next frame
				_currentFrame += _frameInc;
				_frameTimer = _speed + _frames[_currentFrame].delay;
			}
		}
	}
}

// Loop the animation if possible
bool Animation::loop() {
	// Loop
	switch (_loopType) {
		case ANIMATION_LOOPTYPE_NONE:
			// Stop the animation
			stop();

			// Let the caller know that the loop failed
			return false;

		case ANIMATION_LOOPTYPE_LOOP:
			// Standard loop
			_currentFrame = 0;
			_loopCount++;
			break;

		case ANIMATION_LOOPTYPE_PINGPONG:
			// Pingpong
			_frameInc = 0 - _frameInc;
			_currentFrame += _frameInc;
			_loopCount++;
			break;
	}

	// Have we exceeded the number of loops?
	if ((_requestedLoops > 0) && (_loopCount > _requestedLoops)) {
		stop();
		return false;
	}

	// Reset frame timer to standard time plus frame delay
	_frameTimer = _speed + _frames[_currentFrame].delay;

	// Loop successful
	return true;
}

// Play the animation
void Animation::play() {
	
	// Do we need to reset the current frame?
	if (_status == ANIMATION_STATUS_STOPPED) {
		_currentFrame = 0;
	}

	_status = ANIMATION_STATUS_PLAYING;
	_frameTimer = _speed + _frames[_currentFrame].delay;
}

// Stop the animation - does not reset the current frame as doing so
// will cause the animation to jump back to the first frame when it ends
void Animation::stop() {
	_status = ANIMATION_STATUS_STOPPED;
	_frameTimer = 0;
	_loopCount = 0;
}

// Jump to a new frame
void Animation::goToFrame(u16 frame) {
	_currentFrame = frame;
	_frameTimer = _speed + _frames[_currentFrame].delay;
}
