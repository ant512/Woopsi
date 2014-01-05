#include "tabgroup.h"
#include "tab.h"
#include "graphicsport.h"

using namespace WoopsiUI;

TabGroup::TabGroup(s16 x, s16 y, s16 width, s16 height, GadgetStyle* style) : Gadget(x, y, width, height, style) {
	_flags.borderless = true;
	_selectedGadget = NULL;
	_flags.permeable = true;
}

Tab* TabGroup::newTab(const WoopsiString &text) {
	
	Tab* newTab = new Tab(0, 0, getWidth(), getHeight(), text, &_style);
	
	newTab->setGadgetEventHandler(this);
	addGadget(newTab);
	
	if (!_selectedGadget) {
		_selectedGadget = newTab;
		_selectedGadget->activate();
	}

	// All existing tabs need to resize to accommodate the new one.
	u16 width = getWidth() / _gadgets.size();
	u16 remainder = getWidth() % _gadgets.size();
	s16 x = 0;

	for (s32 i = 0; i < _gadgets.size(); ++i) {
		Gadget* gadget = _gadgets[i];
		s16 gadgetWidth = width + (remainder > 0);
		gadget->changeDimensions(x, 0, gadgetWidth, getHeight());
		
		if (remainder) --remainder;
		x += gadgetWidth;
	}
	
	return newTab;
}

const Tab* TabGroup::getSelectedGadget() const {
	return _selectedGadget;
}

const s32 TabGroup::getSelectedIndex() const {
	for (s32 i = 0; i < _gadgets.size(); ++i) {
		if (((Tab*)_gadgets[i]) == _selectedGadget) {
			return i;
		}
	}

	// Nothing selected
	return -1;
}

void TabGroup::setSelectedGadget(Tab* gadget) {
	if (_selectedGadget != gadget) {
		if (_selectedGadget != NULL) {
			_selectedGadget->deactivate();
		}

		_selectedGadget = gadget;

		if (_selectedGadget != NULL) {
			_selectedGadget->activate();
		}

		if (raisesEvents()) {
			_gadgetEventHandler->handleValueChangeEvent(*this);
		}
	}
}

void TabGroup::setSelectedIndex(s32 index) {
	if (index < _gadgets.size()) {
		setSelectedGadget((Tab*)_gadgets[index]);
	}
}

void TabGroup::drawContents(GraphicsPort* port) {
	port->drawFilledRect(0, 0, getWidth(), getHeight(), getBackColour());
}

// Get the preferred dimensions of the gadget
void TabGroup::getPreferredDimensions(Rect& rect) const {
	rect.x = _rect.getX();
	rect.y = _rect.getY();
	rect.width = 0;
	rect.height = 0;

	if (!_flags.borderless) {
		rect.width = _borderSize.left + _borderSize.right;
		rect.height = _borderSize.top + _borderSize.bottom;
	}

	s16 gadgetX = 0;
	s16 gadgetY = 0;

	s16 maxX = 0;
	s16 maxY = 0;

	// Locate largest x and y coords within children
	for (s32 i = 0; i < _gadgets.size(); ++i) {
		gadgetX = _gadgets[i]->getX() + _gadgets[i]->getWidth();
		gadgetY = _gadgets[i]->getY() + _gadgets[i]->getHeight();

		if (gadgetX > maxX) maxX = gadgetX;
		if (gadgetY > maxY) maxY = gadgetY;
	}

	rect.width += maxX - getX();
	rect.height += maxY - getY();
}

void TabGroup::handleDoubleClickEvent(Gadget& source, const WoopsiPoint& point) {
	if (raisesEvents()) {
		_gadgetEventHandler->handleDoubleClickEvent(*this, point);
	}
}

void TabGroup::handleClickEvent(Gadget& source, const WoopsiPoint& point) {
	
	Tab* tab = (Tab*)&source;
	
	if (tab != _selectedGadget) {
		tab->activate();
		_selectedGadget->deactivate();
		_selectedGadget = tab;
	}
	
	if (raisesEvents()) {
		_gadgetEventHandler->handleClickEvent(*this, point);
	}
}

void TabGroup::handleReleaseEvent(Gadget& source, const WoopsiPoint& point) {
	if (raisesEvents()) {
		_gadgetEventHandler->handleReleaseEvent(*this, point);
	}
}

void TabGroup::handleReleaseOutsideEvent(Gadget& source, const WoopsiPoint& point) {

	// Child raised a release outside event, but we need to raise a different
	// event if the release occurred within the bounds of this parent gadget
	if (raisesEvents()) {
		if (checkCollision(point)) {
			_gadgetEventHandler->handleReleaseEvent(*this, point);
		} else {
			_gadgetEventHandler->handleReleaseOutsideEvent(*this, point);
		}
	}
}
