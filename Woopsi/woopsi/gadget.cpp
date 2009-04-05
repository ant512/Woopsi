#include "gadget.h"
#include "graphicsport.h"
#include "woopsi.h"
#include "woopsifuncs.h"
#include "gadgeteventhandler.h"
#include "fontbase.h"
#include "contextmenu.h"
#include "defaultstyle.h"
#include "gadgeteventargs.h"
#include "contextmenu.h"
#include "contextmenuitem.h"
#include "contextmenueventargs.h"

using namespace WoopsiUI;

Gadget::Gadget(s16 x, s16 y, u16 width, u16 height, u32 flags, FontBase* font) {

	// Set properties from parameters
	_x = x;
	_y = y;
	_width = width;
	_height = height;
	_font = font;

	// Do we need to fetch the system font?
	if (_font == NULL) {
		_font = Woopsi::getSystemFont();
	}

	// Mask flags against bitmasks and logical NOT twice to obtain boolean values
	_flags.borderless = (!(!(flags & GADGET_BORDERLESS)));
	_flags.draggable = (!(!(flags & GADGET_DRAGGABLE)));
	_flags.permeable = (!(!(flags & GADGET_PERMEABLE)));
	_flags.doubleClickable = (!(!(flags & GADGET_DOUBLE_CLICKABLE)));
	_flags.decoration = (!(!(flags & GADGET_DECORATION)));

	// Following flags are set to true if not passed in flags parameter
	_flags.shiftClickChildren = (!(flags & GADGET_NO_SHIFT_CLICK_CHILDREN));
	_flags.raisesEvents = (!(flags & GADGET_NO_RAISE_EVENTS));

	// Dragging values
	_grabPointX = 0;
	_grabPointY = 0;
	_newX = 0;
	_newY = 0;

	// Set default colours
	_backColour = DefaultStyle::backColour;
	_shineColour = DefaultStyle::shineColour;
	_highlightColour = DefaultStyle::highlightColour;
	_shadowColour = DefaultStyle::shadowColour;
	_fillColour = DefaultStyle::fillColour;
	_darkColour = DefaultStyle::darkColour;

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
	_doubleClickTime = DOUBLE_CLICK_TIME;

	// Set other default values
	_decorationCount = 0;
	_refcon = 0;
	
	_outline = OUTLINE_CLICK_DEPENDENT;
	_closeType = CLOSE_TYPE_CLOSE;
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
	port->drawFilledRect(0, 0, _width, _height, _backColour);
	delete port;
}

void Gadget::clear() {
	GraphicsPort* port = newInternalGraphicsPort();
	port->drawFilledRect(0, 0, _width, _height, _backColour);
	delete port;
}

// Check for single-point collisions
bool Gadget::checkCollision(s16 x, s16 y) {

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
bool Gadget::checkCollision(s16 x, s16 y, u16 width, u16 height) {

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
bool Gadget::checkCollision(Gadget* gadget) {

	// Get the clipped rect
	Rect rect;
	gadget->getRectClippedToHierarchy(rect);

	return checkCollision(rect.x, rect.y, rect.width, rect.height);
}

void Gadget::raiseClickEvent(s16 x, s16 y) {
	if (raisesEvents()) {
		GadgetEventArgs e(this, x, y, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleClickEvent(e);
		}
	}
}

void Gadget::raiseDoubleClickEvent(s16 x, s16 y) {
	if (raisesEvents()) {
		GadgetEventArgs e(this, x, y, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleDoubleClickEvent(e);
		}
	}
}

void Gadget::raiseShiftClickEvent(s16 x, s16 y) {
	if (raisesEvents()) {
		GadgetEventArgs e(this, x, y, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleShiftClickEvent(e);
		}
	}
}

void Gadget::raiseReleaseEvent(s16 x, s16 y) {
	if (raisesEvents()) {
		GadgetEventArgs e(this, x, y, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleReleaseEvent(e);
		}
	}
}

void Gadget::raiseReleaseOutsideEvent(s16 x, s16 y) {
	if (raisesEvents()) {
		GadgetEventArgs e(this, x, y, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleReleaseOutsideEvent(e);
		}
	}
}

void Gadget::raiseDragEvent(s16 x, s16 y, s16 vX, s16 vY) {
	if (raisesEvents()) {
		GadgetEventArgs e(this, x, y, vX, vY, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleDragEvent(e);
		}
	}
}

void Gadget::raiseMoveForwardEvent() {
	if (raisesEvents()) {
		GadgetEventArgs e(this, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleMoveForwardEvent(e);
		}
	}
}

void Gadget::raiseMoveBackwardEvent() {
	if (raisesEvents()) {
		GadgetEventArgs e(this, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleMoveBackwardEvent(e);
		}
	}
}

void Gadget::raiseKeyPressEvent(KeyCode keyCode) {
	if (raisesEvents()) {
		GadgetEventArgs e(this, 0, 0, 0, 0, keyCode);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleKeyPressEvent(e);
		}
	}
}

void Gadget::raiseKeyReleaseEvent(KeyCode keyCode) {
	if (raisesEvents()) {
		GadgetEventArgs e(this, 0, 0, 0, 0, keyCode);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleKeyReleaseEvent(e);
		}
	}
}

