#include "skinnedwindow.h"
#include "skinnedwindowbordertop.h"
#include "skinnedwindowborderleft.h"
#include "skinnedwindowborderright.h"
#include "skinnedwindowborderbottom.h"
#include "skinnedwindowclosebutton.h"
#include "skinnedwindowdepthbutton.h"
#include "woopsi.h"

using namespace WoopsiUI;

SkinnedWindow::SkinnedWindow(s16 x, s16 y, u16 width, u16 height, const char* title, u32 flags, u32 windowFlags, WindowSkin* skin) : Window(x, y, width, height, title, flags, NULL) {

	_windowBorderTop = NULL;
	_windowBorderLeft = NULL;
	_windowBorderRight = NULL;
	_windowBorderBottom = NULL;
	_closeButton = NULL;
	_depthButton = NULL;

	_skin = skin;

	// Parse skin information
	_flags.borderless = _skin->window.borderless;
	_flags.permeable = _skin->window.permeable;
	_font = _skin->window.font.font;
	_backColour = _skin->window.colours.back;
	_shineColour = _skin->window.colours.shine;
	_highlightColour = _skin->window.colours.highlight;
	_shadowColour = _skin->window.colours.shadow;
	_fillColour = _skin->window.colours.fill;

	_windowFlags.showCloseButton = _skin->closeButton.visible;
	_windowFlags.showDepthButton = _skin->depthButton.visible;

	// Add border to gadget list
	if (!_flags.borderless) {
		createBorder();
	}
}

const u8 SkinnedWindow::getBorderSize() const {
	if (!_flags.borderless) {
		return _windowBorderLeft->getWidth();
	}

	return 0;
}

const u8 SkinnedWindow::getTitleHeight() const {
	if (!_flags.borderless) {
		return _windowBorderTop->getWidth();
	}

	return 0;
}

void SkinnedWindow::setBorderless(bool isBorderless) {
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

		draw();
	}
}

void SkinnedWindow::createBorder() {
	// Add gadgets to the start in reverse order

	// Add close button
	if (_windowFlags.showCloseButton) {
		u16 closeY = (_skin->topCentreBorder.bitmap.height / 2) - (_skin->closeButton.bitmap.height / 2);
		_closeButton = new SkinnedWindowCloseButton(_skin->closeButton.offsetX, closeY, _skin);
		_closeButton->setEventHandler(this);
		insertGadget(_closeButton);
	}

	// Add depth button
	if (_windowFlags.showDepthButton) {
		u16 depthY = (_skin->topCentreBorder.bitmap.height / 2) - (_skin->depthButton.bitmap.height / 2);
		_depthButton = new SkinnedWindowDepthButton(_width - _skin->depthButton.bitmap.width - _skin->depthButton.offsetX, depthY, _skin);
		_depthButton->setEventHandler(this);
		insertGadget(_depthButton);
	}

	_windowBorderTop = new SkinnedWindowBorderTop(0, _width, _title, _skin);
	_windowBorderLeft = new SkinnedWindowBorderLeft(0, _skin->topCentreBorder.bitmap.height, _height - _skin->topCentreBorder.bitmap.height - _skin->bottomCentreBorder.bitmap.height, _skin);
	_windowBorderRight = new SkinnedWindowBorderRight(_width - _skin->rightBorder.bitmap.width, _skin->topCentreBorder.bitmap.height, _height - _skin->topCentreBorder.bitmap.height - _skin->bottomCentreBorder.bitmap.height, _skin);
	_windowBorderBottom = new SkinnedWindowBorderBottom(0, _height - _skin->bottomCentreBorder.bitmap.height, _width, _skin);

	_windowBorderTop->setEventHandler(this);
	_windowBorderLeft->setEventHandler(this);
	_windowBorderRight->setEventHandler(this);
	_windowBorderBottom->setEventHandler(this);

	insertGadget(_windowBorderBottom);
	insertGadget(_windowBorderRight);
	insertGadget(_windowBorderLeft);
	insertGadget(_windowBorderTop);
}

bool SkinnedWindow::click(s16 x, s16 y) {

	if (isEnabled()) {
		if (checkCollision(x, y)) {

			raiseToTop();

			// Try to click a child gadget
			for (s32 i = _gadgets.size() - 1; i > -1; i--) {
				if (_gadgets[i]->click(x, y)) {
					return true;
				}
			}

			// Handle click on window
			_flags.clicked = true;

			setFocusedGadget(NULL);

			// Tell Woopsi that the clicked gadget has changed
			if (woopsiApplication != NULL) {
				woopsiApplication->setClickedGadget(this);
			}

			raiseClickEvent(x, y);

			return true;
		}
	}

	return false;
}

bool SkinnedWindow::focus() {

	if (isEnabled()) {
		if (!_flags.hasFocus) {

			// Handle focus gained on window
			Gadget::focus();

			// Run focus on borders
			if (_windowBorderTop != NULL) {
				_windowBorderBottom->draw();
				_windowBorderTop->draw();
				_windowBorderLeft->draw();
				_windowBorderRight->draw();

				if (_closeButton != NULL) _closeButton->draw();
				if (_depthButton != NULL) _depthButton->draw();
			}

			return true;
		}
	}
	return false;
}

