#include "contextmenu.h"
#include "damagedrectmanager.h"
#include "fontbase.h"
#include "graphicsport.h"
#include "gadgetstyle.h"
#include "hardware.h"
#include "keyboardeventhandler.h"
#include "pad.h"
#include "screen.h"
#include "stylus.h"
#include "woopsi.h"
#include "woopsifuncs.h"
#include "woopsikeyboard.h"
#include "woopsikeyboardscreen.h"
#include "woopsitimer.h"

using namespace WoopsiUI;

// Instantiate singleton
Woopsi* Woopsi::singleton = NULL;

Woopsi::Woopsi(GadgetStyle* style) : Gadget(0, 0, SCREEN_WIDTH, TOP_SCREEN_Y_OFFSET + SCREEN_HEIGHT, style) {
	_lidClosed = false;

	_flags.modal = true;
	_flags.borderless = true;
	
	_clickedGadget = NULL;
	_keyboardScreen = NULL;
	_vblCount = 0;

	// Set up singleton pointer
	singleton = this;

	_damagedRectManager = new DamagedRectManager(this);

	woopsiInitDefaultGadgetStyle();

	// Set up DS display hardware
	Hardware::init();
	
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
			_style.colours.text = defaultGadgetStyle->colours.text;
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
		_style.colours.text = style->colours.text;
		_style.font = style->font;
		_style.glyphFont = style->glyphFont;
	}

	// Create context menu
	_contextMenu = new ContextMenu(style);
	addGadget(_contextMenu);
	_contextMenu->shelve();

	// Create background screens that will sit behind all other screens
	Screen* screen;
	
	if (SCREEN_COUNT == 2) {
		screen = new Screen("", style);
		screen->setDecoration(true);
		addGadget(screen);
		screen->flipToTopScreen();
	}

	screen = new Screen("", style);
	screen->setDecoration(true);
	addGadget(screen);
}

Woopsi::~Woopsi() {
	stopModal();
	singleton = NULL;
	_contextMenu = NULL;

	delete _damagedRectManager;
	_damagedRectManager = NULL;

	Hardware::shutdown();

	woopsiFreeDefaultGadgetStyle();
}

void Woopsi::processOneVBL(Gadget* gadget) {
	handleVBL();
	handleStylus(gadget);
	handleKeys();
	handleLid();
	
	// Redraw all damaged rects
	_damagedRectManager->redraw();
	
	Hardware::waitForVBlank();
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

	const Stylus& stylus = Hardware::getStylus();
	const Pad& pad = Hardware::getPad();

	// All gadgets
	if (stylus.isNewPress()) {
		if (pad.isHeld(Pad::KEY_CODE_L) || pad.isHeld(Pad::KEY_CODE_R)) {
			handleShiftClick(stylus.getX(), stylus.getY(), gadget);
		} else {
			handleClick(stylus.getX(), stylus.getY(), gadget);
		}
	} else if (stylus.isHeld()) {
		if (_clickedGadget != NULL) {
			_clickedGadget->drag(stylus.getX(), stylus.getY(), stylus.getVX(), stylus.getVY());
		}
	} else if (_clickedGadget != NULL) {
		_clickedGadget->release(stylus.getX(), stylus.getY());
	}
}

void Woopsi::handleShiftClick(s16 x, s16 y, Gadget* gadget) {

	// Shelve the existing context menu to ensure that if
	// the click does not result in the menu being redisplayed
	// it is correctly hidden
	shelveContextMenu();

	const Stylus& stylus = Hardware::getStylus();

	// Working with a modal gadget or the whole structure?
	if (gadget == NULL) {

		// All gadgets
		shiftClick(stylus.getX(), stylus.getY());
	} else {

		// One gadget
		gadget->shiftClick(stylus.getX(), stylus.getY());
	}
}

void Woopsi::handleClick(s16 x, s16 y, Gadget* gadget) {

	if (!gadget->click(x, y)) return;
		
	// If the context menu isn't an ancestor of the clicked gadget, we
	// need to close the menu.
	Gadget* ancestor = _clickedGadget;
	bool closeMenu = true;

	while (ancestor != NULL) {
		if (ancestor == _contextMenu) {

			// Either the context menu or one of its descendants has
			// been clicked.  In that case, we shouldn't close the menu.
			closeMenu = false;
			break;
		}

		ancestor = ancestor->getParent();
	}

	if (closeMenu) shelveContextMenu();
}

void Woopsi::handleKey(Pad::KeyCode keyCode) {

	// We do not reset the repeat timers to 0 - instead we reset it back to the
	// initial repeat time.  This prevents the secondary repeat firing before
	// the first repeat without us needing to track whether or not the initial
	// repeat has fired.  Thus, the secondary repeat time is found by adding
	// both values together.
	//
	// This technique only works if the initial repeat time is longer than the
	// secondary repeat.
	const Pad& pad = Hardware::getPad();
	bool newPress = pad.isNewPress(keyCode);
	bool released = pad.isReleased(keyCode);
	s32 heldTime = pad.heldTimeForKey(keyCode);

	s32 secondaryRepeatTime = KEY_INITIAL_REPEAT_TIME + KEY_SECONDARY_REPEAT_TIME;

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
	if (_focusedGadget == NULL) return;

	handleKey(Pad::KEY_CODE_A);
	handleKey(Pad::KEY_CODE_B);
	handleKey(Pad::KEY_CODE_X);
	handleKey(Pad::KEY_CODE_Y);
	handleKey(Pad::KEY_CODE_L);
	handleKey(Pad::KEY_CODE_R);
	handleKey(Pad::KEY_CODE_UP);
	handleKey(Pad::KEY_CODE_DOWN);
	handleKey(Pad::KEY_CODE_LEFT);
	handleKey(Pad::KEY_CODE_RIGHT);
	handleKey(Pad::KEY_CODE_START);
	handleKey(Pad::KEY_CODE_SELECT);
}

void Woopsi::handleLid() {

	// TODO: Fix this!

	/*
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
	 */
}

bool Woopsi::flipScreens(Gadget* gadget) {
	
	// Do not flip if there is only one physical screen available
	if (SCREEN_COUNT == 1) return false;

	// Only flip if there are more than three screens. Two are created by
	// default as background screens, so we can only swap if we have more than 3
	if (_gadgets.size() < 4) return false;

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

		// Move to top of stack
		topGadget->raiseToTop();

		// Move to bottom screen
		((Screen*)topGadget)->flipToBottomScreen();
	}
	
	// Move the requested gadget to the top screen
	((Screen*)gadget)->flipToTopScreen();
	
	return true;
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
		if (bottomScreen) {
			bottomScreen->flipToTopScreen();
		}
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
		if (topScreen) {
			topScreen->flipToBottomScreen();
		}
	}
}
