#include "window.h"	

Window::Window(s16 x, s16 y, u16 width, u16 height, const char* title, u32 flags, FontBase* font) : Gadget(x, y, width, height, flags, font) {
	_title = NULL;
	setTitle(title);
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

	if (isEnabled()) {
		if (_flags.dragging) {

			// Work out where we're moving to
			s16 destX = x - _grabPointX - _parent->getX();
			s16 destY = y - _grabPointY - _parent->getY();

			// Do we need to move?
			if ((destX != _x) && (destY != _y)) {

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

void Window::draw(Rect clipRect) {
	clear(clipRect);
}

void Window::setTitle(const char* title) {

	// Have we already created a block of memory that we need to free?
	if (_title != NULL) {
		// Free the memory
		delete [] _title;
	}

	// Create new memory for string
	_title = new char[strlen(title) + 1];

	// Copy text
	strcpy(_title, title);

	draw();
}
