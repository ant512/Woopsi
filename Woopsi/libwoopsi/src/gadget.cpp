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
	_rect.setX(x);
	_rect.setY(y);
	_rect.setWidth(width);
	_rect.setHeight(height);

	// Do we need to fetch the default style?
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

	// Mask flags against bitmasks and logical NOT twice to obtain boolean values
	_flags.borderless = (!(!(flags & GADGET_BORDERLESS)));
	_flags.draggable = (!(!(flags & GADGET_DRAGGABLE)));
	_flags.permeable = (!(!(flags & GADGET_PERMEABLE)));
	_flags.doubleClickable = (!(!(flags & GADGET_DOUBLE_CLICKABLE)));
	_flags.decoration = (!(!(flags & GADGET_DECORATION)));

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
	_flags.canReceiveFocus = true;

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

	// Set border sizes
	_borderSize.top = 1;
	_borderSize.right = 1;
	_borderSize.bottom = 1;
	_borderSize.left = 1;
	
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

	// Delete children
	while (_gadgets.size() > 0) {
		_gadgets[0]->destroy();
	}

	// Delete shelved children
	while (_shelvedGadgets.size() > 0) {
		_shelvedGadgets[0]->destroy();
	}

	delete _rectCache;
	delete _gadgetEventHandlers;
}

const s16 Gadget::getX() const {
	if (_parent != NULL) {
		return _parent->getX() + _rect.getX();
	}

	return _rect.getX();
}

const s16 Gadget::getY() const {
	if (_parent != NULL) {
		return _parent->getY() + _rect.getY();
	}

	return _rect.getY();
}

const s16 Gadget::getRelativeX() const {
	return _rect.getX();
}

const s16 Gadget::getRelativeY() const {
	return _rect.getY();
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
			return (_flags.drawingEnabled && (!_flags.deleted) && (!_flags.shelved) && (!_flags.hidden));
		}
	} else {
		return (_flags.drawingEnabled && (!_flags.deleted) && (!_flags.shelved) && (!_flags.hidden));
	}

	return false;
}

const bool Gadget::isHidden() const {
	if (_parent != NULL) {
		if (!_parent->isHidden()) {
			// Hidden if the gadget is deleted, shelved or hidden
			return (_flags.deleted || _flags.shelved || _flags.hidden);
		}
	} else {
		return (_flags.deleted || _flags.shelved || _flags.hidden);
	}

	return true;
}

const bool Gadget::isEnabled() const {
	if (_parent != NULL) {
		if (_parent->isEnabled()) {
			// Enabled if the gadget is enabled, not deleted, not shelved and not hidden
			return (_flags.enabled && (!_flags.deleted) && (!_flags.shelved) && (!_flags.hidden));
		}
	} else {
		return (_flags.enabled && (!_flags.deleted) && (!_flags.shelved) && (!_flags.hidden));
	}

	return false;
}

const bool Gadget::isModal() const {
	if ((woopsiApplication != NULL) && (woopsiApplication != this)) {
		return woopsiApplication->isModal() & _flags.modal;
	}

	return _flags.modal;
}

