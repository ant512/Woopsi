#include "woopsi.h"
#include "screen.h"
#include "woopsifuncs.h"
#include "fontbase.h"
#include "font.h"
#include "sysfont.h"
#include "contextmenu.h"

// Instantiate singleton
Woopsi* Woopsi::singleton = NULL;

// Initialise static system font
FontBase* Woopsi::_systemFont = NULL;

// Initialise static VBL listener vector
DynamicArray<Gadget*> Woopsi::_vblListeners;

// Initialise static deletion queue
DynamicArray<Gadget*> Woopsi::_deleteQueue;

// Initialise VBL counter
u32 Woopsi::_vblCount = 0;

Woopsi::Woopsi(FontBase* font) : Gadget(0, 0, SCREEN_WIDTH, TOP_SCREEN_Y_OFFSET + SCREEN_HEIGHT, GADGET_BORDERLESS, font) {
	_lidClosed = woopsiLidClosed();

	if (font == NULL) {
		_font = getSystemFont();
	}

	_contextMenu = new ContextMenu(_font);
	addGadget(_contextMenu);
	_contextMenu->shelve();

	singleton = this;

	_running = false;

	// Set up DS display hardware
	initWoopsiGfxMode();
}

Woopsi::~Woopsi() {
	abortRunLoop();

	deleteSystemFont();
	_font = NULL;
	singleton = NULL;
	_contextMenu = NULL;
}

void Woopsi::runLoop() {

	// Remember that Woopsi is running
	_running = true;

	while (_running) {
		processOneVBL();
	}
}

void Woopsi::processOneVBL(Gadget* gadget) {
	handleVBL();
	handleStylus(gadget);
	handleKeys();
	handleLid();
	woopsiWaitVBL();
}

void Woopsi::handleVBL() {

	// Increase vbl counter
	_vblCount++;

	// VBL
	for (u8 i = 0; i < _vblListeners.size(); i++) {
		_vblListeners[i]->vbl();
	}

	// Delete any queued gadgets
	processDeleteQueue();
}

void Woopsi::draw(Gadget::Rect clipRect) {
	clear(clipRect);
}

// Process all stylus input
void Woopsi::handleStylus(Gadget* gadget) {

	// All gadgets
	if (Stylus.Newpress) {
		if (Pad.Held.L || Pad.Held.R) {

			// Working with a modal gadget or the whole structure?
			if (gadget == NULL) {

				// All gadgets
				shiftClick(Stylus.X, Stylus.Y);
			} else {

				// One gadget
				shiftClick(Stylus.X, Stylus.Y, gadget);
			}
		} else {

			// Working with a modal gadget or the whole structure?
			if (gadget == NULL) {

				// All gadgets
				click(Stylus.X, Stylus.Y);
			} else {

				// One gadget
				click(Stylus.X, Stylus.Y, gadget);
			}
		}
	} else if (Stylus.Held) {
		drag(Stylus.X, Stylus.Y, Stylus.Vx, Stylus.Vy);
	} else if (_flags.clicked) {
		release(Stylus.X, Stylus.Y);
	}
}

bool Woopsi::click(s16 x, s16 y) {

	_flags.clicked = true;

	// Work out which gadget was clicked
	for (s16 i = _gadgets.size() - 1; i > -1; i--) {
		if (_gadgets[i]->click(x, y)) {

			// Do we need to close the context menu?
			if (_gadgets[i] != _contextMenu) {
				shelveContextMenu();
			}

			return true;
		}
	}

	return false;
}

bool Woopsi::click(s16 x, s16 y, Gadget* gadget) {

	_flags.clicked = true;

	// Work out which gadget was clicked
	if (gadget->click(x, y)) {

		// Do we need to close the context menu?
		if (gadget != _contextMenu) {
			shelveContextMenu();
		}

		return true;
	}

	return false;
}

bool Woopsi::shiftClick(s16 x, s16 y) {

	_flags.clicked = true;

	// Shelve the existing context menu
	shelveContextMenu();

	// Work out which gadget was clicked
	for (s16 i = _gadgets.size() - 1; i > -1; i--) {
		if (_gadgets[i]->shiftClick(x, y)) {
			return true;
		}
	}

	return false;
}

bool Woopsi::shiftClick(s16 x, s16 y, Gadget* gadget) {

	_flags.clicked = true;

	// Shelve the existing context menu
	shelveContextMenu();

	// Work out which gadget was clicked
	if (gadget->shiftClick(x, y)) {
		return true;
	}

	return false;
}

bool Woopsi::drag(s16 x, s16 y, s16 vX, s16 vY) {
	if (_clickedGadget != NULL) {
		_clickedGadget->drag(x, y, vX, vY);

		return true;
	}

	return false;
}

bool Woopsi::release(s16 x, s16 y) {

	if (_flags.clicked) {

		// Avoid using the unreliable "Stylus.Released" variable
		_flags.clicked = false;

		// Release clicked gadget
		if (_clickedGadget != NULL) {
			_clickedGadget->release(x, y);
		}

		return true;
	}

	return false;
}

