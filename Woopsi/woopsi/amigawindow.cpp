#include "amigawindow.h"	
#include "windowbordertop.h"
#include "windowborderside.h"
#include "windowborderbottom.h"
#include "windowborderbutton.h"

AmigaWindow::AmigaWindow(s16 x, s16 y, u16 width, u16 height, char* title, u32 flags, FontBase* font) : Window(x, y, width, height, title, flags, font) {
	_title = title;

	_windowBorderTop = NULL;
	_windowBorderLeft = NULL;
	_windowBorderRight = NULL;
	_windowBorderBottom = NULL;
	_closeButton = NULL;
	_depthButton = NULL;

	// Add border to gadget list
	if (!(flags & GADGET_BORDERLESS)) {
		createBorder();
	}
}

const u8 AmigaWindow::getBorderSize() const {
	if (!_flags.borderless) {
		return WINDOW_BORDER_SIZE;
	}

	return 0;
}

const u8 AmigaWindow::getTitleHeight() const {
	if (!_flags.borderless) {
		return WINDOW_TITLE_HEIGHT;
	}

	return 0;
}

void AmigaWindow::setClickedGadget(Gadget* gadget) {
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

void AmigaWindow::setBorderless(bool isBorderless) {
	if (isBorderless != _flags.borderless) {
		if (isBorderless) {
			// Remove borders

			if (_flags.closeable) {
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

void AmigaWindow::createBorder() {
	// Add gadgets to the start in reverse order

	// Add close button
	if (_flags.closeable) {
		_closeButton = new WindowBorderButton(0, 0, WINDOW_CLOSE_BUTTON_WIDTH, WINDOW_TITLE_HEIGHT, GLYPH_WINDOW_CLOSE, GLYPH_WINDOW_CLOSE, _font);
		_closeButton->setEventHandler(this);
		insertGadget(_closeButton);
	}

	// Add depth button
	_depthButton = new WindowBorderButton(_width - WINDOW_DEPTH_BUTTON_WIDTH, 0, WINDOW_DEPTH_BUTTON_WIDTH, WINDOW_TITLE_HEIGHT, GLYPH_WINDOW_DEPTH_UP, GLYPH_WINDOW_DEPTH_DOWN, _font);
	_depthButton->setEventHandler(this);
	insertGadget(_depthButton);

	// Add top border
	if (_flags.closeable) {
		_windowBorderTop = new WindowBorderTop(WINDOW_CLOSE_BUTTON_WIDTH, _width - WINDOW_CLOSE_BUTTON_WIDTH - WINDOW_DEPTH_BUTTON_WIDTH, WINDOW_TITLE_HEIGHT, _title, _font);
	} else {
		_windowBorderTop = new WindowBorderTop(0, _width - WINDOW_DEPTH_BUTTON_WIDTH, WINDOW_TITLE_HEIGHT, _title, _font);
	}

	_windowBorderLeft = new WindowBorderSide(0, WINDOW_TITLE_HEIGHT, WINDOW_BORDER_SIZE, _height - WINDOW_BORDER_SIZE - WINDOW_TITLE_HEIGHT);
	_windowBorderRight = new WindowBorderSide(_width - WINDOW_BORDER_SIZE, WINDOW_TITLE_HEIGHT, WINDOW_BORDER_SIZE, _height - WINDOW_BORDER_SIZE - WINDOW_TITLE_HEIGHT);
	_windowBorderBottom = new WindowBorderBottom(0, _height - WINDOW_BORDER_SIZE, _width, WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE);

	insertGadget(_windowBorderBottom);
	insertGadget(_windowBorderRight);
	insertGadget(_windowBorderLeft);
	insertGadget(_windowBorderTop);
}

bool AmigaWindow::click(s16 x, s16 y) {

	if (_flags.enabled) {
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
				_flags.clicked = true;

				setFocusedGadget(NULL);

				// Tell parent that the clicked gadget has changed
				if (_parent != NULL) {
					_parent->setClickedGadget(this);
				}

				raiseClickEvent(x, y);
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

bool AmigaWindow::focus() {

	if (Gadget::focus()) {

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

	return false;
}

bool AmigaWindow::blur() {

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

bool AmigaWindow::resize(u16 width, u16 height) {

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
	
		// Remember if the gadget is permeable
		bool wasPermeable = _flags.permeable;

		_flags.permeable = true;

		erase();

		_width = width;
		_height = height;

		// Top border
		if (_flags.closeable) {
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

		// Reset the permeable value
		_flags.permeable = wasPermeable;

		draw();

		raiseResizeEvent(width, height);

		return true;
	}

	return false;
}

// Insert the available space for child gadgets into the rect
void AmigaWindow::getClientRect(Rect& rect) const {
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

bool AmigaWindow::handleEvent(const EventArgs& e) {
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
					case CLOSE_TYPE_HIDE:
						// Hide the window
						hide();
						break;
				}

				return true;
			}
		}
	}

	return false;
}
