// Includes
#include "textboxtest.h"
#include "amigascreen.h"
#include "amigawindow.h"
#include "debug.h"

void TextBoxTest::startup() {

	// Call base startup method
	Woopsi::startup();

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
	_textbox = new TextBox(30, 30, 160, 105, "This is some text");
	window->addGadget(_textbox);
	_textbox->addGadgetEventHandler(this);
	_textbox->setDoubleClickable(true);
	_textbox->setRefcon(1);
	_textbox->showCursor();

	// Get preferred dimensions for cycle button and resize
	Debug::printf("getPreferredDimensions()");
	Gadget::Rect rect;
	_textbox->getPreferredDimensions(rect);
	_textbox->resize(rect.width, rect.height);
	
	// Ensure Woopsi can draw itself
	enableDrawing();
	
	// Draw GUI
	redraw();
}

void TextBoxTest::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}

void TextBoxTest::handleClickEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Clicked");
			break;
	}
}

void TextBoxTest::handleDragEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Dragged");
			break;
	}
}

void TextBoxTest::handleReleaseEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Released");
			break;
	}
}

void TextBoxTest::handleReleaseOutsideEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Released outside");
			break;
	}
}

void TextBoxTest::handleKeyPressEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Key pressed");
			break;
	}
}

void TextBoxTest::handleKeyReleaseEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Key released");
			break;
	}
}

void TextBoxTest::handleLidOpenEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Lid opened");
			break;
	}
}

void TextBoxTest::handleLidCloseEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Lid closed");
			break;
	}
}

void TextBoxTest::handleFocusEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Focused");
			break;
	}
}

void TextBoxTest::handleBlurEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Blurred");
			break;
	}
}

void TextBoxTest::handleCloseEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Closed");
			break;
	}
}

void TextBoxTest::handleHideEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Hidden");
			break;
	}
}

void TextBoxTest::handleShowEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Shown");
			break;
	}
}

void TextBoxTest::handleEnableEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Enabled");
			break;
	}
}

void TextBoxTest::handleDisableEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Disabled");
			break;
	}
}

void TextBoxTest::handleValueChangeEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Value changed");
			break;
	}
}

void TextBoxTest::handleResizeEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Resized");
			break;
	}
}

void TextBoxTest::handleMoveEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Moved");
			break;
	}
}

void TextBoxTest::handleScrollEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Scrolled");
			break;
	}
}

void TextBoxTest::handleShiftClickEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Shift clicked");
			break;
	}
}

void TextBoxTest::handleContextMenuSelectionEvent(const ContextMenuEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Context menu selected");
			break;
	}
}

void TextBoxTest::handleDoubleClickEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Double clicked");
			break;
	}
}

void TextBoxTest::handleShelveEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Shelved");
			break;
	}
}

void TextBoxTest::handleUnshelveEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Unshelved");
			break;
	}
}

void TextBoxTest::handleActionEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Action");
			break;
		case 2:
			_textbox->shelve();
			break;
		case 3:
			_textbox->unshelve();
			break;
		case 4:
			_textbox->show();
			break;
		case 5:
			_textbox->hide();
			break;
		case 6:
			{
				u16 newPos;
				if (_textbox->getX() == 10) {
					newPos = 30;
				} else {
					newPos = 10;
				}
				_textbox->moveTo(newPos, newPos);
				break;
			}
		case 7:
			_textbox->resize(10, 10);
			break;
		case 8:
			Rect rect;
			_textbox->getPreferredDimensions(rect);
			_textbox->resize(rect.width, rect.height);
			break;
		case 9:
			_textbox->enable();
			break;
		case 10:
			_textbox->disable();
			break;
		case 11:
			{
				u16 newPos;
				if (_textbox->getX() == 10) {
					newPos = 30;
				} else {
					newPos = 10;
				}
				_textbox->changeDimensions(newPos, newPos, newPos, newPos);
				break;
			}
	}
}

void TextBoxTest::handleMoveForwardEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Moved forwards");
			break;
	}
}

void TextBoxTest::handleMoveBackwardEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Moved backwards");
			break;
	}
}
