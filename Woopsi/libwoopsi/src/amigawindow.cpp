#include "amigawindow.h"	
#include "windowbordertop.h"
#include "windowborderside.h"
#include "windowborderbottom.h"
#include "windowborderbutton.h"
#include "woopsi.h"

using namespace WoopsiUI;

AmigaWindow::AmigaWindow(s16 x, s16 y, u16 width, u16 height, const WoopsiString& title, u32 flags, u32 windowFlags, GadgetStyle* style) : Window(x, y, width, height, title, flags, style) {

	_windowBorderTop = NULL;
	_windowBorderLeft = NULL;
	_windowBorderRight = NULL;
	_windowBorderBottom = NULL;
	_closeButton = NULL;
	_depthButton = NULL;

	// Set window flags
	_windowFlags.showCloseButton = (!(!(windowFlags & AMIGA_WINDOW_SHOW_CLOSE)));
	_windowFlags.showDepthButton = (!(!(windowFlags & AMIGA_WINDOW_SHOW_DEPTH)));

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
			// Remove borders

			// Close buttons if they are open
			if (_windowFlags.showCloseButton) _closeButton->close();
			if (_windowFlags.showDepthButton) _depthButton->close();

			// Close all border gadgets
			_windowBorderLeft->close();
			_windowBorderRight->close();
			_windowBorderBottom->close();
			_windowBorderTop->close();

			// Reset pointers
			_closeButton = NULL;
			_depthButton = NULL;
			_windowBorderBottom = NULL;
			_windowBorderLeft = NULL;
			_windowBorderRight = NULL;
			_windowBorderTop = NULL;

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

		redraw();
	}
}

void AmigaWindow::createBorder() {
	// Add gadgets to the start in reverse order

	u16 topBorderWidth = _width;
	s16 topBorderX = 0;

	// Add close button
	if (_windowFlags.showCloseButton) {
		_closeButton = new WindowBorderButton(0, 0, WINDOW_CLOSE_BUTTON_WIDTH, WINDOW_TITLE_HEIGHT, GLYPH_WINDOW_CLOSE, GLYPH_WINDOW_CLOSE, &_style);
		_closeButton->addGadgetEventHandler(this);
		insertGadget(_closeButton);

		topBorderWidth -= WINDOW_CLOSE_BUTTON_WIDTH;
		topBorderX += WINDOW_CLOSE_BUTTON_WIDTH;
	}

	// Add depth button
	if (_windowFlags.showDepthButton) {
		_depthButton = new WindowBorderButton(_width - WINDOW_DEPTH_BUTTON_WIDTH, 0, WINDOW_DEPTH_BUTTON_WIDTH, WINDOW_TITLE_HEIGHT, GLYPH_WINDOW_DEPTH_UP, GLYPH_WINDOW_DEPTH_DOWN, &_style);
		_depthButton->addGadgetEventHandler(this);
		insertGadget(_depthButton);

		topBorderWidth -= WINDOW_DEPTH_BUTTON_WIDTH;
	}

	// Add borders
	_windowBorderTop = new WindowBorderTop(topBorderX, topBorderWidth, WINDOW_TITLE_HEIGHT, this, &_style);
	_windowBorderLeft = new WindowBorderSide(0, WINDOW_TITLE_HEIGHT, WINDOW_BORDER_SIZE, _height - WINDOW_BORDER_SIZE - WINDOW_TITLE_HEIGHT);
	_windowBorderRight = new WindowBorderSide(_width - WINDOW_BORDER_SIZE, WINDOW_TITLE_HEIGHT, WINDOW_BORDER_SIZE, _height - WINDOW_BORDER_SIZE - WINDOW_TITLE_HEIGHT);
	_windowBorderBottom = new WindowBorderBottom(0, _height - WINDOW_BORDER_SIZE, _width, WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE);

	_windowBorderTop->addGadgetEventHandler(this);
	_windowBorderLeft->addGadgetEventHandler(this);
	_windowBorderRight->addGadgetEventHandler(this);
	_windowBorderBottom->addGadgetEventHandler(this);

	insertGadget(_windowBorderBottom);
	insertGadget(_windowBorderRight);
	insertGadget(_windowBorderLeft);
	insertGadget(_windowBorderTop);
}

void AmigaWindow::onFocus() {
	raiseToTop();
	redrawBorder();
}

void AmigaWindow::onBlur() {
	redrawBorder();
}

void AmigaWindow::redrawBorder() {
	if (_windowBorderTop != NULL) {
		_windowBorderBottom->redraw();
		_windowBorderTop->redraw();
		_windowBorderLeft->redraw();
		_windowBorderRight->redraw();

		if (_depthButton != NULL) _depthButton->redraw();
		if (_closeButton != NULL) _closeButton->redraw();
	}
}

