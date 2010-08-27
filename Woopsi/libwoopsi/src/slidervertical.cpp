#include "slidervertical.h"
#include "sliderverticalgrip.h"
#include "graphicsport.h"

using namespace WoopsiUI;

SliderVertical::SliderVertical(s16 x, s16 y, u16 width, u16 height) : Gadget(x, y, width, height, GADGET_DRAGGABLE) {
	_minimumValue = 0;
	_maximumValue = 0;
	_contentSize = 0;
	_value = 0;
	_minimumGripHeight = 10;
	_pageSize = 1;

	_flags.permeable = false;
	_flags.borderless = false;
	_flags.doubleClickable = false;

	// Create grip
	Rect rect;
	getClientRect(rect);

	_grip = new SliderVerticalGrip(rect.x, rect.y, rect.width, rect.height);
	_grip->addGadgetEventHandler(this);
	addGadget(_grip);

	_gutterHeight = rect.height;
}

const s32 SliderVertical::getGripValue() const {

	// Calculate the current value represented by the top of the grip
	Rect rect;
	getClientRect(rect);

	u32 gripPos = ((_grip->getY() - getY()) - rect.y);
	u32 scrollRatio = (gripPos << 16) / _gutterHeight;
	s32 value = (scrollRatio * _contentSize);

	return value;
}

void SliderVertical::setValueWithBitshift(const s32 value) {

	Rect rect;
	getClientRect(rect);
	
	// Can the grip move?
	if ((rect.height > _grip->getHeight()) && (_maximumValue != _minimumValue)) {

		s32 newValue = value;

		s32 maxValue = getPhysicalMaximumValueWithBitshift();
		
		// Limit to max/min values
		if (newValue > maxValue) newValue = maxValue;
		if (newValue >> 16 < _minimumValue) newValue = _minimumValue << 16;
		
		u32 scrollRatio = newValue / _contentSize;
		s32 newGripY = _gutterHeight * scrollRatio;
		newGripY += newGripY & 0x8000;
		newGripY >>= 16;
		
		newGripY += rect.y;
		
		_grip->moveTo(rect.x, newGripY);

		// Update stored value if necessary
		if (_value != newValue) {
			_value = newValue;
			_gadgetEventHandlers->raiseValueChangeEvent();
		}
	}
}

void SliderVertical::setValue(const s16 value) {
	setValueWithBitshift(value << 16);
}

s32 SliderVertical::getPhysicalMaximumValueWithBitshift() const {
	u32 maxY = _gutterHeight - _grip->getHeight();

	u32 scrollRatio = (maxY << 16) / _gutterHeight;
	s32 value = (scrollRatio * _contentSize);

	return value;
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

	// Which way should the grip move?
	if (y > _grip->getY()) {
		// Move grip down
		setValueWithBitshift(_value + (_pageSize << 16));
	} else {
		// Move grip up
		setValueWithBitshift(_value - (_pageSize << 16));
	}
}

void SliderVertical::handleDragEvent(const GadgetEventArgs& e) {

	// Handle grip events
	if ((e.getSource() == _grip) && (e.getSource() != NULL)) {

		s32 newValue = getGripValue() >> 16;

		// Grip has moved - compare values and raise event if the
		// value has changed.  Compare using integer values rather
		// than fixed-point.
		if (_value >> 16 != newValue) {
			_value = newValue << 16;
			_gadgetEventHandlers->raiseValueChangeEvent();
		}
	}
}

void SliderVertical::resizeGrip() {

	// Get available size
	Rect rect;
	getClientRect(rect);

	// Prevent divide by 0
	s32 gripRatio = _contentSize > 0 ? (_pageSize << 16) / _contentSize : 1 << 16;

	s32 gripSize = rect.height * gripRatio;

	gripSize >>= 16;
	
	_gutterHeight = rect.height;

	if (gripSize < _minimumGripHeight) {
		// TODO: Need to implement scaling here.  If we resize the grip to be artificially larger,
		// we effectively reduce the scale (not just the height) of the gutter.  Each position
		// in the gutter needs to be reduced in value.
	}
	
	_grip->resize(rect.width, gripSize);
}

void SliderVertical::onResize(u16 width, u16 height) {

	// Remember current values
	s32 oldValue = _value;
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

	// If the ratio of content to gutter is greater than or equal to one,
	// the minimum step that the slider can represent will be that ratio.
	u32 gutterRatio = _contentSize << 16 / _gutterHeight;
	gutterRatio += gutterRatio & 0x8000;
	gutterRatio >>= 16;

	if (gutterRatio > 0) return gutterRatio;

	return 1;
}
