#include "animbutton.h"

AnimButton::AnimButton(s16 x, s16 y, u16 width, u16 height, u16 bitmapX, u16 bitmapY, u16 bitmapWidth, u16 bitmapHeight) : Textbox(x, y, width, height, "", NULL) {
	_outline = OUTLINE_CLICK_DEPENDENT;

	_bitmapWidth = bitmapWidth;
	_bitmapHeight = bitmapHeight;
	_bitmapX = bitmapX;
	_bitmapY = bitmapY;

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
		port->drawBitmap(x, y, _animClicked->getCurrentFrame()->width, _animClicked->getCurrentFrame()->height, _animClicked->getCurrentBitmap(), _bitmapX, _bitmapY, _bitmapWidth, _bitmapHeight);
	} else {
		port->drawBitmap(x, y, _animNormal->getCurrentFrame()->width, _animNormal->getCurrentFrame()->height, _animNormal->getCurrentBitmap(), _bitmapX, _bitmapY, _bitmapWidth, _bitmapHeight);
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
