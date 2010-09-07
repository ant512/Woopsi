// Includes
#include "scrollinglistboxtest.h"
#include "amigascreen.h"
#include "amigawindow.h"
#include "debug.h"
#include "scrollbarvertical.h"
#include "slidervertical.h"
#include "sliderverticalgrip.h"

void ScrollingListBoxTest::startup() {

	// Create screen
	AmigaScreen* screen = new AmigaScreen("Test Screen", Gadget::GADGET_DRAGGABLE, AmigaScreen::AMIGA_SCREEN_SHOW_DEPTH | AmigaScreen::AMIGA_SCREEN_SHOW_FLIP);
	woopsiApplication->addGadget(screen);

	// Add window
	AmigaWindow* window = new AmigaWindow(0, 13, 256, 179, "Test Window", Gadget::GADGET_DRAGGABLE, AmigaWindow::AMIGA_WINDOW_SHOW_CLOSE | AmigaWindow::AMIGA_WINDOW_SHOW_DEPTH);
	screen->addGadget(window);
	
	// Add test buttons
	_shelve = new Button(10, 70, 100, 20, "Shelve");
	_unshelve = new Button(10, 90, 100, 20, "Unshelve");
	_show = new Button(10, 110, 100, 20, "Show");
	_hide = new Button(10, 130, 100, 20, "Hide");
	_dimensions = new Button(10, 150, 100, 20, "Dimensions");
	_move = new Button(110, 70, 100, 20, "Move");
	_resize = new Button(110, 90, 100, 20, "Resize");
	_autosize = new Button(110, 110, 100, 20, "Auto size");
	_enable = new Button(110, 130, 100, 20, "Enable");
	_disable = new Button(110, 150, 100, 20, "Disable");
	
	_shelve->setRefcon(2);
	_unshelve->setRefcon(3);
	_show->setRefcon(4);
	_hide->setRefcon(5);
	_move->setRefcon(6);
	_resize->setRefcon(7);
	_autosize->setRefcon(8);
	_enable->setRefcon(9);
	_disable->setRefcon(10);
	_dimensions->setRefcon(11);
	
	window->addGadget(_shelve);
	window->addGadget(_unshelve);
	window->addGadget(_show);
	window->addGadget(_hide);
	window->addGadget(_move);
	window->addGadget(_resize);
	window->addGadget(_autosize);
	window->addGadget(_enable);
	window->addGadget(_disable);
	window->addGadget(_dimensions);
	
	_shelve->addGadgetEventHandler(this);
	_unshelve->addGadgetEventHandler(this);
	_show->addGadgetEventHandler(this);
	_hide->addGadgetEventHandler(this);
	_move->addGadgetEventHandler(this);
	_resize->addGadgetEventHandler(this);
	_autosize->addGadgetEventHandler(this);
	_enable->addGadgetEventHandler(this);
	_disable->addGadgetEventHandler(this);
	_dimensions->addGadgetEventHandler(this);

	// Add cycle button
	_listBox = new ScrollingListBox(30, 30, 100, 100);
	
	for (u8 i = 0; i < 5; ++i) {
	_listBox->addOption("Option 1", 1);
	_listBox->addOption("Option 2", 2);
	_listBox->addOption("Option 3", 3);
	_listBox->addOption("Option 4", 4);
	_listBox->addOption("Option 5", 5);
	_listBox->addOption("Option 6", 6);
	_listBox->addOption("Option 7", 7);
	_listBox->addOption("Option 8", 8);
	_listBox->addOption("Option 9", 9);
	_listBox->addOption("Option 10", 10);
	_listBox->addOption("Option 11", 11);
	_listBox->addOption("Option 12", 12);
	_listBox->addOption("Option 13", 13);
	_listBox->addOption("Option 14", 14);
	_listBox->addOption("Option 15", 15);
	_listBox->addOption("Option 16", 16);
	_listBox->addOption("Option 17", 17);
	_listBox->addOption("Option 18", 18);
	_listBox->addOption("Option 19", 19);
	_listBox->addOption("Option 20", 20);
	_listBox->addOption("Option 21", 21);
	_listBox->addOption("Option 22", 22);
	_listBox->addOption("Option 23", 23);
	_listBox->addOption("Option 24", 24);
	_listBox->addOption("Option 25", 25);
	_listBox->addOption("Option 26", 26);
	_listBox->addOption("Option 27", 27);
	_listBox->addOption("Option 28", 28);
	_listBox->addOption("Option 29", 29);
	_listBox->addOption("Option 30", 30);
	}
	window->addGadget(_listBox);
	_listBox->addGadgetEventHandler(this);
	_listBox->setDoubleClickable(true);
	_listBox->setRefcon(1);

	// Get preferred dimensions for cycle button and resize
	Debug::printf("getPreferredDimensions()");
	//Rect rect;
	//_listBox->getPreferredDimensions(rect);
	//_listBox->resize(rect.width, rect.height);
	
	ScrollbarVertical* scrollbar = ((ScrollbarVertical*)(_listBox->getChild(1)));
	
	Debug::printf("Scrollbar");
	Debug::printf("Height:        %d", scrollbar->getHeight());
	
	Debug::printf("Max val:       %d", scrollbar->getMaximumValue());
	Debug::printf("Min val:       %d", scrollbar->getMinimumValue());
	Debug::printf("Value:         %d", scrollbar->getValue());
	
	Debug::printf("");
	Debug::printf("Slider");
	SliderVertical* slider = (SliderVertical*)scrollbar->getChild(0);
	
	Rect rect;
	slider->getClientRect(rect);
	
	Debug::printf("Client height: %d", rect.height);
	
	
	Debug::printf("");
	Debug::printf("Grip");
	SliderVerticalGrip* grip = (SliderVerticalGrip*)slider->getChild(0);

	Debug::printf("Height:        %d", grip->getHeight());

}

