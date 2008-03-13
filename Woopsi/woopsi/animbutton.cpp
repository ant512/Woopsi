#include "animbutton.h"

AnimButton::AnimButton(s16 x, s16 y, u16 width, u16 height, u16 animX, u16 animY) : Textbox(x, y, width, height, "", NULL) {
	_outline = OUTLINE_CLICK_DEPENDENT;

	_animX = animX;
	_animY = animY;

	_animClicked = new Animation(0, Animation::ANIMATION_LOOPTYPE_LOOP, 0);
	_animNormal = new Animation(0, Animation::ANIMATION_LOOPTYPE_LOOP, 0);

	_initialised = false;
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

bool AnimButton::vbl() {

	if (Gadget::vbl()) {

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

	return false;
}

bool AnimButton::click(s16 x, s16 y) {
	if (Gadget::click(x, y)) {
		_animNormal->stop();
		_animClicked->play();

		return true;
	}

	return false;
}

bool AnimButton::release(s16 x, s16 y) {
	if (Gadget::release(x, y)) {
		_animNormal->play();
		_animClicked->stop();

		return true;
	}

	return false;
}
