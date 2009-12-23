#include "radiobuttongroup.h"
#include "radiobutton.h"

using namespace WoopsiUI;

RadioButtonGroup::RadioButtonGroup(s16 x, s16 y, GadgetStyle* style) : Gadget(x, y, 0, 0, GADGET_BORDERLESS, style) {
	_selectedGadget = NULL;
}

RadioButton* RadioButtonGroup::newRadioButton(s16 x, s16 y, u16 width, u16 height) {
	
	RadioButton* newButton = new RadioButton(x, y, width, height, _style);
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

		_gadgetEventHandlers->raiseValueChangeEvent();
	}
}

void RadioButtonGroup::setSelectedIndex(s32 index) {
	if (index < _gadgets.size()) {
		setSelectedGadget((RadioButton*)_gadgets[index]);

		_gadgetEventHandlers->raiseValueChangeEvent();
	}
}

void RadioButtonGroup::draw(Rect clipRect) {
	clear(clipRect);
}

bool RadioButtonGroup::resize(u16 width, u16 height) {

	// Remember current values
	bool resized = false;
	bool drawing = _flags.drawingEnabled;

	if ((_width != width) || (_height != height)) {

		// Hide and disable drawing
		erase();
		_flags.drawingEnabled = false;

		// Attempt to resize
		if (Gadget::resize(width, height)) {
			resized = true;
		}
	}

	// Show and reset drawing
	_flags.drawingEnabled = drawing;
	redraw();
	
	return resized;
}

// Get the preferred dimensions of the gadget
void RadioButtonGroup::getPreferredDimensions(Rect& rect) const {
	rect.x = _x;
	rect.y = _y;

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

	rect.width = (!_flags.borderless << 1) + maxX - getX();
	rect.height = (!_flags.borderless << 1) + maxY - getY();
}

