#include "gadget.h"
#include "graphicsport.h"
#include "woopsi.h"
#include "woopsifuncs.h"
#include "gadgeteventhandler.h"
#include "fontbase.h"
#include "contextmenu.h"
#include "gadgetstyle.h"
#include "gadgeteventargs.h"
#include "contextmenu.h"
#include "listdataitem.h"
#include "contextmenueventargs.h"
#include "rectcache.h"
#include "framebuffer.h"

using namespace WoopsiUI;

Gadget::Gadget(s16 x, s16 y, u16 width, u16 height, u32 flags, GadgetStyle* style) {

	// Set properties from parameters
	_x = x;
	_y = y;
	_width = width;
	_height = height;
	_style = new GadgetStyle();

	// Do we need to fetch the default style?
	if (style == NULL) {

		// Use default style
		if (defaultGadgetStyle != NULL) {
			_style->colours.back = defaultGadgetStyle->colours.back;
			_style->colours.shine = defaultGadgetStyle->colours.shine;
			_style->colours.highlight = defaultGadgetStyle->colours.highlight;
			_style->colours.shadow = defaultGadgetStyle->colours.shadow;
			_style->colours.fill = defaultGadgetStyle->colours.fill;
			_style->colours.dark = defaultGadgetStyle->colours.dark;
			_style->font = defaultGadgetStyle->font;
		}
	} else {

		// Use specified style
		_style->colours.back = style->colours.back;
		_style->colours.shine = style->colours.shine;
		_style->colours.highlight = style->colours.highlight;
		_style->colours.shadow = style->colours.shadow;
		_style->colours.fill = style->colours.fill;
		_style->colours.dark = style->colours.dark;
		_style->font = style->font;
	}

	// Mask flags against bitmasks and logical NOT twice to obtain boolean values
	_flags.borderless = (!(!(flags & GADGET_BORDERLESS)));
	_flags.draggable = (!(!(flags & GADGET_DRAGGABLE)));
	_flags.permeable = (!(!(flags & GADGET_PERMEABLE)));
	_flags.doubleClickable = (!(!(flags & GADGET_DOUBLE_CLICKABLE)));
	_flags.decoration = (!(!(flags & GADGET_DECORATION)));

	// Following flags are set to true if not passed in flags parameter
	_flags.shiftClickChildren = (!(flags & GADGET_NO_SHIFT_CLICK_CHILDREN));

	// Dragging values
	_grabPointX = 0;
	_grabPointY = 0;
	_newX = 0;
	_newY = 0;

	// Set initial flag values
	_flags.clicked = false;
	_flags.dragging = false;
	_flags.hasFocus = false;
	_flags.deleted = false;
	_flags.drawingEnabled = false;
	_flags.enabled = true;
	_flags.erased = true;
	_flags.shelved = false;
	_flags.visibleRegionCacheInvalid = true;
	_flags.hidden = false;
	_flags.modal = false;

	// Set hierarchy pointers
	_parent = NULL;
	_focusedGadget = NULL;

	// Double-click
	_lastClickTime = 0;
	_lastClickX = 0;
	_lastClickY = 0;
	_doubleClickBounds = DOUBLE_CLICK_BOUNDS;

	// Set other default values
	_decorationCount = 0;
	_refcon = 0;
	
	_outline = OUTLINE_CLICK_DEPENDENT;
	_closeType = CLOSE_TYPE_CLOSE;

	_rectCache = new RectCache(this);

	_gadgetEventHandlers = new GadgetEventHandlerList(this);
}

Gadget::~Gadget() {

	// Ensure that the gadget is disposed of correctly if it has not been sent to the deletion queue.
	if (!_flags.deleted) {

		_flags.deleted = true;

		if (woopsiApplication != NULL) {

			// Close the context menu if we're closing the gadget that opened it
			if (woopsiApplication->getContextMenu()->getOpener() == this) {
				woopsiApplication->shelveContextMenu();
			}

			// Unset the clicked pointer if necessary
			if (woopsiApplication->getClickedGadget() == this) {
				woopsiApplication->setClickedGadget(NULL);
			}
		}
	}
	
	if (_parent != NULL) {
		_parent->removeChild(this);
	}

	// Delete context menu data
	for (s32 i = 0; i < _contextMenuItems.size(); i++) {
		delete [] _contextMenuItems[i].name;
	}

	// Delete children
	for (s32 i = 0; i < _gadgets.size(); i++) {
		_gadgets[i]->destroy();
	}

	// Delete shelved children
	for (s32 i = 0; i < _shelvedGadgets.size(); i++) {
		_shelvedGadgets[i]->destroy();
	}

	_gadgets.clear();
	_shelvedGadgets.clear();

	delete _rectCache;
	delete _style;
	delete _gadgetEventHandlers;
}

const s16 Gadget::getX() const {
	if (_parent != NULL) {
		return _parent->getX() + _x;
	}

	return _x;
}

const s16 Gadget::getY() const {
	if (_parent != NULL) {
		return _parent->getY() + _y;
	}

	return _y;
}

const bool Gadget::isDeleted() const {
	if (_parent != NULL) {
		if (_parent->isDeleted()) {
			return true;
		}
	}

	return _flags.deleted;
}

