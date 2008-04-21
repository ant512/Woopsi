#include "skinnedwindow.h"
#include "skinnedwindowbordertop.h"
#include "skinnedwindowborderleft.h"
#include "skinnedwindowborderright.h"
#include "skinnedwindowborderbottom.h"
#include "skinnedwindowclosebutton.h"
#include "skinnedwindowdepthbutton.h"

SkinnedWindow::SkinnedWindow(s16 x, s16 y, u16 width, u16 height, char* title, u32 flags, WindowSkin* skin) : Window(x, y, width, height, title, flags, NULL) {
	_title = title;

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
	_flags.closable = _skin->closeButton.visible;
	_font = _skin->window.font.font;
	_backColour = _skin->window.colours.back;
	_shineColour = _skin->window.colours.shine;
	_highlightColour = _skin->window.colours.highlight;
	_shadowColour = _skin->window.colours.shadow;
	_fillColour = _skin->window.colours.fill;

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

void SkinnedWindow::setClickedGadget(Gadget* gadget) {
	if (_clickedGadget != gadget) {
		_clickedGadget = gadget;

		// Only remember we clicked a gadget if we didn't click
		// a border gadget
		if ((_clickedGadget == _windowBorderBottom) ||
			(_clickedGadget == _windowBorderLeft) ||
			(_clickedGadget == _windowBorderRight) ||
			(_clickedGadget == _windowBorderTop)) {

			// Forget the clicked gadget
			_clickedGadget = NULL;
		}

		// Notify parent
		if (_parent != NULL) {
			_parent->setClickedGadget(this);
		}
	}
}

void SkinnedWindow::setBorderless(bool isBorderless) {
	if (isBorderless != _flags.borderless) {
		if (isBorderless) {
			// Remove borders

			if (_flags.closable) {
				_closeButton->close();
			}

			_depthButton->close();
			_windowBorderLeft->close();
			_windowBorderRight->close();
			_windowBorderBottom->close();
			_windowBorderTop->close();

			_closeButton = NULL;
			_depthButton = NULL;
			_windowBorderBottom = NULL;
			_windowBorderLeft = NULL;
			_windowBorderRight = NULL;
			_windowBorderTop = NULL;

			_flags.borderless = true;

			// Move all children to compensate
			for (u8 i = 0; i < _gadgets.size(); i++) {
				_gadgets[i]->moveTo(_gadgets[i]->getX() - getX() - WINDOW_BORDER_SIZE, _gadgets[i]->getY() - getY() - WINDOW_TITLE_HEIGHT);
			}
		} else {
			// Add borders

			// Move all children to compensate
			for (u8 i = 0; i < _gadgets.size(); i++) {
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
	if (_flags.closable) {
		u16 closeY = (_skin->topCentreBorder.bitmap.height / 2) - (_skin->closeButton.bitmap.height / 2);
		_closeButton = new SkinnedWindowCloseButton(_skin->closeButton.offsetX, closeY, _skin);
		_closeButton->setEventHandler(this);
		insertGadget(_closeButton);
	}

	// Add depth button
	u16 depthY = (_skin->topCentreBorder.bitmap.height / 2) - (_skin->depthButton.bitmap.height / 2);
	_depthButton = new SkinnedWindowDepthButton(_width - _skin->depthButton.bitmap.width - _skin->depthButton.offsetX, depthY, _skin);
	_depthButton->setEventHandler(this);
	insertGadget(_depthButton);

	_windowBorderTop = new SkinnedWindowBorderTop(0, _width, _title, _skin);
	_windowBorderLeft = new SkinnedWindowBorderLeft(0, _skin->topCentreBorder.bitmap.height, _height - _skin->topCentreBorder.bitmap.height - _skin->bottomCentreBorder.bitmap.height, _skin);
	_windowBorderRight = new SkinnedWindowBorderRight(_width - _skin->rightBorder.bitmap.width, _skin->topCentreBorder.bitmap.height, _height - _skin->topCentreBorder.bitmap.height - _skin->bottomCentreBorder.bitmap.height, _skin);
	_windowBorderBottom = new SkinnedWindowBorderBottom(0, _height - _skin->bottomCentreBorder.bitmap.height, _width, _skin);
	insertGadget(_windowBorderBottom);
	insertGadget(_windowBorderRight);
	insertGadget(_windowBorderLeft);
	insertGadget(_windowBorderTop);
}

bool SkinnedWindow::click(s16 x, s16 y) {

	if (isEnabled()) {
		if (checkCollision(x, y)) {
			bool gotGadget = false;
			_clickedGadget = NULL;

			// Work out which gadget was clicked
			for (s16 i = _gadgets.size() - 1; i > -1; i--) {
				if (_gadgets[i]->click(x, y)) {

					// Only remember we clicked a gadget if we didn't click
					// a border gadget
					if (_clickedGadget != NULL) {
						if ((_clickedGadget != _closeButton) &&
							(_clickedGadget != _depthButton)) {
							gotGadget = true;
						}
					}

					break;
				}
			}

			// Did we click a gadget?
			if (!gotGadget) {
				// Handle click on window
				Gadget::click(x, y);
			}

			// Do we need to draw the XOR rect?
			if (_flags.dragging) {
				// Get a graphics port from the parent screen
				GraphicsPort* port = _parent->newGraphicsPort();

				// Draw rect
				port->drawXORRect(_newX, _newY, _width, _height);

				delete port;
			}

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
				_depthButton->draw();
			}

			// Run focus on close button
			if (_closeButton != NULL) {
				_closeButton->draw();
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
			_depthButton->draw();
		}

		// Run blur on close button
		if (_closeButton != NULL) {
			_closeButton->draw();
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

		// Top border
		if (_flags.closable) {
			_windowBorderTop->resize(_width - WINDOW_CLOSE_BUTTON_WIDTH - WINDOW_DEPTH_BUTTON_WIDTH, WINDOW_TITLE_HEIGHT);
		} else {
			_windowBorderTop->resize(_width - WINDOW_DEPTH_BUTTON_WIDTH, WINDOW_TITLE_HEIGHT);
		}

		_windowBorderLeft->resize(WINDOW_BORDER_SIZE, height - WINDOW_BORDER_SIZE - WINDOW_TITLE_HEIGHT);
		_windowBorderRight->moveTo(_width - WINDOW_BORDER_SIZE, WINDOW_TITLE_HEIGHT);
		_windowBorderRight->resize(WINDOW_BORDER_SIZE, height - WINDOW_BORDER_SIZE - WINDOW_TITLE_HEIGHT);
		_windowBorderBottom->resize(_width, WINDOW_BORDER_SIZE);
		_windowBorderBottom->moveTo(0, _height - WINDOW_BORDER_SIZE);

		// Depth button
		_depthButton->moveTo(_width - WINDOW_DEPTH_BUTTON_WIDTH, 0);

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
	// Only handle release events
	if (e.type == EVENT_RELEASE) {

		// Was an interesting gadget released?
		if (e.gadget != NULL) {

			// Process decoration gadgets only
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
						shelve();
						break;
				}

				return true;
			}
		}
	}

	return false;
}
