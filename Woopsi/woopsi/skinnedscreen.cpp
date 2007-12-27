#include "woopsi.h"
#include "skinnedscreendepthbutton.h"
#include "skinnedscreenflipbutton.h"

SkinnedScreen::SkinnedScreen(char* title, const ScreenSkin* skin) : Screen(title, NULL) {

	_screenTitle = NULL;
	_screenDepthButton = NULL;
	_screenFlipButton = NULL;

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

	setBorderless(!_flags.borderless);
}

SkinnedScreen::~SkinnedScreen() {
}

void SkinnedScreen::setBorderless(bool isBorderless) {
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

			// Calculate button positions
			u16 flipX = _width - _skin->flipButton.bitmap.width;
			u16 depthX = _width - _skin->depthButton.bitmap.width;

			if (_skin->depthButton.visible) {
				flipX -= _skin->depthButton.bitmap.width;
			}

			_screenTitle = new SkinnedScreenTitle(_title, _skin);
			_screenFlipButton = new SkinnedScreenFlipButton(flipX, 0, _skin);
			_screenDepthButton = new SkinnedScreenDepthButton(depthX, 0, _skin);

			insertGadget(_screenFlipButton);
			insertGadget(_screenDepthButton);
			insertGadget(_screenTitle);

			_flags.borderless = false;
		}

		invalidateVisibleRectCache();

		draw();
	}
}