const bool Gadget::isDrawingEnabled() const {
	if (_parent != NULL) {
		if (_parent->isDrawingEnabled()) {
			// Drawing is enabled if the gadget is drawable, not deleted, and not shelved
			return (_flags.drawingEnabled & (!_flags.deleted) & (!_flags.shelved) & (!_flags.hidden));
		}
	} else {
		return (_flags.drawingEnabled & (!_flags.deleted) & (!_flags.shelved) & (!_flags.hidden));
	}

	return false;
}

const bool Gadget::isHidden() const {
	if (_parent != NULL) {
		if (!_parent->isHidden()) {
			// Hidden if the gadget is deleted, shelved or hidden
			return (_flags.deleted | _flags.shelved | _flags.hidden);
		}
	} else {
		return (_flags.deleted | _flags.shelved | _flags.hidden);
	}

	return true;
}

const bool Gadget::isEnabled() const {
	if (_parent != NULL) {
		if (_parent->isEnabled()) {
			// Enabled if the gadget is enabled, not deleted, not shelved and not hidden
			return (_flags.enabled & (!_flags.deleted) & (!_flags.shelved) & (!_flags.hidden));
		}
	} else {
		return (_flags.enabled & (!_flags.deleted) & (!_flags.shelved) & (!_flags.hidden));
	}

	return false;
}

const bool Gadget::isModal() const {
	if ((woopsiApplication != NULL) && (woopsiApplication != this)) {
		return woopsiApplication->isModal() & _flags.modal;
	}

	return _flags.modal;
}

void Gadget::setBorderless(bool isBorderless) {
	_flags.borderless = isBorderless;

	invalidateVisibleRectCache();
}

void Gadget::setFocusedGadget(Gadget* gadget) {

	if (_focusedGadget != gadget) {
		if (_focusedGadget != NULL) {
			// Blur the current active gadget
			_focusedGadget->blur();
		}
	}

	// Remember the new active gadget
	_focusedGadget = gadget;

	// Make this gadget active too
	focus();
}

const u8 Gadget::calculatePhysicalScreenNumber(s16 y) const {
	if (y & TOP_SCREEN_Y_OFFSET) {
		// Top screen
		return 1;
	}

	// Bottom screen
	return 0;
}

const s16 Gadget::calculatePhysicalScreenY(s16 y) const {
	if (y & TOP_SCREEN_Y_OFFSET) {
		return y - TOP_SCREEN_Y_OFFSET;
	}

	return y;
}

void Gadget::clear(Rect clipRect) {
	GraphicsPort* port = newInternalGraphicsPort(clipRect);
	port->drawFilledRect(0, 0, _width, _height, getBackColour());
	delete port;
}

void Gadget::clear() {
	GraphicsPort* port = newInternalGraphicsPort(true);
	port->drawFilledRect(0, 0, _width, _height, getBackColour());
	delete port;
}

// Check for single-point collisions
bool Gadget::checkCollision(s16 x, s16 y) const {

	if (!isHidden()) {

		// Get the clipped rect
		Rect rect;
		getRectClippedToHierarchy(rect);

		if ((x >= rect.x) && (y >= rect.y) && (x < rect.x + rect.width) && (y < rect.y + rect.height)) {
			return true;
		}
	}

	return false;
}

// Check for collisions with another rectangle
bool Gadget::checkCollision(s16 x, s16 y, u16 width, u16 height) const {

	if (!isHidden()) {

		// Get the clipped rect
		Rect rect;
		getRectClippedToHierarchy(rect);

		if ((x + width >= rect.x) && (y + height >= rect.y) && (x < rect.x + rect.width) && (y < rect.y + rect.height)) {
			return true;
		}
	}

	return false;
}

// Check for collisions with another gadget
bool Gadget::checkCollision(Gadget* gadget) const {

	// Get the clipped rect
	Rect rect;
	gadget->getRectClippedToHierarchy(rect);

	return checkCollision(rect.x, rect.y, rect.width, rect.height);
}

void Gadget::drawChildren() {
	for (s32 i = 0; i < _gadgets.size(); i++) {
		_gadgets[i]->redraw();
	}
}

void Gadget::redraw() {
	if (isDrawingEnabled()) {
		cacheVisibleRects();

		// Draw all visible rectangles
		for (s32 i = 0; i < _rectCache->getBackgroundRegions()->size(); i++) {
			draw(_rectCache->getBackgroundRegions()->at(i));
		}

		// Remember that the gadget is no longer erased
		_flags.erased = false;

		drawChildren();
	}
}

// Redraw any regions that have been overlapped
// Called when erasing a gadget higher up the vector
void Gadget::redrawDirty(WoopsiArray<Rect>* invalidRects, Gadget* sender) {

	if (isDrawingEnabled()) {

		// Draw any children first
		redrawDirtyChildren(invalidRects, sender);
	
		// Create an array that will contain all of the rects from the
		// original array that overlap this gadget
		WoopsiArray<Rect>* overlappingRects = new WoopsiArray<Rect>();

		// Remove any non-overlapping rectangles from dirty vector and add to
		// overlapping vector
		_rectCache->splitRectangles(invalidRects, overlappingRects, sender);

		// Create an array that will contain all of the rects that overlap this
		// gadget clipped to its parent
		WoopsiArray<Rect>* rectsToDraw = new WoopsiArray<Rect>();
		
		// Split from overlappingRects into rectsToDraw, giving us an array
		// of rects that overlap only the visible portions of this gadget
		_rectCache->splitRectangles(overlappingRects, rectsToDraw, sender);
		
		// Draw the dirty rects
		for (s32 i = 0; i < rectsToDraw->size(); i++) {
			draw(rectsToDraw->at(i));
		}
		
		// Copy all of the overlapping rects we didn't draw back to the main
		// array of rects that need to be drawn by another gadget
		for (s32 i = 0; i < overlappingRects->size(); i++) {
			invalidRects->push_back(overlappingRects->at(i));
		}

		// Clean up
		delete overlappingRects;
		delete rectsToDraw;
	}
}

