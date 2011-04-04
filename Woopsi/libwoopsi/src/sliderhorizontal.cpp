#include "sliderhorizontal.h"
#include "sliderhorizontalgrip.h"
#include "graphicsport.h"
#include "range.h"

using namespace WoopsiUI;

SliderHorizontal::SliderHorizontal(s16 x, s16 y, u16 width, u16 height) : Gadget(x, y, width, height) {
	_minimumValue = 0;
	_maximumValue = 0;
	_value = 0;
	_minimumGripWidth = 10;
	_pageSize = 1;

	_flags.permeable = false;
	_flags.borderless = false;
	_flags.doubleClickable = false;
	_flags.draggable = true;

	// Create grip
	Rect rect;
	getClientRect(rect);

	_grip = new SliderHorizontalGrip(rect.x, rect.y, rect.width, rect.height);
	_grip->addGadgetEventHandler(this);
	addGadget(_grip);
}

const s32 SliderHorizontal::getValue() const {

	// Calculate the current value represented by the top of the grip
	Rect rect;
	getClientRect(rect);

	s32 gripPos = _grip->getRelativeX() - rect.x;
	
	s32 span = rect.width - _grip->getWidth();
	
	// If pageSize is greater than 1 we aren't dealing with a slider - we have
	// a scrollbar.  In that situation, we need to subtract the page size from
	// the maximum value to cater for the fact that we only scroll when the
	// page is full.
	s32 maxVal = _pageSize > 1 ? _maximumValue - _pageSize : _maximumValue;
	
	Range range(_minimumValue, maxVal, span);
	
	return range.convertScaledToValue(gripPos);
}

void SliderHorizontal::setValue(const s32 value) {

	Rect rect;
	getClientRect(rect);
	
	s32 span = rect.width - _grip->getWidth();
	
	// If pageSize is greater than 1 we aren't dealing with a slider - we have
	// a scrollbar.  In that situation, we need to subtract the page size from
	// the maximum value to cater for the fact that we only scroll when the
	// page is full.
	s32 maxVal = _pageSize > 1 ? _maximumValue - _pageSize : _maximumValue;
	
	Range range(_minimumValue, maxVal, span);

	s32 newGripPos = range.convertValueToScaled(value);
	
	_grip->moveTo(newGripPos + rect.x, rect.y);
	
	s32 newValue = getValue();
	
	if (newValue != _value) {
		_value = newValue;
		_gadgetEventHandlers->raiseValueChangeEvent();
	}
}

void SliderHorizontal::drawContents(GraphicsPort* port) {
	port->drawFilledRect(0, 0, getWidth(), getHeight(), getDarkColour());
}

void SliderHorizontal::drawBorder(GraphicsPort* port) {

	// Stop drawing if the gadget indicates it should not have an outline
	if (isBorderless()) return;

	port->drawBevelledRect(0, 0, getWidth(), getHeight(), getShadowColour(), getShineColour());
}

void SliderHorizontal::onClick(s16 x, s16 y) {

	// Grip will move either by the page size or by the minimum amount,
	// whichever is smaller.
	s32 step = getMinimumStep();
	
	if (step < _pageSize) step = _pageSize;

	// Which way should the grip move?
	if (x > _grip->getX()) {
		// Move grip right
		setValue(getValue() + step);
	} else {
		// Move grip left
		setValue(getValue() - step);
	}
}

void SliderHorizontal::handleDragEvent(const GadgetEventArgs& e) {

	// Handle grip events
	if ((e.getSource() == _grip) && (e.getSource() != NULL)) {

		s32 newValue = getValue();
		
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
	
	Range range(_minimumValue, _maximumValue, rect.width);

	s32 gripSize = range.convertValueToScaled(_pageSize);

	// If max and min values are the same we just set the grip to the size of
	// the gutter.
	if (_minimumValue == _maximumValue) gripSize = rect.width;
	
	if (gripSize < _minimumGripWidth) {
		gripSize = _minimumGripWidth;
	}
	
	_grip->resize(gripSize, rect.height);
}

void SliderHorizontal::onResize(u16 width, u16 height) {

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

s16 SliderHorizontal::getMinimumStep() const {

	Rect rect;
	getClientRect(rect);
	
	s32 span = rect.width - _grip->getWidth();
	
	Range range(_minimumValue, _maximumValue, span);
	
	s16 step = range.convertScaledToValue(1);
	
	// Ensure that we always return a value of at least 1.
	return step > 0 ? step : 1;
}