void ScrollingListBoxTest::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}

void ScrollingListBoxTest::handleClickEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Clicked");
			break;
	}
}

void ScrollingListBoxTest::handleDragEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Dragged");
			break;
	}
}

void ScrollingListBoxTest::handleReleaseEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Released");
			break;
	}
}

void ScrollingListBoxTest::handleReleaseOutsideEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Released outside");
			break;
	}
}

void ScrollingListBoxTest::handleKeyPressEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Key pressed");
			break;
	}
}

void ScrollingListBoxTest::handleKeyReleaseEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Key released");
			break;
	}
}

void ScrollingListBoxTest::handleLidOpenEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Lid opened");
			break;
	}
}

void ScrollingListBoxTest::handleLidCloseEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Lid closed");
			break;
	}
}

void ScrollingListBoxTest::handleFocusEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Focused");
			break;
	}
}

void ScrollingListBoxTest::handleBlurEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Blurred");
			break;
	}
}

void ScrollingListBoxTest::handleCloseEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Closed");
			break;
	}
}

void ScrollingListBoxTest::handleHideEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Hidden");
			break;
	}
}

void ScrollingListBoxTest::handleShowEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Shown");
			break;
	}
}

void ScrollingListBoxTest::handleEnableEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Enabled");
			break;
	}
}

void ScrollingListBoxTest::handleDisableEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Disabled");
			break;
	}
}

void ScrollingListBoxTest::handleValueChangeEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Value changed");
			break;
	}
}

void ScrollingListBoxTest::handleResizeEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Resized");
			break;
	}
}

void ScrollingListBoxTest::handleMoveEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Moved");
			break;
	}
}

void ScrollingListBoxTest::handleScrollEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Scrolled");
			break;
	}
}

void ScrollingListBoxTest::handleShiftClickEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Shift clicked");
			break;
	}
}

void ScrollingListBoxTest::handleContextMenuSelectionEvent(const ContextMenuEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Context menu selected");
			break;
	}
}

void ScrollingListBoxTest::handleDoubleClickEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Double clicked");
			break;
	}
}

void ScrollingListBoxTest::handleShelveEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Shelved");
			break;
	}
}

void ScrollingListBoxTest::handleUnshelveEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Unshelved");
			break;
	}
}

void ScrollingListBoxTest::handleActionEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Action");
			break;
		case 2:
			_listBox->shelve();
			break;
		case 3:
			_listBox->unshelve();
			break;
		case 4:
			_listBox->show();
			break;
		case 5:
			_listBox->hide();
			break;
		case 6:
			{
				u16 newPos;
				if (_listBox->getX() == 10) {
					newPos = 30;
				} else {
					newPos = 10;
				}
				_listBox->moveTo(newPos, newPos);
				break;
			}
		case 7:
			_listBox->resize(50, 30);
			break;
		case 8:
			{
				Rect rect;
				_listBox->getPreferredDimensions(rect);
				_listBox->resize(rect.width, rect.height);
				break;
			}
		case 9:
			_listBox->enable();
			break;
		case 10:
			_listBox->disable();
			break;
		case 11:
			{
				u16 newPos;
				if (_listBox->getX() == 10) {
					newPos = 30;
				} else {
					newPos = 10;
				}
				_listBox->changeDimensions(newPos, newPos, newPos, newPos);
				break;
			}
	}
}

void ScrollingListBoxTest::handleMoveForwardEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Moved forwards");
			break;
	}
}

void ScrollingListBoxTest::handleMoveBackwardEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Moved backwards");
			break;
	}
}
