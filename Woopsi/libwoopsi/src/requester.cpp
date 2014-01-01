#include "requester.h"
#include "button.h"

using namespace WoopsiUI;

Requester::Requester(s16 x, s16 y, u16 width, u16 height, const WoopsiString& title, GadgetStyle* style) : AmigaWindow(x, y, width, height, title, false, true, style) {

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

	_okButton->setGadgetEventHandler(this);
	addGadget(_okButton);

	// Calculate cancel button dimensions
	buttonRect.x = rect.x + rect.width - buttonRect.width;
	buttonRect.y = (rect.y + rect.height) - buttonRect.height;

	// Create cancel button
	_cancelButton = new Button(buttonRect.x, buttonRect.y, buttonRect.width, buttonRect.height, "Cancel");
	_cancelButton->setGadgetEventHandler(this);
	addGadget(_cancelButton);

	// Calculate list box
	Rect listboxRect;
	listboxRect.width = rect.width;
	listboxRect.height = rect.height - buttonRect.height - 2;
	listboxRect.x = rect.x;
	listboxRect.y = rect.y;

	// Create list box
	_listbox = new ScrollingListBox(listboxRect.x, listboxRect.y, listboxRect.width, listboxRect.height, &_style);
	_listbox->setGadgetEventHandler(this);
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

void Requester::handleReleaseEvent(Gadget& source, const WoopsiPoint& point) {
	if (&source == _cancelButton) {
		close();
	} else if (&source == _okButton) {
		if (raisesEvents()) {
			_gadgetEventHandler->handleValueChangeEvent(*this);
			_gadgetEventHandler->handleActionEvent(*this);
		}

		// Close the window
		close();
	}

	AmigaWindow::handleReleaseEvent(*this, point);
}

void Requester::handleDoubleClickEvent(Gadget& source, const WoopsiPoint& point) {
	if (&source == _listbox) {

		if (raisesEvents()) {
			_gadgetEventHandler->handleValueChangeEvent(*this);
		}

		// Close the window
		close();
	}
	
	AmigaWindow::handleDoubleClickEvent(*this, point);
}
