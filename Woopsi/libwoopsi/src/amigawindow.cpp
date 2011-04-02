#include "amigawindow.h"	
#include "windowborderbutton.h"
#include "woopsi.h"

using namespace WoopsiUI;

AmigaWindow::AmigaWindow(s16 x, s16 y, u16 width, u16 height, const WoopsiString& title, u32 flags, bool showCloseButton, bool showDepthButton, GadgetStyle* style) : Window(x, y, width, height, title, flags, style) {

	_closeButton = NULL;
	_depthButton = NULL;

	// Set window flags
	_windowFlags.showCloseButton = showCloseButton;
	_windowFlags.showDepthButton = showDepthButton;

	// Add border to gadget list
	if (!(flags & GADGET_BORDERLESS)) {
		createBorder();
	}

	_borderSize.top = WINDOW_TITLE_HEIGHT;
	_borderSize.right = WINDOW_BORDER_SIZE;
	_borderSize.bottom = WINDOW_BORDER_SIZE;
	_borderSize.left = WINDOW_BORDER_SIZE;
}

void AmigaWindow::setBorderless(bool isBorderless) {
	if (isBorderless != _flags.borderless) {
		if (isBorderless) {

			// Close buttons if they are open
			if (_windowFlags.showCloseButton) _closeButton->close();
			if (_windowFlags.showDepthButton) _depthButton->close();

			// Reset pointers
			_closeButton = NULL;
			_depthButton = NULL;

			_flags.borderless = true;

			// Move all children to compensate
			for (s32 i = 0; i < _gadgets.size(); i++) {
				_gadgets[i]->moveTo(_gadgets[i]->getX() - getX() - WINDOW_BORDER_SIZE, _gadgets[i]->getY() - getY() - WINDOW_TITLE_HEIGHT);
			}
		} else {
			// Add borders

			// Move all children to compensate
			for (s32 i = 0; i < _gadgets.size(); i++) {
				_gadgets[i]->moveTo(_gadgets[i]->getX() - getX() + WINDOW_BORDER_SIZE, _gadgets[i]->getY() - getY() + WINDOW_TITLE_HEIGHT);
			}

			// Create borders
			createBorder();

			_flags.borderless = false;
		}

		invalidateVisibleRectCache();

		markRectsDamaged();
	}
}

void AmigaWindow::createBorder() {
	// Add gadgets to the start in reverse order

	// Add close button
	if (_windowFlags.showCloseButton) {
		_closeButton = new WindowBorderButton(0, 0, WINDOW_CLOSE_BUTTON_WIDTH, WINDOW_TITLE_HEIGHT, GLYPH_WINDOW_CLOSE, GLYPH_WINDOW_CLOSE, &_style);
		_closeButton->addGadgetEventHandler(this);
		insertGadget(_closeButton);
	}

	// Add depth button
	if (_windowFlags.showDepthButton) {
		_depthButton = new WindowBorderButton(getWidth() - WINDOW_DEPTH_BUTTON_WIDTH, 0, WINDOW_DEPTH_BUTTON_WIDTH, WINDOW_TITLE_HEIGHT, GLYPH_WINDOW_DEPTH_UP, GLYPH_WINDOW_DEPTH_DOWN, &_style);
		_depthButton->addGadgetEventHandler(this);
		insertGadget(_depthButton);
	}
}

void AmigaWindow::onFocus() {
	raiseToTop();
	markBorderDirty();
}

void AmigaWindow::markBorderDirty() {
	if (!isBorderless()) {
		
		// Left
		markRectDamaged(Rect(0, 0, _borderSize.left, getHeight()));
		
		// Right
		markRectDamaged(Rect(getWidth() - _borderSize.right, 0, _borderSize.right, getHeight()));
		
		// Top
		markRectDamaged(Rect(_borderSize.left, 0, getWidth() - _borderSize.left - _borderSize.right, _borderSize.top));
		
		// Bottom
		markRectDamaged(Rect(_borderSize.left, getHeight() - _borderSize.bottom, getWidth() - _borderSize.left - _borderSize.right, _borderSize.bottom));
	}	
}

void AmigaWindow::onBlur() {
	markBorderDirty();
}

void AmigaWindow::onResize(u16 width, u16 height) {

	// Depth button
	if (_windowFlags.showDepthButton) _depthButton->moveTo(getWidth() - WINDOW_DEPTH_BUTTON_WIDTH, 0);
}

