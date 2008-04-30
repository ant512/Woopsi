#include "amigascreen.h"
#include "decorationglyphbutton.h"
#include "screentitle.h"

AmigaScreen::AmigaScreen(char* title, u32 flags, u32 screenFlags, FontBase* font) : Screen(title, flags, font) {

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
			_depthButton->close();
			_flipButton->close();

			_screenTitle = NULL;
			_depthButton = NULL;
			_flipButton = NULL;

			_flags.borderless = true;
		} else {
			// Add borders
			_screenTitle = new ScreenTitle(_titleHeight, _title, _font);
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

	// Only handle release events
	if (e.type == EVENT_RELEASE) {

		// Was an interesting gadget released?
		// Ensure we check for NULL so we don't confuse a borderless
		// screen's NULL decorator pointers with any other events
		if (e.gadget != NULL) {

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
			}
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
