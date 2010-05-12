#include "sliderhorizontal.h"
#include "sliderhorizontalgrip.h"
#include "graphicsport.h"

using namespace WoopsiUI;

SliderHorizontal::SliderHorizontal(s16 x, s16 y, u16 width, u16 height) : Gadget(x, y, width, height, GADGET_DRAGGABLE) {
	_minimumValue = 0;
	_maximumValue = 0;
	_value = 0;
	_minimumGripWidth = 10;
	_pageSize = 1;

	_flags.permeable = true;
	_flags.borderless = false;
	_flags.doubleClickable = false;

	// Create grip
	Rect rect;
	getClientRect(rect);

	_grip = new SliderHorizontalGrip(rect.x, rect.y, rect.width, rect.height);
	_grip->addGadgetEventHandler(this);
	addGadget(_grip);

	_gutterWidth = rect.width;
}

const s16 SliderHorizontal::getGripValue() const {

	// Calculate the current value represented by the left edge of the grip
	Rect rect;
	getClientRect(rect);
	
	// Is the grip smaller than the gutter?
	if (rect.width > _grip->getWidth()) {
	
		// Calculate ratio
		u32 ratio = ((_maximumValue - _minimumValue) << 16) / _gutterWidth;
		
		// Calculate value
		u32 val = ((_grip->getX() - getX()) - rect.x) * ratio;

		// Round up
		val += val & 0x8000;

		// Right shift to erase fractional part and return
		return val >> 16;
	} else {
		// Just return the minimum value
		return _minimumValue;
	}
}

void SliderHorizontal::setValue(const s16 value) {

	// Convert the value to co-ordinates using fixed-point fractional values
	// for accuracy
	Rect rect;
	getClientRect(rect);

	s16 newValue = value;

	// Limit to max/min values
	if (newValue > _maximumValue) newValue = _maximumValue;
	if (newValue < _minimumValue) newValue = _minimumValue;
	
	// Can the grip move?
	if ((rect.width > _grip->getWidth()) && (_maximumValue != _minimumValue)) {
	
		// Calculate ratio
		u32 ratio = (_gutterWidth << 16) / (u32)(_maximumValue - _minimumValue);

		// Convert value using ratio
		s32 newGripX = (newValue * ratio);

		// Round up
		newGripX += newGripX & 0x8000;

		// Bitshift down
		newGripX >>= 16;

		// Add containing client co-ords
		newGripX += rect.x;

		// Adjust new x so that it fits within gutter
		if (newGripX + _grip->getWidth() > rect.x + rect.width) {
			newGripX = (rect.x + rect.width) - _grip->getWidth();
		}

		// Move the grip
		_grip->moveTo(newGripX, rect.y);
	}
}

void SliderHorizontal::drawContents(GraphicsPort* port) {
	port->drawFilledRect(0, 0, _width, _height, getDarkColour());
}

void SliderHorizontal::drawBorder(GraphicsPort* port) {

	// Stop drawing if the gadget indicates it should not have an outline
	if (isBorderless()) return;

	port->drawBevelledRect(0, 0, _width, _height, getShadowColour(), getShineColour());
}

void SliderHorizontal::onClick(s16 x, s16 y) {

	// Which way should the grip move?
	if (x > _grip->getX()) {
		// Move grip right
		setValue(getValue() + _pageSize);
	} else {
		// Move grip left
		setValue(getValue() - _pageSize);
	}
}

void SliderHorizontal::handleDragEvent(const GadgetEventArgs& e) {

	// Handle grip events
	if ((e.getSource() == _grip) && (e.getSource() != NULL)) {

		s16 newValue = getGripValue();

		// Grip has moved
		if (_value != newValue) {
			_value = newValue;
			_gadgetEventHandlers->raiseValueChangeEvent();
		}
	}
}

void SliderHorizontal::handleMoveEvent(const GadgetEventArgs& e) {

	// Handle grip events
	if ((e.getSource() == _grip) && (e.getSource() != NULL)) {

		s16 newValue = getGripValue();

		// Grip has moved
		if (_value != newValue) {
			_value = newValue;
			_gadgetEventHandlers->raiseValueChangeEvent();
		}
	}
}

void SliderHorizontal::resizeGrip() {

	// Get available size
	Rect rect;
	getClientRect(rect);

	// Calculate new width
	s32 newWidth = rect.width;
	
	// Calculate the width of the content that has overflowed the viewport
	s32 overspill = ((s32)(_maximumValue - _minimumValue)) - _pageSize;
	
	// Is there any overflow?
	if (overspill > 0) {
	
		// Calculate the ratio of content to gutter
		u32 ratio = (rect.width << 16) / (u32)(_maximumValue - _minimumValue);
		
		// New width is equivalent to the width of the gutter minus
		// the ratio-converted overflow width
		newWidth = (rect.width << 16) - (overspill * ratio);

		// Round up
		newWidth += newWidth & 0x8000;

		// Bitshift to remove fraction
		newWidth >>= 16;

		// Calculate width of the gutter for use in ratio calculations
		_gutterWidth = rect.width;

		// Ensure width is within acceptable boundaries
		if (newWidth < _minimumGripWidth) {

			// Adjust calculated width of gutter to take into account
			// adjusted grip width.  Since we limit the width of the grip
			// to ensure it doesn't get too small, we must treat the gutter
			// as if it shrinks by the amount that the grip has been width
			_gutterWidth -= _minimumGripWidth - newWidth;

			// Limit width of grip
			newWidth = _minimumGripWidth;
		}

		if (newWidth > rect.width) newWidth = rect.width;
	}

	// Perform resize
	_grip->resize(newWidth, rect.height);
}

void SliderHorizontal::jumpGrip(u8 direction) {

	s32 newGripX;

	// Which way should the grip move?
	if (direction == 1) {
		// Move grip right
		newGripX = (_grip->getX() - getX()) + _grip->getWidth();
	} else {
		// Move grip left
		newGripX = (_grip->getX() - getX()) - _grip->getWidth();
	}

	// Get client rect for this gadget
	Rect rect;
	getClientRect(rect);

	// Adjust x value so that it does not exceed boundaries of gutter
	if (newGripX < rect.x) {
		newGripX = rect.x;
	} else if (newGripX + _grip->getWidth() > rect.x + rect.width) {
		newGripX = (rect.width - _grip->getWidth()) + 1;
	}

	// Move the grip
	_grip->moveTo(newGripX, rect.y);
}

void SliderHorizontal::onResize(u16 width, u16 height) {

	// Remember current values
	s16 value = getValue();
	bool events = raisesEvents();

	// Disable event raising
	setRaisesEvents(false);

	resizeGrip();

	// Set back to current value
	setValue(value);

	// Reset event raising
	setRaisesEvents(events);
}

s16 SliderHorizontal::getValuesPerPixel() const {
	return ((_maximumValue - _minimumValue) / _gutterWidth) + 1;
}
