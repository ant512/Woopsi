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

ScrollbarVertical::~ScrollbarVertical() {
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

bool ScrollbarVertical::click(s16 x, s16 y) {
	if (_flags.enabled) {
		if (checkCollision(x, y)) {
			_clickedGadget = NULL;

			// Work out which gadget was clicked
			for (s16 i = _gadgets.size() - 1; i > -1; i--) {
				if (_gadgets[i]->click(x, y)) {
					break;
				}
			}

			// Did we click a gadget?
			if (_clickedGadget == NULL) {

				// Handle click on this gadget
				Gadget::click(x, y);
			}

			return true;
		}
	}

	return false;
}

bool ScrollbarVertical::release(s16 x, s16 y) {
	if (_clickedGadget != NULL) {

		// Release clicked gadget
		_clickedGadget->release(x, y);

		return true;
	} else if (_flags.clicked) {

		// Handle release on this
		Gadget::release(x, y);

		return true;
	}

	return false;
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
