#include "amigascreen.h"
#include "screendepthbutton.h"
#include "screenflipbutton.h"
#include "screentitle.h"

AmigaScreen::AmigaScreen(char* title, FontBase* font) : Screen(title, font) {

	_titleHeight = 13;

	_screenTitle = NULL;
	_screenDepthButton = NULL;
	_screenFlipButton = NULL;

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
			_screenDepthButton->close();
			_screenFlipButton->close();

			_screenTitle = NULL;
			_screenDepthButton = NULL;
			_screenFlipButton = NULL;

			_flags.borderless = true;
		} else {
			// Add borders
			_screenTitle = new ScreenTitle(_titleHeight, _title, _font);
			_screenFlipButton = new ScreenFlipButton(224, 0, 16, _titleHeight, _font);
			_screenDepthButton = new ScreenDepthButton(240, 0, 16, _titleHeight, _font);

			insertGadget(_screenTitle);
			insertGadget(_screenFlipButton);
			insertGadget(_screenDepthButton);

			// Set this as the decoration gadget event hander
			_screenFlipButton->setEventHandler(this);
			_screenDepthButton->setEventHandler(this);

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
			if (e.gadget == _screenFlipButton) {

				// Flip screens
				flipScreens();
				return true;
			} else if (e.gadget == _screenDepthButton) {

				// Depth swap to bottom of stack
				lowerToBottom();
				blur();
				return true;
			}
		}
	}

	return false;
}
