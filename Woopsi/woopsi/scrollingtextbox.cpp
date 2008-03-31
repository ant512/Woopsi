#include "scrollingtextbox.h"
#include "scrollbarvertical.h"

ScrollingTextBox::ScrollingTextBox(s16 x, s16 y, u16 width, u16 height, char* text, u32 flags, s16 maxRows, FontBase* font) : Gadget(x, y, width, height, flags, font) {
	_scrollbarWidth = 9;

	setBorderless(true);

	_textbox = new MultiLineTextBox(0, 0, width - _scrollbarWidth, height, text, GADGET_DRAGGABLE | flags, maxRows, font);
	_textbox->setEventHandler(this);
	
	// Create scrollbar
	Rect rect;
	_textbox->getClientRect(rect);
	_scrollbar = new ScrollbarVertical(width - _scrollbarWidth, 0, _scrollbarWidth, height, font);
	_scrollbar->setMinimumValue(0);
	_scrollbar->setMaximumValue(0);
	_scrollbar->setPageSize(rect.height);
	_scrollbar->setEventHandler(this);

	// Add children to child array
	addGadget(_textbox);
	addGadget(_scrollbar);
}

ScrollingTextBox::~ScrollingTextBox() {
}

void ScrollingTextBox::setTextPositionHoriz(MultiLineTextBox::TextPositionHoriz position) {
	_textbox->setTextPositionHoriz(position);
}

void ScrollingTextBox::setTextPositionVert(MultiLineTextBox::TextPositionVert position) {
	_textbox->setTextPositionVert(position);
}

char* ScrollingTextBox::getRawText() {
	return _textbox->getRawText();
}

const Text* ScrollingTextBox::getText() const {
	return _textbox->getText();
}

void ScrollingTextBox::setText(char* text) {
	_textbox->setText(text);
}

void ScrollingTextBox::addText(char* text) {
	_textbox->addText(text);
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

bool ScrollingTextBox::resize(u16 width, u16 height) {
	return false;
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
				case EVENT_DRAG:
					if (_scrollbar != NULL) {
						_scrollbar->setRaisesEvents(false);
						_scrollbar->setValue(0 - _textbox->getCanvasY());
						_scrollbar->setRaisesEvents(true);
						return true;
					}
					break;
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
	}
	
	return false;
}

void ScrollingTextBox::draw() {
	Gadget::draw();
}

void ScrollingTextBox::draw(Rect clipRect) {
	clear(clipRect);
}
