#include "woopsi.h"
#include "screen.h"
#include "woopsifuncs.h"
#include "fontbase.h"
#include "contextmenu.h"
#include "gadgetstyle.h"
#include "woopsitimer.h"
#include "graphicsport.h"
#include "woopsikeyboardscreen.h"
#include "woopsikeyboard.h"
#include "keyboardeventhandler.h"
#include "screen.h"

using namespace WoopsiUI;

// Instantiate singleton
Woopsi* Woopsi::singleton = NULL;

// Initialise static VBL listener vector
WoopsiArray<WoopsiTimer*> Woopsi::_vblListeners;

// Initialise static deletion queue
WoopsiArray<Gadget*> Woopsi::_deleteQueue;

// Initialise VBL counter
u32 Woopsi::_vblCount = 0;

Woopsi::Woopsi(GadgetStyle* style) : Gadget(0, 0, SCREEN_WIDTH, TOP_SCREEN_Y_OFFSET + SCREEN_HEIGHT, GADGET_BORDERLESS, style) {
	_lidClosed = false;
	_flags.modal = true;
	_clickedGadget = NULL;
	_keyboardScreen = NULL;

	// Set up singleton pointer
	singleton = this;

	// Set up DS display hardware
	initWoopsiGfxMode();
	
	// Do we need to fetch the default style?
	// We need to do this again here because the gadget's constructor will be
	// run before the initWoopsiGfxMode() call, which means that we've got a
	// NULL font pointer
	if (style == NULL) {
		
		// Use default style
		if (defaultGadgetStyle != NULL) {
			_style.colours.back = defaultGadgetStyle->colours.back;
			_style.colours.shine = defaultGadgetStyle->colours.shine;
			_style.colours.highlight = defaultGadgetStyle->colours.highlight;
			_style.colours.shadow = defaultGadgetStyle->colours.shadow;
			_style.colours.fill = defaultGadgetStyle->colours.fill;
			_style.colours.dark = defaultGadgetStyle->colours.dark;
			_style.font = defaultGadgetStyle->font;
			_style.glyphFont = defaultGadgetStyle->glyphFont;
		}
	} else {
		
		// Use specified style
		_style.colours.back = style->colours.back;
		_style.colours.shine = style->colours.shine;
		_style.colours.highlight = style->colours.highlight;
		_style.colours.shadow = style->colours.shadow;
		_style.colours.fill = style->colours.fill;
		_style.colours.dark = style->colours.dark;
		_style.font = style->font;
		_style.glyphFont = style->glyphFont;
	}

	// Create context menu
	_contextMenu = new ContextMenu(style);
	addGadget(_contextMenu);
	_contextMenu->shelve();

	// Create background screens that will sit behind all other screens
	Screen* screen = new Screen("", GADGET_DECORATION);
	addGadget(screen);
	screen->flipToTopScreen();

	screen = new Screen("", GADGET_DECORATION);
	addGadget(screen);
}

Woopsi::~Woopsi() {
	stopModal();
	singleton = NULL;
	_contextMenu = NULL;

	woopsiFreeFrameBuffers();
	woopsiFreeDefaultGadgetStyle();
}

void Woopsi::goModal() {

	// Loop until no longer modal
	while (_flags.modal) {
		processOneVBL();
	}
}

void Woopsi::processOneVBL(Gadget* gadget) {
	handleVBL();
	handleStylus(gadget);
	handleKeys();
	handleLid();
	woopsiWaitVBL();

#ifdef USING_SDL

	// SDL event pump
	SDL_Event event;

	// Check for SDL quit
	while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                stopModal();
				return;
            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == 53) {
                    // Escape pressed
					stopModal();
					return;
                }
                break;
        }
	}

#endif
}

void Woopsi::handleVBL() {

	// Increase vbl counter
	_vblCount++;

	// Delete any queued gadgets
	processDeleteQueue();

	// VBL
	s32 i = 0;
	while (i < _vblListeners.size()) {
		_vblListeners[i]->run();
		i++;
	}
}

// Process all stylus input
void Woopsi::handleStylus(Gadget* gadget) {

	// All gadgets
	if (Stylus.Newpress) {
		if (Pad.Held.L || Pad.Held.R) {
			handleShiftClick(Stylus.X, Stylus.Y, gadget);
		} else {
			handleClick(Stylus.X, Stylus.Y, gadget);
		}
	} else if (Stylus.Held) {
		if (_clickedGadget != NULL) {
			_clickedGadget->drag(Stylus.X, Stylus.Y, Stylus.Vx, Stylus.Vy);
		}
	} else if (_clickedGadget != NULL) {
		_clickedGadget->release(Stylus.X, Stylus.Y);
	}
}

