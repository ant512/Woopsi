#include "requester.h"
#include "button.h"

using namespace WoopsiUI;

Requester::Requester(s16 x, s16 y, u16 width, u16 height, const char* title, GadgetStyle* style) : AmigaWindow(x, y, width, height, title, GADGET_DRAGGABLE, AMIGA_WINDOW_SHOW_DEPTH, style) {

	_flags.shiftClickChildren = false;

	// Padding around the gadgets
	u8 padding = 2;

	Rect rect;
	getClientRect(rect);

	// Calculate list box
	Rect listboxRect;
	listboxRect.width = rect.width - (padding << 1);
	listboxRect.height = rect.height - (padding * 5) - getFont()->getHeight();
	listboxRect.x = rect.x + padding;
	listboxRect.y = rect.y + padding;

	// Create list box
	_listbox = new ScrollingListBox(listboxRect.x, listboxRect.y, listboxRect.width, listboxRect.height, _style);
	_listbox->addGadgetEventHandler(this);
	_listbox->setOutlineType(OUTLINE_OUT);
	addGadget(_listbox);

	// Calculate OK button dimensions
	Rect buttonRect;
	buttonRect.width = (rect.width - (padding * 3)) >> 1;
	buttonRect.height = getFont()->getHeight() + (padding << 1);
	buttonRect.x = rect.x + padding;
	buttonRect.y = (rect.y + rect.height) - (buttonRect.height + padding);

	// Create OK button
	_okButton = new Button(buttonRect.x, buttonRect.y, buttonRect.width, buttonRect.height, "OK");
	_okButton->addGadgetEventHandler(this);
	addGadget(_okButton);

	// Calculate cancel button dimensions
	buttonRect.x = rect.x + (padding << 1) + buttonRect.width;
	buttonRect.y = (rect.y + rect.height) - (buttonRect.height + padding);

	// Create cancel button
	_cancelButton = new Button(buttonRect.x, buttonRect.y, buttonRect.width, buttonRect.height, "Cancel");
	_cancelButton->addGadgetEventHandler(this);
	addGadget(_cancelButton);
}

bool Requester::resize(u16 width, u16 height) {
	return false;
}

void Requester::handleReleaseEvent(const GadgetEventArgs& e) {
	if (e.getSource() != NULL) {

		if (e.getSource() == _cancelButton) {

			// Close the window
			close();
		} else if (e.getSource() == _okButton) {

			// Raise value changed event to event handler
			_gadgetEventHandlers->raiseValueChangeEvent();

			// Close the window
			close();
		}
	}

	AmigaWindow::handleReleaseEvent(e);
}

void Requester::handleDoubleClickEvent(const GadgetEventArgs& e) {
	if (e.getSource() != NULL) {
		if (e.getSource() == _listbox) {

			// Raise value changed event to event handler
			_gadgetEventHandlers->raiseValueChangeEvent();

			// Close the window
			close();
		}
	}
	
	AmigaWindow::handleDoubleClickEvent(e);
}
