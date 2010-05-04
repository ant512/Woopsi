#include "requester.h"
#include "button.h"

using namespace WoopsiUI;

Requester::Requester(s16 x, s16 y, u16 width, u16 height, const WoopsiString& title, GadgetStyle* style) : AmigaWindow(x, y, width, height, title, GADGET_DRAGGABLE, AMIGA_WINDOW_SHOW_DEPTH, style) {

	Rect rect;
	getClientRect(rect);

	// Adjust rect to leave 2px padding around children
	rect.x += 2;
	rect.y += 2;
	rect.width -= 4;
	rect.height -= 4;

	// Create OK button
	_okButton = new Button(0, 0, 0, 0, "OK");

	Rect buttonRect;
	_okButton->getPreferredDimensions(buttonRect);

	// Calculate OK button dimensions
	buttonRect.width = (rect.width >> 1) - 1;
	buttonRect.x = rect.x;
	buttonRect.y = (rect.y + rect.height) - buttonRect.height;

	_okButton->changeDimensions(buttonRect.x, buttonRect.y, buttonRect.width, buttonRect.height);

	_okButton->addGadgetEventHandler(this);
	addGadget(_okButton);

	// Calculate cancel button dimensions
	buttonRect.x = rect.x + rect.width - buttonRect.width;
	buttonRect.y = (rect.y + rect.height) - buttonRect.height;

	// Create cancel button
	_cancelButton = new Button(buttonRect.x, buttonRect.y, buttonRect.width, buttonRect.height, "Cancel");
	_cancelButton->addGadgetEventHandler(this);
	addGadget(_cancelButton);

	// Calculate list box
	Rect listboxRect;
	listboxRect.width = rect.width;
	listboxRect.height = rect.height - buttonRect.height - 2;
	listboxRect.x = rect.x;
	listboxRect.y = rect.y;

	// Create list box
	_listbox = new ScrollingListBox(listboxRect.x, listboxRect.y, listboxRect.width, listboxRect.height, &_style);
	_listbox->addGadgetEventHandler(this);
	addGadget(_listbox);
}

void Requester::onResize(u16 width, u16 height) {

	// Call base class' method to ensure the basic window resizes correctly
	AmigaWindow::onResize(width, height);

	Rect rect;
	getClientRect(rect);

	// Adjust rect to leave 2px padding around children
	rect.x += 2;
	rect.y += 2;
	rect.width -= 4;
	rect.height -= 4;

	// Calculate OK button dimensions
	Rect buttonRect;
	_okButton->getPreferredDimensions(buttonRect);

	buttonRect.width = (rect.width >> 1) - 1;
	buttonRect.x = rect.x;
	buttonRect.y = (rect.y + rect.height) - buttonRect.height;

	_okButton->changeDimensions(buttonRect.x, buttonRect.y, buttonRect.width, buttonRect.height);

	// Calculate cancel button dimensions
	buttonRect.x = rect.x + rect.width - buttonRect.width;
	buttonRect.y = (rect.y + rect.height) - buttonRect.height;

	_cancelButton->changeDimensions(buttonRect.x, buttonRect.y, buttonRect.width, buttonRect.height);

	// Calculate list box dimensions
	Rect listboxRect;
	listboxRect.width = rect.width;
	listboxRect.height = rect.height - buttonRect.height - 2;
	listboxRect.x = rect.x;
	listboxRect.y = rect.y;

	_listbox->changeDimensions(listboxRect.x, listboxRect.y, listboxRect.width, listboxRect.height);
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
