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

			_screenTitle = new SkinnedScreenTitle(_title, _skin);
			_screenTitle->setEventHandler(this);
			insertGadget(_screenTitle);

			// Create depth button
			if (_screenFlags.showDepthButton) {
				_depthButton = new SkinnedScreenDepthButton(depthX, 0, _skin);
				_depthButton->setEventHandler(this);
				addGadget(_depthButton);
			}

			// Create flip button
			if (_screenFlags.showFlipButton) {
				_flipButton = new SkinnedScreenFlipButton(flipX, 0, _skin);
				_flipButton->setEventHandler(this);
				addGadget(_flipButton);
			}

			_flags.borderless = false;
		}

		invalidateVisibleRectCache();

		draw();
	}
}

bool SkinnedScreen::handleEvent(const EventArgs& e) {
	if (e.gadget != NULL) {
		switch (e.type) {
			case EVENT_RELEASE:

				// Process decoration gadgets only
				if (e.gadget == _flipButton) {

					// Flip screens
					flipScreens();
					return true;
				} else if (e.gadget == _depthButton) {

					// Depth swap to bottom of stack
					lowerToBottom();
					blur();
					return true;
				} else if (e.gadget == _screenTitle) {

					release(e.eventX, e.eventY);
					return true;
				}
				break;

			case EVENT_CLICK:

				if (e.gadget == _screenTitle) {
					setDragging(e.eventX, e.eventY);
				}
				break;

			case EVENT_DRAG:

				if (e.gadget == _screenTitle) {
					drag(e.eventX, e.eventY, e.eventVX, e.eventVY);
					return true;
				}
				break;

			case EVENT_RELEASE_OUTSIDE:

				if (e.gadget == _screenTitle) {
					release(e.eventX, e.eventY);
					return true;
				}
				break;

			default:
				break;
		}
	}

	return false;
}

void SkinnedScreen::showFlipButton() {
	if ((!_flags.borderless) && (!_screenFlags.showFlipButton)) {
		_screenFlags.showFlipButton = true;

		// Calculate button position
		u16 flipX = _width - _skin->flipButton.bitmap.width;

		// Recreate flip button
		_flipButton = new SkinnedScreenFlipButton(flipX, 0, _skin);
		_flipButton->setEventHandler(this);
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
		_depthButton->setEventHandler(this);
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
