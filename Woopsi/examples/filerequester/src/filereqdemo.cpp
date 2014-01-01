// Includes
#include "filereqdemo.h"
#include "woopsiheaders.h"
#include "filerequester.h"

using namespace WoopsiUI;

void FileReqDemo::startup() {

	// Create screen
	AmigaScreen* screen = new AmigaScreen("Output Screen", true, true);
	woopsiApplication->addGadget(screen);

	// Add window
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "Output Window", true, true);
	screen->addGadget(window);

	// Get available area within window
	Rect rect;
	window->getClientRect(rect);
	
	// Add textbox - this will tell the user that we're waiting for the FAT system to start up
	_textbox = new TextBox(rect.x, rect.y, rect.width, rect.height, "Initialising FAT...");
	_textbox->setTextAlignmentVert(TextBox::TEXT_ALIGNMENT_VERT_TOP);
	window->addGadget(_textbox);

	// Attempt to initialise the FAT system
	if (fatInitDefault()) {
	
		// FAT initialisation worked!  Update the textbox to tell the user that all is OK
		_textbox->setText("FAT initialised");
		
		// Create file requester at the root of the file system
		FileRequester* req = new FileRequester(10, 10, 150, 150, "Files", "/");
		req->setRefcon(1);
		req->setGadgetEventHandler(this);
		screen->addGadget(req);
	} else {
	
		// FAT initialisation failed!  Update textbox to tell user that we're stuck
		_textbox->setText("Failed to initialise FAT");
	}
}

void FileReqDemo::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}

void FileReqDemo::handleValueChangeEvent(Gadget& source) {

	// Is the gadget the file requester?
	if (source.getRefcon() == 1) {
		
		// Update the textbox with the name of the requested file
		_textbox->setText(((FileRequester*)&source)->getSelectedOption()->getText());
	}
}