void Woopsi::handleShiftClick(s16 x, s16 y, Gadget* gadget) {

	// Shelve the existing context menu to ensure that if
	// the click does not result in the menu being redisplayed
	// it is correctly hidden
	shelveContextMenu();

	// Working with a modal gadget or the whole structure?
	if (gadget == NULL) {

		// All gadgets
		shiftClick(Stylus.X, Stylus.Y);
	} else {

		// One gadget
		gadget->shiftClick(Stylus.X, Stylus.Y);
	}
}

void Woopsi::handleClick(s16 x, s16 y, Gadget* gadget) {

	// Working with a modal gadget or the whole structure?
	if (gadget == NULL) {

		// All gadgets
		for (s32 i = _gadgets.size() - 1; i > -1; i--) {
			if (_gadgets[i]->click(x, y)) {

				// Do we need to close the context menu?
				if (_gadgets[i] != _contextMenu) {
					shelveContextMenu();
				}

				return;
			}
		}
	} else {

		// One gadget
		if (gadget->click(x, y)) {

			// Do we need to close the context menu?
			if (gadget != _contextMenu) {
				shelveContextMenu();
			}
		}
	}
}

void Woopsi::handleKey(bool newPress, bool released, u32& heldTime, KeyCode keyCode) {

	// We do not reset the repeat timers to 0 - instead we reset it back to the initial repeat time.
	// This prevents the secondary repeat firing before the first repeat without us needing to
	// track whether or not the initial repeat has fired.
	// Thus, the secondary repeat time is found by adding both values together.
	u32 secondaryRepeatTime = KEY_INITIAL_REPEAT_TIME + KEY_SECONDARY_REPEAT_TIME;

	if (newPress) {
		_focusedGadget->keyPress(keyCode);
	} else if (released) {
		_focusedGadget->keyRelease(keyCode);
	}
		
	if (heldTime == KEY_INITIAL_REPEAT_TIME) {
		_focusedGadget->keyRepeat(keyCode);
	} else if (heldTime == secondaryRepeatTime) {
		_focusedGadget->keyRepeat(keyCode);
		heldTime = KEY_INITIAL_REPEAT_TIME;
	}
}

// Process all key input
void Woopsi::handleKeys() {
	if (_focusedGadget != NULL) {
		handleKey(Pad.Newpress.A, Pad.Released.A, Pad.HeldTime.A, KEY_CODE_A);
		handleKey(Pad.Newpress.B, Pad.Released.B, Pad.HeldTime.B, KEY_CODE_B);
		handleKey(Pad.Newpress.X, Pad.Released.X, Pad.HeldTime.X, KEY_CODE_X);
		handleKey(Pad.Newpress.Y, Pad.Released.Y, Pad.HeldTime.Y, KEY_CODE_Y);
		handleKey(Pad.Newpress.L, Pad.Released.L, Pad.HeldTime.L, KEY_CODE_L);
		handleKey(Pad.Newpress.R, Pad.Released.R, Pad.HeldTime.R, KEY_CODE_R);
		handleKey(Pad.Newpress.Up, Pad.Released.Up, Pad.HeldTime.Up, KEY_CODE_UP);
		handleKey(Pad.Newpress.Down, Pad.Released.Down, Pad.HeldTime.Down, KEY_CODE_DOWN);
		handleKey(Pad.Newpress.Left, Pad.Released.Left, Pad.HeldTime.Left, KEY_CODE_LEFT);
		handleKey(Pad.Newpress.Right, Pad.Released.Right, Pad.HeldTime.Right, KEY_CODE_RIGHT);
		handleKey(Pad.Newpress.Start, Pad.Released.Start, Pad.HeldTime.Start, KEY_CODE_START);
		handleKey(Pad.Newpress.Select, Pad.Released.Select, Pad.HeldTime.Select, KEY_CODE_SELECT);
	}
}

void Woopsi::handleLid() {

	// Check for lid closed event
	if (Pad.Held.Lid && !_lidClosed) {

		// Lid has just been closed
		_lidClosed = true;

		// Run lid closed on all gadgets
		s32 i = 0;
		while (i < _gadgets.size()) {
			_gadgets[i]->lidClose();
			i++;
		}

	} else if (!Pad.Held.Lid && _lidClosed) {

		// Lid has just been opened
		_lidClosed = false;

		// Run lid opened on all gadgets
		s32 i = 0;
		while (i < _gadgets.size()) {
			_gadgets[i]->lidOpen();
			i++;
		}
	}
}

