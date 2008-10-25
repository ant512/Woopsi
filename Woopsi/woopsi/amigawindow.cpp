#include "amigawindow.h"	
#include "windowbordertop.h"
#include "windowborderside.h"
#include "windowborderbottom.h"
#include "windowborderbutton.h"
#include "woopsi.h"

using namespace WoopsiUI;

AmigaWindow::AmigaWindow(s16 x, s16 y, u16 width, u16 height, const char* title, u32 flags, u32 windowFlags, FontBase* font) : Window(x, y, width, height, title, flags, font) {

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

		draw();
	}
}

void AmigaWindow::createBorder() {
	// Add gadgets to the start in reverse order

	u16 topBorderWidth = _width;
	s16 topBorderX = 0;

	// Add close button
	if (_windowFlags.showCloseButton) {
		_closeButton = new WindowBorderButton(0, 0, WINDOW_CLOSE_BUTTON_WIDTH, WINDOW_TITLE_HEIGHT, GLYPH_WINDOW_CLOSE, GLYPH_WINDOW_CLOSE, _font);
		_closeButton->setEventHandler(this);
		insertGadget(_closeButton);

		topBorderWidth -= WINDOW_CLOSE_BUTTON_WIDTH;
		topBorderX += WINDOW_CLOSE_BUTTON_WIDTH;
	}

	// Add depth button
	if (_windowFlags.showDepthButton) {
		_depthButton = new WindowBorderButton(_width - WINDOW_DEPTH_BUTTON_WIDTH, 0, WINDOW_DEPTH_BUTTON_WIDTH, WINDOW_TITLE_HEIGHT, GLYPH_WINDOW_DEPTH_UP, GLYPH_WINDOW_DEPTH_DOWN, _font);
		_depthButton->setEventHandler(this);
		insertGadget(_depthButton);

		topBorderWidth -= WINDOW_DEPTH_BUTTON_WIDTH;
	}

	// Add borders
	_windowBorderTop = new WindowBorderTop(topBorderX, topBorderWidth, WINDOW_TITLE_HEIGHT, _title, _font);
	_windowBorderLeft = new WindowBorderSide(0, WINDOW_TITLE_HEIGHT, WINDOW_BORDER_SIZE, _height - WINDOW_BORDER_SIZE - WINDOW_TITLE_HEIGHT);
	_windowBorderRight = new WindowBorderSide(_width - WINDOW_BORDER_SIZE, WINDOW_TITLE_HEIGHT, WINDOW_BORDER_SIZE, _height - WINDOW_BORDER_SIZE - WINDOW_TITLE_HEIGHT);
	_windowBorderBottom = new WindowBorderBottom(0, _height - WINDOW_BORDER_SIZE, _width, WINDOW_BORDER_SIZE, WINDOW_BORDER_SIZE);

	_windowBorderTop->setEventHandler(this);
	_windowBorderLeft->setEventHandler(this);
	_windowBorderRight->setEventHandler(this);
	_windowBorderBottom->setEventHandler(this);

	insertGadget(_windowBorderBottom);
	insertGadget(_windowBorderRight);
	insertGadget(_windowBorderLeft);
	insertGadget(_windowBorderTop);
}

