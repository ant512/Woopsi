#include "sliderhorizontal.h"
#include "sliderhorizontalgrip.h"
#include "graphicsport.h"

using namespace WoopsiUI;

SliderHorizontal::SliderHorizontal(s16 x, s16 y, u16 width, u16 height) : Gadget(x, y, width, height, GADGET_DRAGGABLE) {
	_minimumValue = 0;
	_maximumValue = 0;
	_contentSize = 0;
	_value = 0;
	_minimumGripWidth = 10;
	_pageSize = 1;

	_flags.permeable = false;
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

const s32 SliderHorizontal::getGripValue() const {

	// Calculate the current value represented by the top of the grip
	Rect rect;
	getClientRect(rect);

	u32 gripPos = ((_grip->getX() - getX()) - rect.x);
	u32 scrollRatio = (gripPos << 16) / _gutterWidth;
	s32 value = (scrollRatio * _contentSize);

	return value;
}

void SliderHorizontal::setValueWithBitshift(const s32 value) {

	Rect rect;
	getClientRect(rect);
	
	// Can the grip move?
	if ((rect.width > _grip->getWidth()) && (_maximumValue != _minimumValue)) {

		s32 newValue = value;

		s32 maxValue = getPhysicalMaximumValueWithBitshift();
		
		// Limit to max/min values
		if (newValue > maxValue) newValue = maxValue;
		if (newValue >> 16 < _minimumValue) newValue = _minimumValue << 16;
		
		u32 scrollRatio = newValue / _contentSize;
		s32 newGripX = _gutterWidth * scrollRatio;
		newGripX += newGripX & 0x8000;
		newGripX >>= 16;
		
		newGripX += rect.x;
		
		_grip->moveTo(newGripX, rect.y);

		// Update stored value if necessary
		if (_value != newValue) {
			_value = newValue;
			_gadgetEventHandlers->raiseValueChangeEvent();
		}
	}
}

void SliderHorizontal::setValue(const s16 value) {
	setValueWithBitshift(value << 16);
}

s32 SliderHorizontal::getPhysicalMaximumValueWithBitshift() const {
	u32 maxX = _gutterWidth - _grip->getWidth();

	u32 scrollRatio = (maxX << 16) / _gutterWidth;
	s32 value = (scrollRatio * _contentSize);

	return value;
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
		setValueWithBitshift(_value + (_pageSize << 16));
	} else {
		// Move grip left
		setValueWithBitshift(_value - (_pageSize << 16));
	}
}

void SliderHorizontal::handleDragEvent(const GadgetEventArgs& e) {

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

void SliderHorizontal::resizeGrip() {

	// Get available size
	Rect rect;
	getClientRect(rect);

	s32 gripRatio = (_pageSize << 16) / _contentSize;

	s32 gripSize = rect.width * gripRatio;

	gripSize >>= 16;
	
	_gutterWidth = rect.width;

	if (gripSize < _minimumGripWidth) {
		// TODO: Need to implement scaling here.  If we resize the grip to be artificially larger,
		// we effectively reduce the scale (not just the height) of the gutter.  Each position
		// in the gutter needs to be reduced in value.
	}
	
	_grip->resize(gripSize, rect.height);
}

void SliderHorizontal::onResize(u16 width, u16 height) {

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

s16 SliderHorizontal::getMinimumStep() const {

	// If the ratio of content to gutter is greater than or equal to one,
	// the minimum step that the slider can represent will be that ratio.
	u32 gutterRatio = _contentSize << 16 / _gutterWidth;
	gutterRatio += gutterRatio & 0x8000;
	gutterRatio >>= 16;

	if (gutterRatio > 0) return gutterRatio;

	return 1;
}
