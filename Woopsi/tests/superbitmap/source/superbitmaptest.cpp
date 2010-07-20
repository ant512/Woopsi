// Includes
#include "superbitmaptest.h"
#include "amigascreen.h"
#include "amigawindow.h"
#include "debug.h"
#include "woopsifuncs.h"
#include "graphics.h"

void SuperBitmapTest::startup() {

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

	// Add superbitmap
	_superBitmap = new SuperBitmap(30, 30, 160, 105, 500, 500, false);
	window->addGadget(_superBitmap);
	_superBitmap->addGadgetEventHandler(this);
	_superBitmap->setDoubleClickable(true);
	_superBitmap->setRefcon(1);
	
	// Draw a picture on the superbitmap
	Graphics* gfx = _superBitmap->getGraphics();
	gfx->drawLine(0, 0, 500, 500, woopsiRGB(31, 0, 0));
	gfx->drawLine(500, 0, 0, 500, woopsiRGB(31, 0, 0));
	
	gfx->drawLine(150, 0, 350, 500, woopsiRGB(0, 0, 31));
	gfx->drawLine(350, 0, 150, 500, woopsiRGB(0, 0, 31));
	
	gfx->drawLine(0, 150, 500, 350, woopsiRGB(0, 31, 0));
	gfx->drawLine(0, 350, 500, 150, woopsiRGB(0, 31, 0));
	
	gfx->drawRect(0, 0, 500, 500, woopsiRGB(0, 31, 31));
	gfx->drawRect(50, 50, 400, 400, woopsiRGB(31, 0, 31));
	gfx->drawRect(100, 100, 300, 300, woopsiRGB(0, 0, 0));
	
	// Get preferred dimensions for label and resize
	Debug::printf("getPreferredDimensions()");
	Rect rect;
	_superBitmap->getPreferredDimensions(rect);
	_superBitmap->resize(rect.width, rect.height);
	
	// Ensure Woopsi can draw itself
	enableDrawing();
	
	// Draw GUI
	redraw();
}

void SuperBitmapTest::shutdown() {

	// Call base shutdown method
	Woopsi::shutdown();
}

void SuperBitmapTest::handleClickEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Clicked");
			break;
	}
}

void SuperBitmapTest::handleDragEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Dragged");
			break;
	}
}

void SuperBitmapTest::handleReleaseEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Released");
			break;
	}
}

void SuperBitmapTest::handleReleaseOutsideEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Released outside");
			break;
	}
}

void SuperBitmapTest::handleKeyPressEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Key pressed");
			break;
	}
}

void SuperBitmapTest::handleKeyReleaseEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Key released");
			break;
	}
}

void SuperBitmapTest::handleLidOpenEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Lid opened");
			break;
	}
}

void SuperBitmapTest::handleLidCloseEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Lid closed");
			break;
	}
}

void SuperBitmapTest::handleFocusEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Focused");
			break;
	}
}

void SuperBitmapTest::handleBlurEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Blurred");
			break;
	}
}

void SuperBitmapTest::handleCloseEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Closed");
			break;
	}
}

void SuperBitmapTest::handleHideEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Hidden");
			break;
	}
}

void SuperBitmapTest::handleShowEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Shown");
			break;
	}
}

void SuperBitmapTest::handleEnableEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Enabled");
			break;
	}
}

void SuperBitmapTest::handleDisableEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Disabled");
			break;
	}
}

void SuperBitmapTest::handleValueChangeEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Value changed");
			break;
	}
}

void SuperBitmapTest::handleResizeEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Resized");
			break;
	}
}

void SuperBitmapTest::handleMoveEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Moved");
			break;
	}
}

void SuperBitmapTest::handleScrollEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Scrolled");
			break;
	}
}

void SuperBitmapTest::handleShiftClickEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Shift clicked");
			break;
	}
}

void SuperBitmapTest::handleContextMenuSelectionEvent(const ContextMenuEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Context menu selected");
			break;
	}
}

void SuperBitmapTest::handleDoubleClickEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Double clicked");
			break;
	}
}

void SuperBitmapTest::handleShelveEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Shelved");
			break;
	}
}

void SuperBitmapTest::handleUnshelveEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Unshelved");
			break;
	}
}

void SuperBitmapTest::handleActionEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Action");
			break;
		case 2:
			_superBitmap->shelve();
			break;
		case 3:
			_superBitmap->unshelve();
			break;
		case 4:
			_superBitmap->show();
			break;
		case 5:
			_superBitmap->hide();
			break;
		case 6:
			{
				u16 newPos;
				if (_superBitmap->getX() == 10) {
					newPos = 30;
				} else {
					newPos = 10;
				}
				_superBitmap->moveTo(newPos, newPos);
				break;
			}
		case 7:
			_superBitmap->resize(10, 10);
			break;
		case 8:
			{
				Rect rect;
				_superBitmap->getPreferredDimensions(rect);
				_superBitmap->resize(rect.width, rect.height);
				break;
			}
		case 9:
			_superBitmap->enable();
			break;
		case 10:
			_superBitmap->disable();
			break;
		case 11:
			{
				u16 newPos;
				if (_superBitmap->getX() == 10) {
					newPos = 30;
				} else {
					newPos = 10;
				}
				_superBitmap->changeDimensions(newPos, newPos, newPos, newPos);
				break;
			}
	}
}

void SuperBitmapTest::handleMoveForwardEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Moved forwards");
			break;
	}
}

void SuperBitmapTest::handleMoveBackwardEvent(const GadgetEventArgs& e) {
	switch (e.getSource()->getRefcon()) {
		case 1:
			Debug::printf("Moved backwards");
			break;
	}
}
