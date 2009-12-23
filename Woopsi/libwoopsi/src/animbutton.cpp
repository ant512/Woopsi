#include "animbutton.h"
#include "graphicsport.h"
#include "woopsi.h"
#include "woopsitimer.h"

using namespace WoopsiUI;

// Set const values
const int AnimButton::ANIM_BUTTON_DEFAULT_WIDTH = 10;
const int AnimButton::ANIM_BUTTON_DEFAULT_HEIGHT = 10;

AnimButton::AnimButton(s16 x, s16 y, u16 width, u16 height, u16 animX, u16 animY) : Gadget(x, y, width, height, 0, NULL) {
	_outline = OUTLINE_CLICK_DEPENDENT;

	_animX = animX;
	_animY = animY;

	_animClicked = new Animation(0, Animation::ANIMATION_LOOPTYPE_LOOP, 0);
	_animNormal = new Animation(0, Animation::ANIMATION_LOOPTYPE_LOOP, 0);

	_initialised = false;

	_timer = new WoopsiTimer(1, true);
	addGadget(_timer);
	_timer->addGadgetEventHandler(this);
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
		if (isEnabled()) {
			port->drawBitmap(x, y, _animClicked->getCurrentBitmap()->getWidth(), _animClicked->getCurrentBitmap()->getHeight(), _animClicked->getCurrentBitmap(), _animX, _animY);
		} else {
			port->drawBitmapGreyScale(x, y, _animClicked->getCurrentBitmap()->getWidth(), _animClicked->getCurrentBitmap()->getHeight(), _animClicked->getCurrentBitmap(), _animX, _animY);
		}
	} else {
		if (isEnabled()) {
			port->drawBitmap(x, y, _animNormal->getCurrentBitmap()->getWidth(), _animNormal->getCurrentBitmap()->getHeight(), _animNormal->getCurrentBitmap(), _animX, _animY);
		} else {
			port->drawBitmapGreyScale(x, y, _animNormal->getCurrentBitmap()->getWidth(), _animNormal->getCurrentBitmap()->getHeight(), _animNormal->getCurrentBitmap(), _animX, _animY);
		}
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

void AnimButton::handleActionEvent(const GadgetEventArgs& e) {

	if (e.getSource() != NULL) {
		if (e.getSource() == _timer) {

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

			redraw();
		}
	}
}

bool AnimButton::click(s16 x, s16 y) {
	if (Gadget::click(x, y)) {

		if (isEnabled()) {

			// Swap animations
			_animNormal->stop();
			_animClicked->play();
		}

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
			_gadgetEventHandlers->raiseReleaseEvent(x, y);

			// Also raise "action" event
			_gadgetEventHandlers->raiseActionEvent(x, y, 0, 0, KEY_CODE_NONE);
		} else {
			// Release occurred outside gadget; raise release
			_gadgetEventHandlers->raiseReleaseOutsideEvent(x, y);
		}

		// Swap animations
		_animNormal->play();
		_animClicked->stop();

		return true;
	}

	return false;
}

// Get the preferred dimensions of the gadget
void AnimButton::getPreferredDimensions(Rect& rect) const {
	rect.x = _x;
	rect.y = _y;

	// Get the preferred size from the dimensions of one of the bitmaps in the anims
	if (_animNormal->getFrameCount() > 0) {

		// Get the size from the first bitmap in the normal animation
		rect.width = ((!_flags.borderless) << 1) + _animNormal->getCurrentBitmap()->getWidth();
		rect.height = ((!_flags.borderless) << 1) + _animNormal->getCurrentBitmap()->getHeight();
	} else if (_animClicked->getFrameCount() > 0) {

		// Get the size from the first bitmap in the clicked animation
		rect.width = ((!_flags.borderless) << 1) + _animClicked->getCurrentBitmap()->getWidth();
		rect.height = ((!_flags.borderless) << 1) + _animClicked->getCurrentBitmap()->getHeight();
	} else {

		// No bitmaps available - use defaults
		rect.width = ANIM_BUTTON_DEFAULT_WIDTH;
		rect.height = ANIM_BUTTON_DEFAULT_HEIGHT;
	}
}

bool AnimButton::disable() {
	if (Gadget::disable()) {

		// Pause running animations
		_animNormal->pause();
		_animClicked->pause();
		_timer->pause();

		return true;
	}

	return false;
}

bool AnimButton::enable() {
	if (Gadget::enable()) {
		
		// Restart animations
		if (_flags.clicked) {
			_animClicked->play();
		} else {
			_animNormal->play();
		}

		_timer->start();

		return true;
	}

	return false;
}
