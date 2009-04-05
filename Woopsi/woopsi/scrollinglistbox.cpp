#include "scrollinglistbox.h"
#include "scrollbarvertical.h"
#include "debug.h"

using namespace WoopsiUI;

ScrollingListBox::ScrollingListBox(s16 x, s16 y, u16 width, u16 height, FontBase* font) : Gadget(x, y, width, height, 0, font) {
	_scrollbarWidth = 9;

	setBorderless(true);

	_flags.shiftClickChildren = false;

	_listbox = new ListBox(0, 0, width - _scrollbarWidth, height, font);
	_listbox->addGadgetEventHandler(this);

	// Create scrollbar
	Rect rect;
	_listbox->getClientRect(rect);
	_scrollbar = new ScrollbarVertical(width - _scrollbarWidth, 0, _scrollbarWidth, height, font);
	_scrollbar->setMinimumValue(0);
	_scrollbar->setMaximumValue(0);
	_scrollbar->setPageSize(rect.height / _listbox->getOptionHeight());
	_scrollbar->addGadgetEventHandler(this);
	_scrollbar->resizeGrip();

	// Add children to child array
	addGadget(_listbox);
	addGadget(_scrollbar);
}

void ScrollingListBox::draw(Rect clipRect) {
	clear(clipRect);
}

void ScrollingListBox::handleValueChangeEvent(const GadgetEventArgs& e) {

	if (e.getSource() != NULL) {
		if (e.getSource() == _scrollbar) {

			if (_listbox != NULL) {
				_listbox->setRaisesEvents(false);
				_listbox->jump(0, 0 - (_scrollbar->getValue() * _listbox->getOptionHeight()));
				_listbox->setRaisesEvents(true);
			}
		}
	}
}

void ScrollingListBox::handleScrollEvent(const GadgetEventArgs& e) {

	if (e.getSource() != NULL) {
		if (e.getSource() == _listbox) {

			if (_scrollbar != NULL) {
				_scrollbar->setRaisesEvents(false);
				_scrollbar->setValue((0 - _listbox->getCanvasY()) / _listbox->getOptionHeight());
				_scrollbar->setRaisesEvents(true);
			}
		}
	}
}

void ScrollingListBox::handleDoubleClickEvent(const GadgetEventArgs& e) {

	if (e.getSource() != NULL) {
		if (e.getSource() == _listbox) {

			// Raise double-click events from list box to event handler
			raiseDoubleClickEvent(e.getX(), e.getY());
		}
	}
}

bool ScrollingListBox::resize(u16 width, u16 height) {

	// Prevent drawing
	bool drawing = _flags.drawingEnabled;
	_flags.drawingEnabled = false;

	// Ensure children are free to adjust
	setPermeable(true);

	// Resize the gadget
	Gadget::resize(width, height);

	// Resize the children
	_listbox->resize(width - _scrollbarWidth, height);
	_scrollbar->resize(_scrollbarWidth, height);

	// Adjust scrollbar page size
	Rect rect;
	getClientRect(rect);
	_scrollbar->setPageSize(rect.height / _listbox->getOptionHeight());

	// Move the scrollbar
	_scrollbar->moveTo(width - _scrollbarWidth, 0);

	// Reset permeable
	setPermeable(false);

	// Reset drawing
	_flags.drawingEnabled = drawing;

	redraw();

	return true;
}

void ScrollingListBox::setFont(FontBase* font) {
	_font = font;
	_listbox->setFont(font);
	_scrollbar->setFont(font);
}

void ScrollingListBox::addOption(const char* text, const u32 value) {
	_listbox->addOption(text, value);
	_scrollbar->setMaximumValue(_listbox->getOptionCount());
	_scrollbar->resizeGrip();
}

void ScrollingListBox::addOption(const char* text, const u32 value, const u16 normalTextColour, const u16 normalBackColour, const u16 selectedTextColour, const u16 selectedBackColour) {
	_listbox->addOption(text, value, normalTextColour, normalBackColour, selectedTextColour, selectedBackColour);
	_scrollbar->setMaximumValue(_listbox->getOptionCount());
	_scrollbar->resizeGrip();
}

void ScrollingListBox::removeOption(const s32 index) {
	_listbox->removeOption(index);
	_scrollbar->setMaximumValue(_listbox->getOptionCount());
	_scrollbar->resizeGrip();

	// Reposition grip if necessary
	if (_scrollbar->getValue() > _listbox->getOptionCount()) _scrollbar->setValue(0);
}

void ScrollingListBox::removeAllOptions() {
	_listbox->removeAllOptions();
	_scrollbar->setMaximumValue(0);
	_scrollbar->setValue(0);
};
