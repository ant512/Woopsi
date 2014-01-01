#include "amigascreen.h"
#include "decorationglyphbutton.h"
#include "graphicsport.h"

using namespace WoopsiUI;

AmigaScreen::AmigaScreen(const WoopsiString& title, bool showFlipButton, bool showDepthButton, GadgetStyle* style) : Screen(title, style) {

	_flags.draggable = true;

	_titleHeight = SCREEN_TITLE_HEIGHT;

	_depthButton = NULL;
	_flipButton = NULL;

	// Set screen flags
	_screenFlags.showFlipButton = showFlipButton;
	_screenFlags.showDepthButton = showDepthButton;

	// Abuse the borderless system into setting the border up for us
	_flags.borderless = true;
	setBorderless(false);
}

void AmigaScreen::setBorderless(bool isBorderless) {
	if (isBorderless == _flags.borderless) return;

	if (isBorderless) {

		// Remove borders
		if (_depthButton != NULL) _depthButton->close();
		if (_flipButton != NULL) _flipButton->close();

		_depthButton = NULL;
		_flipButton = NULL;

		_flags.borderless = true;
	} else {
		// Add borders
		s16 buttonX = _rect.getWidth();

		// Create depth button
		if (_screenFlags.showDepthButton) {
			_depthButton = new DecorationGlyphButton(buttonX - SCREEN_DEPTH_BUTTON_WIDTH, 0, SCREEN_DEPTH_BUTTON_WIDTH, _titleHeight, GLYPH_SCREEN_DEPTH_UP, GLYPH_SCREEN_DEPTH_DOWN, &_style);
			_depthButton->setGadgetEventHandler(this);
			addGadget(_depthButton);

			buttonX -= SCREEN_DEPTH_BUTTON_WIDTH;
		}

		// Create flip button
		if (_screenFlags.showFlipButton) {
			_flipButton = new DecorationGlyphButton(buttonX - SCREEN_FLIP_BUTTON_WIDTH, 0, SCREEN_FLIP_BUTTON_WIDTH, _titleHeight, GLYPH_SCREEN_FLIP_UP, GLYPH_SCREEN_FLIP_DOWN, &_style);
			_flipButton->setGadgetEventHandler(this);
			addGadget(_flipButton);
		}

		_flags.borderless = false;
	}

	invalidateVisibleRectCache();

	markRectsDamaged();
}

void AmigaScreen::handleReleaseEvent(Gadget &source, const WoopsiPoint &point) {

	// Process decoration gadgets only
	if (&source == _flipButton) {

		// Flip screens
		flipScreens();
	} else if (&source == _depthButton) {

		// Depth swap to bottom of stack
		if (lowerToBottom()) blur();
	}
}

void AmigaScreen::onClick(s16 x, s16 y) {
	if (isBorderless()) return;

	if (y - getY() < SCREEN_TITLE_HEIGHT) {
		startDragging(x, y);
	}
}

void AmigaScreen::showFlipButton() {
	if ((!_flags.borderless) && (!_screenFlags.showFlipButton)) {
		_screenFlags.showFlipButton = true;

		// Adjust X co-ords to allow for existence/non-existence of depth button
		s16 buttonX = getWidth() - SCREEN_FLIP_BUTTON_WIDTH;
		if (_screenFlags.showDepthButton) buttonX -= SCREEN_DEPTH_BUTTON_WIDTH;
		
		// Recreate flip button
		_flipButton = new DecorationGlyphButton(buttonX, 0, SCREEN_FLIP_BUTTON_WIDTH, _titleHeight, GLYPH_SCREEN_FLIP_UP, GLYPH_SCREEN_FLIP_DOWN, &_style);
		_flipButton->setGadgetEventHandler(this);
		addGadget(_flipButton);
	}
}

void AmigaScreen::showDepthButton() {
	if ((!_flags.borderless) && (!_screenFlags.showDepthButton)) {
		_screenFlags.showDepthButton = true;
		
		// Recreate depth button
		_depthButton = new DecorationGlyphButton(getWidth() - SCREEN_DEPTH_BUTTON_WIDTH, 0, SCREEN_DEPTH_BUTTON_WIDTH, _titleHeight, GLYPH_SCREEN_DEPTH_UP, GLYPH_SCREEN_DEPTH_DOWN, &_style);
		_depthButton->setGadgetEventHandler(this);
		addGadget(_depthButton);

		// Move the flip button if necessary
		if (_screenFlags.showFlipButton) {
			_flipButton->moveTo(getWidth() - SCREEN_DEPTH_BUTTON_WIDTH - SCREEN_FLIP_BUTTON_WIDTH, 0);
		}
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
			_flipButton->moveTo(getWidth() - SCREEN_FLIP_BUTTON_WIDTH, 0);
		}
	}
}

void AmigaScreen::drawBorder(GraphicsPort* port) {

	// Background
	port->drawFilledRect(0, 0, getWidth(), getHeight(), getBackColour());

	// Title bar
	if (!isBorderless()) {
		port->drawFilledRect(0, 0, getWidth(), _titleHeight - 1, getShineColour());					// Background
		port->drawLine(0, _titleHeight - 1, getWidth() - 1, _titleHeight - 1, getShadowColour());	// Bottom
		port->drawText(2, 1, getFont(), getTitle(), 0, getTitle().getLength(), getTextColour());	// Title text
	}
}
