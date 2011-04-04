#include "slidervertical.h"
#include "sliderverticalgrip.h"
#include "graphicsport.h"
#include "range.h"

using namespace WoopsiUI;

SliderVertical::SliderVertical(s16 x, s16 y, u16 width, u16 height) : Gadget(x, y, width, height) {
	_minimumValue = 0;
	_maximumValue = 0;
	_value = 0;
	_minimumGripHeight = 10;
	_pageSize = 1;

	_flags.permeable = false;
	_flags.borderless = false;
	_flags.doubleClickable = false;
	_flags.draggable = true;

	// Create grip
	Rect rect;
	getClientRect(rect);

	_grip = new SliderVerticalGrip(rect.x, rect.y, rect.width, rect.height);
	_grip->addGadgetEventHandler(this);
	addGadget(_grip);
}

const s32 SliderVertical::getValue() const {

	// Calculate the current value represented by the top of the grip
	Rect rect;
	getClientRect(rect);

	s32 gripPos = _grip->getRelativeY() - rect.y;
	
	s32 span = rect.height - _grip->getHeight();
	
	// If pageSize is greater than 1 we aren't dealing with a slider - we have
	// a scrollbar.  In that situation, we need to subtract the page size from
	// the maximum value to cater for the fact that we only scroll when the
	// page is full.
	s32 maxVal = _pageSize > 1 ? _maximumValue - _pageSize : _maximumValue;
	
	Range range(_minimumValue, maxVal, span);
	
	return range.convertScaledToValue(gripPos);
}

void SliderVertical::setValue(const s32 value) {
	
	Rect rect;
	getClientRect(rect);
	
	s32 span = rect.height - _grip->getHeight();
	
	// If pageSize is greater than 1 we aren't dealing with a slider - we have
	// a scrollbar.  In that situation, we need to subtract the page size from
	// the maximum value to cater for the fact that we only scroll when the
	// page is full.
	s32 maxVal = _pageSize > 1 ? _maximumValue - _pageSize : _maximumValue;
	
	Range range(_minimumValue, maxVal, span);

	s32 newGripPos = range.convertValueToScaled(value);
	
	_grip->moveTo(rect.x, newGripPos + rect.y);
	
	s32 newValue = getValue();
	
	if (newValue != _value) {
		_value = newValue;
		_gadgetEventHandlers->raiseValueChangeEvent();
	}
}

void SliderVertical::drawContents(GraphicsPort* port) {
	port->drawFilledRect(0, 0, getWidth(), getHeight(), getDarkColour());
}

void SliderVertical::drawBorder(GraphicsPort* port) {

	// Stop drawing if the gadget indicates it should not have an outline
	if (isBorderless()) return;

	port->drawBevelledRect(0, 0, getWidth(), getHeight(), getShadowColour(), getShineColour());
}

void SliderVertical::onClick(s16 x, s16 y) {

	// Grip will move either by the page size or by the minimum amount,
	// whichever is smaller.
	s32 step = getMinimumStep();
	
	if (step < _pageSize) step = _pageSize;

	// Which way should the grip move?
	if (y > _grip->getY()) {
		// Move grip down
		setValue(getValue() + step);
	} else {
		// Move grip up
		setValue(getValue() - step);
	}
}

void SliderVertical::handleDragEvent(const GadgetEventArgs& e) {

	// Handle grip events
	if ((e.getSource() == _grip) && (e.getSource() != NULL)) {
		
		s32 newValue = getValue();
		
		if (_value != newValue) {
			_value = newValue;
			_gadgetEventHandlers->raiseValueChangeEvent();
		}
	}
}

void SliderVertical::resizeGrip() {

	// Get available size
	Rect rect;
	getClientRect(rect);
	
	Range range(_minimumValue, _maximumValue, rect.height);

	s32 gripSize = range.convertValueToScaled(_pageSize);

	// If max and min values are the same we just set the grip to the size of
	// the gutter.
	if (_minimumValue == _maximumValue) gripSize = rect.height;
	
	if (gripSize < _minimumGripHeight) {
		gripSize = _minimumGripHeight;
	}
	
	_grip->resize(rect.width, gripSize);
}

void SliderVertical::onResize(u16 width, u16 height) {

	// Remember current values
	s32 oldValue = getValue();
	bool events = raisesEvents();

	// Disable event raising
	setRaisesEvents(false);

	resizeGrip();

	// Set back to current value
	setValue(oldValue);

	// Reset event raising
	setRaisesEvents(events);
}

s16 SliderVertical::getMinimumStep() const {

	Rect rect;
	getClientRect(rect);
	
	s32 span = rect.height - _grip->getHeight();
	
	Range range(_minimumValue, _maximumValue, span);
	
	s16 step = range.convertScaledToValue(1);
	
	// Ensure that we always return a value of at least 1.
	return step > 0 ? step : 1;
}
