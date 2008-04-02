#include "scrollbarvertical.h"
#include "button.h"
#include "slidervertical.h"

ScrollbarVertical::ScrollbarVertical(s16 x, s16 y, u16 width, u16 height, FontBase* font) : Gadget(x, y, width, height, GADGET_BORDERLESS) {
	
	_buttonHeight = 10;
	_buttonScrollAmount = 10;

	// Create the children
	_slider = new SliderVertical(0, 0, width, height - (_buttonHeight << 1));
	_slider->setEventHandler(this);

	_upButton = new Button(0, height - (_buttonHeight << 1), width, _buttonHeight, GLYPH_ARROW_UP, _font);
	_upButton->setEventHandler(this);

	_downButton = new Button(0, height - _buttonHeight, width, _buttonHeight, GLYPH_ARROW_DOWN, _font);
	_downButton->setEventHandler(this);

	addGadget(_slider);
	addGadget(_upButton);
	addGadget(_downButton);
}

const s16 ScrollbarVertical::getMinimumValue() const {
	return _slider->getMinimumValue();
}

const s16 ScrollbarVertical::getMaximumValue() const {
	return _slider->getMaximumValue();
}

const s16 ScrollbarVertical::getValue() const {
	return _slider->getValue();
}

const s16 ScrollbarVertical::getPageSize() const {
	return _slider->getPageSize();
}

void ScrollbarVertical::setMinimumValue(const s16 value) {
	_slider->setMinimumValue(value);
}

void ScrollbarVertical::setMaximumValue(const s16 value) {
	_slider->setMaximumValue(value);
}

void ScrollbarVertical::setValue(const s16 value) {
	_slider->setValue(value);
}

void ScrollbarVertical::setPageSize(s16 pageSize) {
	_slider->setPageSize(pageSize);
}

void ScrollbarVertical::draw() {
	Gadget::draw();
}

void ScrollbarVertical::draw(Rect clipRect) {
}

bool ScrollbarVertical::drag(s16 x, s16 y, s16 vX, s16 vY) {
	// Handle child dragging
	if (_clickedGadget != NULL) {
		return _clickedGadget->drag(x, y, vX, vY);
	}

	return false;
}

bool ScrollbarVertical::handleEvent(const EventArgs& e) {

	// Check which gadget fired the event
	if (e.gadget == _slider) {
	
		// Raise slider events to event handler, replacing
		// the gadget pointer with a pointer to this
		if ((_eventHandler != NULL) && (_flags.raisesEvents)) {

			EventArgs newEvent;
			newEvent.eventX = e.eventX;
			newEvent.eventY = e.eventY;
			newEvent.gadget = this;
			newEvent.keyCode = e.keyCode;
			newEvent.type = e.type;

			_eventHandler->handleEvent(newEvent);
		}
	} else if (e.gadget == _upButton) {

		// Up button; only interested in the release event
		if (e.type == EVENT_RELEASE) {

			// Move the grip up
			_slider->setValue(_slider->getValue() - _buttonScrollAmount);
		}
	} else if (e.gadget == _downButton) {

		// Down button; only handle release event
		if (e.type == EVENT_RELEASE) {

			// Move the grip down
			_slider->setValue(_slider->getValue() + _buttonScrollAmount);
		}
	}

	return false;
}

void ScrollbarVertical::resizeGrip() {
	_slider->resizeGrip();
}

void ScrollbarVertical::jumpGrip(u8 direction) {
	_slider->jumpGrip(direction);
}

void ScrollbarVertical::setButtonScrollAmount(const u16 buttonScrollAmount) {
	_buttonScrollAmount = buttonScrollAmount;
}

bool ScrollbarVertical::resize(u16 width, u16 height) {

	// Remember current value
	s16 value = getValue();
	bool resized = false;
	bool events = raisesEvents();
	bool visible = isVisible();

	// Disable event raising
	setRaisesEvents(false);

	// Hide and disable drawing
	erase();
	setVisible(false);

	if (Gadget::resize(width, height)) {

		// Resize and move children
		_slider->resize(width, height - (_buttonHeight << 1));
		_upButton->moveTo(0, _slider->getHeight());
		_downButton->moveTo(0, _slider->getHeight() + _buttonHeight);

		// Set back to current value
		setValue(value);

		resized = true;
	}

	// Show and reset drawing
	setVisible(visible);
	draw();

	// Reset event raising
	setRaisesEvents(events);

	return resized;
}