void Gadget::raiseLidCloseEvent() {
	if (raisesEvents()) {
		GadgetEventArgs e(this, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleLidCloseEvent(e);
		}
	}
}

void Gadget::raiseLidOpenEvent() {
	if (raisesEvents()) {
		GadgetEventArgs e(this, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleLidOpenEvent(e);
		}
	}
}

void Gadget::raiseFocusEvent() {
	if (raisesEvents()) {
		GadgetEventArgs e(this, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleFocusEvent(e);
		}
	}
}

void Gadget::raiseBlurEvent() {
	if (raisesEvents()) {
		GadgetEventArgs e(this, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleBlurEvent(e);
		}
	}
}

void Gadget::raiseCloseEvent() {
	if (raisesEvents()) {
		GadgetEventArgs e(this, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleCloseEvent(e);
		}
	}
}

void Gadget::raiseHideEvent() {
	if (raisesEvents()) {
		GadgetEventArgs e(this, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleHideEvent(e);
		}
	}
}

void Gadget::raiseShowEvent() {
	if (raisesEvents()) {
		GadgetEventArgs e(this, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleShowEvent(e);
		}
	}
}

void Gadget::raiseShelveEvent() {
	if (raisesEvents()) {
		GadgetEventArgs e(this, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleShelveEvent(e);
		}
	}
}

void Gadget::raiseUnshelveEvent() {
	if (raisesEvents()) {
		GadgetEventArgs e(this, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleUnshelveEvent(e);
		}
	}
}

void Gadget::raiseEnableEvent() {
	if (raisesEvents()) {
		GadgetEventArgs e(this, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleEnableEvent(e);
		}
	}
}

void Gadget::raiseDisableEvent() {
	if (raisesEvents()) {
		GadgetEventArgs e(this, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleDisableEvent(e);
		}
	}
}

void Gadget::raiseValueChangeEvent() {
	if (raisesEvents()) {
		GadgetEventArgs e(this, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleValueChangeEvent(e);
		}
	}
}

void Gadget::raiseResizeEvent(u16 width, u16 height) {
	if (raisesEvents()) {
		GadgetEventArgs e(this, 0, 0, 0, 0, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleResizeEvent(e);
		}
	}
}

void Gadget::raiseMoveEvent(s16 x, s16 y, s16 vX, s16 vY) {
	if (raisesEvents()) {
		GadgetEventArgs e(this, x, y, vX, vY, KEY_CODE_NONE);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleMoveEvent(e);
		}
	}
}

void Gadget::raiseActionEvent(s16 x, s16 y, s16 vX, s16 vY, KeyCode keyCode) {
	if (raisesEvents()) {
		GadgetEventArgs e(this, x, y, vX, vY, keyCode);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleActionEvent(e);
		}
	}
}

void Gadget::raiseContextMenuSelectionEvent(const ContextMenu* contextMenu, const ContextMenuItem* contextMenuItem) {
	if (raisesEvents()) {
		ContextMenuEventArgs e(contextMenu, contextMenuItem);

		for (int i = 0; i < _gadgetEventHandlers.size(); ++i) {
			_gadgetEventHandlers.at(i)->handleContextMenuSelectionEvent(e);
		}
	}
}