bool Woopsi::swapGadgetDepth(Gadget* gadget) {
	// Do we have more than one screen?
	if (_gadgets.size() > 1) {

		s32 gadgetSource = getGadgetIndex(gadget);
		s32 gadgetDest = 0;

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

			gadget->redraw();

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

		for (s32 i = 0; i < _gadgets.size(); i++) {
			if ((_gadgets[i]->isDrawingEnabled()) && (!_gadgets[i]->isDeleted()) && (!_gadgets[i]->isDecoration())) {
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
				for (s32 i = _gadgets.size() - 1; i > -1; i--) {
					if ((gadget != _gadgets[i]) && (_gadgets[i]->isDecoration())) {
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
	WoopsiArray<Rect>* invalidRectangles = new WoopsiArray<Rect>();

	if (invalidRectangles != NULL) {
		 
		// Add rectangle into the vector
		invalidRectangles->push_back(rect);

		// Refresh children
		for (s32 i = _gadgets.size() - 1; i > -1 ; i--) {
			if (invalidRectangles->size() > 0) {
				_gadgets[i]->redrawDirty(invalidRectangles, NULL);
			} else {
				break;
			}
		}

		// Tidy up
		delete invalidRectangles;
	}
}

// Add a timer to the list of timers that receive VBL events
void Woopsi::registerForVBL(WoopsiTimer* timer) {

	// Ensure gadget is not already in the list
	for (s32 i = 0; i < _vblListeners.size(); i++) {
		if (_vblListeners[i] == timer) {
			return;
		}
	}

	_vblListeners.push_back(timer);
}

// Remove a timer from the VBL list
void Woopsi::unregisterFromVBL(WoopsiTimer* timer) {
	
	// Locate gadget in the list
	for (s32 i = 0; i < _vblListeners.size(); i++) {
		if (_vblListeners[i] == timer) {
			_vblListeners.erase(i);
			return;
		}
	}
}


// Delete all gadgets in the queue
void Woopsi::processDeleteQueue() {

	s32 i = 0;
	while (i < _deleteQueue.size()) {
		_deleteQueue[i]->destroy();
		i++;
	}

	_deleteQueue.clear();
}

void Woopsi::addToDeleteQueue(Gadget* gadget) {
	_deleteQueue.push_back(gadget);
}

void Woopsi::shelveContextMenu() {
	if (!_contextMenu->isShelved()) {
		_contextMenu->shelve();
		_contextMenu->reset();
	}
}

void Woopsi::setClickedGadget(Gadget* gadget) {

	// Do we have a clicked gadget already?
	if (_clickedGadget != NULL) {

		// Ensure that the existing clicked gadget is released *outside* its bounds
		_clickedGadget->release(_clickedGadget->getX() - 10, 0);
	}
	
	// Update the pointer
	_clickedGadget = gadget;
}

void Woopsi::showKeyboard(KeyboardEventHandler* opener) {

	if (_keyboardScreen == NULL) {
		_keyboardScreen = new WoopsiKeyboardScreen(opener);
	}

	if (_gadgets.size() > 0) {

		// Locate the topmost bottom screen
		Screen* bottomScreen = NULL;
		for (s32 i = _gadgets.size() - 1; i > -1; --i) {
			if ((_gadgets[i]->isDrawingEnabled()) && (!_gadgets[i]->isDeleted()) && (!_gadgets[i]->isDecoration())) {
				if (_gadgets[i]->getPhysicalScreenNumber() == 0) {
					bottomScreen = (Screen*)_gadgets[i];
					break;
				}
			}
		}

		// Flip the screen to the top display
		bottomScreen->flipToTopScreen();
	}

	// Append the keyboard screen
	addGadget(_keyboardScreen);
}

void Woopsi::hideKeyboard() {
	if (_keyboardScreen == NULL) return;

	_keyboardScreen->close();
	_keyboardScreen = NULL;

	if (_gadgets.size() > 0) {

		// Locate the topmost top screen
		Screen* topScreen = NULL;
		for (s32 i = _gadgets.size() - 1; i > -1; --i) {
			if ((_gadgets[i]->isDrawingEnabled()) && (!_gadgets[i]->isDeleted()) && (!_gadgets[i]->isDecoration())) {
				if (_gadgets[i]->getPhysicalScreenNumber() == 1) {
					topScreen = (Screen*)_gadgets[i];
					break;
				}
			}
		}

		// Flip the screen to the bottom display
		topScreen->flipToBottomScreen();
	}
}
