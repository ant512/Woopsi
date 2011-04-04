#include <nds.h>
#include "filelistbox.h"
#include "button.h"
#include "filepath.h"
#include "graphicsport.h"

#ifndef USING_SDL

#include <fcntl.h>
#include <dirent.h>

#endif

using namespace WoopsiUI;

FileListBox::FileListBox(s16 x, s16 y, u16 width, u16 height, GadgetStyle* style) : Gadget(x, y, width, height, style) {

	_path = NULL;

	setBorderless(true);

	// Create list box
	_listbox = new ScrollingListBox(0, 0, getWidth(), getHeight(), &_style);
	_listbox->addGadgetEventHandler(this);
	_listbox->setAllowMultipleSelections(false);
	_listbox->setSortInsertedItems(true);
	addGadget(_listbox);
}

FileListBox::~FileListBox() {
	if (_path) delete _path;
}

void FileListBox::onResize(u16 width, u16 height) {
	_listbox->resize(width, height);
}

void FileListBox::drawContents(GraphicsPort* port) {
	port->drawFilledRect(0, 0, getWidth(), getHeight(), getBackColour());
}

void FileListBox::handleValueChangeEvent(const GadgetEventArgs& e) {
	if (e.getSource() != NULL) {
		if (e.getSource() == _listbox) {
			
			// File selected; raise event
			_gadgetEventHandlers->raiseValueChangeEvent();
		}
	}
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

					// File selected; raise action event
					_gadgetEventHandlers->raiseActionEvent();
				}
			}
			
			// Raise event
			_gadgetEventHandlers->raiseValueChangeEvent();
			
			return;
		}
	}
}

void FileListBox::readDirectory() {

	// Clear current options
	_listbox->removeAllOptions();

#ifdef USING_SDL

	// Build file list using SDL
	// We build a dummy list.  Most OSes use different
	// methods for accessing the file system.  We could
	// target POSIX, but then we cut out Windows; or we
	// could target Windows and POSIX, but then we cut
	// out any number of other OSes.  Simplest solution
	// is just to present a list of imaginary files.

	// Which path are we building?
	if (_path->getPath().compareTo("/") == 0) {

		// Root path

		// Directories
		_listbox->addOption(new FileListBoxDataItem("Directory1", 0, getShineColour(), getBackColour(), getShineColour(), getHighlightColour(), true));
		_listbox->addOption(new FileListBoxDataItem("Directory2", 0, getShineColour(), getBackColour(), getShineColour(), getHighlightColour(), true));
		_listbox->addOption(new FileListBoxDataItem("Directory3", 0, getShineColour(), getBackColour(), getShineColour(), getHighlightColour(), true));
		_listbox->addOption(new FileListBoxDataItem("Directory4", 0, getShineColour(), getBackColour(), getShineColour(), getHighlightColour(), true));

		// Files
		_listbox->addOption(new FileListBoxDataItem("File1", 0, getShadowColour(), getBackColour(), getShadowColour(), getHighlightColour(), false));
		_listbox->addOption(new FileListBoxDataItem("File2", 0, getShadowColour(), getBackColour(), getShadowColour(), getHighlightColour(), false));
		_listbox->addOption(new FileListBoxDataItem("File3", 0, getShadowColour(), getBackColour(), getShadowColour(), getHighlightColour(), false));
		_listbox->addOption(new FileListBoxDataItem("File4", 0, getShadowColour(), getBackColour(), getShadowColour(), getHighlightColour(), false));
	} else if (_path->getPath().compareTo("/Directory1/") == 0) {

		// Directory 1

		// Directories
		_listbox->addOption(new FileListBoxDataItem("..", 0, getShineColour(), getBackColour(), getShineColour(), getHighlightColour(), true));
		_listbox->addOption(new FileListBoxDataItem("Subdir1", 0, getShineColour(), getBackColour(), getShineColour(), getHighlightColour(), true));
		_listbox->addOption(new FileListBoxDataItem("Subdir2", 0, getShineColour(), getBackColour(), getShineColour(), getHighlightColour(), true));

		// Files
		_listbox->addOption(new FileListBoxDataItem("Subfile1", 0, getShadowColour(), getBackColour(), getShadowColour(), getHighlightColour(), false));
	}

#else

	// Build file list using libfat
	struct stat st;

	// Get a copy of the path char array so that it can be used with libfat
	char* path = new char[_path->getPath().getByteCount() + 1];
	_path->getPath().copyToCharArray(path);

	DIR* dir = opendir(path);

	delete [] path;

	// Did we get the dir successfully?
	if (dir == NULL) return;
	
	// Read data into options list
	struct dirent* ent;

	while ((ent = readdir(dir)) != 0) {
		
		// Bypass "." directory
		if (strcmp(ent->d_name, ".") == 0) continue;

		char* newPath = new char[strlen(ent->d_name) + _path->getPath().getByteCount() + 2];
		_path->getPath().copyToCharArray(newPath);
		strcat(newPath, "/");
		strcat(newPath, ent->d_name);
		int result = stat(newPath, &st);
		delete [] newPath;
		if (result) {
			continue;
		}

		// st.st_mode & S_IFDIR indicates a directory
		if (st.st_mode & S_IFDIR) {

			// Directory
			_listbox->addOption(new FileListBoxDataItem(ent->d_name, 0, getShineColour(), getBackColour(), getShineColour(), getHighlightColour(), true));
		} else {

			// File
			_listbox->addOption(new FileListBoxDataItem(ent->d_name, 0, getShadowColour(), getBackColour(), getShadowColour(), getHighlightColour(), false));
		}
	}

	// Close the directory
	closedir(dir);

#endif
	
	markRectsDamaged();
}

void FileListBox::setPath(const WoopsiString& path) {

	if (_path == NULL) {
		_path = new FilePath(path);
	} else {
		_path->setPath(path);
	}

	// Fetch new directory data
	readDirectory();
}

void FileListBox::appendPath(const WoopsiString& path) {
	_path->appendPath(path);

	// Fetch new directory data
	readDirectory();
}

const FilePath* FileListBox::getPath() const {
	return _path;
}