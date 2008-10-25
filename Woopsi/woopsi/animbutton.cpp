#include "animbutton.h"
#include "graphicsport.h"
#include "woopsi.h"
#include "woopsitimer.h"

using namespace WoopsiUI;

AnimButton::AnimButton(s16 x, s16 y, u16 width, u16 height, u16 animX, u16 animY) : Gadget(x, y, width, height, 0, NULL) {
	_outline = OUTLINE_CLICK_DEPENDENT;

	_animX = animX;
	_animY = animY;

	_animClicked = new Animation(0, Animation::ANIMATION_LOOPTYPE_LOOP, 0);
	_animNormal = new Animation(0, Animation::ANIMATION_LOOPTYPE_LOOP, 0);

	_initialised = false;

	_timer = new WoopsiTimer(1, true);
	addGadget(_timer);
	_timer->setEventHandler(this);
	_timer->start();
}

AnimButton::~AnimButton() {
	delete _animClicked;
	delete _animNormal;
}

void AnimButton::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	s16 x = !_flags.borderless;
	s16 y = x;

	// Draw bitmaps
	if (_flags.clicked) {
		port->drawBitmap(x, y, _animClicked->getCurrentFrame()->width, _animClicked->getCurrentFrame()->height, _animClicked->getCurrentBitmap(), _animX, _animY, _animClicked->getCurrentFrame()->width, _animClicked->getCurrentFrame()->height);
	} else {
		port->drawBitmap(x, y, _animNormal->getCurrentFrame()->width, _animNormal->getCurrentFrame()->height, _animNormal->getCurrentBitmap(), _animX, _animY, _animClicked->getCurrentFrame()->width, _animClicked->getCurrentFrame()->height);
	}

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;
}

Animation* const AnimButton::getNormalAnimation() {
	return _animNormal;
}

Animation* const AnimButton::getClickedAnimation() {
	return _animClicked;
}

bool AnimButton::handleEvent(const EventArgs& e) {

	if (e.gadget != NULL) {
		if (e.gadget == _timer) {
			if (e.type == EVENT_ACTION) {

				// Ensure the animations are running
				if (!_initialised) {
					_animNormal->play();
					_initialised = true;
				}

				// Run the animations
				if (_flags.clicked) {
					_animClicked->run();
				} else {
					_animNormal->run();
				}

				Gadget::draw();

				return true;
			}
		}
	}

	return false;
}

bool AnimButton::click(s16 x, s16 y) {
	if (Gadget::click(x, y)) {

		// Swap animations
		_animNormal->stop();
		_animClicked->play();

		return true;
	}

	return false;
}

bool AnimButton::release(s16 x, s16 y) {

	if (_flags.clicked) {
		_flags.clicked = false;
		_flags.dragging = false;

		if (woopsiApplication->getClickedGadget() == this) {
			woopsiApplication->setClickedGadget(NULL);
		}

		// Determine which release event to fire
		if (checkCollision(x, y)) {
			// Release occurred within gadget; raise release
			raiseReleaseEvent(x, y);

			// Also raise "action" event
			raiseActionEvent(x, y, 0, 0, KEY_CODE_NONE);
		} else {
			// Release occurred outside gadget; raise release
			raiseReleaseOutsideEvent(x, y);
		}

		// Swap animations
		_animNormal->play();
		_animClicked->stop();

		return true;
	}

	return false;
}