bool SkinnedWindow::blur() {

	if (Gadget::blur()) {

		// Run blur on borders
		if (_windowBorderTop != NULL) {
			_windowBorderBottom->draw();
			_windowBorderTop->draw();
			_windowBorderLeft->draw();
			_windowBorderRight->draw();

			if (_closeButton != NULL) _closeButton->draw();
			if (_depthButton != NULL) _depthButton->draw();
		}

		// Take focus away from child gadgets
		if (_focusedGadget != NULL) {
			_focusedGadget->blur();
			_focusedGadget = NULL;
		}

		return true;
	}

	return false;
}

bool SkinnedWindow::resize(u16 width, u16 height) {

	// Enforce gadget to stay within parent confines if necessary
	if (_parent != NULL) {
		if (!_parent->isPermeable()) {

			Rect parentRect;
			_parent->getClientRect(parentRect);

			// Check width
			if (_x + width > parentRect.width) {
				width = parentRect.width - _x;
			}

			// Check height
			if (_y + height > parentRect.height) {
				height = parentRect.height - _y;
			}
		}
	}

	if ((_width != width) || (_height != height)) {
		erase();

		_width = width;
		_height = height;

		_windowBorderTop->resize(_width, WINDOW_TITLE_HEIGHT);
		_windowBorderLeft->resize(WINDOW_BORDER_SIZE, height - WINDOW_BORDER_SIZE - WINDOW_TITLE_HEIGHT);
		_windowBorderRight->moveTo(_width - WINDOW_BORDER_SIZE, WINDOW_TITLE_HEIGHT);
		_windowBorderRight->resize(WINDOW_BORDER_SIZE, height - WINDOW_BORDER_SIZE - WINDOW_TITLE_HEIGHT);
		_windowBorderBottom->resize(_width, WINDOW_BORDER_SIZE);
		_windowBorderBottom->moveTo(0, _height - WINDOW_BORDER_SIZE);

		// Depth button
		if (_windowFlags.showDepthButton) _depthButton->moveTo(_width - WINDOW_DEPTH_BUTTON_WIDTH, 0);

		draw();

		raiseResizeEvent(width, height);

		return true;
	}

	return false;
}

// Insert the available space for child gadgets into the rect
void SkinnedWindow::getClientRect(Rect& rect) const {
	if (!_flags.borderless) {
		rect.x = _windowBorderLeft->getWidth();
		rect.y = _windowBorderTop->getHeight();
		rect.width = _width - _windowBorderLeft->getWidth() - _windowBorderRight->getWidth();
		rect.height = _height - _windowBorderBottom->getHeight() - _windowBorderTop->getHeight();
	} else {
		rect.x = 0;
		rect.y = 0;
		rect.width = _width;
		rect.height = _height;
	}
}

bool SkinnedWindow::handleEvent(const EventArgs& e) {

	if (e.gadget != NULL) {
		switch (e.type) {
			case EVENT_RELEASE:

				if (e.gadget == _depthButton) {

					// Swap depths
					swapDepth();
					return true;
				} else if (e.gadget == _closeButton) {

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

					return true;
				} else if (e.gadget == _windowBorderTop) {

					// Release the window
					release(e.eventX, e.eventY);
					return true;
				}
				break;

			case EVENT_CLICK:

				if (e.gadget == _windowBorderTop) {
		
					// Top border - focus and drag
					focus();
					setDragging(e.eventX, e.eventY);
					return true;

				} else if ((e.gadget == _windowBorderBottom) ||
					(e.gadget == _windowBorderLeft) ||
					(e.gadget == _windowBorderRight) ||
					(e.gadget == _depthButton) ||
					(e.gadget == _closeButton)) {

					// Other borders - focus only
					focus();
					return true;
				}
				break;

			case EVENT_DRAG:

				if (e.gadget == _windowBorderTop) {
					drag(e.eventX, e.eventY, e.eventVX, e.eventVY);
					return true;
				}
				break;

			case EVENT_RELEASE_OUTSIDE:

				if (e.gadget == _windowBorderTop) {
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

void SkinnedWindow::showCloseButton() {
	if ((!_flags.borderless) && (!_windowFlags.showCloseButton)) {
		_windowFlags.showCloseButton = true;
		
		// Recreate close button
		u16 closeY = (_skin->topCentreBorder.bitmap.height / 2) - (_skin->closeButton.bitmap.height / 2);
		_closeButton = new SkinnedWindowCloseButton(_skin->closeButton.offsetX, closeY, _skin);
		_closeButton->setEventHandler(this);
		addGadget(_closeButton);

		_windowBorderTop->draw();
		_closeButton->draw();
	}
}

void SkinnedWindow::showDepthButton() {
	if ((!_flags.borderless) && (!_windowFlags.showDepthButton)) {
		_windowFlags.showDepthButton = true;
		
		// Recreate depth button
		u16 depthY = (_skin->topCentreBorder.bitmap.height / 2) - (_skin->depthButton.bitmap.height / 2);
		_depthButton = new SkinnedWindowDepthButton(_width - _skin->depthButton.bitmap.width - _skin->depthButton.offsetX, depthY, _skin);
		_depthButton->setEventHandler(this);
		addGadget(_depthButton);

		_depthButton->draw();
	}
}

void SkinnedWindow::hideCloseButton() {
	if ((!_flags.borderless) && (_windowFlags.showCloseButton)) {
		_windowFlags.showCloseButton = false;
		
		// Delete close button
		_closeButton->close();
		_closeButton = NULL;

		_windowBorderTop->draw();
	}
}

void SkinnedWindow::hideDepthButton() {
	if ((!_flags.borderless) && (_windowFlags.showDepthButton)) {
		_windowFlags.showDepthButton = false;
		
		// Delete close button
		_depthButton->close();
		_depthButton = NULL;
	}
}