const bool Gadget::canReceiveFocus() const {
	return (isEnabled() & _flags.canReceiveFocus & (!_flags.decoration));
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

// Check for single-point collisions
bool Gadget::checkCollision(s16 x, s16 y) const {

	if (isHidden()) return false;

	// Get the clipped rect
	Rect rect;
	getRectClippedToHierarchy(rect);

	return rect.contains(x, y);
}

bool Gadget::checkCollisionWithForegroundRects(s16 x, s16 y) const {

	if (isHidden()) return false;

	cacheVisibleRects();

	Rect* rect;

	for (s32 i = 0; i < _rectCache->getForegroundRegions()->size(); ++i) {
		rect = &(_rectCache->getForegroundRegions()->at(i));

		if (rect->contains(x, y)) return true;
	}

	return false;
}

// Check for collisions with another rectangle
bool Gadget::checkCollision(s16 x, s16 y, u16 width, u16 height) const {

	if (isHidden()) return false;

	// Get the clipped rect
	Rect rect;
	getRectClippedToHierarchy(rect);

	return rect.intersects(Rect(x, y, width, height));
}

// Check for collisions with another gadget
bool Gadget::checkCollision(Gadget* gadget) const {

	// Get the clipped rect
	Rect rect;
	gadget->getRectClippedToHierarchy(rect);

	return rect.intersects(_rect);
}

void Gadget::drawChildren() {
	for (s32 i = 0; i < _gadgets.size(); i++) {
		_gadgets[i]->redraw();
	}
}

void Gadget::redraw() {
	if (isDrawingEnabled()) {
		cacheVisibleRects();

		if (_rectCache->getBackgroundRegions()->size() > 0) {

			// Create internal and standard graphics ports
			GraphicsPort* internalPort = newInternalGraphicsPort(_rectCache->getBackgroundRegions()->at(0));
			GraphicsPort* port = newGraphicsPort(_rectCache->getBackgroundRegions()->at(0));

			// Draw all visible rectangles
			for (s32 i = 0; i < _rectCache->getBackgroundRegions()->size(); i++) {

				internalPort->setClipRect(_rectCache->getBackgroundRegions()->at(i));
				port->setClipRect(_rectCache->getBackgroundRegions()->at(i));

				drawBorder(internalPort);
				drawContents(port);
			}

			delete internalPort;
			delete port;
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
		if (rectsToDraw->size() > 0) {

			GraphicsPort* internalPort = newInternalGraphicsPort(rectsToDraw->at(0));
			GraphicsPort* port = newGraphicsPort(rectsToDraw->at(0));

			for (s32 i = 0; i < rectsToDraw->size(); i++) {

				internalPort->setClipRect(rectsToDraw->at(i));
				port->setClipRect(rectsToDraw->at(i));

				drawBorder(internalPort);
				drawContents(port);
			}

			delete internalPort;
			delete port;
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
		
		// Unset clicked gadget if necessary
		Gadget* clickedGadget = woopsiApplication->getClickedGadget();
		if (clickedGadget == this) {
			release(clickedGadget->getX(), clickedGadget->getY());
		}

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
		_gadgetEventHandlers->disable();

		_flags.shelved = true;
		_flags.drawingEnabled = false;

		// Unset clicked gadget if necessary
		Gadget* clickedGadget = woopsiApplication->getClickedGadget();
		if (clickedGadget == this) {
			release(clickedGadget->getX(), clickedGadget->getY());
		}

		// Ensure the gadget isn't running modally
		stopModal();

		erase();

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

		_gadgetEventHandlers->enable();
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
	if (gadget == NULL) return;

	// Decrease decoration count if necessary
	if (gadget->isDecoration()) {
		_decorationCount--;
	}

	// Ensure gadget knows it is being shelved
	gadget->shelve();

	// Do we need to give another gadget focus?
	if (_focusedGadget == gadget) {

		_focusedGadget = NULL;

		// Try to choose highest gadget
		for (s32 i = _gadgets.size() - 1; i > -1; i--) {
			if ((_gadgets[i] != gadget) && (!_gadgets[i]->isHidden())) {
				_focusedGadget = _gadgets[i];
			}
		}

		// Where should the focus go?
		if (_focusedGadget != NULL) {
			// Send focus to the new active gadget
			_focusedGadget->focus();
		} else {
			// Give focus to this
			setFocusedGadget(NULL);
		}
	}

	if (woopsiApplication != NULL) {

		// Close the context menu if we're closing the gadget that opened it
		if (woopsiApplication->getContextMenu()->getOpener() == gadget) {
			woopsiApplication->shelveContextMenu();
		}
	}

	moveChildToShelvedList(gadget);
}

// Close a child
void Gadget::closeChild(Gadget* gadget) {
	if (gadget == NULL) return;
	
	// Decrease decoration count if necessary
	if (gadget->isDecoration()) {
		_decorationCount--;
	}
		
	// Ensure gadget knows it is being closed
	gadget->close();

	// Do we need to make another gadget active?
	if (_focusedGadget == gadget) {

		_focusedGadget = NULL;

		// Try to choose highest gadget
		for (s32 i = _gadgets.size() - 1; i > -1; i--) {
			if ((_gadgets[i] != gadget) && (!_gadgets[i]->isHidden())) {
				_focusedGadget = _gadgets[i];
			}
		}

		// Where should the focus go?
		if (_focusedGadget != NULL) {
			// Send focus to the new active gadget
			_focusedGadget->focus();
		} else {
			// Give focus to this
			setFocusedGadget(NULL);
		}
	}

	if (woopsiApplication != NULL) {

		// Close the context menu if we're closing the gadget that opened it
		if (woopsiApplication->getContextMenu()->getOpener() == gadget) {
			woopsiApplication->shelveContextMenu();
		}
	}

	moveChildToDeleteQueue(gadget);
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
		
		onEnable();

		redraw();

		_gadgetEventHandlers->raiseEnableEvent();

		return true;
	}

	return false;
}

bool Gadget::disable() {
	if (_flags.enabled) {
		_flags.enabled = false;
		
		onDisable();

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
				if (x + getWidth() > parentRect.x + parentRect.width) {
					return false;
				}
			} else if (x + getWidth() > parentRect.x + parentRect.width) {
				x = (parentRect.x + parentRect.x) - getWidth();

				// Check new x value
				if (x < parentRect.x) {
					return false;
				}
			}

			// Check y co-ordinate
			if (y < parentRect.y) {
				y = parentRect.y;

				// Check height against new value
				if (y + getHeight() > parentRect.y + parentRect.height) {
					return false;
				}
			} else if (y + getHeight() > parentRect.y + parentRect.height) {
				y = (parentRect.y + parentRect.y) - getHeight();

				// Check new y value
				if (y < parentRect.y) {
					return false;
				}
			}
		}
	}
			
	// Perform move if necessary
	if ((_rect.getX() != x) || (_rect.getY() != y)) {
		erase();

		s16 oldX = _rect.getX();
		s16 oldY = _rect.getY();

		_rect.setX(x);
		_rect.setY(y);

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
			if (_rect.getX() + width > parentRect.x + parentRect.width) {
				width = parentRect.x + parentRect.width - _rect.getX();
			}

			// Check height
			if (_rect.getY() + height > parentRect.y + parentRect.height) {
				height = parentRect.y + parentRect.height - _rect.getY();
			}
		}
	}

	if ((getWidth() != width) || (getHeight() != height)) {
	
		// Remember if the gadget is permeable
		bool wasPermeable = _flags.permeable;

		// Remember if gadget was drawing
		bool wasDrawEnabled = _flags.drawingEnabled;

		_flags.permeable = true;
	
		erase();
		
		disableDrawing();

		_rect.setWidth(width);
		_rect.setHeight(height);

		// Handle visible region caching
		if (_parent != NULL) {
			_parent->invalidateLowerGadgetsVisibleRectCache(this);
		}

		onResize(width, height);
		
		// Reset the permeable value
		_flags.permeable = wasPermeable;

		// Reset drawing value
		_flags.drawingEnabled = wasDrawEnabled;

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
	if (_flags.doubleClickable && hasFocus() && (woopsiApplication != NULL) && Stylus.DblClick) {

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

	if (!isEnabled()) return false;
	if (!checkCollision(x, y)) return false;

	// Check for a double-click
	if (isDoubleClick(x, y)) return doubleClick(x, y);

	// Work out which child was clicked
	for (s32 i = _gadgets.size() - 1; i > -1; i--) {
		if (_gadgets[i]->click(x, y)) {
			return true;
		}
	}

	// Ensure that the click has occurred on a region of this gadget
	// not obscured by its siblings
	if (!checkCollisionWithForegroundRects(x, y)) return false;

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

	// Run any code in the inherited class
	onClick(x, y);

	_gadgetEventHandlers->raiseClickEvent(x, y);

	return true;
}

bool Gadget::doubleClick(s16 x, s16 y) {

	if (!isEnabled()) return false;
	if (!checkCollision(x, y)) return false;

	// Work out which child was clicked.  Allow the
	// child to determine if it has been double-clicked or not
	// in case the second click has fallen on a different
	// child to the first.
	for (s32 i = _gadgets.size() - 1; i > -1; i--) {
		if (_gadgets[i]->click(x, y)) {
			return true;
		}
	}

	// Ensure that the click has occurred on a region of this gadget
	// not obscured by its siblings
	if (!checkCollisionWithForegroundRects(x, y)) return false;

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

	onDoubleClick(x, y);

	_gadgetEventHandlers->raiseDoubleClickEvent(x, y);

	return true;
}

bool Gadget::shiftClick(s16 x, s16 y) {

	if (!isEnabled()) return false;
	if (!checkCollision(x, y)) return false;

	// Work out which child was clicked
	for (s32 i = _gadgets.size() - 1; i > -1; i--) {
		if (_gadgets[i]->shiftClick(x, y)) {
			return true;
		}
	}

	// Do not handle shift clicks if this gadget does not define a
	// context menu
	if (_contextMenuItems.size() == 0) return false;

	// Ensure that the click has occurred on a region of this gadget
	// not obscured by its siblings
	if (!checkCollisionWithForegroundRects(x, y)) return false;

	// Take focus away from child gadgets
	setFocusedGadget(NULL);

	// Set up the context menu
	showContextMenu(x, y);

	onShiftClick(x, y);

	_gadgetEventHandlers->raiseShiftClickEvent(x, y);

	return true;
}

bool Gadget::release(s16 x, s16 y) {

	if (!_flags.clicked) return false;

	_flags.clicked = false;

	stopDragging(x, y);

	if (woopsiApplication->getClickedGadget() == this) {
		woopsiApplication->setClickedGadget(NULL);
	}

	// Determine which release event to fire
	if (checkCollision(x, y)) {

		onRelease(x, y);

		// Release occurred within gadget; raise release
		_gadgetEventHandlers->raiseReleaseEvent(x, y);
	} else {

		onReleaseOutside(x, y);

		// Release occurred outside gadget; raise release
		_gadgetEventHandlers->raiseReleaseOutsideEvent(x, y);
	}

	return true;
}

bool Gadget::drag(s16 x, s16 y, s16 vX, s16 vY) {

#ifdef USING_SDL
	// Abort dragging if not dragging the bottom screen; will only be an issue in SDL code
	if (calculatePhysicalScreenNumber(y) != 0) return false;
#endif

	if ((isEnabled()) && (_flags.dragging)) {
		if ((vX != 0) || (vY != 0)) {

			onDrag(x, y, vX, vY);

			_gadgetEventHandlers->raiseDragEvent(x, y, vX, vY);
		}

		return true;
	}

	return false;
}

bool Gadget::keyPress(KeyCode keyCode) {
	if (!isEnabled()) return false;
		
	onKeyPress(keyCode);
	
	// Raise keypress for this gadget
	_gadgetEventHandlers->raiseKeyPressEvent(keyCode);

	// Handle active child
	if (_focusedGadget != NULL) {
		_focusedGadget->keyPress(keyCode);
	}

	return true;
}

bool Gadget::keyRepeat(KeyCode keyCode) {
	if (!isEnabled()) return false;
		
	onKeyRepeat(keyCode);
	
	// Raise key repeat for this gadget
	_gadgetEventHandlers->raiseKeyRepeatEvent(keyCode);

	// Handle active child
	if (_focusedGadget != NULL) {
		_focusedGadget->keyRepeat(keyCode);
	}

	return true;
}

bool Gadget::keyRelease(KeyCode keyCode) {
	if (!isEnabled()) return false;
		
	onKeyRelease(keyCode);

	// Raise key release for this gadget
	_gadgetEventHandlers->raiseKeyReleaseEvent(keyCode);

	// Handle active child
	if (_focusedGadget != NULL) {
		_focusedGadget->keyRelease(keyCode);
	}

	return true;
}

void Gadget::lidClose() {
	
	onLidClose();
	
	_gadgetEventHandlers->raiseLidCloseEvent();

	// Run lid closed on all gadgets
	for (s32 i = 0; i < _gadgets.size(); i++) {
		_gadgets[i]->lidClose();
	}
}

void Gadget::lidOpen() {
	
	onLidOpen();
	
	_gadgetEventHandlers->raiseLidOpenEvent();

	// Run lid opened on all gadgets
	for (s32 i = 0; i < _gadgets.size(); i++) {
		_gadgets[i]->lidOpen();
	}
}

bool Gadget::focus() {
	if (!canReceiveFocus()) return false;

	// Remember if the gadget has focus
	bool hadFocus = _flags.hasFocus;

	_flags.hasFocus = true;

	// Notify parent that this gadget has focus
	if (_parent != NULL) {
		_parent->setFocusedGadget(this);
	}

	// Raise an event only if the gadget did not have focus
	if (!hadFocus) {
		onFocus();
		
		_gadgetEventHandlers->raiseFocusEvent();
		return true;
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
		onBlur();
		
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
	rect.x = _rect.getX();
	rect.y = _rect.getY();
	rect.width = _rect.getWidth();
	rect.height = _rect.getHeight();
}

// Insert the available space for child gadgets into the rect
void Gadget::getClientRect(Rect& rect) const {
	if (_flags.borderless) {
		rect.x = 0;
		rect.y = 0;
		rect.width = getWidth();
		rect.height = getHeight();
	} else {
		rect.x = _borderSize.left;
		rect.y = _borderSize.top;
		rect.width = getWidth() - (_borderSize.left + _borderSize.right);
		rect.height = getHeight() - (_borderSize.top + _borderSize.bottom);
	}
}

void Gadget::startDragging(s16 x, s16 y) {
	if (_flags.draggable) {
		_flags.dragging = true;
		_flags.clicked = true;
		_grabPointX = x - getX();
		_grabPointY = y - getY();
		_newX = _rect.getX();
		_newY = _rect.getY();

		onDragStart();
	}
}

void Gadget::stopDragging(s16 x, s16 y) {
	if (_flags.dragging) {
		onDragStop();
		_flags.dragging = false;

		_gadgetEventHandlers->raiseDropEvent(x, y);
	}
}

void Gadget::cacheVisibleRects() const {
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

	return new GraphicsPort(rect.x + getX(), rect.y + getY(), rect.width, rect.height, isDrawingEnabled(), bitmap, clipList, NULL);
}

// Return the client graphics port for a specific clipping rect
GraphicsPort* Gadget::newGraphicsPort(Rect clipRect) {

	Rect rect;
	getClientRect(rect);

	FrameBuffer* bitmap = frameBuffer[getPhysicalScreenNumber()];

	// Ensure visible region cache is up to date
	cacheVisibleRects();

	return new GraphicsPort(rect.x + getX(), rect.y + getY(), rect.width, rect.height, isDrawingEnabled(), bitmap, NULL, &clipRect);
}

// Return the internal graphics port for a specific clipping rect
GraphicsPort* Gadget::newInternalGraphicsPort(Rect clipRect) {

	// Ensure visible region cache is up to date
	cacheVisibleRects();

	FrameBuffer* bitmap = frameBuffer[getPhysicalScreenNumber()];

	return new GraphicsPort(getX(), getY(), getWidth(), getHeight(), isDrawingEnabled(), bitmap, NULL, &clipRect);
}

// Return vector of visible rects, including any covered by children
WoopsiArray<Rect>* Gadget::getForegroundRegions() {
	return _rectCache->getForegroundRegions();
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

		rect.clipToIntersect(thisRect);

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
	return _style.font;
}

void Gadget::setFont(FontBase* font) {
	_style.font = font;
}

FontBase* Gadget::getGlyphFont() const {
	return _style.glyphFont;
}

void Gadget::setGlyphFont(FontBase* font) {
	_style.glyphFont = font;
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

void Gadget::addContextMenuItem(const WoopsiString& name, u32 value) {

	NameValuePair newItem;
	newItem.name = name;
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

void Gadget::setBorderSize(const GadgetBorderSize& borderSize) {
	_borderSize.top = borderSize.top;
	_borderSize.right = borderSize.right;
	_borderSize.bottom = borderSize.bottom;
	_borderSize.left = borderSize.left;
}

const u8 Gadget::getPhysicalScreenNumber() const {
	if (_parent != NULL) {
		return _parent->getPhysicalScreenNumber();
	} else{
		return calculatePhysicalScreenNumber(getY());
	}
}
