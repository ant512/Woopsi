#include "scrollbarvertical.h"
#include "button.h"
#include "slidervertical.h"
#include "woopsi.h"
#include "woopsitimer.h"

using namespace WoopsiUI;

ScrollbarVertical::ScrollbarVertical(s16 x, s16 y, u16 width, u16 height, GadgetStyle* style) : Gadget(x, y, width, height, GADGET_BORDERLESS, style) {

	_buttonHeight = 10;

	// Create the children
	_slider = new SliderVertical(0, 0, width, height - (_buttonHeight << 1));
	_slider->addGadgetEventHandler(this);

	GadgetBorderSize borderSize;
	borderSize.top = 1;
	borderSize.right = 1;
	borderSize.bottom = 1;
	borderSize.left = 1;

	_upButton = new Button(0, height - (_buttonHeight << 1), width, _buttonHeight, GLYPH_ARROW_UP, &_style);
	_upButton->addGadgetEventHandler(this);
	_upButton->setBorderSize(borderSize);
	_upButton->setFont(getGlyphFont());

	_downButton = new Button(0, height - _buttonHeight, width, _buttonHeight, GLYPH_ARROW_DOWN, &_style);
	_downButton->addGadgetEventHandler(this);
	_downButton->setBorderSize(borderSize);
	_downButton->setFont(getGlyphFont());
	_downButton->addGadgetEventHandler(this);

	// Create timer
	_scrollTimeout = 10;

	_timer = new WoopsiTimer(_scrollTimeout, true);
	_timer->addGadgetEventHandler(this);

	addGadget(_slider);
	addGadget(_upButton);
	addGadget(_downButton);
	addGadget(_timer);
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

void ScrollbarVertical::handleActionEvent(const GadgetEventArgs& e) {

	// Check which gadget fired the event
	if (e.getSource() == _timer) {

		// Which gadget is clicked?
		if (_upButton->isClicked()) {

			// Move the grip up
			_slider->setValue(_slider->getValue() - _slider->getValuesPerPixel());
		} else if (_downButton->isClicked()) {

			// Move the grip down
			_slider->setValue(_slider->getValue() + _slider->getValuesPerPixel());
		}

		_gadgetEventHandlers->raiseValueChangeEvent();
	}
}

void ScrollbarVertical::handleValueChangeEvent(const GadgetEventArgs& e) {
	if (e.getSource() == _slider) {
		_gadgetEventHandlers->raiseValueChangeEvent();
	}
}

void ScrollbarVertical::handleClickEvent(const GadgetEventArgs& e) {

	if (e.getSource() == _upButton) {

		// Start the timer
		_timer->start();

		// Move the grip up
		_slider->setValue(_slider->getValue() - _slider->getValuesPerPixel());

		_gadgetEventHandlers->raiseValueChangeEvent();

	} else if (e.getSource() == _downButton) {

		// Start the timer
		_timer->start();

		// Move the grip down
		_slider->setValue(_slider->getValue() + _slider->getValuesPerPixel());

		_gadgetEventHandlers->raiseValueChangeEvent();
	}
}

void ScrollbarVertical::handleReleaseEvent(const GadgetEventArgs& e) {

	if ((e.getSource() == _upButton) || (e.getSource() == _downButton)) {

		// Stop the timer
		_timer->stop();
	}
}

void ScrollbarVertical::handleReleaseOutsideEvent(const GadgetEventArgs& e) {

	if ((e.getSource() == _upButton) || (e.getSource() == _downButton)) {

		// Stop the timer
		_timer->stop();
	}
}

void ScrollbarVertical::onResize(u16 width, u16 height) {

	// Remember current values
	s16 value = getValue();
	bool events = raisesEvents();

	// Disable event raising
	setRaisesEvents(false);

	// Resize and move children
	_slider->resize(width, height - (_buttonHeight << 1));
	_upButton->moveTo(0, _slider->getHeight());
	_downButton->moveTo(0, _slider->getHeight() + _buttonHeight);

	// Set back to current value
	setValue(value);

	// Reset event raising
	setRaisesEvents(events);
}

s16 ScrollbarVertical::getValuesPerPixel() const {
	return _slider->getValuesPerPixel();
}