void Gadget::redrawDirtyChildren(WoopsiArray<Rect>* invalidRects, Gadget* sender) {

	for (s32 i = _gadgets.size() - 1; i > -1 ; i--) {
		if (invalidRects->size() > 0) {
			if (_gadgets.at(i) != sender) {
				_gadgets[i]->redrawDirty(invalidRects, sender);
			}
		} else {
			break;
		}
	}
}

// Erase this gadget from the screen
void Gadget::erase() {

	if (!_flags.erased) {
		cacheVisibleRects();

		if (_parent != NULL) {
			_parent->eraseGadget(this);
		}

		// Remember that the gadget has been erased
		_flags.erased = true;

		invalidateVisibleRectCache();
	}
}

// Erase a child gadget from the screen
void Gadget::eraseGadget(Gadget* gadget) {

	// Locate the gadget
	s32 gadgetIndex = getGadgetIndex(gadget);

	// Ensure rect cache is up to date
	gadget->cacheVisibleRects();

	// Order all lower gadgets to redraw themselves based on the erased gadget's
	// visible rect cache
	for (s32 i = gadgetIndex - 1; i > -1; i--) {
		_gadgets[i]->redrawDirty(gadget->getForegroundRegions(), gadget);
	}

	// Order this gadget to redraw itself based on any remaining rectangles
	// in the erased gadget's rect cache
	redrawDirty(gadget->getForegroundRegions(), gadget);

	invalidateVisibleRectCache();
}

// Marks the gadget as deleted and adds it to the deletion queue
void Gadget::close() {

	if (!_flags.deleted) {
		_gadgetEventHandlers->raiseCloseEvent();
		_gadgetEventHandlers->disable();

		_flags.deleted = true;
		_flags.drawingEnabled = false;

		// Ensure the gadget isn't running modally
		stopModal();

		erase();

		if (_parent != NULL) {
			_parent->closeChild(this);
		}

		// Ensure that this gadget can no longer affect the decoration count
		_flags.decoration = false;
	}
}

// Erases the gadget from the display and prevents it from being redrawn
bool Gadget::shelve() {

	if (!_flags.shelved) {

		_gadgetEventHandlers->raiseShelveEvent();

		erase();

		_flags.shelved = true;
		_flags.drawingEnabled = false;

		// Ensure the gadget isn't running modally
		stopModal();

		if (_parent != NULL) {
			_parent->shelveChild(this);
		}

		return true;
	}

	return false;
}

// Re-enables drawing and draws the gadget to the display
bool Gadget::unshelve() {

	if (_flags.shelved) {

		_gadgetEventHandlers->raiseUnshelveEvent();

		_flags.drawingEnabled = true;
		_flags.shelved = false;

		if (_parent != NULL) {
			_parent->moveShelvedToChildList(this);
			_parent->invalidateVisibleRectCache();	
		}

		redraw();

		return true;
	}
	return false;
}

// Add a gadget to the deletion queue ready for later processing
void Gadget::moveChildToDeleteQueue(Gadget* gadget) {
	// Locate gadget in main vector
	for (s32 i = 0; i < _gadgets.size(); i++) {
		if (_gadgets[i] == gadget) {

			// Add gadget to Woopsi's delete vector
			Woopsi::addToDeleteQueue(gadget);

			// Remove gadget from main vector
			_gadgets.erase(i);

			break;
		}
	}
}

// Add a gadget to the shelved list ready for later processing
bool Gadget::moveChildToShelvedList(Gadget* gadget) {

	// Locate gadget in main vector
	for (s32 i = 0; i < _gadgets.size(); i++) {
		if (_gadgets[i] == gadget) {

			// Add gadget to shelved vector
			_shelvedGadgets.push_back(gadget);

			// Remove gadget from main vector
			_gadgets.erase(i);

			return true;
		}
	}

	return false;
}

// Move a shelved gadget back to the child list
bool Gadget::moveShelvedToChildList(Gadget* gadget) {

	// Locate gadget in shelved vector
	for (s32 i = 0; i < _shelvedGadgets.size(); i++) {
		if (_shelvedGadgets[i] == gadget) {

			// Process decorations and gadgets differently
			if (gadget->isDecoration()) {

				// Add gadget to end of decoration list
				_gadgets.insert(_decorationCount, gadget);
				_decorationCount++;
			} else {

				// Add gadget to end of main vector
				_gadgets.push_back(gadget);
			}

			// Remove gadget from shelved vector
			_shelvedGadgets.erase(i);

			return true;
		}
	}

	return false;
}