// Process all key input
void Woopsi::handleKeys() {
	if (_focusedGadget != NULL) {
		if (Pad.Newpress.A) {
			_focusedGadget->keyPress(KEY_CODE_A);
		} else if (Pad.Released.A) {
			_focusedGadget->keyRelease(KEY_CODE_A);
		}

		if (Pad.Newpress.B) {
			_focusedGadget->keyPress(KEY_CODE_B);
		} else if (Pad.Released.B) {
			_focusedGadget->keyRelease(KEY_CODE_B);
		}

		if (Pad.Newpress.X) {
			_focusedGadget->keyPress(KEY_CODE_X);
		} else if (Pad.Released.X) {
			_focusedGadget->keyRelease(KEY_CODE_X);
		}

		if (Pad.Newpress.Y) {
			_focusedGadget->keyPress(KEY_CODE_Y);
		} else if (Pad.Released.Y) {
			_focusedGadget->keyRelease(KEY_CODE_Y);
		}

		if (Pad.Newpress.L) {
			_focusedGadget->keyPress(KEY_CODE_L);
		} else if (Pad.Released.L) {
			_focusedGadget->keyRelease(KEY_CODE_L);
		}

		if (Pad.Newpress.R) {
			_focusedGadget->keyPress(KEY_CODE_R);
		} else if (Pad.Released.R) {
			_focusedGadget->keyRelease(KEY_CODE_R);
		}

		if (Pad.Newpress.Up) {
			_focusedGadget->keyPress(KEY_CODE_UP);
		} else if (Pad.Released.Up) {
			_focusedGadget->keyRelease(KEY_CODE_UP);
		}

		if (Pad.Newpress.Down) {
			_focusedGadget->keyPress(KEY_CODE_DOWN);
		} else if (Pad.Released.Down) {
			_focusedGadget->keyRelease(KEY_CODE_DOWN);
		}

		if (Pad.Newpress.Left) {
			_focusedGadget->keyPress(KEY_CODE_LEFT);
		} else if (Pad.Released.Left) {
			_focusedGadget->keyRelease(KEY_CODE_LEFT);
		}

		if (Pad.Newpress.Right) {
			_focusedGadget->keyPress(KEY_CODE_RIGHT);
		} else if (Pad.Released.Right) {
			_focusedGadget->keyRelease(KEY_CODE_RIGHT);
		}

		if (Pad.Newpress.Start) {
			_focusedGadget->keyPress(KEY_CODE_START);
		} else if (Pad.Released.Start) {
			_focusedGadget->keyRelease(KEY_CODE_START);
		}

		if (Pad.Newpress.Select) {
			_focusedGadget->keyPress(KEY_CODE_SELECT);
		} else if (Pad.Released.Select) {
			_focusedGadget->keyRelease(KEY_CODE_SELECT);
		}
	}
}

void Woopsi::handleLid() {

	// Check for lid closed event
	if (woopsiLidClosed() && !_lidClosed) {

		// Lid has just been closed
		_lidClosed = true;

		// Run lid closed on all gadgets
		for (u8 i = 0; i < _gadgets.size(); i++) {
			_gadgets[i]->lidClosed();
		}

	} else if (!woopsiLidClosed() && _lidClosed) {

		// Lid has just been opened
		_lidClosed = false;

		// Run lid opened on all gadgets
		for (u8 i = 0; i < _gadgets.size(); i++) {
			_gadgets[i]->lidOpened();
		}
	}
}

bool Woopsi::swapGadgetDepth(Gadget* gadget) {
	// Do we have more than one screen?
	if (_gadgets.size() > 1) {

		u8 gadgetSource = getGadgetIndex(gadget);
		u8 gadgetDest = 0;

		// Raise or lower?
		if (gadgetSource < _gadgets.size() - 1) {
			// Raise
			gadgetDest = gadgetSource + 1;
		} else {
			// Lower
			gadgetDest = 0;
		}

		if (gadgetSource != gadgetDest) {
			eraseGadget(gadget);

			// Swap
			Gadget* tmp = _gadgets[gadgetSource];
			_gadgets[gadgetSource] = _gadgets[gadgetDest];
			_gadgets[gadgetDest] = tmp;

			// Invalidate from the top screen down
			_gadgets[_gadgets.size() - 1]->invalidateVisibleRectCache();
			invalidateLowerGadgetsVisibleRectCache(_gadgets[_gadgets.size() - 1]);

			gadget->draw();

			return true;
		}
	}

	return false;
}

