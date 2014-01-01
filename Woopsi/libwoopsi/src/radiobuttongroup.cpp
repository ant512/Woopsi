#include "radiobuttongroup.h"
#include "radiobutton.h"
#include "graphicsport.h"

using namespace WoopsiUI;

RadioButtonGroup::RadioButtonGroup(s16 x, s16 y, GadgetStyle* style) : Gadget(x, y, 0, 0, style) {
	_flags.borderless = true;
	_selectedGadget = NULL;
}

RadioButton* RadioButtonGroup::newRadioButton(s16 x, s16 y, u16 width, u16 height) {
	
	RadioButton* newButton = new RadioButton(x, y, width, height, &_style);
	newButton->setGadgetEventHandler(this);
	addGadget(newButton);

	// Do we need to resize?
	u16 newWidth = getWidth();
	u16 newHeight = getHeight();

	if (newWidth < x + width) {
		newWidth = x + width;
	}

	if (newHeight < y + height) {
		newHeight = y + height;
	}

	resize(newWidth, newHeight);

	return newButton;
}

const RadioButton* RadioButtonGroup::getSelectedGadget() const {
	return (RadioButton*)_selectedGadget;
}

const s32 RadioButtonGroup::getSelectedIndex() const {
	for (s32 i = 0; i < _gadgets.size(); i++) {
		if (((RadioButton*)_gadgets[i]) == _selectedGadget) {
			return i;
		}
	}

	// Nothing selected
	return -1;
}

void RadioButtonGroup::setSelectedGadget(RadioButton* gadget) {
	if (_selectedGadget != gadget) {
		if (_selectedGadget != NULL) {
			_selectedGadget->setState(RadioButton::RADIO_BUTTON_STATE_OFF);
		}

		_selectedGadget = gadget;

		if (_selectedGadget != NULL) {
			_selectedGadget->setState(RadioButton::RADIO_BUTTON_STATE_ON);
		}

		if (raisesEvents()) {
			_gadgetEventHandler->handleValueChangeEvent(*this);
		}
	}
}

void RadioButtonGroup::setSelectedIndex(s32 index) {
	if (index < _gadgets.size()) {
		setSelectedGadget((RadioButton*)_gadgets[index]);

		if (raisesEvents()) {
			_gadgetEventHandler->handleValueChangeEvent(*this);
		};
	}
}

void RadioButtonGroup::drawContents(GraphicsPort* port) {
	port->drawFilledRect(0, 0, getWidth(), getHeight(), getBackColour());
}

// Get the preferred dimensions of the gadget
void RadioButtonGroup::getPreferredDimensions(Rect& rect) const {
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

void RadioButtonGroup::handleDoubleClickEvent(Gadget& source, const WoopsiPoint& point) {
	if (raisesEvents()) {
		_gadgetEventHandler->handleDoubleClickEvent(*this, point);
	}
}

void RadioButtonGroup::handleClickEvent(Gadget& source, const WoopsiPoint& point) {
	if (raisesEvents()) {
		_gadgetEventHandler->handleClickEvent(*this, point);
	}
}

void RadioButtonGroup::handleReleaseEvent(Gadget& source, const WoopsiPoint& point) {
	if (raisesEvents()) {
		_gadgetEventHandler->handleReleaseEvent(*this, point);
	}
}

void RadioButtonGroup::handleReleaseOutsideEvent(Gadget& source, const WoopsiPoint& point) {

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
