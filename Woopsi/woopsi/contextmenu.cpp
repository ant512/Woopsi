#include "contextmenu.h"
#include "graphicsport.h"
#include "contextmenuitem.h"

using namespace WoopsiUI;

ContextMenu::ContextMenu(FontBase* font) : Gadget(0, 0, 20, 20, 0, font) {
	setBorderless(false);
	_opener = NULL;
	_value = 0;
}

ContextMenuItem* ContextMenu::newMenuItem(const char* text, u32 value) {
	// Create menu item
	ContextMenuItem* item = new ContextMenuItem(0, 0, 0, 0, text, value, _font);
	item->setEventHandler(this);
	addGadget(item);

	// Get client area
	Rect clientRect;
	getClientRect(clientRect);

	// Get menu item's preferred size
	Rect preferredRect;
	item->getPreferredDimensions(preferredRect);

	// Adjust rect's co-ordinates
	preferredRect.x = clientRect.x;
	preferredRect.y = clientRect.y + ((_gadgets.size() - 1) * preferredRect.height);

	// Adjust gadget's co-ordinates
	setPermeable(true);
	item->moveTo(preferredRect.x, preferredRect.y);
	setPermeable(false);

	// Calculate new width of menu
	if (preferredRect.width > clientRect.width) {
		clientRect.width = preferredRect.width;
	}

	// Compensate for border
	clientRect.width += (clientRect.x << 1);

	// Calculate new height of menu
	clientRect.height = (_gadgets.size() * preferredRect.height) + (clientRect.y << 1);

	// Ensure dimensions fit within screen
	if (clientRect.width > SCREEN_WIDTH) clientRect.width = SCREEN_WIDTH;
	if (clientRect.height > SCREEN_HEIGHT) clientRect.height = SCREEN_HEIGHT;

	// Attempt to position menu so that it does not exceed the screen boundaries
	s16 newX = _x;
	s16 newY = _y;
	if (getX() + clientRect.width > SCREEN_WIDTH) {
		newX = SCREEN_WIDTH - clientRect.width;
	}

	if (getY() + clientRect.height > SCREEN_HEIGHT) {
		newY = SCREEN_HEIGHT - clientRect.height;
	}

	// Move
	disableDrawing();
	moveTo(newX, newY);
	enableDrawing();

	resize(clientRect.width, clientRect.height);

	return item;
}

bool ContextMenu::handleEvent(const EventArgs& e) {
	// Only handle release events
	if (e.type == EVENT_RELEASE) {
		if (e.gadget != NULL) {

			// Remember the value of the gadget
			_value = ((ContextMenuItem*)e.gadget)->getValue();

			// Notify the opener that a selection has been made
			if (_opener != NULL) {
				_opener->handleContextMenuSelection(((ContextMenuItem*)e.gadget)->getValue());
			}
	
			shelve();
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

		erase();
		disableDrawing();

		_width = width;
		_height = height;

		setPermeable(true);

		Rect clientRect;
		Rect preferredRect;

		getClientRect(clientRect);

		if (_gadgets.size() > 0) {

			_gadgets[0]->getPreferredDimensions(preferredRect);

			// Resize all children
			for (s32 i = 0; i < _gadgets.size(); i++) {
				_gadgets[i]->resize((u16)clientRect.width, (u16)preferredRect.height);
			}
		}

		setPermeable(false);

		enableDrawing();
		draw();

		raiseResizeEvent(width, height);

		return true;
	}

	return false;
}

void ContextMenu::reset() {

	// Delete children
	for (s32 i = 0; i < _gadgets.size(); i++) {
		_gadgets[i]->destroy();
	}

	_gadgets.clear();
	_opener = NULL;
	_value = 0;

	// Reset dimensions
	_x = 0;
	_y = 0;
	_width = 0;
	_height = 0;
}
