#include <nds.h>
#include <fcntl.h>
#include <dirent.h>
#include "filerequester.h"
#include "button.h"
#include "filepath.h"

using namespace WoopsiUI;

FileRequester::FileRequester(s16 x, s16 y, u16 width, u16 height, const char* title, const char* path, u32 flags, GadgetStyle* style) : AmigaWindow(x, y, width, height, title, flags, AMIGA_WINDOW_SHOW_DEPTH, style) {

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
	_listbox = new FileListBox(listboxRect.x, listboxRect.y, listboxRect.width, listboxRect.height, path, 0, style);
	_listbox->addGadgetEventHandler(this);
	_listbox->setAllowMultipleSelections(false);
	_listbox->setPath(path);
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

bool FileRequester::resize(u16 width, u16 height) {
	return false;
}

void FileRequester::handleReleaseEvent(const GadgetEventArgs& e) {
	if (e.getSource() != NULL) {
		if (e.getSource() == _cancelButton) {

			// Close the window
			close();
			return;
		} else if (e.getSource() == _okButton) {

			// Raise value changed event to event handler
			raiseValueChangeEvent();

			// Close the window
			close();
			return;
		}
	}
	
	AmigaWindow::handleReleaseEvent(e);
}

void FileRequester::handleValueChangeEvent(const GadgetEventArgs& e) {
	if (e.getSource() != NULL) {
		if (e.getSource() == _listbox) {

			// File selected; raise event
			raiseValueChangeEvent();

			// Close the window
			close();
		}
	}
}

void FileRequester::setPath(const char* path) {
	_listbox->setPath(path);
}

void FileRequester::appendPath(const char* path) {
	_listbox->appendPath(path);
}