bool Woopsi::flipScreens(Gadget* gadget) {

	// Only flip if there is more than one screen
	if (_gadgets.size() > 1) {
		// Locate the top gadget
		Gadget* topGadget = NULL;

		for (u8 i = 0; i < _gadgets.size(); i++) {
			if ((_gadgets[i]->isDrawingEnabled()) && (!_gadgets[i]->isDeleted())) {
				if (_gadgets[i]->getPhysicalScreenNumber() == 1) {
					topGadget = _gadgets[i];
					break;
				}
			}
		}

		// Did we find it?
		if (topGadget != NULL) {

			// Is the top gadget the current gadget?
			if (gadget == topGadget) {
				// Get a pointer to the highest gadget in the bottom screen
				// that isn't the top gadget
				for (s16 i = _gadgets.size() - 1; i > -1; i--) {
					if (gadget != _gadgets[i]) {
						gadget = _gadgets[i];
						break;
					}
				}
			}

			topGadget->disableDrawing();

			// Move to top of stack
			topGadget->raiseToTop();

			topGadget->enableDrawing();

			// Move to bottom screen
			((Screen*)topGadget)->flipToBottomScreen();
		}
		
		// Move the requested gadget to the top screen
		((Screen*)gadget)->flipToTopScreen();
		
		return true;
	}

	return false;
}

// Erase a rectangular area of the screen
void Woopsi::eraseRect(Rect rect) {

	// Create pointer to a vector to store the invalid rectangles
	DynamicArray<Rect>* invalidRectangles = new DynamicArray<Rect>();

	if (invalidRectangles != NULL) {
		 
		// Add rectangle into the vector
		invalidRectangles->push_back(rect);

		// Refresh children
		for (s16 i = _gadgets.size() - 1; i > -1 ; i--) {
			if (invalidRectangles->size() > 0) {
				_gadgets[i]->redrawDirty(invalidRectangles, NULL);
			} else {
				break;
			}
		}

		// Refresh screen
		for (u8 i = 0; i < invalidRectangles->size(); i++) {
			if (invalidRectangles->size() > 0) {
				clear(invalidRectangles->at(i));
			} else {
				break;
			}
		}

		// Tidy up
		delete invalidRectangles;
	}
}

// Return a pointer to the static system font
FontBase* Woopsi::getSystemFont() {

	// Create font instance if it does not exist yet
	if (_systemFont == NULL) {
		_systemFont = new Font(sysfont_Bitmap, 256, 50, 8, 10, 64543);
	}

	return _systemFont;
}

// Delete the static font
void Woopsi::deleteSystemFont() {
	if (_systemFont != NULL) {
		delete _systemFont;
		_systemFont = NULL;
	}
}

// Add a gadget to the list of gadgets that receive VBL events
void Woopsi::registerForVBL(Gadget* gadget) {

	// Ensure gadget is not already in the list
	for (u8 i = 0; i < _vblListeners.size(); i++) {
		if (_vblListeners[i] == gadget) {
			return;
		}
	}

	_vblListeners.push_back(gadget);
}

// Remove a gadget from the VBL list
void Woopsi::unregisterFromVBL(Gadget* gadget) {
	
	// Locate gadget in the list
	for (u8 i = 0; i < _vblListeners.size(); i++) {
		if (_vblListeners[i] == gadget) {
			_vblListeners.erase(_vblListeners.begin() + i);
			return;
		}
	}
}


// Delete all gadgets in the queue
void Woopsi::processDeleteQueue() {

	if (_deleteQueue.size() > 0) {

		for (u8 i = 0; i < _deleteQueue.size(); i++) {
			_deleteQueue[i]->destroy();
		}

		_deleteQueue.clear();
	}
}

void Woopsi::addToDeleteQueue(Gadget* gadget) {
	_deleteQueue.push_back(gadget);
}

// Close a child
void Woopsi::closeChild(Gadget* gadget) {
	if (gadget != NULL) {

		// Ensure gadget knows it is being closed
		if (!gadget->isDeleted()) {
			gadget->close();
		}

		// Do we need to make another gadget active?
		if (_focusedGadget == gadget) {
			_focusedGadget = NULL;
		}

		// Unset clicked gadget if necessary
		if (_clickedGadget == gadget) {
			_clickedGadget = NULL;
		}

		// Decrease decoration count if necessary
		if (gadget->isDecoration()) {
			_decorationCount--;
		}

		moveChildToDeleteQueue(gadget);
	}
}

// Shelve a child
void Woopsi::shelveChild(Gadget* gadget) {
	if (gadget != NULL) {

		// Ensure gadget knows it is being shelved
		if (!gadget->isShelved()) {
			gadget->shelve();
		}

		// Do we need to make another gadget active?
		if (_focusedGadget == gadget) {
			_focusedGadget = NULL;
		}

		// Unset clicked gadget if necessary
		if (_clickedGadget == gadget) {
			_clickedGadget = NULL;
		}

		// Decrease decoration count if necessary
		if (gadget->isDecoration()) {
			_decorationCount--;
		}

		moveChildToShelvedList(gadget);
	}
}

const u32 Woopsi::getContextMenuValue() const {
	return _contextMenu->getValue();
}

void Woopsi::shelveContextMenu() {
	if (!_contextMenu->isShelved()) {
		_contextMenu->shelve();
		_contextMenu->reset();
	}
}
