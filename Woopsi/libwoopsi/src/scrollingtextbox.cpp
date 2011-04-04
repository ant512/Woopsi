#include "scrollingtextbox.h"
#include "scrollbarvertical.h"
#include "graphicsport.h"

using namespace WoopsiUI;

ScrollingTextBox::ScrollingTextBox(s16 x, s16 y, u16 width, u16 height, const WoopsiString& text, s16 maxRows, GadgetStyle* style) : Gadget(x, y, width, height, style) {
	_scrollbarWidth = 10;

	setBorderless(true);

	_textbox = new MultiLineTextBox(0, 0, width - _scrollbarWidth, height, text, maxRows, &_style);
	_textbox->addGadgetEventHandler(this);
	
	// Create scrollbar
	Rect rect;
	_textbox->getClientRect(rect);
	_scrollbar = new ScrollbarVertical(width - _scrollbarWidth, 0, _scrollbarWidth, height, &_style);

	updateScrollbar();

	_scrollbar->addGadgetEventHandler(this);

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

const Document* ScrollingTextBox::getDocument() const {
	return _textbox->getDocument();
}

void ScrollingTextBox::setText(const WoopsiString& text) {
	_textbox->setText(text);
}

void ScrollingTextBox::appendText(const WoopsiString& text) {
	_textbox->appendText(text);
}

void ScrollingTextBox::removeText(const u32 startIndex) {
	_textbox->removeText(startIndex);
}

void ScrollingTextBox::removeText(const u32 startIndex, const u32 count) {
	_textbox->removeText(startIndex, count);
}

void ScrollingTextBox::setFont(FontBase* font) {
	_style.font = font;
	_textbox->setFont(font);
	_scrollbar->setFont(font);
}

const u32 ScrollingTextBox::getTextLength() const {
	return _textbox->getTextLength();
}

void ScrollingTextBox::showCursor() {
	_textbox->showCursor();
}

void ScrollingTextBox::hideCursor() {
	_textbox->hideCursor();
}

void ScrollingTextBox::moveCursorToPosition(const s32 position) {
	_textbox->moveCursorToPosition(position);
}

const s32 ScrollingTextBox::getCursorPosition() const {
	return _textbox->getCursorPosition();
}

void ScrollingTextBox::insertText(const WoopsiString& text, const u32 index) {
	_textbox->insertText(text, index);
}
		
void ScrollingTextBox::insertTextAtCursor(const WoopsiString& text) {
	_textbox->insertTextAtCursor(text);
}

const u16 ScrollingTextBox::getPageCount() const {
	return _textbox->getPageCount();
}

const u16 ScrollingTextBox::getCurrentPage() const {
	return _textbox->getCurrentPage();
}

void ScrollingTextBox::handleValueChangeEvent(const GadgetEventArgs& e) {

	if (e.getSource() != NULL) {
		if (e.getSource() == _scrollbar) {

			if (_textbox != NULL) {
				_textbox->setRaisesEvents(false);
				_textbox->jump(0, 0 - _scrollbar->getValue() * _textbox->getDocument()->getLineHeight());
				_textbox->setRaisesEvents(true);
			}
		} else if (e.getSource() == _textbox) {
			updateScrollbar();
		}
	}
}

void ScrollingTextBox::updateScrollbar() {

	if (_scrollbar == NULL) return;

	_scrollbar->setRaisesEvents(false);
				
	Rect rect;
	_textbox->getClientRect(rect);
	
	// Use same scaling method used in Range class to ensure we round correctly
	// when calculating page size
	u32 div = rect.height / _textbox->getDocument()->getLineHeight();
	u32 mod = rect.height % _textbox->getDocument()->getLineHeight();
	
	s32 pageSize = div + (2 * mod + _textbox->getDocument()->getLineHeight()) / (2 * _textbox->getDocument()->getLineHeight());

	_scrollbar->setMaximumValue(_textbox->getDocument()->getLineCount());
	_scrollbar->setPageSize(pageSize);
	
	// Ditto for value
	div = (0 - _textbox->getCanvasY()) / _textbox->getDocument()->getLineHeight();
	mod = (0 - _textbox->getCanvasY()) % _textbox->getDocument()->getLineHeight();
	
	s32 value = div + (2 * mod + _textbox->getDocument()->getLineHeight()) / (2 * _textbox->getDocument()->getLineHeight());
				
	_scrollbar->setValue(value);

	_scrollbar->setRaisesEvents(true);
}

void ScrollingTextBox::handleScrollEvent(const GadgetEventArgs& e) {

	if (e.getSource() != NULL) {
		if (e.getSource() == _textbox) {
			updateScrollbar();
		}
	}
}

void ScrollingTextBox::drawContents(GraphicsPort* port) {
	port->drawFilledRect(0, 0, getWidth(), getHeight(), getBackColour());
}

void ScrollingTextBox::onResize(u16 width, u16 height) {

	// Resize the children
	_textbox->resize(width - _scrollbarWidth, height);
	_scrollbar->resize(_scrollbarWidth, height);

	// Move the scrollbar
	_scrollbar->moveTo(width - _scrollbarWidth, 0);
	
	updateScrollbar();
}

const s32 ScrollingTextBox::getCanvasX() const {
	return _textbox->getCanvasX();
}

const s32 ScrollingTextBox::getCanvasY() const {
	return _textbox->getCanvasY();
}

const s32 ScrollingTextBox::getCanvasWidth() const {
	return _textbox->getCanvasWidth();
}

const s32 ScrollingTextBox::getCanvasHeight() const {
	return _textbox->getCanvasHeight();
}

void ScrollingTextBox::scroll(s32 dx, s32 dy) {
	_textbox->scroll(dx, dy);
}

void ScrollingTextBox::jump(s32 x, s32 y) {
	_textbox->jump(x, y);
}

void ScrollingTextBox::setAllowsVerticalScroll(bool allow) {
	_textbox->setAllowsVerticalScroll(allow);
}

void ScrollingTextBox::setAllowsHorizontalScroll(bool allow) {
	// NOP
}

void ScrollingTextBox::setCanvasWidth(const s32 width) {
	// NOP
}

void ScrollingTextBox::setCanvasHeight(const s32 height) {
	// NOP
}

bool ScrollingTextBox::allowsVerticalScroll() const {
	return _textbox->allowsVerticalScroll();
}

bool ScrollingTextBox::allowsHorizontalScroll() const {
	return _textbox->allowsHorizontalScroll();
}
