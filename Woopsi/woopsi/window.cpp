#include "window.h"	
#include "screen.h"

Window::Window(s16 x, s16 y, u16 width, u16 height, char* title, u32 flags, FontBase* font) : Gadget(x, y, width, height, flags, font) {
	_title = title;
}

Window::~Window() {
}

void Window::draw() {
	Gadget::draw();
}

bool Window::click(s16 x, s16 y) {

	if (_flags.enabled) {
		if (checkCollision(x, y)) {
			_clickedGadget = NULL;

			// Work out which gadget was clicked
			for (s16 i = _gadgets.size() - 1; i > -1; i--) {
				if (_gadgets[i]->click(x, y)) {
					break;
				}
			}

			// Did we click a gadget?
			if (_clickedGadget == NULL) {
				// Handle click on window
				Gadget::click(x, y);
			}

			return true;
		}
	}

	return false;
}

bool Window::release(s16 x, s16 y) {

	if (_flags.dragging) {

		// Get a graphics port from the parent screen
		GraphicsPort* port = _parent->newGraphicsPort();

		// Erase the old rect
		port->drawXORRect(_newX, _newY, _width, _height);
		delete port;

		moveTo(_newX, _newY);

		_clickedGadget = NULL;

		Gadget::release(x, y);

		return true;
	} else if (_clickedGadget != NULL) {

		// Release clicked gadget
		_clickedGadget->release(x, y);

		return true;
	} else if (_flags.clicked) {

		// Handle release on window
		Gadget::release(x, y);

		return true;
	}

	return false;
}

bool Window::drag(s16 x, s16 y, s16 vX, s16 vY) {

	if (_flags.enabled) {
		if (_flags.dragging) {

			// Work out where we're moving to
			s16 destX = x - _grabPointX - _parent->getX();
			s16 destY = y - _grabPointY - _parent->getY();

			// Do we need to move?
			if ((destX != x) && (destY != y)) {

				// Prevent window from moving outside screen
				if (!_parent->isPermeable()) {
					if (destX < 0) {
						destX = 0;
					} else if (destX + _width > _parent->getWidth()) {
						destX = _parent->getWidth() - _width;
					}

					if (destY + _height > _parent->getY() + _parent->getHeight()) {
						destY = _parent->getHeight() - _height;
					}
				}

				// Prevent window from leaving top of screen
				if (destY < 0) {
					destY = 0;
				}

				// Get parent re

				// Get a graphics port from the parent screen
				GraphicsPort* port = _parent->newGraphicsPort();

				// Erase the old rect
				port->drawXORRect(_newX, _newY, _width, _height);

				// Perform move
				_newX = destX;
				_newY = destY;

				// Draw the new rect
				port->drawXORRect(_newX, _newY, _width, _height);

				delete port;
			}

			raiseDragEvent(x, y, vX, vY);

			return true;
		} else {
			// Handle child dragging
			if (_clickedGadget != NULL) {
				_clickedGadget->drag(x, y, vX, vY);

				return true;
			}
		}
	}
	
	return false;
}

bool Window::keyPress(KeyCode keyCode) {

	if (_flags.enabled) {
		// Handle key press on window
		Gadget::keyPress(keyCode);

		if (_activeGadget != NULL) {
			// Run key press on active gadget
			_activeGadget->keyPress(keyCode);
		}

		return true;
	}

	return false;
}

bool Window::keyRelease(KeyCode keyCode) {

	if (_flags.enabled) {
		// Handle key release on window
		Gadget::keyRelease(keyCode);

		if (_activeGadget != NULL) {
			// Run key release on active gadget
			_activeGadget->keyRelease(keyCode);
		}

		return true;
	}

	return false;
}

void Window::lidClosed() {
	// Handle lid closed on window
	Gadget::lidClosed();

	// Run lid closed on all gadgets
	for (u8 i = 0; i < _gadgets.size(); i++) {
		_gadgets[i]->lidClosed();
	}
}

void Window::lidOpened() {
	// Handle lid closed on window
	Gadget::lidOpened();

	// Run lid closed on all gadgets
	for (u8 i = 0; i < _gadgets.size(); i++) {
		_gadgets[i]->lidOpened();
	}
}

bool Window::focus() {

	if (_flags.enabled) {
		if (!_flags.active) {

			// Handle focus gained on window
			Gadget::focus();

			return true;
		}
	}
	return false;
}

bool Window::blur() {

	if (_flags.active) {

		// Handle focus lost on window
		Gadget::blur();

		// Take focus away from child gadgets
		if (_activeGadget != NULL) {
			_activeGadget->blur();
			_activeGadget = NULL;
		}

		return true;
	}

	return false;
}

void Window::draw(Rect clipRect) {
	clear(clipRect);
}
