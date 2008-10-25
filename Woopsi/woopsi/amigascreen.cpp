#include "amigascreen.h"
#include "decorationglyphbutton.h"
#include "screentitle.h"

using namespace WoopsiUI;

AmigaScreen::AmigaScreen(const char* title, u32 flags, u32 screenFlags, FontBase* font) : Screen(title, flags, font) {

	_titleHeight = SCREEN_TITLE_HEIGHT;

	_screenTitle = NULL;
	_depthButton = NULL;
	_flipButton = NULL;

	// Set screen flags
	_screenFlags.showFlipButton = (!(!(screenFlags & AMIGA_SCREEN_SHOW_FLIP)));
	_screenFlags.showDepthButton = (!(!(screenFlags & AMIGA_SCREEN_SHOW_DEPTH)));

	// Abuse the borderless system into setting the border up for us
	_flags.borderless = true;
	setBorderless(false);
}

void AmigaScreen::setBorderless(bool isBorderless) {
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
			_screenTitle = new ScreenTitle(_titleHeight, _title, _font);
			_screenTitle->setEventHandler(this);
			insertGadget(_screenTitle);

			s16 buttonX = _width;

			// Create depth button
			if (_screenFlags.showDepthButton) {
				_depthButton = new DecorationGlyphButton(buttonX - SCREEN_DEPTH_BUTTON_WIDTH, 0, SCREEN_DEPTH_BUTTON_WIDTH, _titleHeight, GLYPH_SCREEN_DEPTH_UP, GLYPH_SCREEN_DEPTH_DOWN, _font);
				_depthButton->setEventHandler(this);
				addGadget(_depthButton);

				buttonX -= SCREEN_DEPTH_BUTTON_WIDTH;
			}

			// Create flip button
			if (_screenFlags.showFlipButton) {
				_flipButton = new DecorationGlyphButton(buttonX - SCREEN_FLIP_BUTTON_WIDTH, 0, SCREEN_FLIP_BUTTON_WIDTH, _titleHeight, GLYPH_SCREEN_FLIP_UP, GLYPH_SCREEN_FLIP_DOWN, _font);
				_flipButton->setEventHandler(this);
				addGadget(_flipButton);
			}

			_flags.borderless = false;
		}

		invalidateVisibleRectCache();

		Gadget::draw();
	}
}

bool AmigaScreen::handleEvent(const EventArgs& e) {

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

void AmigaScreen::showFlipButton() {
	if ((!_flags.borderless) && (!_screenFlags.showFlipButton)) {
		_screenFlags.showFlipButton = true;

		// Adjust X co-ords to allow for existence/non-existence of depth button
		s16 buttonX = _width - SCREEN_FLIP_BUTTON_WIDTH;
		if (_screenFlags.showDepthButton) buttonX -= SCREEN_DEPTH_BUTTON_WIDTH;
		
		// Recreate flip button
		_flipButton = new DecorationGlyphButton(buttonX, 0, SCREEN_FLIP_BUTTON_WIDTH, _titleHeight, GLYPH_SCREEN_FLIP_UP, GLYPH_SCREEN_FLIP_DOWN, _font);
		_flipButton->setEventHandler(this);
		addGadget(_flipButton);

		_flipButton->draw();
	}
}

void AmigaScreen::showDepthButton() {
	if ((!_flags.borderless) && (!_screenFlags.showDepthButton)) {
		_screenFlags.showDepthButton = true;
		
		// Recreate depth button
		_depthButton = new DecorationGlyphButton(_width - SCREEN_DEPTH_BUTTON_WIDTH, 0, SCREEN_DEPTH_BUTTON_WIDTH, _titleHeight, GLYPH_SCREEN_DEPTH_UP, GLYPH_SCREEN_DEPTH_DOWN, _font);
		_depthButton->setEventHandler(this);
		addGadget(_depthButton);

		// Move the flip button if necessary
		if (_screenFlags.showFlipButton) {
			_flipButton->moveTo(_width - SCREEN_DEPTH_BUTTON_WIDTH - SCREEN_FLIP_BUTTON_WIDTH, 0);
		}
		
		_depthButton->draw();
	}
}

void AmigaScreen::hideFlipButton() {
	if ((!_flags.borderless) && (_screenFlags.showFlipButton)) {
		_screenFlags.showFlipButton = false;
		
		// Delete flip button
		_flipButton->close();
		_flipButton = NULL;
	}
}

void AmigaScreen::hideDepthButton() {
	if ((!_flags.borderless) && (_screenFlags.showDepthButton)) {
		_screenFlags.showDepthButton = false;
		
		// Delete close button
		_depthButton->close();
		_depthButton = NULL;

		// Move the flip button if necessary
		if (_screenFlags.showFlipButton) {
			_flipButton->moveTo(_width - SCREEN_FLIP_BUTTON_WIDTH, 0);
		}
	}
}