// Shelve a child
void Gadget::shelveChild(Gadget* gadget) {
	if (gadget != NULL) {

		// Ensure gadget knows it is being closed
		if (!gadget->isShelved()) {
			gadget->shelve();
		}

		// Unset clicked gadget if necessary
		Gadget* clickedGadget = woopsiApplication->getClickedGadget();
		if (clickedGadget == gadget) {
			clickedGadget->release(clickedGadget->getX(), clickedGadget->getY());
		}

		// Do we need to give another gadget focus?
		if (_focusedGadget == gadget) {

			_focusedGadget = NULL;

			// Try to choose highest gadget
			for (s32 i = _gadgets.size() - 1; i > -1; i--) {
				if ((_gadgets[i] != gadget) && (!_gadgets[i]->isHidden())) {
					_focusedGadget = _gadgets[i];
				}
			}
		}

		// Decrease decoration count if necessary
		if (gadget->isDecoration()) {
			_decorationCount--;
		}

		// Where should the focus go?
		if (_focusedGadget != NULL) {
			// Send focus to the new active gadget
			_focusedGadget->focus();
		} else {
			// Give focus to this
			setFocusedGadget(NULL);
		}

		moveChildToShelvedList(gadget);
	}
}

// Close a child
void Gadget::closeChild(Gadget* gadget) {
	if (gadget != NULL) {

		// Ensure gadget knows it is being closed
		if (!gadget->isDeleted()) {
			gadget->close();
		}

		// Unset clicked gadget if necessary
		Gadget* clickedGadget = woopsiApplication->getClickedGadget();
		if (clickedGadget == gadget) {
			clickedGadget->release(clickedGadget->getX(), clickedGadget->getY());
		}

		// Do we need to make another gadget active?
		if (_focusedGadget == gadget) {

			_focusedGadget = NULL;

			// Try to choose highest gadget
			for (s32 i = _gadgets.size() - 1; i > -1; i--) {
				if ((_gadgets[i] != gadget) && (!_gadgets[i]->isHidden())) {
					_focusedGadget = _gadgets[i];
				}
			}
		}

		// Decrease decoration count if necessary
		if (gadget->isDecoration()) {
			_decorationCount--;
		}

		// Where should the focus go?
		if (_focusedGadget != NULL) {
			// Send focus to the new active gadget
			_focusedGadget->focus();
		} else {
			// Give focus to this
			setFocusedGadget(NULL);
		}

		// Ensure that gadget is no longer receiving VBL events
		if (woopsiApplication != NULL) {

			// Close the context menu if we're closing the gadget that opened it
			if (woopsiApplication->getContextMenu()->getOpener() == gadget) {
				woopsiApplication->shelveContextMenu();
			}
		}

		moveChildToDeleteQueue(gadget);
	}
}

bool Gadget::swapGadgetDepth(Gadget* gadget) {
	
	// Can we swap?
	if ((_gadgets.size() > 1) && (!gadget->isDecoration())) {

		s32 gadgetSource = 0;
		s32 gadgetDest = 0;

		// Locate the gadget in the vector
		gadgetSource = getGadgetIndex(gadget);

		// Attempt to raise up
		s32 i = getHigherVisibleGadget(gadgetSource);

		if (i > -1) {
			// Raise
			gadgetDest = i;
		} else {
			// Lower to bottom of stack
			gadgetDest = _decorationCount;
		}
		
		// Erase the gadget from the screen
		eraseGadget(gadget);

		// Swap
		Gadget* tmp = _gadgets[gadgetSource];
		_gadgets[gadgetSource] = _gadgets[gadgetDest];
		_gadgets[gadgetDest] = tmp;

		// Invalidate the gadgets below the top affected gadget
		if (gadgetSource < gadgetDest) {
			// Source lower; invalidate from dest down
			_gadgets[gadgetDest]->invalidateVisibleRectCache();
			invalidateLowerGadgetsVisibleRectCache(_gadgets[gadgetDest]);
		} else {
			// Dest lower; invalidate from source down
			_gadgets[gadgetSource]->invalidateVisibleRectCache();
			invalidateLowerGadgetsVisibleRectCache(_gadgets[gadgetSource]);
		}
		
		// Redraw the gadget
		gadget->redraw();

		return true;
	}

	return false;
}

bool Gadget::swapDepth() {
	if (_parent != NULL) {
		return _parent->swapGadgetDepth(this);
	}

	return false;
}

bool Gadget::enable() {
	if (!_flags.enabled) {
		_flags.enabled = true;

		redraw();

		_gadgetEventHandlers->raiseEnableEvent();

		return true;
	}

	return false;
}

bool Gadget::disable() {
	if (_flags.enabled) {
		_flags.enabled = false;

		redraw();

		_gadgetEventHandlers->raiseDisableEvent();

		return true;
	}

	return false;
}

bool Gadget::moveTo(s16 x, s16 y) {

	// Enforce gadget to stay within parent confines if necessary
	if (_parent != NULL) {
		if (!_parent->isPermeable()) {

			Rect parentRect;
			_parent->getClientRect(parentRect);

			// Check x co-ordinate
			if (x < parentRect.x) {
				x = parentRect.x;

				// Check width against new value
				if (x + _width > parentRect.x + parentRect.width) {
					return false;
				}
			} else if (x + _width > parentRect.x + parentRect.width) {
				x = (parentRect.x + parentRect.x) - _width;

				// Check new x value
				if (x < parentRect.x) {
					return false;
				}
			}

			// Check y co-ordinate
			if (y < parentRect.y) {
				y = parentRect.y;

				// Check height against new value
				if (y + _height > parentRect.y + parentRect.height) {
					return false;
				}
			} else if (y + _height > parentRect.y + parentRect.height) {
				y = (parentRect.y + parentRect.y) - _height;

				// Check new y value
				if (y < parentRect.y) {
					return false;
				}
			}
		}
	}
			
	// Perform move if necessary
	if ((_x != x) || (_y != y)) {
		erase();

		s16 oldX = _x;
		s16 oldY = _y;

		_x = x;
		_y = y;

		redraw();

		_gadgetEventHandlers->raiseMoveEvent(x, y, x - oldX, y - oldY);

		return true;
	}

	return false;
}