bool AmigaWindow::click(s16 x, s16 y) {

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

bool AmigaWindow::focus() {

	if (Gadget::focus()) {

		// Run focus on borders
		if (_windowBorderTop != NULL) {
			_windowBorderBottom->draw();
			_windowBorderTop->draw();
			_windowBorderLeft->draw();
			_windowBorderRight->draw();

			if (_depthButton != NULL) _depthButton->draw();
			if (_closeButton != NULL) _closeButton->draw();
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
			
			if (_depthButton != NULL) _depthButton->draw();
			if (_closeButton != NULL) _closeButton->draw();
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

		// Calculate top border size
		u16 topBorderWidth = _width;
		if (_windowFlags.showCloseButton) topBorderWidth -= WINDOW_CLOSE_BUTTON_WIDTH;
		if (_windowFlags.showCloseButton) topBorderWidth -= WINDOW_DEPTH_BUTTON_WIDTH;

		// Resize borders
		_windowBorderTop->resize(topBorderWidth, WINDOW_TITLE_HEIGHT);
		_windowBorderLeft->resize(WINDOW_BORDER_SIZE, height - WINDOW_BORDER_SIZE - WINDOW_TITLE_HEIGHT);
		_windowBorderRight->moveTo(_width - WINDOW_BORDER_SIZE, WINDOW_TITLE_HEIGHT);
		_windowBorderRight->resize(WINDOW_BORDER_SIZE, height - WINDOW_BORDER_SIZE - WINDOW_TITLE_HEIGHT);
		_windowBorderBottom->resize(_width, WINDOW_BORDER_SIZE);
		_windowBorderBottom->moveTo(0, _height - WINDOW_BORDER_SIZE);

		// Depth button
		if (_windowFlags.showDepthButton)	_depthButton->moveTo(_width - WINDOW_DEPTH_BUTTON_WIDTH, 0);

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

void AmigaWindow::showCloseButton() {
	if ((!_flags.borderless) && (!_windowFlags.showCloseButton)) {
		_windowFlags.showCloseButton = true;
		
		// Recreate close button
		_closeButton = new WindowBorderButton(0, 0, WINDOW_CLOSE_BUTTON_WIDTH, WINDOW_TITLE_HEIGHT, GLYPH_WINDOW_CLOSE, GLYPH_WINDOW_CLOSE, _font);
		_closeButton->setEventHandler(this);
		insertGadget(_closeButton);

		// Resize and move the title bar
		bool wasPermeable = isPermeable();
		setPermeable(true);

		_windowBorderTop->changeDimensions(WINDOW_CLOSE_BUTTON_WIDTH, 0, _windowBorderTop->getWidth() - WINDOW_CLOSE_BUTTON_WIDTH, _windowBorderTop->getHeight());

		setPermeable(wasPermeable);

		_closeButton->draw();
	}
}

void AmigaWindow::showDepthButton() {
	if ((!_flags.borderless) && (!_windowFlags.showDepthButton)) {
		_windowFlags.showDepthButton = true;
		
		// Recreate depth button
		_depthButton = new WindowBorderButton(_width - WINDOW_DEPTH_BUTTON_WIDTH, 0, WINDOW_DEPTH_BUTTON_WIDTH, WINDOW_TITLE_HEIGHT, GLYPH_WINDOW_DEPTH_UP, GLYPH_WINDOW_DEPTH_DOWN, _font);
		_depthButton->setEventHandler(this);
		insertGadget(_depthButton);

		// Resize the title bar
		bool wasPermeable = isPermeable();
		setPermeable(true);

		_windowBorderTop->resize(_windowBorderTop->getWidth() - WINDOW_DEPTH_BUTTON_WIDTH, _windowBorderTop->getHeight());

		setPermeable(wasPermeable);

		_depthButton->draw();
	}
}

void AmigaWindow::hideCloseButton() {
	if ((!_flags.borderless) && (_windowFlags.showCloseButton)) {
		_windowFlags.showCloseButton = false;
		
		// Delete close button
		_closeButton->close();
		_closeButton = NULL;

		// Resize and move the title bar
		bool wasPermeable = isPermeable();
		setPermeable(true);

		_windowBorderTop->changeDimensions(0, 0, _windowBorderTop->getWidth() + WINDOW_CLOSE_BUTTON_WIDTH, _windowBorderTop->getHeight());

		setPermeable(wasPermeable);
	}
}

void AmigaWindow::hideDepthButton() {
	if ((!_flags.borderless) && (_windowFlags.showDepthButton)) {
		_windowFlags.showDepthButton = false;
		
		// Delete close button
		_depthButton->close();
		_depthButton = NULL;

		// Resize and move the title bar
		bool wasPermeable = isPermeable();
		setPermeable(true);

		_windowBorderTop->resize(_windowBorderTop->getWidth() + WINDOW_CLOSE_BUTTON_WIDTH, _windowBorderTop->getHeight());

		setPermeable(wasPermeable);
	}
}
