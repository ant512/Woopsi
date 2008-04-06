#include "contextmenu.h"
#include "graphicsport.h"
#include "contextmenuitem.h"

ContextMenu::ContextMenu(FontBase* font) : Gadget(0, 0, 0, 0, 0, font) {
	setBorderless(false);
}

ContextMenuItem* ContextMenu::newMenuItem(char* text) {
	// Create menu item
	ContextMenuItem* item = new ContextMenuItem(0, 0, 10, 10, text, _font);
	addGadget(item);

	setPermeable(true);

	// Get client area
	Rect clientRect;
	getClientRect(clientRect);

	// Get gadget's preferred size
	Rect rect;
	item->getPreferredDimensions(rect);

	// Adjust rect's co-ordinates
	rect.x = clientRect.x;
	rect.y = (_gadgets.size() - 1) * rect.height;

	// Adjust gadget's dimensions
	item->changeDimensions(rect.x, rect.y, rect.width, rect.height);

	// Resize this gadget if necessary
	if (rect.width > clientRect.width) {
		clientRect.width = rect.width + (_width - clientRect.width);
	}

	clientRect.height = _height + rect.height;

	setPermeable(false);

	resize(clientRect.width, clientRect.height);

	return item;
}

bool ContextMenu::handleEvent(const EventArgs& e) {
	// Only handle release events
	if (e.type == EVENT_RELEASE) {
		if (e.gadget != NULL) {
			//if (e.gadget == _button) {
				close();
				return true;
			//}
		}
	}

	// Handle other events
	if ((_eventHandler != NULL) && (_flags.raisesEvents)) {

		EventArgs newEvent;
		newEvent.eventX = e.eventX;
		newEvent.eventY = e.eventY;
		newEvent.gadget = this;
		newEvent.keyCode = e.keyCode;
		newEvent.type = e.type;

		return _eventHandler->handleEvent(newEvent);
	}

	return false;
}

void ContextMenu::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	port->drawFilledRect(0, 0, _width, _height, _shineColour);

	// Draw outline
	port->drawRect(0, 0, _width, _height, _shadowColour);

	delete port;
}

bool ContextMenu::resize(u16 width, u16 height) {

	if ((_width != width) || (_height != height)) {
		_width = width;
		_height = height;

		raiseResizeEvent(width, height);

		return true;
	}

	return false;
}