void AmigaWindow::onResize(u16 width, u16 height) {

	// Calculate top border size
	u16 topBorderWidth = _width;
	if (_windowFlags.showCloseButton) topBorderWidth -= WINDOW_CLOSE_BUTTON_WIDTH;
	if (_windowFlags.showDepthButton) topBorderWidth -= WINDOW_DEPTH_BUTTON_WIDTH;

	// Resize borders
	_windowBorderTop->resize(topBorderWidth, WINDOW_TITLE_HEIGHT);
	_windowBorderLeft->resize(WINDOW_BORDER_SIZE, height - WINDOW_BORDER_SIZE - WINDOW_TITLE_HEIGHT);
	_windowBorderRight->moveTo(_width - WINDOW_BORDER_SIZE, WINDOW_TITLE_HEIGHT);
	_windowBorderRight->resize(WINDOW_BORDER_SIZE, height - WINDOW_BORDER_SIZE - WINDOW_TITLE_HEIGHT);
	_windowBorderBottom->resize(_width, WINDOW_BORDER_SIZE);
	_windowBorderBottom->moveTo(0, _height - WINDOW_BORDER_SIZE);

	// Depth button
	if (_windowFlags.showDepthButton) _depthButton->moveTo(_width - WINDOW_DEPTH_BUTTON_WIDTH, 0);
}

void AmigaWindow::handleReleaseEvent(const GadgetEventArgs& e) {
	
	if (e.getSource() != NULL) {

		if (e.getSource() == _depthButton) {

			// Swap depths
			swapDepth();
		} else if (e.getSource() == _closeButton) {

			// Work out which close type to use
			switch (getCloseType()) {
				case CLOSE_TYPE_CLOSE:
					// Close the window
					close();
					break;
				case CLOSE_TYPE_SHELVE:
					// Shelve the window
					shelve();
					break;
				case CLOSE_TYPE_HIDE:
					// Hide the window
					hide();
					break;
			}
		} else if (e.getSource() == _windowBorderTop) {

			// Release the window
			release(e.getX(), e.getY());
		}
	}
}

void AmigaWindow::handleClickEvent(const GadgetEventArgs& e) {
	
	if (e.getSource() != NULL) {

		if (e.getSource() == _windowBorderTop) {

			// Top border - focus and drag
			focus();
			startDragging(e.getX(), e.getY());

		} else if ((e.getSource() == _windowBorderBottom) ||
			(e.getSource() == _windowBorderLeft) ||
			(e.getSource() == _windowBorderRight) ||
			(e.getSource() == _depthButton) ||
			(e.getSource() == _closeButton)) {

			// Other borders - focus only
			focus();
		}
	}
}

void AmigaWindow::handleDragEvent(const GadgetEventArgs& e) {
	if (e.getSource() != NULL) {
		if (e.getSource() == _windowBorderTop) {
			drag(e.getX(), e.getY(), e.getVX(), e.getVY());
		}
	}
}

void AmigaWindow::handleReleaseOutsideEvent(const GadgetEventArgs& e) {
	if (e.getSource() != NULL) {
		if (e.getSource() == _windowBorderTop) {
			release(e.getX(), e.getY());
		}
	}
}

void AmigaWindow::showCloseButton() {
	if ((!_flags.borderless) && (!_windowFlags.showCloseButton)) {
		_windowFlags.showCloseButton = true;
		
		// Recreate close button
		_closeButton = new WindowBorderButton(0, 0, WINDOW_CLOSE_BUTTON_WIDTH, WINDOW_TITLE_HEIGHT, GLYPH_WINDOW_CLOSE, GLYPH_WINDOW_CLOSE, &_style);
		_closeButton->addGadgetEventHandler(this);
		insertGadget(_closeButton);

		resizeTitleBarToFit();

		_closeButton->redraw();
	}
}

void AmigaWindow::showDepthButton() {
	if ((!_flags.borderless) && (!_windowFlags.showDepthButton)) {
		_windowFlags.showDepthButton = true;
		
		// Recreate depth button
		_depthButton = new WindowBorderButton(_width - WINDOW_DEPTH_BUTTON_WIDTH, 0, WINDOW_DEPTH_BUTTON_WIDTH, WINDOW_TITLE_HEIGHT, GLYPH_WINDOW_DEPTH_UP, GLYPH_WINDOW_DEPTH_DOWN, &_style);
		_depthButton->addGadgetEventHandler(this);
		insertGadget(_depthButton);

		resizeTitleBarToFit();

		_depthButton->redraw();
	}
}

void AmigaWindow::hideCloseButton() {
	if ((!_flags.borderless) && (_windowFlags.showCloseButton)) {
		_windowFlags.showCloseButton = false;
		
		// Delete close button
		_closeButton->close();
		_closeButton = NULL;

		resizeTitleBarToFit();
	}
}

void AmigaWindow::hideDepthButton() {
	if ((!_flags.borderless) && (_windowFlags.showDepthButton)) {
		_windowFlags.showDepthButton = false;
		
		// Delete depth button
		_depthButton->close();
		_depthButton = NULL;

		resizeTitleBarToFit();
	}
}

void AmigaWindow::resizeTitleBarToFit() {
	bool wasPermeable = isPermeable();
	setPermeable(true);
	
	s16 newX = _closeButton == NULL ? 0 : _closeButton->getWidth();
	s16 newY = 0;
	u16 newWidth = getWidth();
	u16 newHeight = _windowBorderTop->getHeight();
	
	if (_depthButton != NULL) newWidth -= _depthButton->getWidth();

	_windowBorderTop->changeDimensions(newX, newY, newWidth, newHeight);

	setPermeable(wasPermeable);
}
