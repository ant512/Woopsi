#include "scrollbarhorizontal.h"
#include "button.h"
#include "sliderhorizontal.h"
#include "woopsi.h"
#include "woopsitimer.h"

using namespace WoopsiUI;

ScrollbarHorizontal::ScrollbarHorizontal(s16 x, s16 y, u16 width, u16 height, GadgetStyle* style) : Gadget(x, y, width, height, GADGET_BORDERLESS, style) {
	
	_buttonWidth = 10;

	// Create the children
	_slider = new SliderHorizontal(0, 0, width - (_buttonWidth << 1), height);
	_slider->addGadgetEventHandler(this);

	GadgetBorderSize borderSize;
	borderSize.top = 1;
	borderSize.right = 1;
	borderSize.bottom = 1;
	borderSize.left = 1;

	_leftButton = new Button(width - (_buttonWidth << 1), 0, _buttonWidth, height, GLYPH_ARROW_LEFT, &_style);
	_leftButton->addGadgetEventHandler(this);
	_leftButton->setFont(getGlyphFont());
	_leftButton->setBorderSize(borderSize);

	_rightButton = new Button(width - _buttonWidth, 0, _buttonWidth, height, GLYPH_ARROW_RIGHT, &_style);
	_rightButton->addGadgetEventHandler(this);
	_rightButton->setFont(getGlyphFont());
	_rightButton->setBorderSize(borderSize);

	// Create timer
	_scrollTimeout = 10;

	_timer = new WoopsiTimer(_scrollTimeout, true);
	_timer->addGadgetEventHandler(this);

	addGadget(_slider);
	addGadget(_leftButton);
	addGadget(_rightButton);
	addGadget(_timer);
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

void ScrollbarHorizontal::setValueWithBitshift(const s32 value) {
	_slider->setValueWithBitshift(value);
}

void ScrollbarHorizontal::setPageSize(s16 pageSize) {
	_slider->setPageSize(pageSize);
}

void ScrollbarHorizontal::handleActionEvent(const GadgetEventArgs& e) {

	// Check which gadget fired the event
	if (e.getSource() == _timer) {

		// Which gadget is clicked?
		if (_leftButton->isClicked()) {

			// Move the grip left
			_slider->setValue(_slider->getValue() - _slider->getMinimumStep());
		} else if (_rightButton->isClicked()) {

			// Move the grip right
			_slider->setValue(_slider->getValue() + _slider->getMinimumStep());
		}
	}
}

void ScrollbarHorizontal::handleValueChangeEvent(const GadgetEventArgs& e) {
	if (e.getSource() == _slider) {
		_gadgetEventHandlers->raiseValueChangeEvent();
	}
}

void ScrollbarHorizontal::handleClickEvent(const GadgetEventArgs& e) {

	if (e.getSource() == _leftButton) {

		// Start the timer
		_timer->start();

		// Move the grip left
		_slider->setValue(_slider->getValue() - _slider->getMinimumStep());

	} else if (e.getSource() == _rightButton) {

		// Start the timer
		_timer->start();

		// Move the grip right
		_slider->setValue(_slider->getValue() + _slider->getMinimumStep());
	}
}

void ScrollbarHorizontal::handleReleaseEvent(const GadgetEventArgs& e) {

	if ((e.getSource() == _leftButton) || (e.getSource() == _rightButton)) {

		// Stop the timer
		_timer->stop();
	}
}

void ScrollbarHorizontal::handleReleaseOutsideEvent(const GadgetEventArgs& e) {

	if ((e.getSource() == _leftButton) || (e.getSource() == _rightButton)) {

		// Stop the timer
		_timer->stop();
	}
}

void ScrollbarHorizontal::onResize(u16 width, u16 height) {

	// Remember current values
	s16 value = getValue();
	bool events = raisesEvents();

	// Disable event raising
	setRaisesEvents(false);

	// Resize and move children
	_slider->resize(width - (_buttonWidth << 1), height);
	_leftButton->moveTo(_slider->getWidth(), 0);
	_rightButton->moveTo(_slider->getWidth() + _buttonWidth, 0);

	// Set back to current value
	setValue(value);

	// Reset event raising
	setRaisesEvents(events);
}