bool Gadget::resize(u16 width, u16 height) {

	// Enforce gadget to stay within parent confines if necessary
	if (_parent != NULL) {
		if (!_parent->isPermeable()) {

			Rect parentRect;
			_parent->getClientRect(parentRect);

			// Check width
			if (_x + width > parentRect.x + parentRect.width) {
				width = parentRect.width - _x;
			}

			// Check height
			if (_y + height > parentRect.y + parentRect.height) {
				height = parentRect.height - _y;
			}
		}
	}

	if ((_width != width) || (_height != height)) {
		erase();

		_width = width;
		_height = height;

		// Handle visible region caching
		if (_parent != NULL) {
			_parent->invalidateLowerGadgetsVisibleRectCache(this);
		}

		redraw();

		_gadgetEventHandlers->raiseResizeEvent(width, height);

		return true;
	}

	return false;
}

bool Gadget::changeDimensions(s16 x, s16 y, u16 width, u16 height) {
	bool wasDrawing = _flags.drawingEnabled;
	_flags.drawingEnabled = false;
	bool moved = moveTo(x, y);
	_flags.drawingEnabled = wasDrawing;
	return (resize(width, height) | moved);
}

bool Gadget::isDoubleClick(s16 x, s16 y) {

	// Check for a double-click
	if ((_flags.doubleClickable) && hasFocus() && (woopsiApplication != NULL) && (Stylus.DblClick)) {

		// Within the allowed region?
		if ((_lastClickX > x - _doubleClickBounds) && (_lastClickX < x + _doubleClickBounds)) {
			if ((_lastClickY > y - _doubleClickBounds) && (_lastClickY < y + _doubleClickBounds)) {
				return true;
			}
		}
	}

	return false;
}

bool Gadget::click(s16 x, s16 y) {

	// Check for a double-click
	if (isDoubleClick(x, y)) {

		// Process click as a double-click
		return doubleClick(x, y);
	}

	if (checkCollision(x, y)) {

		// Work out which child was clicked
		for (s32 i = _gadgets.size() - 1; i > -1; i--) {
			if (_gadgets[i]->click(x, y)) {
				return true;
			}
		}

		if (isEnabled()) {

			// Handle clicks on this
			_flags.clicked = true;

			// Record data for double-click
			if (woopsiApplication != NULL) {
				_lastClickTime = woopsiApplication->getVBLCount();
			} else {
				_lastClickTime = 0;
			}

			_lastClickX = x;
			_lastClickY = y;

			// Take focus away from child gadgets
			setFocusedGadget(NULL);

			// Tell Woopsi that the clicked gadget has changed
			if (woopsiApplication != NULL) {
				woopsiApplication->setClickedGadget(this);
			}

			// Enable dragging
			setDragging(x, y);

			_gadgetEventHandlers->raiseClickEvent(x, y);
		}

		return true;
	}

	return false;
}

bool Gadget::doubleClick(s16 x, s16 y) {

	if (isEnabled()) {
		if (checkCollision(x, y)) {

			// Work out which child was clicked.  Allow the
			// child to determine if it has been double-clicked or not
			// in case the second click has fallen on a different
			// child to the first.
			for (s32 i = _gadgets.size() - 1; i > -1; i--) {
				if (_gadgets[i]->click(x, y)) {
					return true;
				}
			}

			_flags.clicked = true;

			// Record data for double-click
			if (woopsiApplication != NULL) {
				_lastClickTime = woopsiApplication->getVBLCount();
			} else {
				_lastClickTime = 0;
			}

			_lastClickX = x;
			_lastClickY = y;

			// Take focus away from child gadgets
			setFocusedGadget(NULL);

			// Tell Woopsi that the clicked gadget has changed
			if (woopsiApplication != NULL) {
				woopsiApplication->setClickedGadget(this);
			}

			// Enable dragging
			setDragging(x, y);

			_gadgetEventHandlers->raiseDoubleClickEvent(x, y);

			return true;
		}
	}

	return false;
}

bool Gadget::shiftClick(s16 x, s16 y) {

	if (isEnabled()) {
		if (checkCollision(x, y)) {

			// Work out which child was clicked
			if (_flags.shiftClickChildren) {
				for (s32 i = _gadgets.size() - 1; i > -1; i--) {
					if (_gadgets[i]->shiftClick(x, y)) {
						return true;
					}
				}
			}

			// Handle clicks on this

			// Take focus away from child gadgets
			setFocusedGadget(NULL);

			// Set up the context menu
			showContextMenu(x, y);

			_gadgetEventHandlers->raiseShiftClickEvent(x, y);

			return true;
		}
	}

	return false;
}

bool Gadget::release(s16 x, s16 y) {

	if (_flags.clicked) {
		_flags.clicked = false;
		_flags.dragging = false;

		if (woopsiApplication->getClickedGadget() == this) {
			woopsiApplication->setClickedGadget(NULL);
		}

		// Determine which release event to fire
		if (checkCollision(x, y)) {
			// Release occurred within gadget; raise release
			_gadgetEventHandlers->raiseReleaseEvent(x, y);
		} else {
			// Release occurred outside gadget; raise release
			_gadgetEventHandlers->raiseReleaseOutsideEvent(x, y);
		}

		return true;
	}

	return false;
}

