#include "window.h"	
#include "woopsi.h"
#include "damagedrectmanager.h"

using namespace WoopsiUI;

Window::Window(s16 x, s16 y, u16 width, u16 height, const WoopsiString& title, u32 flags, GadgetStyle* style) : Gadget(x, y, width, height, flags, style) {
	_title = title;
}

void Window::onDragStop() {
	
	woopsiApplication->getDamagedRectManager()->redraw();
	
	// Get a graphics port from the parent screen
	GraphicsPort* port = _parent->newGraphicsPort(true);
	
	// Since we're drawing to the parent gadget's graphics port,
	// we need to ensure that we subtract the parent's client rect
	// offset from our drawing co-ordinates
	Rect rect;
	_parent->getClientRect(rect);
	
	// Draw rect
	port->drawXORRect(_newX - rect.x, _newY - rect.y, getWidth(), getHeight());
	
	delete port;
	
	moveTo(_newX, _newY);
}

void Window::onDrag(s16 x, s16 y, s16 vX, s16 vY) {

	// Work out where we're moving to
	s16 destX = x - _grabPointX - _parent->getX();
	s16 destY = y - _grabPointY - _parent->getY();

	// Do we need to move?
	if ((destX != _rect.getX()) || (destY != _rect.getY())) {

		// Prevent window from moving outside screen
		if (!_parent->isPermeable()) {

			Rect rect;
			_parent->getClientRect(rect);

			if (destX < rect.x) {
				destX = rect.x;
			} else if (destX + getWidth() > rect.x + rect.width) {
				destX = rect.x + rect.width - getWidth();
			}

			if (destY < rect.y) {
				destY = rect.y;
			} else if (destY + getHeight() > rect.y + rect.height) {
				destY = rect.y + rect.height - getHeight();
			}
		}

		// Prevent window from leaving top of screen
		if (destY < 0) {
			destY = 0;
		}
		
		woopsiApplication->getDamagedRectManager()->redraw();

		// Get a graphics port from the parent screen
		GraphicsPort* port = _parent->newGraphicsPort(true);

		// Since we're drawing to the parent gadget's graphics port,
		// we need to ensure that we subtract the parent's client rect
		// offset from our drawing co-ordinates
		Rect rect;
		_parent->getClientRect(rect);

		// Erase the old rect
		port->drawXORRect(_newX - rect.x, _newY - rect.y, getWidth(), getHeight());

		// Perform move
		_newX = destX;
		_newY = destY;

		// Draw the new rect
		port->drawXORRect(_newX - rect.x, _newY - rect.y, getWidth(), getHeight());

		delete port;
	}
}

void Window::drawBorder(GraphicsPort* port) {
	port->drawFilledRect(0, 0, getWidth(), getHeight(), getBackColour());
}

void Window::setTitle(const WoopsiString& title) {
	_title = title;

	markRectsDamaged();
}

void Window::onDragStart() {
	
	woopsiApplication->getDamagedRectManager()->redraw();

	// Draw XOR rect

	// Get a graphics port from the parent screen
	GraphicsPort* port = _parent->newGraphicsPort(true);

	// Since we're drawing to the parent gadget's graphics port,
	// we need to ensure that we subtract the parent's client rect
	// offset from our drawing co-ordinates
	Rect rect;
	_parent->getClientRect(rect);

	// Draw rect
	port->drawXORRect(_newX - rect.x, _newY - rect.y, getWidth(), getHeight());

	delete port;
}
