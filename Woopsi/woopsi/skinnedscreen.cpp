#include "skinnedscreendepthbutton.h"
#include "skinnedscreenflipbutton.h"
#include "skinnedscreentitle.h"
#include "skinnedscreen.h"

using namespace WoopsiUI;

SkinnedScreen::SkinnedScreen(const char* title, u32 flags, u32 screenFlags, const ScreenSkin* skin) : Screen(title, flags, NULL) {

	_screenTitle = NULL;
	_depthButton = NULL;
	_flipButton = NULL;

	_skin = skin;

	// Parse skin information
	_titleHeight = _skin->titleBar.bitmap.height;
	_flags.borderless = _skin->screen.borderless;
	_flags.permeable = _skin->screen.permeable;
	_font = _skin->screen.font.font;
	_backColour = _skin->screen.colours.back;
	_shineColour = _skin->screen.colours.shine;
	_highlightColour = _skin->screen.colours.highlight;
	_shadowColour = _skin->screen.colours.shadow;
	_fillColour = _skin->screen.colours.fill;

	_screenFlags.showFlipButton = _skin->flipButton.visible;
	_screenFlags.showDepthButton = _skin->depthButton.visible;

	setBorderless(!_flags.borderless);
}

void SkinnedScreen::setBorderless(bool isBorderless) {
	if (isBorderless != _flags.borderless) {
		if (isBorderless) {

			// Remove borders
			_screenTitle->close();
			if (_depthButton != NULL) _depthButton->close();
			if (_flipButton != NULL) _flipButton->close();

			_screenTitle = NULL;
			_depthButton = NULL;
			_flipButton = NULL;

			_flags.borderless = true;
		} else {
			// Add borders

			// Calculate button positions
			u16 flipX = _width - _skin->flipButton.bitmap.width;
			u16 depthX = _width - _skin->depthButton.bitmap.width;

			if (_skin->depthButton.visible) {
				flipX -= _skin->depthButton.bitmap.width;
			}

			_screenTitle = new SkinnedScreenTitle(this, _skin);
			_screenTitle->addGadgetEventHandler(this);
			insertGadget(_screenTitle);

			// Create depth button
			if (_screenFlags.showDepthButton) {
				_depthButton = new SkinnedScreenDepthButton(depthX, 0, _skin);
				_depthButton->addGadgetEventHandler(this);
				addGadget(_depthButton);
			}

			// Create flip button
			if (_screenFlags.showFlipButton) {
				_flipButton = new SkinnedScreenFlipButton(flipX, 0, _skin);
				_flipButton->addGadgetEventHandler(this);
				addGadget(_flipButton);
			}

			_flags.borderless = false;
		}

		invalidateVisibleRectCache();

		draw();
	}
}

void SkinnedScreen::handleReleaseEvent(const GadgetEventArgs& e) {
	if (e.getSource() != NULL) {

		// Process decoration gadgets only
		if (e.getSource() == _flipButton) {

			// Flip screens
			flipScreens();
		} else if (e.getSource() == _depthButton) {

			// Depth swap to bottom of stack
			lowerToBottom();
			blur();
		} else if (e.getSource() == _screenTitle) {

			release(e.getX(), e.getY());
		}
	}
}

void SkinnedScreen::handleClickEvent(const GadgetEventArgs& e) {
	if (e.getSource() != NULL) {
		if (e.getSource() == _screenTitle) {
			setDragging(e.getX(), e.getY());
		}
	}
}

void SkinnedScreen::handleDragEvent(const GadgetEventArgs& e) {
	if (e.getSource() != NULL) {
		if (e.getSource() == _screenTitle) {
			drag(e.getX(), e.getY(), e.getVX(), e.getVY());
		}
	}
}

void SkinnedScreen::handleReleaseOutsideEvent(const GadgetEventArgs& e) {
	if (e.getSource() != NULL) {
		if (e.getSource() == _screenTitle) {
			release(e.getX(), e.getY());
		}
	}
}

void SkinnedScreen::showFlipButton() {
	if ((!_flags.borderless) && (!_screenFlags.showFlipButton)) {
		_screenFlags.showFlipButton = true;

		// Calculate button position
		u16 flipX = _width - _skin->flipButton.bitmap.width;

		// Recreate flip button
		_flipButton = new SkinnedScreenFlipButton(flipX, 0, _skin);
		_flipButton->addGadgetEventHandler(this);
		addGadget(_flipButton);

		_flipButton->draw();
	}
}

void SkinnedScreen::showDepthButton() {
	if ((!_flags.borderless) && (!_screenFlags.showDepthButton)) {
		_screenFlags.showDepthButton = true;

		// Calculate button position
		u16 depthX = _width - _skin->depthButton.bitmap.width;
		
		// Recreate depth button
		_depthButton = new SkinnedScreenDepthButton(depthX, 0, _skin);
		_depthButton->addGadgetEventHandler(this);
		addGadget(_depthButton);

		_depthButton->draw();
	}
}

void SkinnedScreen::hideFlipButton() {
	if ((!_flags.borderless) && (_screenFlags.showFlipButton)) {
		_screenFlags.showFlipButton = false;
		
		// Delete flip button
		_flipButton->close();
		_flipButton = NULL;
	}
}

void SkinnedScreen::hideDepthButton() {
	if ((!_flags.borderless) && (_screenFlags.showDepthButton)) {
		_screenFlags.showDepthButton = false;
		
		// Delete close button
		_depthButton->close();
		_depthButton = NULL;
	}
}
