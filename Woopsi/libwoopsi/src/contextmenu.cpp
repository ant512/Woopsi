#include "contextmenu.h"
#include "graphicsport.h"
#include "contextmenueventargs.h"

using namespace WoopsiUI;

ContextMenu::ContextMenu(GadgetStyle* style) : Gadget(0, 0, 20, 20, 0, style) {
	setBorderless(false);
	_opener = NULL;

	_listbox = new ListBox(1, 1, 0, 0, style);
	_listbox->addGadgetEventHandler(this);
	_listbox->setAllowMultipleSelections(false);
	_listbox->setBorderless(true);
	addGadget(_listbox);
}

void ContextMenu::addOption(const char* text, u32 value) {
	_listbox->addOption(text, value, getShadowColour(), getShineColour(), getShineColour(), getShadowColour());

	// Get preferred size of the gadget so we can resize it
	Rect rect;
	getPreferredDimensions(rect);

	// Move and resize
	changeDimensions(rect.x, rect.y, rect.width, rect.height);
}

void ContextMenu::handleReleaseEvent(const GadgetEventArgs& e) {

	if (e.getSource() != NULL) {
		if (e.getSource() == _listbox) {

			// Notify the gadget that opened this menu that an event has
			// occurred, and send the item.
			_opener->handleContextMenuSelection(_listbox->getSelectedOption());

			shelve();
		}
	}
}

void ContextMenu::handleReleaseOutsideEvent(const GadgetEventArgs& e) {

	if (e.getSource() != NULL) {
		if (e.getSource() == _listbox) {

			// Reset any selections
			s32 selectedIndex = _listbox->getSelectedIndex();

			if (selectedIndex > -1) {
				_listbox->deselectOption(selectedIndex);
			}
		}
	}
}

void ContextMenu::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	port->drawFilledRect(0, 0, _width, _height, getShineColour());

	// Draw outline
	port->drawRect(0, 0, _width, _height, getShadowColour());

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
		getClientRect(clientRect);

		_listbox->resize(clientRect.width, clientRect.height);

		setPermeable(false);

		enableDrawing();
		redraw();

		_gadgetEventHandlers->raiseResizeEvent(width, height);

		return true;
	}

	return false;
}

void ContextMenu::reset() {

	_listbox->removeAllOptions();

	_opener = NULL;

	// Reset dimensions
	_x = 0;
	_y = 0;
	_width = 0;
	_height = 0;
}

// Get the preferred dimensions of the gadget
void ContextMenu::getPreferredDimensions(Rect& rect) const {

	// Get the listbox's preferred dimensions
	_listbox->getPreferredDimensions(rect);

	// Adjust width to compensate for border
	rect.width += ((!_flags.borderless) << 1);

	// Adjust x/y as the listbox has no border
	rect.x = _x;
	rect.y = _y;

	// Adjust height so that the gadget shows all options
	rect.height = ((!_flags.borderless) << 1) + (_listbox->getOptionHeight() * _listbox->getOptionCount());

	// Adjust width/height if larger than screen
	if (rect.width > SCREEN_WIDTH) rect.width = SCREEN_WIDTH;
	if (rect.height > SCREEN_HEIGHT) rect.height = SCREEN_HEIGHT;

	// Adjust x/y if the gadget will be displayed off-screen
	if (rect.x + rect.width > SCREEN_WIDTH) {
		rect.x = SCREEN_WIDTH - rect.width;
	}

	if (rect.y + rect.height > SCREEN_HEIGHT) {
		rect.y = SCREEN_HEIGHT - rect.height;
	}
}
