#include "animbutton.h"
#include "graphicsport.h"
#include "woopsi.h"
#include "woopsitimer.h"

using namespace WoopsiUI;

// Set const values
const int AnimButton::ANIM_BUTTON_DEFAULT_WIDTH = 10;
const int AnimButton::ANIM_BUTTON_DEFAULT_HEIGHT = 10;

AnimButton::AnimButton(s16 x, s16 y, u16 width, u16 height, u16 animX, u16 animY) : Gadget(x, y, width, height, 0, NULL) {

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

void AnimButton::drawContents(GraphicsPort* port) {

	Animation* anim = isClicked() ? _animClicked : _animNormal;

	if (isEnabled()) {
		port->drawBitmap(0, 0, anim->getCurrentBitmap()->getWidth(), anim->getCurrentBitmap()->getHeight(), anim->getCurrentBitmap(), _animX, _animY);
	} else {
		port->drawBitmapGreyScale(0, 0, anim->getCurrentBitmap()->getWidth(), anim->getCurrentBitmap()->getHeight(), anim->getCurrentBitmap(), _animX, _animY);
	}
}

void AnimButton::drawBorder(GraphicsPort* port) {
	drawOutline(port);
}

void AnimButton::drawOutline(GraphicsPort* port) {

	// Stop drawing if the gadget indicates it should not have an outline
	if (isBorderless()) return;
	
	// Work out which colours to use
	u16 col1;
	u16 col2;
	
	if (isClicked()) {
		// Bevelled into the screen
		col1 = getShadowColour();
		col2 = getShineColour();
	} else {
		// Bevelled out of the screen
		col1 = getShineColour();
		col2 = getShadowColour();
	}
	
	port->drawBevelledRect(0, 0, getWidth(), getHeight(), col1, col2);
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

			markRectsDamaged();
		}
	}
}

void AnimButton::onClick(s16 x, s16 y) {

	// Swap animations
	_animNormal->stop();
	_animClicked->play();
}

void AnimButton::onRelease(s16 x, s16 y) {
	_gadgetEventHandlers->raiseActionEvent();

	// Swap animations
	_animNormal->play();
	_animClicked->stop();
}

void AnimButton::onReleaseOutside(s16 x, s16 y) {

	// Swap animations
	_animNormal->play();
	_animClicked->stop();
}

// Get the preferred dimensions of the gadget
void AnimButton::getPreferredDimensions(Rect& rect) const {
	rect.x = _rect.getX();
	rect.y = _rect.getY();
	rect.width = 0;
	rect.height = 0;

	// Add border sizes to width and height
	if (!_flags.borderless) {
		rect.width = _borderSize.left + _borderSize.right;
		rect.height = _borderSize.top + _borderSize.bottom;
	}

	// Get the preferred size from the dimensions of one of the bitmaps in the anims
	if (_animNormal->getFrameCount() > 0) {

		// Get the size from the first bitmap in the normal animation
		rect.width += _animNormal->getCurrentBitmap()->getWidth();
		rect.height += _animNormal->getCurrentBitmap()->getHeight();
	} else if (_animClicked->getFrameCount() > 0) {

		// Get the size from the first bitmap in the clicked animation
		rect.width += _animClicked->getCurrentBitmap()->getWidth();
		rect.height += _animClicked->getCurrentBitmap()->getHeight();
	} else {

		// No bitmaps available - use defaults
		rect.width = ANIM_BUTTON_DEFAULT_WIDTH;
		rect.height = ANIM_BUTTON_DEFAULT_HEIGHT;
	}
}

void AnimButton::onDisable() {
	
	// Pause running animations
	_animNormal->pause();
	_animClicked->pause();
	_timer->pause();
}

void AnimButton::onEnable() {
	
	// Restart animations
	if (_flags.clicked) {
		_animClicked->play();
	} else {
		_animNormal->play();
	}

	_timer->start();
}
