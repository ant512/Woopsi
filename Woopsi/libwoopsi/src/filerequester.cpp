#include <nds.h>
#include "filerequester.h"
#include "button.h"
#include "filepath.h"

using namespace WoopsiUI;

FileRequester::FileRequester(s16 x, s16 y, u16 width, u16 height, const WoopsiString& title, const WoopsiString& path, GadgetStyle* style) : AmigaWindow(x, y, width, height, title, false, true, style) {

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
	
	// Create text box
	Rect textBoxRect;
	_fileNameTextBox = new TextBox(rect.x, buttonRect.y - buttonRect.height - 2, rect.width, buttonRect.height, "");
	_fileNameTextBox->setTextAlignmentHoriz(TextBox::TEXT_ALIGNMENT_HORIZ_LEFT);
	addGadget(_fileNameTextBox);

	// Calculate list box
	Rect listboxRect;
	listboxRect.width = rect.width;
	listboxRect.height = rect.height - ((buttonRect.height + 2) * 2);
	listboxRect.x = rect.x;
	listboxRect.y = rect.y;

	// Create list box
	_listbox = new FileListBox(listboxRect.x, listboxRect.y, listboxRect.width, listboxRect.height, style);
	_listbox->setGadgetEventHandler(this);
	_listbox->setAllowMultipleSelections(false);
	_listbox->setPath(path);
	addGadget(_listbox);
}

void FileRequester::onResize(u16 width, u16 height) {

	// Call base class' method to ensure the basic window resizes correctly
	AmigaWindow::onResize(width, height);

	Rect rect;
	getClientRect(rect);

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

void FileRequester::handleReleaseEvent(Gadget& source, const WoopsiPoint& point) {
	if (&source == _cancelButton) {

		// Close the window
		close();
		return;
	} else if (&source == _okButton) {

		// Raise value changed event to event handler
		if (raisesEvents()) {
			_gadgetEventHandler->handleValueChangeEvent(*this);
		}

		// Close the window
		close();
		return;
	}
	
	AmigaWindow::handleReleaseEvent(*this, point);
}

void FileRequester::handleActionEvent(Gadget &source) {
	if (&source == _listbox) {
		
		// File selected; raise event
		if (raisesEvents()) {
			_gadgetEventHandler->handleValueChangeEvent(*this);
		}
		
		// Update the filename box
		_fileNameTextBox->setText(_listbox->getSelectedOption()->getText());
		
		// Close the window
		close();
	}
}

void FileRequester::handleValueChangeEvent(Gadget &source) {
	if (&source == _listbox) {
		
		// Update the filename box
		const ListDataItem* item = _listbox->getSelectedOption();
		
		if (item != NULL) {
			_fileNameTextBox->setText(item->getText());
		}
	}
}

void FileRequester::setPath(const char* path) {
	_listbox->setPath(path);
}

void FileRequester::appendPath(const char* path) {
	_listbox->appendPath(path);
}

const FilePath* FileRequester::getPath() const {
	return _listbox->getPath();
}

const WoopsiString& FileRequester::getTextBoxFileName() const {
	return _fileNameTextBox->getText();
}
