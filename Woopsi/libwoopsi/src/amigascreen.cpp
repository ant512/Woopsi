#include "amigascreen.h"
#include "decorationglyphbutton.h"
#include "screentitle.h"

using namespace WoopsiUI;

AmigaScreen::AmigaScreen(const char* title, u32 flags, u32 screenFlags, GadgetStyle* style) : Screen(title, flags, style) {

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
			_screenTitle = new ScreenTitle(_titleHeight, this, _style);
			_screenTitle->addGadgetEventHandler(this);
			insertGadget(_screenTitle);

			s16 buttonX = _width;

			// Create depth button
			if (_screenFlags.showDepthButton) {
				_depthButton = new DecorationGlyphButton(buttonX - SCREEN_DEPTH_BUTTON_WIDTH, 0, SCREEN_DEPTH_BUTTON_WIDTH, _titleHeight, GLYPH_SCREEN_DEPTH_UP, GLYPH_SCREEN_DEPTH_DOWN, _style);
				_depthButton->addGadgetEventHandler(this);
				addGadget(_depthButton);

				buttonX -= SCREEN_DEPTH_BUTTON_WIDTH;
			}

			// Create flip button
			if (_screenFlags.showFlipButton) {
				_flipButton = new DecorationGlyphButton(buttonX - SCREEN_FLIP_BUTTON_WIDTH, 0, SCREEN_FLIP_BUTTON_WIDTH, _titleHeight, GLYPH_SCREEN_FLIP_UP, GLYPH_SCREEN_FLIP_DOWN, _style);
				_flipButton->addGadgetEventHandler(this);
				addGadget(_flipButton);
			}

			_flags.borderless = false;
		}

		invalidateVisibleRectCache();

		redraw();
	}
}

void AmigaScreen::handleReleaseEvent(const GadgetEventArgs& e) {

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

void AmigaScreen::handleClickEvent(const GadgetEventArgs& e) {
	if (e.getSource() != NULL) {
		if (e.getSource() == _screenTitle) {
			setDragging(e.getX(), e.getY());
		}
	}
}

void AmigaScreen::handleDragEvent(const GadgetEventArgs& e) {
	if (e.getSource() != NULL) {
		if (e.getSource() == _screenTitle) {
			drag(e.getX(), e.getY(), e.getVX(), e.getVY());
		}
	}
}

void AmigaScreen::handleReleaseOutsideEvent(const GadgetEventArgs& e) {
	if (e.getSource() != NULL) {
		if (e.getSource() == _screenTitle) {
			release(e.getX(), e.getY());
		}
	}
}

void AmigaScreen::showFlipButton() {
	if ((!_flags.borderless) && (!_screenFlags.showFlipButton)) {
		_screenFlags.showFlipButton = true;

		// Adjust X co-ords to allow for existence/non-existence of depth button
		s16 buttonX = _width - SCREEN_FLIP_BUTTON_WIDTH;
		if (_screenFlags.showDepthButton) buttonX -= SCREEN_DEPTH_BUTTON_WIDTH;
		
		// Recreate flip button
		_flipButton = new DecorationGlyphButton(buttonX, 0, SCREEN_FLIP_BUTTON_WIDTH, _titleHeight, GLYPH_SCREEN_FLIP_UP, GLYPH_SCREEN_FLIP_DOWN, _style);
		_flipButton->addGadgetEventHandler(this);
		addGadget(_flipButton);

		_flipButton->redraw();
	}
}

void AmigaScreen::showDepthButton() {
	if ((!_flags.borderless) && (!_screenFlags.showDepthButton)) {
		_screenFlags.showDepthButton = true;
		
		// Recreate depth button
		_depthButton = new DecorationGlyphButton(_width - SCREEN_DEPTH_BUTTON_WIDTH, 0, SCREEN_DEPTH_BUTTON_WIDTH, _titleHeight, GLYPH_SCREEN_DEPTH_UP, GLYPH_SCREEN_DEPTH_DOWN, _style);
		_depthButton->addGadgetEventHandler(this);
		addGadget(_depthButton);

		// Move the flip button if necessary
		if (_screenFlags.showFlipButton) {
			_flipButton->moveTo(_width - SCREEN_DEPTH_BUTTON_WIDTH - SCREEN_FLIP_BUTTON_WIDTH, 0);
		}
		
		_depthButton->redraw();
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
