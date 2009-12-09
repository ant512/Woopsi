#include <nds.h>
#include <fcntl.h>
#include <dirent.h>
#include "filelistbox.h"
#include "button.h"
#include "filepath.h"

using namespace WoopsiUI;

FileListBox::FileListBox(s16 x, s16 y, u16 width, u16 height, const char* path, u32 flags, GadgetStyle* style) : Gadget(x, y, width, height, flags, style) {

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
	listboxRect.height = rect.height - (padding * 5) - getFont()->getHeight();
	listboxRect.x = rect.x + padding;
	listboxRect.y = rect.y + padding;

	// Create list box
	_listbox = new ScrollingListBox(listboxRect.x, listboxRect.y, listboxRect.width, listboxRect.height, _style);
	_listbox->addGadgetEventHandler(this);
	_listbox->setOutlineType(OUTLINE_OUT);
	_listbox->setAllowMultipleSelections(false);
	_listbox->setSortInsertedItems(true);
	addGadget(_listbox);

	setPath(path);
}

FileListBox::~FileListBox() {
	delete _path;
}

bool FileListBox::resize(u16 width, u16 height) {
	return false;
}

void FileListBox::handleDoubleClickEvent(const GadgetEventArgs& e) {
	if (e.getSource() != NULL) {
		if (e.getSource() == _listbox) {

			// Work out which option was clicked - if it was a directory, we move to the new path
			const FileListBoxDataItem* selected = getSelectedOption();

			if (selected != NULL) {

				// Detect type by examining text colour
				if (selected->getNormalTextColour() == getShineColour()) {

					// Got a directory
					appendPath(selected->getText());
				} else {

					// File selected; raise event
					raiseValueChangeEvent();
				}
			}
			
			return;
		}
	}
}

void FileListBox::readDirectory() {

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
			_listbox->addOption(new FileListBoxDataItem(storedFilename, 0, getShineColour(), getBackColour(), getShineColour(), getHighlightColour(), true));
		} else {

			// File
			_listbox->addOption(new FileListBoxDataItem(storedFilename, 0, getShadowColour(), getBackColour(), getShadowColour(), getHighlightColour(), false));
		}
	}
	
	// Re-enable drawing now that the list is complete
	enableDrawing();
	redraw();

	// Close the directory
	closedir(dir);
}

void FileListBox::setPath(const char* path) {
	_path->setPath(path);

	// Fetch new directory data
	readDirectory();
}

void FileListBox::appendPath(const char* path) {
	_path->appendPath(path);

	// Fetch new directory data
	readDirectory();
}