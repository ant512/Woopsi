#include "contextmenu.h"
#include "graphicsport.h"
#include "contextmenuitem.h"

ContextMenu::ContextMenu(FontBase* font) : Gadget(0, 0, 0, 0, 0, font) {
	setBorderless(false);
}

ContextMenuItem* ContextMenu::newMenuItem(char* text) {
	// Create menu item
	ContextMenuItem* item = new ContextMenuItem(0, 0, 0, 0, text, _font);
	addGadget(item);

	setPermeable(true);

	// Get client area
	Rect clientRect;
	getClientRect(clientRect);

	// Get gadget's preferred size
	Rect preferredRect;
	item->getPreferredDimensions(preferredRect);

	// Adjust rect's co-ordinates
	preferredRect.x = clientRect.x;
	preferredRect.y = clientRect.y + ((_gadgets.size() - 1) * preferredRect.height);

	// Adjust gadget's co-ordinates
	item->moveTo(preferredRect.x, preferredRect.y);

	// Resize this gadget if necessary
	if (preferredRect.width > clientRect.width) {
		clientRect.width = preferredRect.width + (_width - clientRect.width);
	}

	clientRect.height = (_gadgets.size() * preferredRect.height) + (clientRect.y << 1);

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

		setPermeable(true);

		Rect clientRect;
		Rect preferredRect;

		getClientRect(clientRect);

		if (_gadgets.size() > 0) {

			_gadgets[0]->getPreferredDimensions(preferredRect);

			// Resize all children
			for (u8 i = 0; i < _gadgets.size(); i++) {
				_gadgets[i]->resize((u16)clientRect.width, (u16)preferredRect.height);
			}
		}

		if (_hiddenGadgets.size() > 0) {

			_hiddenGadgets[0]->getPreferredDimensions(preferredRect);

			// Resize all hidden children
			for (u8 i = 0; i < _hiddenGadgets.size(); i++) {
				_hiddenGadgets[i]->resize((u16)clientRect.width, (u16)preferredRect.height);
			}
		}

		setPermeable(false);

		raiseResizeEvent(width, height);

		return true;
	}

	return false;
}
