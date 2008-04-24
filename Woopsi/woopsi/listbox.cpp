#include "listbox.h"
#include "listboxitem.h"
#include "graphicsport.h"

ListBox::ListBox(s16 x, s16 y, u16 width, u16 height, FontBase* font) : Gadget(x, y, width, height, 0, font) {
	_selectedGadget = NULL;
	_outline = OUTLINE_IN;
}

ListBoxItem* ListBox::newListBoxItem(const char* text, const u32 value, const u16 normalTextColour, const u16 normalBackColour, const u16 selectedTextColour, const u16 selectedBackColour) {
	// Create listbox item
	ListBoxItem* item = new ListBoxItem(0, 0, 0, 0, text, value, normalTextColour, normalBackColour, selectedTextColour, selectedBackColour, _font);
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
	preferredRect.width = clientRect.width;

	// Adjust gadget's co-ordinates
	setPermeable(true);
	item->changeDimensions(preferredRect.x, preferredRect.y, preferredRect.width, preferredRect.height);
	setPermeable(false);

	return item;
}

ListBoxItem* ListBox::newListBoxItem(const char* text, const u32 value) {
	return newListBoxItem(text, value, _shadowColour, _backColour, _shadowColour, _highlightColour);
}

bool ListBox::handleEvent(const EventArgs& e) {
	// Handle click events
	if (e.type == EVENT_CLICK) {
		if (e.gadget != NULL) {

			// Selecting or deselecting?
			if (e.gadget != _selectedGadget) {
	
				// Select the gadget
				setSelectedGadget(e.gadget);
			} else {

				// Unselect the gadget
				setSelectedGadget(NULL);
			}

			return true;
		}
	}

	// Handle double-click events
	if (e.type == EVENT_DOUBLE_CLICK) {
		if (e.gadget != NULL) {

			if (e.gadget != _selectedGadget) {
	
				// Select the gadget
				setSelectedGadget(e.gadget);
			}

			return true;
		}
	}

	return false;
}

void ListBox::draw(Rect clipRect) {

	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	port->drawFilledRect(0, 0, _width, _height, _backColour);

	// Draw outline
	port->drawBevelledRect(0, 0, _width, _height);

	delete port;
}

void ListBox::setSelectedIndex(const s32 index) {
	setSelectedGadget(_gadgets[index]);
}

void ListBox::setSelectedGadget(Gadget* gadget) {

	if (gadget != _selectedGadget) {

		// Unselect the current selected item
		if (_selectedGadget != NULL) {
			((ListBoxItem*)_selectedGadget)->unselect();
		}

		// Select the new item
		_selectedGadget = gadget;

		if (_selectedGadget != NULL) {
			((ListBoxItem*)_selectedGadget)->select();
		}

		// Raise value changed event
		if ((_eventHandler != NULL) && (_flags.raisesEvents)) {

			EventArgs newEvent;
			newEvent.eventX = 0;
			newEvent.eventY = 0;
			newEvent.gadget = this;
			newEvent.keyCode = KEY_CODE_NONE;
			newEvent.type = EVENT_VALUE_CHANGE;

			_eventHandler->handleEvent(newEvent);
		}
	}
}
