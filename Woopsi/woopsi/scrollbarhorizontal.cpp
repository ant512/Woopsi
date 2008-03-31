#include "scrollbarhorizontal.h"
#include "button.h"
#include "sliderhorizontal.h"

ScrollbarHorizontal::ScrollbarHorizontal(s16 x, s16 y, u16 width, u16 height, FontBase* font) : Gadget(x, y, width, height, GADGET_BORDERLESS) {
	
	_buttonWidth = 10;
	_buttonScrollAmount = 10;

	// Create the children
	_slider = new SliderHorizontal(0, 0, width - (_buttonWidth << 1), height);
	_slider->setEventHandler(this);

	_leftButton = new Button(width - (_buttonWidth << 1), 0, _buttonWidth, height, GLYPH_ARROW_LEFT, _font);
	_leftButton->setEventHandler(this);

	_rightButton = new Button(width - _buttonWidth, 0, _buttonWidth, height, GLYPH_ARROW_RIGHT, _font);
	_rightButton->setEventHandler(this);

	addGadget(_slider);
	addGadget(_leftButton);
	addGadget(_rightButton);
}

ScrollbarHorizontal::~ScrollbarHorizontal() {
}

const s16 ScrollbarHorizontal::getMinimumValue() const {
	return _slider->getMinimumValue();
}

const s16 ScrollbarHorizontal::getMaximumValue() const {
	return _slider->getMaximumValue();
}

const s16 ScrollbarHorizontal::getValue() const {
	return _slider->getValue();
}

const s16 ScrollbarHorizontal::getPageSize() const {
	return _slider->getPageSize();
}

void ScrollbarHorizontal::setMinimumValue(const s16 value) {
	_slider->setMinimumValue(value);
}

void ScrollbarHorizontal::setMaximumValue(const s16 value) {
	_slider->setMaximumValue(value);
}

void ScrollbarHorizontal::setValue(const s16 value) {
	_slider->setValue(value);
}

void ScrollbarHorizontal::setPageSize(s16 pageSize) {
	_slider->setPageSize(pageSize);
}

void ScrollbarHorizontal::draw() {
	Gadget::draw();
}

void ScrollbarHorizontal::draw(Rect clipRect) {
}

bool ScrollbarHorizontal::drag(s16 x, s16 y, s16 vX, s16 vY) {
	// Handle child dragging
	if (_clickedGadget != NULL) {
		return _clickedGadget->drag(x, y, vX, vY);
	}

	return false;
}

bool ScrollbarHorizontal::handleEvent(const EventArgs& e) {

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
	} else if (e.gadget == _leftButton) {

		// Left button; only interested in the release event
		if (e.type == EVENT_RELEASE) {

			// Move the grip up
			_slider->setValue(_slider->getValue() - _buttonScrollAmount);
		}
	} else if (e.gadget == _rightButton) {

		// Down button; only handle release event
		if (e.type == EVENT_RELEASE) {

			// Move the grip down
			_slider->setValue(_slider->getValue() + _buttonScrollAmount);
		}
	}

	return false;
}

void ScrollbarHorizontal::resizeGrip() {
	_slider->resizeGrip();
}

void ScrollbarHorizontal::jumpGrip(u8 direction) {
	_slider->jumpGrip(direction);
}

void ScrollbarHorizontal::setButtonScrollAmount(const u16 buttonScrollAmount) {
	_buttonScrollAmount = buttonScrollAmount;
}

bool ScrollbarHorizontal::resize(u16 width, u16 height) {

	// Remember current values
	s16 value = getValue();
	bool resized = false;
	bool events = raisesEvents();
	bool visible = isVisible();

	// Hide and disable drawing
	erase();
	setVisible(false);

	// Disable event raising
	setRaisesEvents(false);

	if (Gadget::resize(width, height)) {

		// Resize and move children
		_slider->resize(width - (_buttonWidth << 1), height);
		_leftButton->moveTo(_slider->getWidth(), 0);
		_rightButton->moveTo(_slider->getWidth() + _buttonWidth, 0);

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