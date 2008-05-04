#include <nds.h>
#include <fat.h>
#include <sys/dir.h>
#include "filerequester.h"
#include "button.h"
#include "dynamicarray.h"

FileRequester::FileRequester(s16 x, s16 y, u16 width, u16 height, char* title, char* path, FontBase* font) : AmigaWindow(x, y, width, height, title, GADGET_DRAGGABLE, AMIGA_WINDOW_SHOW_DEPTH, font) {

	_flags.shiftClickChildren = false;

	// Padding around the gadgets
	u8 padding = 2;

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
					if ((_eventHandler != NULL) && (_flags.raisesEvents)) {
						EventArgs newEvent;
						newEvent.eventX = e.eventX;
						newEvent.eventY = e.eventY;
						newEvent.gadget = this;
						newEvent.keyCode = e.keyCode;
						newEvent.type = EVENT_VALUE_CHANGE;

						_eventHandler->handleEvent(newEvent);
					}

					// Close the window
					close();
					return true;
				}
				break;
			case EVENT_DOUBLE_CLICK:
				if (e.gadget == _listbox) {

					// Work out which option was clicked - if it was a directory, we move to the new path
					const ListBox::ListBoxOption* selected = getSelectedOption();

					if (selected != NULL) {

						// Detect type by examining text colour
						if (selected->normalTextColour == _shineColour) {

							// Got a directory
							appendPath(selected->text);
						} else {

							// File selected; raise event
							if ((_eventHandler != NULL) && (_flags.raisesEvents)) {
								EventArgs newEvent;
								newEvent.eventX = e.eventX;
								newEvent.eventY = e.eventY;
								newEvent.gadget = this;
								newEvent.keyCode = e.keyCode;
								newEvent.type = EVENT_VALUE_CHANGE;

								_eventHandler->handleEvent(newEvent);
							}

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

	DIR_ITER* dir = diropen(_path);

	// Did we get the dir successfully?
	if (dir == NULL) return;

	// Create vectors to store file and directory data
	DynamicArray<char*> files;
	DynamicArray<char*> directories;
	DynamicArray<char*>* currentArray = NULL;

	// Read data into options list
	while (dirnext(dir, fileName, &st) == 0) 
	{
		// Create memory to store the filename in the array
		char* storedFilename = new char[strlen(fileName) + 1];
		strcpy(storedFilename, fileName);

		// st.st_mode & S_IFDIR indicates a directory
		if (st.st_mode & S_IFDIR) {

			// Directory
			currentArray = &directories;
		} else {

			// File
			currentArray = &files;
		}

		// Sorted insert into correct array
		s32 i = 0;

		// Locate slot where new name should go
		while ((i < currentArray->size()) && (strcmp(currentArray->at(i), storedFilename) < 0)) {
			i++;
		}

		// Insert new name
		currentArray->insert(i, storedFilename);
	}

	// Push all directories and files into options list and delete all stored filenames
	for (s32 i = 0; i < directories.size(); i++) {

		// Add option
		addOption(directories[i], fileNumber, _shineColour, _backColour, _shineColour, _highlightColour);

		// Delete data
		delete [] directories[i];
		
		fileNumber++;
	}
	for (s32 i = 0; i < files.size(); i++) {

		// Add option
		addOption(files[i], fileNumber);

		// Delete data
		delete [] files[i];
		
		fileNumber++;
	}

	// Close the directory
	dirclose(dir);

	// Redraw the list
	_listbox->draw();
}

void FileRequester::setPath(const char* path) {
	// Delete existing path memory
	if (_path != NULL) {
		delete [] _path;
	}

	// Set new path
	_path = new char[strlen(path) + 1];

	strcpy(_path, path);

	// Fetch new directory data
	readDirectory();
}

void FileRequester::appendPath(const char* path) {

	// Ensure path exists
	if (_path == NULL) {
		setPath(path);
		return;
	}

	// Abort if path is current directory
	if (strcmp(path, ".") == 0) return;

	// Handle parent directory
	if (strcmp(path, "..") == 0) {

		// Abort if we're at the root directory already
		if (strlen(_path) == 1) return;

		// Locate start of the previous directory in the path string
		// by moving backwards along the string hunting slashes
		char* lastSlash = _path + strlen(_path) - 2;
		while (*lastSlash != '/') {
			lastSlash--;
		}

		// Append terminator after previous slash to truncate string
		lastSlash[1] = '\0';

		// Fetch new directory data
		readDirectory();

		return;
	}

	// Append valid path to existing path

	// Create new path memory
	char* newPath = new char[strlen(path) + strlen(_path) + 2];

	// Concat strings
	strcpy(newPath, _path);
	strcat(newPath, path);
	strcat(newPath, "/");

	// Delete old path
	delete [] _path;

	// Update pointer
	_path = newPath;

	// Fetch new directory data
	readDirectory();
}