bool Gadget::drag(s16 x, s16 y, s16 vX, s16 vY) {
	if ((isEnabled()) && (_flags.dragging)) {
		if ((vX != 0) || (vY != 0)) {
			_gadgetEventHandlers->raiseDragEvent(x, y, vX, vY);
		}

		return true;
	}

	return false;
}

bool Gadget::keyPress(KeyCode keyCode) {
	if (isEnabled()) {
		
		// Raise keypress for this gadget
		_gadgetEventHandlers->raiseKeyPressEvent(keyCode);

		// Handle active child
		if (_focusedGadget != NULL) {
			_focusedGadget->keyPress(keyCode);
		}

		return true;
	}

	return false;
}

bool Gadget::keyRelease(KeyCode keyCode) {
	if (isEnabled()) {

		// Raise key release for this gadget
		_gadgetEventHandlers->raiseKeyReleaseEvent(keyCode);

		// Handle active child
		if (_focusedGadget != NULL) {
			_focusedGadget->keyRelease(keyCode);
		}

		return true;
	}

	return false;
}

void Gadget::lidClose() {
	_gadgetEventHandlers->raiseLidCloseEvent();

	// Run lid closed on all gadgets
	for (s32 i = 0; i < _gadgets.size(); i++) {
		_gadgets[i]->lidClose();
	}
}

void Gadget::lidOpen() {
	_gadgetEventHandlers->raiseLidOpenEvent();

	// Run lid opened on all gadgets
	for (s32 i = 0; i < _gadgets.size(); i++) {
		_gadgets[i]->lidOpen();
	}
}

bool Gadget::focus() {
	if (isEnabled()) {

		// Remember if the gadget has focus
		bool hadFocus = _flags.hasFocus;

		_flags.hasFocus = true;

		// Notify parent that this gadget has focus
		if (_parent != NULL) {
			_parent->setFocusedGadget(this);
		}

		// Raise an event only if the gadget did not have focus
		if (!hadFocus) {
			_gadgetEventHandlers->raiseFocusEvent();
			return true;
		}
	}

	return false;
}

bool Gadget::blur() {

	// Remember if the gadget had focus
	bool hadFocus = _flags.hasFocus;

	_flags.hasFocus = false;

	// Take focus away from child gadgets
	if (_focusedGadget != NULL) {
		_focusedGadget->blur();
		_focusedGadget = NULL;
	}

	// Raise an event only if the gadget had focus
	if (hadFocus) {
		_gadgetEventHandlers->raiseBlurEvent();
		return true;
	}

	return false;
}

bool Gadget::raiseToTop() {
	if (_parent != NULL) {
		if (_parent->raiseGadgetToTop(this)) {
			_gadgetEventHandlers->raiseMoveForwardEvent();
			return true;
		}
	}

	return false;
}

bool Gadget::lowerToBottom() {
	if (_parent != NULL) {
		if (_parent->lowerGadgetToBottom(this)) {
			_gadgetEventHandlers->raiseMoveBackwardEvent();
			return true;
		}
	}

	return false;
}

const s32 Gadget::getGadgetIndex(const Gadget* gadget) const {
	for (s32 i = 0; i < _gadgets.size(); i++) {
		if (_gadgets[i] == gadget) {
			return i;
		}
	}

	return -1;
}

const Gadget* Gadget::getChild(const u32 index) const {
	if (index < (u32)_gadgets.size()) return _gadgets[index];
	return NULL;
}

bool Gadget::raiseGadgetToTop(Gadget* gadget) {

	// Locate gadget in the stack
	s32 index = getGadgetIndex(gadget);

	if ((index > -1) && (index < _gadgets.size() - 1)) {
		_gadgets.erase(index);
		_gadgets.push_back(gadget);

		gadget->invalidateVisibleRectCache();

		// Invalidate all gadgets that collide with the depth-swapped gadget
		for (s32 i = 0; i < _gadgets.size(); i++) {
			if (_gadgets[i]->checkCollision(gadget)) {
				_gadgets[i]->invalidateVisibleRectCache();
			}
		}

		gadget->redraw();

		return true;
	}

	return false;
}

bool Gadget::lowerGadgetToBottom(Gadget* gadget) {

	// Locate gadget in the stack
	s32 index = getGadgetIndex(gadget);

	if (index > _decorationCount) {
		gadget->erase();

		// Handle visible region caching
		gadget->invalidateVisibleRectCache();
		invalidateLowerGadgetsVisibleRectCache(gadget);

		_gadgets.erase(index);
		_gadgets.insert(_decorationCount, gadget);

		gadget->redraw();

		return true;
	}

	return false;
}

// Append a gadget to the end of the gadget list
void Gadget::addGadget(Gadget* gadget) {
	if (gadget->getParent() == NULL) {
		gadget->setParent(this);

		// Process decorations and standard gadgets differently
		if (gadget->isDecoration()) {
			_gadgets.insert(_decorationCount, gadget);

			// Increase the decoration count
			_decorationCount++;
		} else {
			_gadgets.push_back(gadget);
		}

		// Should the gadget steal the focus?
		if (gadget->hasFocus()) {
			setFocusedGadget(gadget);
		}

		gadget->enableDrawing();

		invalidateVisibleRectCache();

		gadget->redraw();
	}
}

