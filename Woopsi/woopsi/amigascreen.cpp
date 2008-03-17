#include "amigascreen.h"
#include "decorationglyphbutton.h"
#include "screentitle.h"

AmigaScreen::AmigaScreen(char* title, FontBase* font) : Screen(title, font) {

	_titleHeight = 13;

	_screenTitle = NULL;
	_depthButton = NULL;
	_flipButton = NULL;

	_flags.borderless = true;
	setBorderless(false);
}

AmigaScreen::~AmigaScreen() {
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
			_flipButton = new DecorationGlyphButton(224, 0, 16, _titleHeight, GLYPH_SCREEN_FLIP_UP, GLYPH_SCREEN_FLIP_DOWN, _font);
			_depthButton = new DecorationGlyphButton(240, 0, 16, _titleHeight, GLYPH_SCREEN_DEPTH_UP, GLYPH_SCREEN_DEPTH_DOWN, _font);

			insertGadget(_screenTitle);
			insertGadget(_flipButton);
			insertGadget(_depthButton);

			// Set this as the decoration gadget event hander
			_flipButton->setEventHandler(this);
			_depthButton->setEventHandler(this);

			_flags.borderless = false;
		}

		invalidateVisibleRectCache();

		draw();
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
