#include "scrollinglistbox.h"
#include "scrollbarvertical.h"
#include "graphicsport.h"
#include "woopsipoint.h"

using namespace WoopsiUI;

ScrollingListBox::ScrollingListBox(s16 x, s16 y, u16 width, u16 height, GadgetStyle* style) : Gadget(x, y, width, height, style) {
	_scrollbarWidth = 10;

	setBorderless(true);

	_listbox = new ListBox(0, 0, width - _scrollbarWidth, height, &_style);
	_listbox->setGadgetEventHandler(this);

	// Create scrollbar
	Rect rect;
	_listbox->getClientRect(rect);
	_scrollbar = new ScrollbarVertical(width - _scrollbarWidth, 0, _scrollbarWidth, height, &_style);

	updateScrollbar();
	
	_scrollbar->setGadgetEventHandler(this);

	// Add children to child array
	addGadget(_listbox);
	addGadget(_scrollbar);
}

void ScrollingListBox::drawContents(GraphicsPort* port) {
	port->drawFilledRect(0, 0, getWidth(), getHeight(), getBackColour());
}

void ScrollingListBox::handleValueChangeEvent(Gadget &source) {
	if (&source == _scrollbar) {
		if (_listbox != NULL) {
			_listbox->setRaisesEvents(false);
			_listbox->jump(0, 0 - (_scrollbar->getValue() * _listbox->getOptionHeight()));
			_listbox->setRaisesEvents(true);
		}
	} else if (&source == _listbox) {
		if (raisesEvents()) {
			_gadgetEventHandler->handleValueChangeEvent(*this);
		}
	}
}

void ScrollingListBox::handleScrollEvent(Gadget &source, const WoopsiPoint &delta) {
	if (&source == _listbox) {
		updateScrollbar();
	}
}

void ScrollingListBox::updateScrollbar() {

	if (_scrollbar == NULL) return;

	_scrollbar->setRaisesEvents(false);
				
	Rect rect;
	_listbox->getClientRect(rect);
	
	s32 pageSize = rect.height / _listbox->getOptionHeight();

	_scrollbar->setMaximumValue(_listbox->getOptionCount());
	_scrollbar->setPageSize(pageSize);
	
	// Ditto for value
	u32 div = (0 - _listbox->getCanvasY()) / _listbox->getOptionHeight();
	u32 mod = (0 - _listbox->getCanvasY()) % _listbox->getOptionHeight();
	
	s32 value = div + (2 * mod + _listbox->getOptionHeight()) / (2 * _listbox->getOptionHeight());
				
	_scrollbar->setValue(value);

	_scrollbar->setRaisesEvents(true);
}

void ScrollingListBox::handleDoubleClickEvent(Gadget& source, const WoopsiPoint& point) {
	if (raisesEvents()) {
		_gadgetEventHandler->handleDoubleClickEvent(*this, point);
	}
}

void ScrollingListBox::handleClickEvent(Gadget &source, const WoopsiPoint &point) {
	if (raisesEvents()) {
		_gadgetEventHandler->handleClickEvent(*this, point);
	}
}

void ScrollingListBox::handleReleaseEvent(Gadget &source, const WoopsiPoint &point) {
	if (raisesEvents()) {
		_gadgetEventHandler->handleReleaseEvent(*this, point);
	}
}

void ScrollingListBox::handleReleaseOutsideEvent(Gadget &source, const WoopsiPoint &point) {

	// Child raised a release outside event, but we need to raise a different
	// event if the release occurred within the bounds of this parent gadget
	if (raisesEvents()) {
		if (checkCollision(point.getX(), point.getY())) {
			_gadgetEventHandler->handleReleaseEvent(*this, point);
		} else {
			_gadgetEventHandler->handleReleaseOutsideEvent(*this, point);
		}
	}
}

void ScrollingListBox::handleActionEvent(Gadget &source) {
	if (&source == _listbox) {
		if (raisesEvents()) {
			_gadgetEventHandler->handleActionEvent(*this);
		}
	}
}

void ScrollingListBox::onResize(u16 width, u16 height) {

	// Resize the children
	_listbox->resize(width - _scrollbarWidth, height);
	_scrollbar->resize(_scrollbarWidth, height);

	// Move the scrollbar
	_scrollbar->moveTo(width - _scrollbarWidth, 0);
	
	updateScrollbar();
}

void ScrollingListBox::setFont(FontBase* font) {
	_style.font = font;
	_listbox->setFont(font);
	_scrollbar->setFont(font);
}

void ScrollingListBox::addOption(ListBoxDataItem* item) {
	_listbox->addOption(item);
	_scrollbar->setMaximumValue(_listbox->getOptionCount() - 1);
	
	updateScrollbar();
}

void ScrollingListBox::addOption(const WoopsiString& text, const u32 value) {
	_listbox->addOption(text, value);
	_scrollbar->setMaximumValue(_listbox->getOptionCount() - 1);
	
	updateScrollbar();
}

void ScrollingListBox::addOption(const WoopsiString& text, const u32 value, const u16 normalTextColour, const u16 normalBackColour, const u16 selectedTextColour, const u16 selectedBackColour) {
	_listbox->addOption(text, value, normalTextColour, normalBackColour, selectedTextColour, selectedBackColour);
	_scrollbar->setMaximumValue(_listbox->getOptionCount() - 1);
	
	updateScrollbar();
}

void ScrollingListBox::removeOption(const s32 index) {
	_listbox->removeOption(index);
	_scrollbar->setMaximumValue(_listbox->getOptionCount() - 1);
	
	updateScrollbar();
}

void ScrollingListBox::removeAllOptions() {
	_listbox->removeAllOptions();
	_scrollbar->setMaximumValue(0);
	_scrollbar->setValue(0);
	
	updateScrollbar();
};

// Get the preferred dimensions of the gadget
void ScrollingListBox::getPreferredDimensions(Rect& rect) const {

	// Get the listbox's preferred dimensions
	_listbox->getPreferredDimensions(rect);

	// Add on the scrollbar width
	rect.width += _scrollbarWidth;
}
