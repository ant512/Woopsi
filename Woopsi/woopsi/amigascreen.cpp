#include "woopsi.h"
#include "screendepthbutton.h"
#include "screenflipbutton.h"

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

			_flags.borderless = false;
		}

		invalidateVisibleRectCache();

		draw();
	}
}
