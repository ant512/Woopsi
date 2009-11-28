#include <nds.h>
#include <fcntl.h>
#include <dirent.h>
#include "filerequester.h"
#include "button.h"
#include "filepath.h"
#include "filerequesterdataitem.h"

using namespace WoopsiUI;

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
	_listbox->addGadgetEventHandler(this);
	_listbox->setOutlineType(OUTLINE_OUT);
	_listbox->setAllowMultipleSelections(false);
	_listbox->setSortInsertedItems(true);
	addGadget(_listbox);

	// Calculate OK button dimensions
	Rect buttonRect;
	buttonRect.width = (rect.width - (padding * 3)) >> 1;
	buttonRect.height = _font->getHeight() + (padding << 1);
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

	setPath(path);
}

FileRequester::~FileRequester() {
	delete _path;
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

void FileRequester::handleDoubleClickEvent(const GadgetEventArgs& e) {
	if (e.getSource() != NULL) {
		if (e.getSource() == _listbox) {

			// Work out which option was clicked - if it was a directory, we move to the new path
			const ListDataItem* selected = getSelectedOption();

			if (selected != NULL) {

				// Detect type by examining text colour
				if (selected->getNormalTextColour() == _colours.shine) {

					// Got a directory
					appendPath(selected->getText());
				} else {

					// File selected; raise event
					raiseValueChangeEvent();

					// Close the window
					close();
				}
			}
			
			return;
		}
	}

	// Handle other window events
	AmigaWindow::handleDoubleClickEvent(e);
}

void FileRequester::readDirectory() {

	// Clear current options
	_listbox->removeAllOptions();
	
	// Add "Loading..." option to display whilst directory is enumerated
	_listbox->addOption("Loading...", 0);

	// Disable drawing for speed
	disableDrawing();
	
	// Remove the "Loading..." option
	_listbox->removeAllOptions();
	
	struct stat st;

	DIR* dir = opendir(_path->getPath());

	// Did we get the dir successfully?
	if (dir == NULL) return;
	
	// Read data into options list
	struct dirent* ent;

	while ((ent = readdir(dir)) != 0) {

		char* newPath = new char[strlen(ent->d_name) + strlen(_path->getPath()) + 2];
		strcpy(newPath, _path->getPath());
		strcat(newPath, "/");
		strcat(newPath, ent->d_name);
		int result = stat(newPath, &st);
		delete [] newPath;
		if (result) {
			continue;
		}

		// Create memory to store the filename in the array
		char* storedFilename = new char[strlen(ent->d_name) + 1];
		strcpy(storedFilename, ent->d_name);

		// st.st_mode & S_IFDIR indicates a directory
		if (st.st_mode & S_IFDIR) {

			// Directory
			_listbox->addOption(new FileRequesterDataItem(storedFilename, 0, _colours.shine, _colours.back, _colours.shine, _colours.highlight, true));
		} else {

			// File
			_listbox->addOption(new FileRequesterDataItem(storedFilename, 0, _colours.shadow, _colours.back, _colours.shadow, _colours.highlight, false));
		}
	}
	
	// Re-enable drawing now that the list is complete
	enableDrawing();
	redraw();

	// Close the directory
	closedir(dir);
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