void Gadget::drawChildren() {
	for (s32 i = 0; i < _gadgets.size(); i++) {
		_gadgets[i]->draw();
	}
}

void Gadget::draw() {
	if (isDrawingEnabled()) {
		cacheVisibleRects();

		// Create pointer to a vector to store the overlapped rectangles
		// We can discard this later as we don't need it
		WoopsiArray<Rect>* invisibleRects = new WoopsiArray<Rect>();

		// Create a pointer to store rects that are not overlapped by
		// children
		WoopsiArray<Rect>* visibleRects = new WoopsiArray<Rect>();

		// Copy all visible regions into the new vector
		for (s32 i = 0; i < _visibleRegionCache.size(); i++) {
			visibleRects->push_back(_visibleRegionCache[i]);
		}

		// Remove all child rects from the visible vector
		for (s32 i = 0; i < _gadgets.size(); i++) {
			if (visibleRects->size() > 0) {
				_gadgets[i]->splitRectangles(visibleRects, invisibleRects, this);
			} else {
				break;
			}
		}

		// Tidy up
		delete invisibleRects;

		// Draw all visible rectangles
		for (s32 i = 0; i < visibleRects->size(); i++) {
			draw(visibleRects->at(i));
		}

		// Tidy up
		delete visibleRects;

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
		splitRectangles(invalidRects, overlappingRects, sender);

		// Create an array that will contain all of the rects that overlap this
		// gadget clipped to its parent
		WoopsiArray<Rect>* rectsToDraw = new WoopsiArray<Rect>();
		
		// Split from overlappingRects into rectsToDraw, giving us an array
		// of rects that overlap only the visible portions of this gadget
		splitRectangles(overlappingRects, rectsToDraw, sender);
		
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


// Remove any rectangles that this gadget overlaps from the visible vector
// and add them to the invisible vector
// Called when drawing a gadget to check that no higher gadgets get overwritten
void Gadget::removeOverlappedRects(WoopsiArray<Rect>* visibleRects, WoopsiArray<Rect>* invisibleRects, Gadget* gadget) {

	// Locate gadget in the list; we add one to the index to
	// ensure that we deal with the next gadget up in the z-order
	s32 gadgetIndex = getGadgetIndex(gadget) + 1;

	// Gadget should never be the bottom item on the screen
	if (gadgetIndex > 0) {

		// Remove any overlapped rectangles
		for (s32 i = gadgetIndex; i < _gadgets.size(); i++) {
			if (visibleRects->size() > 0) {
				_gadgets[i]->splitRectangles(visibleRects, invisibleRects, gadget);
			} else {
				break;
			}
		}
	}

	// Send vectors to parent
	if (_parent != NULL) {
		if (visibleRects->size() > 0) {
			_parent->removeOverlappedRects(visibleRects, invisibleRects, this);
		}
	}
}

// Split rectangles into valid and invalid sub-rectangles
// Used when calculating which portions of a gadget to draw
void Gadget::splitRectangles(WoopsiArray<Rect>* invalidRects, WoopsiArray<Rect>* validRects, Gadget* sender) {

	// Check for collisions with any rectangles in the vector
	for (s32 i = 0; i < invalidRects->size(); i++) {

		// Get rectangle to check
		Rect checkRect = invalidRects->at(i);
		s16 splitX[4];
		s16 splitY[4];
		u32 rectXCount = 0;
		u32 rectYCount = 0;
		u32 overlapXRect = 0;
		u32 overlapYRect = 0;

		if (checkCollision(checkRect.x, checkRect.y, checkRect.width, checkRect.height)) {
			// Got a collision.  We need to split this rectangle

			// Get clipped dimensions of gadget
			Rect gadgetRect;
			getRectClippedToHierarchy(gadgetRect);

			// Vertical split

			// Start at left edge of rectangle
			splitX[0] = checkRect.x;

			// Check for second split
			if (checkRect.x < gadgetRect.x) {
				// Gadget is to the right of the invalid rectangle (or in the centre)
				if (splitX[rectXCount] != gadgetRect.x) {
					rectXCount++;
					splitX[rectXCount] = gadgetRect.x;

					// The next rectangle is the overlap
					overlapXRect = rectXCount;
				}
			} else {
				// Gadget rectangle is on the left of the invalid rectangle
				if (splitX[rectXCount] != gadgetRect.x + gadgetRect.width) {

					// We've found the start of the overlapping rectangle!
					overlapXRect = rectXCount;
					rectXCount++;

					// Split is either the end of the gadget or the end of the
					// invalid rect, whichever comes first
					if (gadgetRect.x + gadgetRect.width <= checkRect.x + checkRect.width) {
						splitX[rectXCount] = gadgetRect.x + gadgetRect.width;
					} else {
						splitX[rectXCount] = checkRect.x + checkRect.width;
					}
					
				} else {
					// Found the start of the overlapping rectangle
					overlapXRect = rectXCount;
				}
			}

			// Check for third split
			if (gadgetRect.x + gadgetRect.width <= checkRect.x + checkRect.width) {
				// Gadget ends before the invalid rectangle
				if (splitX[rectXCount] != gadgetRect.x + gadgetRect.width) {

					// Record end of overlap
					rectXCount++;
					splitX[rectXCount] = gadgetRect.x + gadgetRect.width;
				}
			}

			// Store end of invalid rectangle
			if (splitX[rectXCount] < checkRect.x + checkRect.width) {
				rectXCount++;
				splitX[rectXCount] = checkRect.x + checkRect.width;
			}


			// Horizontal split

			// Start at left edge of rectangle
			splitY[0] = checkRect.y;

			// Check for second split
			if (checkRect.y < gadgetRect.y) {
				// Gadget below the invalid rectangle (or in the centre)
				if (splitY[rectYCount] != gadgetRect.y) {
					rectYCount++;
					splitY[rectYCount] = gadgetRect.y;

					// The next rectangle is the overlap
					overlapYRect = rectYCount;
				}
			} else {
				// Gadget rectangle above the invalid rectangle
				if (splitY[rectYCount] != gadgetRect.y + gadgetRect.height) {

					// We've found the start of the overlapping rectangle!
					overlapYRect = rectYCount;
					rectYCount++;

					// Split is either the end of the gadget or the end of the
					// invalid rect, whichever comes first
					if (gadgetRect.y + gadgetRect.height <= checkRect.y + checkRect.height) {
						splitY[rectYCount] = gadgetRect.y + gadgetRect.height;
					} else {
						splitY[rectYCount] = checkRect.y + checkRect.height;
					}
					
				} else {
					// Found the start of the overlapping rectangle
					overlapYRect = rectYCount;
				}
			}

			// Check for third split
			if (gadgetRect.y + gadgetRect.height < checkRect.y + checkRect.height) {
				// Gadget ends before the invalid rectangle
				if (splitY[rectYCount] != gadgetRect.y + gadgetRect.height) {

					// Record end of overlap
					rectYCount++;
					splitY[rectYCount] = gadgetRect.y + gadgetRect.height;
				}
			}

			// Store end of invalid rectangle
			if (splitY[rectYCount] < checkRect.y + checkRect.height) {
				rectYCount++;
				splitY[rectYCount] = checkRect.y + checkRect.height;
			}

			// Remove the original rectangle
			invalidRects->erase(i);

			// Force the loop to re-examine the new rectangle at this index
			i--;

			// Add the new rectangles (not the overlap; that's the one we need to draw)
			for (u32 xRects = 0; xRects < rectXCount; xRects++) {
				for (u32 yRects = 0; yRects < rectYCount; yRects++) {

					// Is this the overlap?
					if ((overlapXRect == xRects) && (overlapYRect == yRects)) {

						// Got the overlap, so set the output values
						Rect overlapRect;
						overlapRect.x = splitX[xRects];
						overlapRect.y = splitY[yRects];
						overlapRect.width = splitX[xRects + 1] - splitX[xRects];
						overlapRect.height = splitY[yRects + 1] - splitY[yRects];

						// Clip the rectangle to the display
						if ((overlapRect.x < SCREEN_WIDTH) && ((overlapRect.y < SCREEN_HEIGHT) || ((overlapRect.y >= TOP_SCREEN_Y_OFFSET) && (overlapRect.y < TOP_SCREEN_Y_OFFSET + SCREEN_HEIGHT)))) {

							// Height
							if ((overlapRect.y + overlapRect.height > SCREEN_HEIGHT) && (overlapRect.y + overlapRect.height < TOP_SCREEN_Y_OFFSET)) {
								overlapRect.height = SCREEN_HEIGHT - overlapRect.y;
							} else if (overlapRect.y + overlapRect.height > TOP_SCREEN_Y_OFFSET + SCREEN_HEIGHT) {
								overlapRect.height = TOP_SCREEN_Y_OFFSET + SCREEN_HEIGHT - overlapRect.y;
							}

							// Width
							if (overlapRect.x + overlapRect.width > SCREEN_WIDTH) {
								overlapRect.width = SCREEN_WIDTH - overlapRect.x;
							}

							if ((overlapRect.height > 0) && (overlapRect.width > 0)) {
								validRects->push_back(overlapRect);
							}
						}
					} else {
						// Not an overlap; add to vector
						Rect newRect;
						newRect.x = splitX[xRects];
						newRect.y = splitY[yRects];
						newRect.width = splitX[xRects + 1] - splitX[xRects];
						newRect.height = splitY[yRects + 1] - splitY[yRects];

						// Clip the rectangle to the display
						if ((newRect.x < SCREEN_WIDTH) && ((newRect.y < SCREEN_HEIGHT) || ((newRect.y >= TOP_SCREEN_Y_OFFSET) && (newRect.y < TOP_SCREEN_Y_OFFSET + SCREEN_HEIGHT)))) {

							// Height
							if ((newRect.y + newRect.height > SCREEN_HEIGHT) && ( newRect.y + newRect.height < TOP_SCREEN_Y_OFFSET)) {
								newRect.height = SCREEN_HEIGHT - newRect.y;
							} else if (newRect.y + newRect.height > TOP_SCREEN_Y_OFFSET + SCREEN_HEIGHT) {
								newRect.height = TOP_SCREEN_Y_OFFSET + SCREEN_HEIGHT - newRect.y;
							}

							// Width
							if (newRect.x + newRect.width > SCREEN_WIDTH) {
								newRect.width = SCREEN_WIDTH - newRect.x;
							}

							// Insert the new rectangle at the start so we don't
							// test it again
							if ((newRect.height > 0) && (newRect.width > 0)) {
								invalidRects->insert(0, newRect);
							}

							// Increase iterator to compensate for insertion
							i++;
						}
					}
				}
			}
		}
	}
}

// Erase this gadget from the screen
void Gadget::erase() {

	if (!_flags.erased) {
		invalidateVisibleRectCache();
		cacheVisibleRects();

		if (_parent != NULL) {
			_parent->eraseGadget(this);
		}

		// Remember that the gadget has been erased
		_flags.erased = true;
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
		_gadgets[i]->redrawDirty(gadget->getVisibleRectCache(), gadget);
	}

	// Order this gadget to redraw itself based on any remaining rectangles
	// in the erased gadget's rect cache
	redrawDirty(gadget->getVisibleRectCache(), gadget);

	invalidateVisibleRectCache();
}

// Marks the gadget as deleted and adds it to the deletion queue
void Gadget::close() {

	if (!_flags.deleted) {
		raiseCloseEvent();

		_flags.deleted = true;
		_flags.drawingEnabled = false;
		_flags.raisesEvents = false;

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

		raiseShelveEvent();

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

		raiseUnshelveEvent();

		_flags.drawingEnabled = true;
		_flags.shelved = false;

		if (_parent != NULL) {
			_parent->moveShelvedToChildList(this);
			_parent->invalidateVisibleRectCache();	
		}

		draw();

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
		gadget->draw();

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

		raiseEnableEvent();

		return true;
	}

	return false;
}

bool Gadget::disable() {
	if (_flags.enabled) {
		_flags.enabled = false;

		raiseDisableEvent();

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

		draw();

		raiseMoveEvent(x, y, x - oldX, y - oldY);

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

		draw();

		raiseResizeEvent(width, height);

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

bool Gadget::click(s16 x, s16 y) {

	// Check for a double-click
	if (_flags.doubleClickable) {

		// Within the allowed time?
		if (woopsiApplication != NULL) {
			if (Stylus.DblClick) {

				// Within the allowed region?
				if ((_lastClickX > x - _doubleClickBounds) && (_lastClickX < x + _doubleClickBounds)) {
					if ((_lastClickY > y - _doubleClickBounds) && (_lastClickY < y + _doubleClickBounds)) {

						// Process click as a double-click
						return doubleClick(x, y);
					}
				}
			}
		}
	}

	if (isEnabled()) {
		if (checkCollision(x, y)) {

			// Work out which child was clicked
			for (s32 i = _gadgets.size() - 1; i > -1; i--) {
				if (_gadgets[i]->click(x, y)) {
					return true;
				}
			}

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

			raiseClickEvent(x, y);

			return true;
		}
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

			raiseDoubleClickEvent(x, y);

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

			raiseShiftClickEvent(x, y);

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
			raiseReleaseEvent(x, y);
		} else {
			// Release occurred outside gadget; raise release
			raiseReleaseOutsideEvent(x, y);
		}

		return true;
	}

	return false;
}

bool Gadget::drag(s16 x, s16 y, s16 vX, s16 vY) {
	if ((isEnabled()) && (_flags.dragging)) {
		raiseDragEvent(x, y, vX, vY);

		return true;
	}

	return false;
}

bool Gadget::keyPress(KeyCode keyCode) {
	if (isEnabled()) {
		
		// Raise keypress for this gadget
		raiseKeyPressEvent(keyCode);

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
		raiseKeyReleaseEvent(keyCode);

		// Handle active child
		if (_focusedGadget != NULL) {
			_focusedGadget->keyRelease(keyCode);
		}

		return true;
	}

	return false;
}

void Gadget::lidClose() {
	raiseLidCloseEvent();

	// Run lid closed on all gadgets
	for (s32 i = 0; i < _gadgets.size(); i++) {
		_gadgets[i]->lidClose();
	}
}

void Gadget::lidOpen() {
	raiseLidOpenEvent();

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
			raiseFocusEvent();
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
		raiseBlurEvent();
		return true;
	}

	return false;
}

bool Gadget::raiseToTop() {
	if (_parent != NULL) {
		if (_parent->raiseGadgetToTop(this)) {
			raiseMoveForwardEvent();
			return true;
		}
	}

	return false;
}

bool Gadget::lowerToBottom() {
	if (_parent != NULL) {
		if (_parent->lowerGadgetToBottom(this)) {
			raiseMoveBackwardEvent();
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

		gadget->draw();

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

		gadget->draw();

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

		gadget->draw();
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

		gadget->draw();
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

	if (_flags.visibleRegionCacheInvalid) {
		// Use internal region cache to store the non-overlapped rectangles
		// We will use this to clip the gadget
		_visibleRegionCache.clear();

		// Create pointer to a vector to store the overlapped rectangles
		// We can discard this later as we don't need it
		WoopsiArray<Rect>* invisibleRects = new WoopsiArray<Rect>();

		// Copy the clipped gadget dimensions into a rect
		Rect rect;
		getRectClippedToHierarchy(rect);

		// Do we have a visible region left?
		if ((rect.height > 0) && (rect.width > 0)) {

			// Add rect to list
			_visibleRegionCache.push_back(rect);
			
			// Request refresh
			if (_parent != NULL) {
				_parent->removeOverlappedRects(&_visibleRegionCache, invisibleRects, this);
			}
		}

		// Tidy up
		delete invisibleRects;

		_flags.visibleRegionCacheInvalid = false;
	}
}

void Gadget::invalidateVisibleRectCache() {
	_flags.visibleRegionCacheInvalid = true;

	// Invalidate child cache
	for (s32 i = 0; i < _gadgets.size(); i++) {
		_gadgets[i]->invalidateVisibleRectCache();
	}
}

// Return the client graphics port
GraphicsPort* Gadget::newGraphicsPort() {

	Rect rect;
	getClientRect(rect);

	u16* bitmap = DrawBg[getPhysicalScreenNumber()];

	// Ensure visible region cache is up to date
	cacheVisibleRects();

	return new GraphicsPort(this, rect.x, rect.y, rect.width, rect.height, bitmap, SCREEN_WIDTH, SCREEN_HEIGHT);
}

// Return the client graphics port for a specific clipping rect
GraphicsPort* Gadget::newGraphicsPort(Rect clipRect) {

	Rect rect;
	getClientRect(rect);

	u16* bitmap = DrawBg[getPhysicalScreenNumber()];

	// Ensure visible region cache is up to date
	cacheVisibleRects();

	return new GraphicsPort(this, rect.x, rect.y, rect.width, rect.height, bitmap, SCREEN_WIDTH, SCREEN_HEIGHT, &clipRect);
}

// Return the internal graphics port - allows drawing over entire gadget space
GraphicsPort* Gadget::newInternalGraphicsPort() {

	u16* bitmap = DrawBg[getPhysicalScreenNumber()];

	// Ensure visible region cache is up to date
	cacheVisibleRects();

	return new GraphicsPort(this, 0, 0, _width, _height, bitmap, SCREEN_WIDTH, SCREEN_HEIGHT);
}

// Return the internal graphics port for a specific clipping rect
GraphicsPort* Gadget::newInternalGraphicsPort(Rect clipRect) {

	// Ensure visible region cache is up to date
	cacheVisibleRects();

	// Remaining code OK
	u16* bitmap = DrawBg[getPhysicalScreenNumber()];

	return new GraphicsPort(this, 0, 0, _width, _height, bitmap, SCREEN_WIDTH, SCREEN_HEIGHT, &clipRect);
}

// Return vector of visible rects
WoopsiArray<Gadget::Rect>* Gadget::getVisibleRectCache() {
	return &_visibleRegionCache;
}

// Recursively move up hierarchy, clipping rect to each ancestor
void Gadget::clipRectToHierarchy(Rect& rect, Gadget* sender) {

	Rect thisRect;

	// Copy this gadget's properties into the rect
	if (sender->isDecoration()) {

		// Decorations can draw over entire gadget
		thisRect.x = getX();
		thisRect.y = getY();
		thisRect.width = getWidth();
		thisRect.height = getHeight();
	} else {

		// Standard gadgets can draw into client space
		getClientRect(thisRect);

		// Adjust rect to screen space
		thisRect.x += getX();
		thisRect.y += getY();
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
	if (_parent != NULL) {
		_parent->clipRectToHierarchy(rect, this);
	}
}

// Get the gadget's rect clipped to the dimensions of the parent
void Gadget::getRectClippedToHierarchy(Rect& rect) {

	// Copy the gadget's properties into the rect
	rect.x = getX();
	rect.y = getY();
	rect.width = getWidth();
	rect.height = getHeight();

	if (_parent != NULL) {
		_parent->clipRectToHierarchy(rect, this);
	}
}

FontBase* Gadget::getFont() const {
	return _font;
}

void Gadget::setFont(FontBase* font) {
	_font = font;
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
				woopsiApplication->getContextMenu()->newMenuItem(_contextMenuItems[i].name, _contextMenuItems[i].value);
			}
			
			woopsiApplication->getContextMenu()->unshelve();
		}
	}
}

bool Gadget::handleContextMenuSelection(const ContextMenuEventArgs& e) {
	raiseContextMenuSelectionEvent(e.getSource(), e.getItem());

	return true;
}

bool Gadget::show() {
	if (_flags.hidden) {
		_flags.hidden = false;

		raiseShowEvent();
		draw();
		return true;
	}

	return false;
}

bool Gadget::hide() {
	if (!_flags.hidden) {
		_flags.hidden = true;

		// Ensure the gadget isn't running modally
		stopModal();

		raiseHideEvent();
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

void Gadget::removeGadgetEventHandler(GadgetEventHandler* eventHandler) {
	for (s32 i = 0; i < _gadgetEventHandlers.size(); ++i) {
		if (_gadgetEventHandlers.at(i) == eventHandler) {
			_gadgetEventHandlers.erase(i);
			return;
		}
	}
}
