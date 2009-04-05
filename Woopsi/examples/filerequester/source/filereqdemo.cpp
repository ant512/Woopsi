// Includes
#include "filereqdemo.h"
#include "woopsiheaders.h"
#include "filerequester.h"

using namespace WoopsiUI;

void FileReqDemo::startup() {

	// Call base startup method
	Woopsi::startup();
	
	// Create screen
	AmigaScreen* screen = new AmigaScreen("Output Screen", Gadget::GADGET_DRAGGABLE, AmigaScreen::AMIGA_SCREEN_SHOW_DEPTH | AmigaScreen::AMIGA_SCREEN_SHOW_FLIP);
	woopsiApplication->addGadget(screen);

	// Add window
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "Output Window", Gadget::GADGET_DRAGGABLE, AmigaWindow::AMIGA_WINDOW_SHOW_CLOSE | AmigaWindow::AMIGA_WINDOW_SHOW_DEPTH);
	screen->addGadget(window);

	// Get available area within window
	Gadget::Rect rect;
	window->getClientRect(rect);
	
	// Add textbox - this will tell the user that we're waiting for the FAT system to start up
	_textbox = new TextBox(rect.x, rect.y, rect.width, rect.height, "Initialising FAT...");
	_textbox->setTextAlignmentVert(TextBox::TEXT_ALIGNMENT_VERT_TOP);
	window->addGadget(_textbox);

	// Ensure Woopsi can draw itself
	enableDrawing();
	
	// Draw GUI
	redraw();
	
	// Attempt to initialise the FAT system
	if (fatInitDefault()) {
	
		// FAT initialisation worked!  Update the textbox to tell the user that all is OK
		_textbox->setText("FAT initialised");
		
		// Create file requester at the root of the file system
		FileRequester* req = new FileRequester(10, 10, 150, 150, "Files", "/", GADGET_DRAGGABLE | GADGET_DOUBLE_CLICKABLE);
		req->setRefcon(1);
		req->addGadgetEventHandler(this);
		screen->addGadget(req);
		req->redraw();
	} else {
	
		// FAT initialisation failed!  Update textbox to tell user that we're stuck
		_textbox->setText("Failed to initialise FAT");
	}
}

void FileReqDemo::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}

void FileReqDemo::handleValueChangeEvent(const GadgetEventArgs& e) {

	// Did a gadget fire this event?
	if (e.getSource() != NULL) {
	
		// Is the gadget the file requester?
		if (e.getSource()->getRefcon() == 1) {
		
			// Update the textbox with the name of the requested file
			_textbox->setText(((FileRequester*)e.getSource())->getSelectedOption()->text);
		}
	}
}