// Insert a gadget into the gadget list after the decorations
void Gadget::insertGadget(Gadget* gadget) {
	if (gadget->getParent() == NULL) {
		gadget->setParent(this);

		// Process decorations and standard gadgets differently
		if (gadget->isDecoration()) {
			_gadgets.insert(0, gadget);

			// Increate the decoration count
			_decorationCount++;
		} else {
			_gadgets.insert(_decorationCount, gadget);
		}

		gadget->enableDrawing();

		invalidateVisibleRectCache();

		gadget->redraw();
	}
}

// Invalidate any gadgets below the supplied index
void Gadget::invalidateLowerGadgetsVisibleRectCache(Gadget* gadget) {

	// Find the gadget
	s32 gadgetIndex = getGadgetIndex(gadget);	

	// Invalidate lower gadgets
	for (s32 i = gadgetIndex - 1; i > -1; i--) {
		if (_gadgets[i]->checkCollision(gadget)) {
			_gadgets[i]->invalidateVisibleRectCache();
		}
	}
}

// Set the ID of this gadget and return the old ID
u32 Gadget::setRefcon(u32 refcon) {
	u32 old = _refcon;
	_refcon = refcon;

	return old;
}

// Get the preferred dimensions of the gadget
void Gadget::getPreferredDimensions(Rect& rect) const {
	rect.x = _x;
	rect.y = _y;
	rect.width = _width;
	rect.height = _height;
}

// Insert the available space for child gadgets into the rect
void Gadget::getClientRect(Rect& rect) const {
	rect.x = !_flags.borderless;
	rect.y = rect.x;
	rect.width = ((s16)_width) - (rect.x * 2);
	rect.height = ((s16)_height) - (rect.y * 2);
}

// Set the drag point and tell that gadget that it is being dragged
void Gadget::setDragging(u16 x, u16 y) {
	if (_flags.draggable) {
		_flags.dragging = true;
		_flags.clicked = true;
		_grabPointX = x - getX();
		_grabPointY = y - getY();
		_newX = _x;
		_newY = _y;
	}
}

void Gadget::cacheVisibleRects() {
	_rectCache->cache();
}

void Gadget::invalidateVisibleRectCache() {
	_rectCache->invalidate();

	// Invalidate child cache
	for (s32 i = 0; i < _gadgets.size(); i++) {
		_gadgets[i]->invalidateVisibleRectCache();
	}
}

// Return the client graphics port
GraphicsPort* Gadget::newGraphicsPort(bool isForeground) {

	Rect rect;
	getClientRect(rect);

	FrameBuffer* bitmap = frameBuffer[getPhysicalScreenNumber()];

	// Ensure visible region cache is up to date
	cacheVisibleRects();

	// Choose the rect cache to use as the clipping rect list
	WoopsiArray<Rect>* clipList = isForeground ? _rectCache->getForegroundRegions() : _rectCache->getBackgroundRegions();

	return new GraphicsPort(this, rect.x, rect.y, rect.width, rect.height, bitmap, clipList, NULL);
}

// Return the client graphics port for a specific clipping rect
GraphicsPort* Gadget::newGraphicsPort(Rect clipRect) {

	Rect rect;
	getClientRect(rect);

	FrameBuffer* bitmap = frameBuffer[getPhysicalScreenNumber()];

	// Ensure visible region cache is up to date
	cacheVisibleRects();

	return new GraphicsPort(this, rect.x, rect.y, rect.width, rect.height, bitmap, NULL, &clipRect);
}

// Return the internal graphics port - allows drawing over entire gadget space
GraphicsPort* Gadget::newInternalGraphicsPort(bool isForeground) {

	FrameBuffer* bitmap = frameBuffer[getPhysicalScreenNumber()];

	// Ensure visible region cache is up to date
	cacheVisibleRects();

	// Choose the rect cache to use as the clipping rect list
	WoopsiArray<Rect>* clipList = isForeground ? _rectCache->getForegroundRegions() : _rectCache->getBackgroundRegions();
	
	return new GraphicsPort(this, 0, 0, _width, _height, bitmap, clipList, NULL);
}

// Return the internal graphics port for a specific clipping rect
GraphicsPort* Gadget::newInternalGraphicsPort(Rect clipRect) {

	// Ensure visible region cache is up to date
	cacheVisibleRects();

	// Remaining code OK
	FrameBuffer* bitmap = frameBuffer[getPhysicalScreenNumber()];

	return new GraphicsPort(this, 0, 0, _width, _height, bitmap, NULL, &clipRect);
}

// Return vector of visible rects, including any covered by children
WoopsiArray<Rect>* Gadget::getForegroundRegions() {
	return _rectCache->getForegroundRegions();
}

// Return vector of visible rects, not including any covered by children
WoopsiArray<Rect>* Gadget::getBackgroundRegions() {
	return _rectCache->getBackgroundRegions();
}

