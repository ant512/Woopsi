#include <nds.h>
#include <fat.h>
#include <sys/dir.h>
#include "filerequester.h"
#include "button.h"
#include "filepath.h"

FileRequester::FileRequester(s16 x, s16 y, u16 width, u16 height, const char* title, const char* path, u32 flags, FontBase* font) : AmigaWindow(x, y, width, height, title, flags, AMIGA_WINDOW_SHOW_DEPTH, font) {

	_flags.shiftClickChildren = false;

	// Padding around the gadgets
	u8 padding = 2;

	// Create the path object
	_path = new FilePath(path);

	Rect rect;
	getClientRect(rect);

	// Calculate list box
	Rect listboxRect;
	listboxRect.width = rect.width - (padding << 1);
	listboxRect.height = rect.height - (padding * 5) - _font->getHeight();
	listboxRect.x = rect.x + padding;
	listboxRect.y = rect.y + padding;

	// Create list box
	_listbox = new ScrollingListBox(listboxRect.x, listboxRect.y, listboxRect.width, listboxRect.height, font);
	_listbox->setEventHandler(this);
	_listbox->setOutlineType(OUTLINE_OUT);
	_listbox->setAllowMultipleSelections(false);
	addGadget(_listbox);

	// Calculate OK button dimensions
	Rect buttonRect;
	buttonRect.width = (rect.width - (padding * 3)) >> 1;
	buttonRect.height = _font->getHeight() + (padding << 1);
	buttonRect.x = rect.x + padding;
	buttonRect.y = (rect.y + rect.height) - (buttonRect.height + padding);

	// Create OK button
	_okButton = new Button(buttonRect.x, buttonRect.y, buttonRect.width, buttonRect.height, "OK");
	_okButton->setEventHandler(this);
	addGadget(_okButton);

	// Calculate cancel button dimensions
	buttonRect.x = rect.x + (padding << 1) + buttonRect.width;
	buttonRect.y = (rect.y + rect.height) - (buttonRect.height + padding);

	// Create cancel button
	_cancelButton = new Button(buttonRect.x, buttonRect.y, buttonRect.width, buttonRect.height, "Cancel");
	_cancelButton->setEventHandler(this);
	addGadget(_cancelButton);

	_path = NULL;
	setPath(path);
}

FileRequester::~FileRequester() {
	delete _path;
}

bool FileRequester::resize(u16 width, u16 height) {
	return false;
}

bool FileRequester::handleEvent(const EventArgs& e) {
	if (e.gadget != NULL) {
		switch (e.type) {
			case EVENT_RELEASE:
				if (e.gadget == _cancelButton) {

					// Close the window
					close();
					return true;
				} else if (e.gadget == _okButton) {

					// Raise value changed event to event handler
					raiseValueChangeEvent();

					// Close the window
					close();
					return true;
				}
				break;
			case EVENT_DOUBLE_CLICK:
				if (e.gadget == _listbox) {

					// Work out which option was clicked - if it was a directory, we move to the new path
					const ListData::ListDataItem* selected = getSelectedOption();

					if (selected != NULL) {

						// Detect type by examining text colour
						if (selected->normalTextColour == _shineColour) {

							// Got a directory
							appendPath(selected->text);
						} else {

							// File selected; raise event
							raiseValueChangeEvent();

							// Close the window
							close();
							return true;
						}
					}
				}
				break;
			default:
				// Ignore other events
				break;
		}
	}

	// Handle other window events
	return AmigaWindow::handleEvent(e);
}

void FileRequester::readDirectory() {

	// Clear current options
	_listbox->removeAllOptions();

	struct stat st;
	char fileName[256];
	s32 fileNumber = 0;

	DIR_ITER* dir = diropen(_path->getPath());

	// Did we get the dir successfully?
	if (dir == NULL) return;

	// Create lists to store file and directory data
	ListData files;
	ListData directories;

	// Ensure lists automatically sort their data
	files.setSortInsertedItems(true);
	directories.setSortInsertedItems(true);

	// Read data into options list
	while (dirnext(dir, fileName, &st) == 0) 
	{
		// Create memory to store the filename in the array
		char* storedFilename = new char[strlen(fileName) + 1];
		strcpy(storedFilename, fileName);

		// st.st_mode & S_IFDIR indicates a directory
		if (st.st_mode & S_IFDIR) {

			// Directory
			directories.addItem(storedFilename, 0, _shineColour, _backColour, _shineColour, _highlightColour);
		} else {

			// File
			files.addItem(storedFilename, 0, _shadowColour, _backColour, _shadowColour, _highlightColour);
		}
	}

	// Push all directories and files into options list and delete all stored filenames
	for (s32 i = 0; i < directories.getItemCount(); i++) {

		// Add directory
		_listbox->addOption(
			directories.getItem(i)->text,
			fileNumber,
			directories.getItem(i)->normalTextColour,
			directories.getItem(i)->normalBackColour,
			directories.getItem(i)->selectedTextColour,
			directories.getItem(i)->selectedBackColour
		);
		
		fileNumber++;
	}
	for (s32 i = 0; i < files.getItemCount(); i++) {

		// Add file
		_listbox->addOption(
			files.getItem(i)->text,
			fileNumber,
			files.getItem(i)->normalTextColour,
			files.getItem(i)->normalBackColour,
			files.getItem(i)->selectedTextColour,
			files.getItem(i)->selectedBackColour
		);
		
		fileNumber++;
	}

	// Close the directory
	dirclose(dir);

	// Redraw the list
	_listbox->draw();
}

void FileRequester::setPath(const char* path) {
	_path->setPath(path);

	// Fetch new directory data
	readDirectory();
}

void FileRequester::appendPath(const char* path) {
	_path->appendPath(path);

	// Fetch new directory data
	readDirectory();
}
