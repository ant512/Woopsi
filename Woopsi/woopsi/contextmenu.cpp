#include "contextmenu.h"
#include "graphicsport.h"
#include "contextmenuitem.h"

ContextMenu::ContextMenu(FontBase* font) : Gadget(0, 0, 20, 20, 0, font) {
	setBorderless(false);
}

ContextMenuItem* ContextMenu::newMenuItem(char* text, u32 value) {
	// Create menu item
	ContextMenuItem* item = new ContextMenuItem(0, 0, 0, 0, text, value, _font);
	item->setEventHandler(this);
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
	if (preferredRect.width + (clientRect.x << 1) > clientRect.width) {
		clientRect.width = preferredRect.width + (clientRect.x << 1);
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

			// Notify the opener that a selection has been made
			_opener->handleContextMenuSelection(((ContextMenuItem*)e.gadget)->getValue());
	
			hide();
			return true;
		}
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

void ContextMenu::reset() {

	// Delete children
	for (u8 i = 0; i < _gadgets.size(); i++) {
		_gadgets[i]->destroy();
	}

	_gadgets.clear();

	// Reset dimensions
	_x = 0;
	_y = 0;
	_width = 0;
	_height = 0;
}