void AmigaWindow::handleReleaseEvent(const GadgetEventArgs& e) {
	if (e.getSource() == NULL) return;

	if (e.getSource() == _depthButton) {
		swapDepth();
	} else if (e.getSource() == _closeButton) {
		close();
	}
}

void AmigaWindow::onClick(s16 x, s16 y) {
	if (y < getY() + _borderSize.top) {
		startDragging(x, y);
	}
}

void AmigaWindow::showCloseButton() {
	if ((!_flags.borderless) && (!_windowFlags.showCloseButton)) {
		_windowFlags.showCloseButton = true;
		
		// Recreate close button
		_closeButton = new WindowBorderButton(0, 0, WINDOW_CLOSE_BUTTON_WIDTH, WINDOW_TITLE_HEIGHT, GLYPH_WINDOW_CLOSE, GLYPH_WINDOW_CLOSE, &_style);
		_closeButton->addGadgetEventHandler(this);
		insertGadget(_closeButton);
	}
}

void AmigaWindow::showDepthButton() {
	if ((!_flags.borderless) && (!_windowFlags.showDepthButton)) {
		_windowFlags.showDepthButton = true;
		
		// Recreate depth button
		_depthButton = new WindowBorderButton(getWidth() - WINDOW_DEPTH_BUTTON_WIDTH, 0, WINDOW_DEPTH_BUTTON_WIDTH, WINDOW_TITLE_HEIGHT, GLYPH_WINDOW_DEPTH_UP, GLYPH_WINDOW_DEPTH_DOWN, &_style);
		_depthButton->addGadgetEventHandler(this);
		insertGadget(_depthButton);
	}
}

void AmigaWindow::hideCloseButton() {
	if ((!_flags.borderless) && (_windowFlags.showCloseButton)) {
		_windowFlags.showCloseButton = false;
		
		_closeButton->close();
		_closeButton = NULL;
	}
}

void AmigaWindow::hideDepthButton() {
	if ((!_flags.borderless) && (_windowFlags.showDepthButton)) {
		_windowFlags.showDepthButton = false;

		_depthButton->close();
		_depthButton = NULL;
	}
}

void AmigaWindow::drawBorder(GraphicsPort* port) {
	port->drawFilledRect(0, 0, getWidth(), getHeight(), getBackColour());						// Background
	port->drawBevelledRect(0, 0, getWidth(), getHeight(), getShineColour(), getShadowColour());	// Outline

	// Choose a fill colour depending on active state
	u16 colour = hasFocus() ? getHighlightColour() : getFillColour();

	// Draw top border
	port->drawFilledRect(1, 1, getWidth() - 2, WINDOW_TITLE_HEIGHT - 2, colour);	// Background

	s16 textX = 4;

	if (hasCloseButton()) {

		// Adjust text X co-ord to allow for presence of close button
		textX += _closeButton->getWidth();

		// Draw left edge
		port->drawLine(_closeButton->getWidth(), 1, _closeButton->getWidth(), _borderSize.top - 2, getShineColour());
	}

	if (hasDepthButton()) {

		// Draw right edge
		s16 lineX = getWidth() - _depthButton->getWidth() - 1;
		port->drawLine(lineX, 1, lineX, _borderSize.top - 2, getShadowColour());
	}

	// Line below title
	port->drawLine(1, _borderSize.top - 1, getWidth() - 3, _borderSize.top - 1, getShadowColour());

	// Draw title text
	port->drawText(textX, 1, getFont(), getTitle(), 0, getTitle().getLength(), getTextColour());

	port->drawFilledRect(1, _borderSize.top, _borderSize.left - 2, getHeight() - _borderSize.top - _borderSize.bottom, colour);	// Left
	port->drawFilledRect(getWidth() - WINDOW_BORDER_SIZE + 1, 1, WINDOW_BORDER_SIZE - 2, getHeight() - 2, colour);	// Right
	port->drawFilledRect(1, getHeight() - WINDOW_BORDER_SIZE + 1, getWidth() - 2, WINDOW_BORDER_SIZE - 2, colour);	// Bottom

	port->drawBevelledRect(WINDOW_BORDER_SIZE - 1, WINDOW_TITLE_HEIGHT - 1, getWidth() - (WINDOW_BORDER_SIZE * 2) + 2, getHeight() - WINDOW_BORDER_SIZE - WINDOW_TITLE_HEIGHT + 2, getShadowColour(), getShineColour());	// Inner outline
}
