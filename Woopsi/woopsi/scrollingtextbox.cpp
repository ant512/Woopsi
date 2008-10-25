#include "scrollingtextbox.h"
#include "scrollbarvertical.h"

using namespace WoopsiUI;

ScrollingTextBox::ScrollingTextBox(s16 x, s16 y, u16 width, u16 height, const char* text, u32 flags, s16 maxRows, FontBase* font) : Gadget(x, y, width, height, flags, font) {
	_scrollbarWidth = 9;

	setBorderless(true);

	_flags.shiftClickChildren = false;

	_textbox = new MultiLineTextBox(0, 0, width - _scrollbarWidth, height, text, flags, maxRows, font);
	_textbox->setEventHandler(this);
	
	// Create scrollbar
	Rect rect;
	_textbox->getClientRect(rect);
	_scrollbar = new ScrollbarVertical(width - _scrollbarWidth, 0, _scrollbarWidth, height, font);
	_scrollbar->setMinimumValue(0);
	_scrollbar->setMaximumValue(_textbox->getCanvasHeight());
	_scrollbar->setPageSize(rect.height);
	_scrollbar->setButtonScrollAmount(10);
	_scrollbar->setEventHandler(this);
	_scrollbar->resizeGrip();

	// Add children to child array
	addGadget(_textbox);
	addGadget(_scrollbar);
}

void ScrollingTextBox::setTextAlignmentHoriz(MultiLineTextBox::TextAlignmentHoriz alignment) {
	_textbox->setTextAlignmentHoriz(alignment);
}

void ScrollingTextBox::setTextAlignmentVert(MultiLineTextBox::TextAlignmentVert alignment) {
	_textbox->setTextAlignmentVert(alignment);
}

const Text* ScrollingTextBox::getText() const {
	return _textbox->getText();
}

void ScrollingTextBox::setText(const char* text) {
	_textbox->setText(text);
	_scrollbar->draw();
}

void ScrollingTextBox::setText(const char text) {
	_textbox->setText(text);
	_scrollbar->draw();
}

void ScrollingTextBox::appendText(const char* text) {
	_textbox->appendText(text);
	_scrollbar->draw();
}

void ScrollingTextBox::appendText(const char text) {
	_textbox->appendText(text);
	_scrollbar->draw();
}

void ScrollingTextBox::setFont(FontBase* font) {
	_font = font;
	_textbox->setFont(font);
	_scrollbar->setFont(font);
}

const u16 ScrollingTextBox::getPageCount() const {
	return _textbox->getPageCount();
}

const u16 ScrollingTextBox::getCurrentPage() const {
	return _textbox->getCurrentPage();
}

bool ScrollingTextBox::handleEvent(const EventArgs& e) {

	if (e.gadget != NULL) {
		if (e.gadget == _scrollbar) {

			// Slider events
			switch (e.type) {
				case EVENT_VALUE_CHANGE:
					if (_textbox != NULL) {
						_textbox->setRaisesEvents(false);
						_textbox->jump(0, 0 - _scrollbar->getValue());
						_textbox->setRaisesEvents(true);
						return true;
					}
					break;
				default:
					break;
			}
		} else if (e.gadget == _textbox) {

			// Textbox events
			switch (e.type) {
				case EVENT_SCROLL:
					if (_scrollbar != NULL) {
						_scrollbar->setRaisesEvents(false);
						_scrollbar->setMaximumValue(_textbox->getCanvasHeight());
						_scrollbar->resizeGrip();
						_scrollbar->setValue(0 - _textbox->getCanvasY());
						_scrollbar->setRaisesEvents(true);
						return true;
					}
					break;
				default:
					break;
			}
		}

		// Raise events to event handler
		if ((_eventHandler != NULL) && (_flags.raisesEvents)) {
			EventArgs newEvent;
			newEvent.eventX = e.eventX;
			newEvent.eventY = e.eventY;
			newEvent.gadget = this;
			newEvent.keyCode = e.keyCode;
			newEvent.type = e.type;

			_eventHandler->handleEvent(newEvent);
		}
	}

	return false;
}

void ScrollingTextBox::draw(Rect clipRect) {
	clear(clipRect);
}

bool ScrollingTextBox::resize(u16 width, u16 height) {

	// Prevent drawing
	bool drawing = _flags.drawingEnabled;
	_flags.drawingEnabled = false;

	// Ensure children are free to adjust
	setPermeable(true);

	// Resize the gadget
	Gadget::resize(width, height);

	// Resize the children
	_textbox->resize(width - _scrollbarWidth, height);
	_scrollbar->resize(_scrollbarWidth, height);

	// Move the scrollbar
	_scrollbar->moveTo(width - _scrollbarWidth, 0);

	// Reset permeable
	setPermeable(false);

	// Reset drawing
	_flags.drawingEnabled = drawing;

	draw();

	return true;
}
