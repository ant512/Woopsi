#include "radiobuttongroup.h"
#include "radiobutton.h"

using namespace WoopsiUI;

RadioButtonGroup::RadioButtonGroup(s16 x, s16 y, FontBase* font) : Gadget(x, y, 0, 0, GADGET_BORDERLESS, font) {
	_selectedGadget = NULL;
}

RadioButton* RadioButtonGroup::newRadioButton(s16 x, s16 y, u16 width, u16 height) {
	
	RadioButton* newButton = new RadioButton(x, y, width, height, _font);
	addGadget(newButton);

	// Do we need to resize?
	u16 newWidth = _width;
	u16 newHeight = _height;

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

		raiseValueChangeEvent();
	}
}

void RadioButtonGroup::setSelectedIndex(s32 index) {
	if (index < _gadgets.size()) {
		setSelectedGadget((RadioButton*)_gadgets[index]);

		raiseValueChangeEvent();
	}
}

void RadioButtonGroup::draw(Rect clipRect) {
	clear(clipRect);
}

bool RadioButtonGroup::resize(u16 width, u16 height) {

	if ((_width != width) || (_height != height)) {
		_width = width;
		_height = height;

		raiseResizeEvent(width, height);

		return true;
	}

	return false;
}
