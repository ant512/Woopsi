#include "contextmenu.h"
#include "graphicsport.h"
#include "contextmenueventargs.h"

using namespace WoopsiUI;

ContextMenu::ContextMenu(GadgetStyle* style) : Gadget(0, 0, 20, 20, 0, style) {
	setBorderless(false);
	_opener = NULL;
	
	_flags.canReceiveFocus = false;

	_listbox = new ListBox(1, 1, 0, 0, style);
	_listbox->addGadgetEventHandler(this);
	_listbox->setAllowMultipleSelections(false);
	_listbox->setBorderless(true);
	addGadget(_listbox);
}

void ContextMenu::addOption(const WoopsiString& text, u32 value) {
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

void ContextMenu::drawContents(GraphicsPort* port) {
	port->drawFilledRect(0, 0, getWidth(), getHeight(), getShineColour());
}

void ContextMenu::drawBorder(GraphicsPort* port) {
	port->drawRect(0, 0, getWidth(), getHeight(), getShadowColour());
}

void ContextMenu::onResize(u16 width, u16 height) {

	Rect clientRect;
	getClientRect(clientRect);

	_listbox->resize(clientRect.width, clientRect.height);
}

void ContextMenu::reset() {

	_listbox->removeAllOptions();

	_opener = NULL;

	// Reset dimensions
	_rect.setX(0);
	_rect.setY(0);
	_rect.setWidth(0);
	_rect.setHeight(0);
}

// Get the preferred dimensions of the gadget
void ContextMenu::getPreferredDimensions(Rect& rect) const {

	// Get the listbox's preferred dimensions
	_listbox->getPreferredDimensions(rect);

	// Override the rect's height because we need to show all options
	rect.height = 0;

	// Adjust width to compensate for border
	if (!_flags.borderless) {
		rect.width += _borderSize.left + _borderSize.right;
		rect.height = _borderSize.top + _borderSize.bottom;
	}

	// Adjust x/y as the listbox has no border
	rect.x = _rect.getX();
	rect.y = _rect.getY();

	// Adjust height so that the gadget shows all options
	rect.height += _listbox->getOptionHeight() * _listbox->getOptionCount();

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