// Move up hierarchy, clipping rect to each ancestor
void Gadget::clipRectToHierarchy(Rect& rect) const {

	const Gadget* parent = _parent;
	const Gadget* gadget = this;
	Rect thisRect;

	while (parent != NULL) {

		// Copy parent's properties into the rect
		if (gadget->isDecoration()) {

			// Decorations can draw over entire gadget
			thisRect.x = parent->getX();
			thisRect.y = parent->getY();
			thisRect.width = parent->getWidth();
			thisRect.height = parent->getHeight();
		} else {

			// Standard gadgets can draw into client space
			parent->getClientRect(thisRect);

			// Adjust rect to screen space
			thisRect.x += parent->getX();
			thisRect.y += parent->getY();
		}

		// Clip horizontal position
		if (rect.x < thisRect.x) {
			rect.width -= (thisRect.x - rect.x);
			rect.x = thisRect.x;
		}

		// Clip vertical position
		if (rect.y < thisRect.y) {
			rect.height -= (thisRect.y - rect.y);
			rect.y = thisRect.y;
		}

		// Clip width
		if (rect.x + rect.width > thisRect.x + thisRect.width) {
			rect.width = (thisRect.x + thisRect.width) - rect.x;
		}

		// Clip height
		if (rect.y + rect.height > thisRect.y + thisRect.height) {
			rect.height = (thisRect.y + thisRect.height) - rect.y;
		}

		// Send up to parent
		gadget = parent;
		parent = parent->getParent();
	}
}

// Get the gadget's rect clipped to the dimensions of the parent
void Gadget::getRectClippedToHierarchy(Rect& rect) const {

	// Copy the gadget's properties into the rect
	rect.x = getX();
	rect.y = getY();
	rect.width = getWidth();
	rect.height = getHeight();

	clipRectToHierarchy(rect);
}

FontBase* Gadget::getFont() const {
	return _style->font;
}

void Gadget::setFont(FontBase* font) {
	_style->font = font;
}

bool Gadget::remove() {
	if (_parent != NULL) {
		return _parent->removeChild(this);
	}

	return false;
}

bool Gadget::removeChild(Gadget* gadget) {

	// Do we need to make another gadget active?
	if (_focusedGadget == gadget) {
		_focusedGadget = NULL;
	}

	// Unset clicked gadget if necessary
	if (woopsiApplication != NULL) {
		Gadget* clickedGadget = woopsiApplication->getClickedGadget();
		if (clickedGadget == gadget) {
			clickedGadget->release(clickedGadget->getX(), clickedGadget->getY());
		}
	}

	// Decrease decoration count if necessary
	if (gadget->isDecoration()) {
		_decorationCount--;
	}

	// Close the context menu if we're removing the gadget that opened it
	if (woopsiApplication != NULL) {
		if (woopsiApplication->getContextMenu()->getOpener() == gadget) {
			woopsiApplication->shelveContextMenu();
		}
	}

	// Divorce child from parent
	gadget->setParent(NULL);
	
	gadget->disableDrawing();

	// Locate gadget in main vector
	for (s32 i = 0; i < _gadgets.size(); i++) {
		if (_gadgets[i] == gadget) {

			// Remove gadget from main vector
			_gadgets.erase(i);

			return true;
		}
	}

	// Try to locate in shelved vector
	for (s32 i = 0; i < _shelvedGadgets.size(); i++) {
		if (_shelvedGadgets[i] == gadget) {

			// Divorce child from parent
			_shelvedGadgets[i]->setParent(NULL);

			// Remove gadget from shelved vector
			_shelvedGadgets.erase(i);

			gadget->disableDrawing();

			return true;
		}
	}

	return false;
}

void Gadget::addContextMenuItem(const char* name, u32 value) {
	
	// Create a copy of the string
	char* newName = new char[strlen(name) + 1];
	strcpy(newName, name);

	NameValuePair newItem;
	newItem.name = newName;
	newItem.value = value;

	_contextMenuItems.push_back(newItem);
}

void Gadget::showContextMenu(s16 x, s16 y) {

	if (woopsiApplication != NULL) {
		if (_contextMenuItems.size() > 0) {
			woopsiApplication->getContextMenu()->reset();
			woopsiApplication->getContextMenu()->moveTo(x, y);
			woopsiApplication->getContextMenu()->setOpener(this);

			for (s32 i = 0; i < _contextMenuItems.size(); i++) {
				woopsiApplication->getContextMenu()->addOption(_contextMenuItems[i].name, _contextMenuItems[i].value);
			}
			
			woopsiApplication->getContextMenu()->unshelve();
		}
	}
}

bool Gadget::handleContextMenuSelection(const ListDataItem* item) {
	_gadgetEventHandlers->raiseContextMenuSelectionEvent(item);

	return true;
}

bool Gadget::show() {
	if (_flags.hidden) {
		_flags.hidden = false;

		// Ensure that gadgets behind this do not draw over the
		// top of the newly-visible gadget
		_parent->invalidateLowerGadgetsVisibleRectCache(this);

		_gadgetEventHandlers->raiseShowEvent();
		redraw();
		return true;
	}

	return false;
}

bool Gadget::hide() {
	if (!_flags.hidden) {
		_flags.hidden = true;

		// Ensure the gadget isn't running modally
		stopModal();

		_gadgetEventHandlers->raiseHideEvent();
		erase();
		return true;
	}

	return false;
}

const s32 Gadget::getHigherVisibleGadget(const s32 startIndex) const {
	for (s32 i = startIndex; i < _gadgets.size(); i++) {
		if (!_gadgets[i]->isHidden()) {
			return i;
		}
	}

	return -1;
}

const s32 Gadget::getLowerVisibleGadget(const s32 startIndex) const {
	for (s32 i = startIndex; i > -1; i--) {
		if (!_gadgets[i]->isHidden()) {
			return i;
		}
	}

	return -1;
}

void Gadget::goModal() {

	// Remember that we're running modally
	_flags.modal = true;

	// Steal focus
	focus();

	// Loop until no longer modal
	while (isModal() && (woopsiApplication != NULL)) {
		woopsiApplication->processOneVBL(this);
	}
}
