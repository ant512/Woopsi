#include "contextmenu.h"
#include "contextmenueventargs.h"
#include "gadgeteventargs.h"
#include "gadgetstyle.h"
#include "gadget.h"
#include "gadgeteventhandler.h"
#include "graphicsport.h"
#include "fontbase.h"
#include "framebuffer.h"
#include "listdataitem.h"
#include "rectcache.h"
#include "woopsi.h"
#include "woopsifuncs.h"

using namespace WoopsiUI;

Gadget::Gadget(s16 x, s16 y, u16 width, u16 height, GadgetStyle* style) {

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

	// Dragging values
	_grabPointX = 0;
	_grabPointY = 0;
	_newX = 0;
	_newY = 0;

	// Set initial flag values
	_flags.borderless = false;
	_flags.canReceiveFocus = true;
	_flags.clicked = false;
	_flags.decoration = false;
	_flags.deleted = false;
	_flags.doubleClickable = false;
	_flags.draggable = false;
	_flags.dragging = false;
	_flags.enabled = true;
	_flags.erased = true;
	_flags.hasFocus = false;
	_flags.hidden = false;
	_flags.modal = false;
	_flags.permeable = false;
	_flags.shelved = false;

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

	_rectCache = new RectCache(this);

	_gadgetEventHandlers = new GadgetEventHandlerList(this);
}

Gadget::~Gadget() {

	// Ensure that the gadget is disposed of correctly if it has not been sent
	// to the deletion queue.
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

			// Drawing is enabled if the gadget is drawable, not deleted, and
			// not shelved
			return ((!_flags.deleted) && (!_flags.shelved) && (!_flags.hidden));
		}
	} else {
		return ((!_flags.deleted) && (!_flags.shelved) && (!_flags.hidden));
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
			// Enabled if the gadget is enabled, not deleted, not shelved and
			// not hidden
			return (_flags.enabled && (!_flags.deleted) && (!_flags.shelved) && (!_flags.hidden));
		}
	} else {
		return (_flags.enabled && (!_flags.deleted) && (!_flags.shelved) && (!_flags.hidden));
	}

	return false;
}

const bool Gadget::isModal() const {

	// A gadget is only running modally if the Woopsi UI is also running.  That
	// means we can stop a gadget from running modally by stopping the Woopsi
	// UI top-level gadget from running modally.  This is very useful if we want
	// to kill the app but have a gadget *somewhere* in the hierarchy running
	// modally.
	if ((woopsiApplication != NULL) && (woopsiApplication != this)) {
		return woopsiApplication->isModal() & _flags.modal;
	}

	// If we reach this point, we *are* the top-level gadget, so we return our
	// modal state.
	return _flags.modal;
}

const bool Gadget::canReceiveFocus() const {
	return (isEnabled() & _flags.canReceiveFocus & (!_flags.decoration));
}

void Gadget::setBorderless(bool isBorderless) {
	if (_flags.borderless == isBorderless) return;

	_flags.borderless = isBorderless;

	invalidateVisibleRectCache();
}

void Gadget::setDecoration(bool isDecoration) {
	if (_flags.decoration == isDecoration) return;

	// If we don't have a parent of the gadget is shelved, we can just update
	// the flag without breaking anything
	if ((_parent == NULL) || _flags.shelved) {
		_flags.decoration = isDecoration;
		return;
	}

	// To ensure that resetting the child's decoration state cannot invalidate
	// the order of gadgets in the parent, we have to remove the child from
	// the parent, update its state, and re-add the child
	Gadget* parent = _parent;

	parent->removeChild(this);
	_flags.decoration = isDecoration;
	parent->addGadget(this);
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

bool Gadget::checkCollision(s16 x, s16 y) const {

	if (isHidden()) return false;

	// Get the clipped rect
	Rect rect;
	getRectClippedToHierarchy(rect);

	return rect.contains(x, y);
}

bool Gadget::checkCollision(const Rect& rect) const {

	if (isHidden()) return false;

	// Get the clipped rect
	Rect gadgetRect;
	getRectClippedToHierarchy(gadgetRect);

	return gadgetRect.intersects(rect);
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
bool Gadget::checkCollision(const Gadget* gadget) const {

	if (gadget->isHidden()) return false;
	if (gadget == this) return true;

	// Get the clipped rects
	Rect gadgetRect;
	gadget->getRectClippedToHierarchy(gadgetRect);

	Rect rect;
	getRectClippedToHierarchy(rect);

	return rect.intersects(gadgetRect);
}

void Gadget::redraw(const Rect& rect) {

	// Create internal and standard graphics ports
	GraphicsPort* internalPort = newInternalGraphicsPort(rect);
	GraphicsPort* port = newGraphicsPort(rect);

	drawBorder(internalPort);
	drawContents(port);

	delete internalPort;
	delete port;
}

void Gadget::markRectsDamaged() {
	cacheVisibleRects();
	_rectCache->markRectsDamaged();
}

void Gadget::markRectDamaged(const Rect& rect) {
	cacheVisibleRects();

	// Convert the rect from gadget space to Woopsi space co-ordinates
	Rect adjustedRect = rect;
	
	adjustedRect.x += getX();
	adjustedRect.y += getY();
	
	// Get the rect cache to redraw the rect.  It will automatically
	// clip the rect to the visible portions of the gadget so we
	// don't need to
	_rectCache->markRectDamaged(adjustedRect);
}	

// Marks the gadget as deleted and adds it to the deletion queue
void Gadget::close() {

	// Do not attempt to close deleted gadgets.  They should only ever be
	// deleted after being closed, so this suggests that an attempt is being
	// made to close the gadget twice.
	if (_flags.deleted) return;

	_gadgetEventHandlers->raiseCloseEvent();
	_gadgetEventHandlers->disable();
	
	_parent->invalidateVisibleRectCache();
	markRectsDamaged();

	_flags.deleted = true;
	
	// Unset clicked gadget if necessary
	Gadget* clickedGadget = woopsiApplication->getClickedGadget();
	if (clickedGadget == this) {
		release(clickedGadget->getX(), clickedGadget->getY());
	}

	// Ensure the gadget isn't running modally
	stopModal();

	if (_parent != NULL) {
		_parent->closeChild(this);
	}

	// Ensure that this gadget can no longer affect the decoration count
	_flags.decoration = false;
}

// Erases the gadget from the display and prevents it from being redrawn
bool Gadget::shelve() {

 	// Never attempt to shelve a gadget that has already been shelved
	if (_flags.shelved) return false;

	_gadgetEventHandlers->raiseShelveEvent();
	_gadgetEventHandlers->disable();

	markRectsDamaged();

	_flags.shelved = true;

	// Unset clicked gadget if necessary
	Gadget* clickedGadget = woopsiApplication->getClickedGadget();
	if (clickedGadget == this) {
		release(clickedGadget->getX(), clickedGadget->getY());
	}

	// Ensure the gadget isn't running modally
	stopModal();

	if (_parent != NULL) {
		_parent->shelveChild(this);
	}

	return true;
}

// Re-enables drawing and draws the gadget to the display
bool Gadget::unshelve() {

	// Never attempt to unshelve a gadget that is not already shelved
	if (!_flags.shelved) return false;

	_gadgetEventHandlers->enable();
	_gadgetEventHandlers->raiseUnshelveEvent();

	_flags.shelved = false;

	if (_parent != NULL) {
		_parent->moveShelvedToChildList(this);
		_parent->invalidateVisibleRectCache();	
	}

	markRectsDamaged();

	return true;
}

// Add a gadget to the deletion queue ready for later processing
void Gadget::moveChildToDeleteQueue(Gadget* gadget) {
	// Locate gadget in main vector
	for (s32 i = 0; i < _gadgets.size(); i++) {
		if (_gadgets[i] == gadget) {

			// Add gadget to Woopsi's delete vector
			if (woopsiApplication != NULL) {
				woopsiApplication->addToDeleteQueue(gadget);
			}

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

	invalidateVisibleRectCache();
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

// Ignores collisions with decorations
s32 Gadget::getHighestCollidingGadgetIndex(const Gadget* gadget) const {
	if (gadget == NULL) return -1;
	if (_gadgets.size() - _decorationCount < 2) return -1;

	for (s32 i = _gadgets.size() - 1; i >= _decorationCount; --i) {
		if (gadget != _gadgets[i]) {
			if (_gadgets[i]->checkCollision(gadget)) {
				return i;
			}
		}
	}

	return -1;
}

// Ignores collisions with decorations
s32 Gadget::getLowestCollidingGadgetIndex(const Gadget* gadget) const {
	if (gadget == NULL) return -1;
	if (_gadgets.size() - _decorationCount < 2) return -1;

	for (s32 i = _decorationCount; i < _gadgets.size(); ++i) {
		if (gadget != _gadgets[i]) {
			if (_gadgets[i]->checkCollision(gadget)) {
				return i;
			}
		}
	}

	return -1;
}

bool Gadget::changeGadgetDepth(s32 sourceIndex, s32 destinationIndex) {

	Gadget* gadget = _gadgets[sourceIndex];

	if (gadget->isDecoration()) return false;
	if (destinationIndex == sourceIndex) return false;

	// Ensure the gadget gets erased
	gadget->markRectsDamaged();
	gadget->invalidateVisibleRectCache();

	_gadgets.erase(sourceIndex);
	_gadgets.insert(destinationIndex, gadget);

	// Invalidate rect cache of all gadgets that collide with the swapped gadget
	for (s32 i = 0; i < _gadgets.size(); ++i) {
		if (_gadgets[i]->checkCollision(gadget)) {
			_gadgets[i]->invalidateVisibleRectCache();
		}
	}

	// Ensure the gadget gets correctly redrawn
	gadget->markRectsDamaged();

	// Shift focus to the frontmost gadget if the moved gadget had focus and
	// moved backwards
	if ((destinationIndex < sourceIndex) && (gadget->hasFocus())) {
		_gadgets[_gadgets.size() - 1]->focus();
	}

	return true;
}

bool Gadget::swapDepth() {

	if (_parent == NULL) return false;
	if (isDecoration()) return false;

	s32 lowestIndex = _parent->getLowestCollidingGadgetIndex(this);
	s32 highestIndex = _parent->getHighestCollidingGadgetIndex(this);
	s32 sourceIndex = _parent->getGadgetIndex(this);

	// Cannot swap if there is nowhere to swap to
	if ((lowestIndex == -1) && (highestIndex == -1)) return false;

	s32 destinationIndex = -1;

	// If we are behind another gadget, move to the front.  If not, move to the
	// back
	if (highestIndex > sourceIndex) {
		destinationIndex = highestIndex;
	} else {
		destinationIndex = lowestIndex > -1 ? lowestIndex : highestIndex;
	}

	return _parent->changeGadgetDepth(sourceIndex, destinationIndex);
}

bool Gadget::enable() {

	// Never enable a gadget that is already enabled
	if (_flags.enabled) return false;

	_flags.enabled = true;
	
	onEnable();

	markRectsDamaged();

	_gadgetEventHandlers->raiseEnableEvent();

	return true;
}

bool Gadget::disable() {

	// Never disable a gadget that is already disabled
	if (!_flags.enabled) return false;

	_flags.enabled = false;
	
	onDisable();

	markRectsDamaged();

	_gadgetEventHandlers->raiseDisableEvent();

	return true;
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
		
		markRectsDamaged();

		s16 oldX = _rect.getX();
		s16 oldY = _rect.getY();

		_rect.setX(x);
		_rect.setY(y);

		if (_parent != NULL) {
			_parent->invalidateVisibleRectCache();
		}

		markRectsDamaged();

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

		_flags.permeable = true;
	
		markRectsDamaged();

		_rect.setWidth(width);
		_rect.setHeight(height);

		// Handle visible region caching
		if (_parent != NULL) {
			_parent->invalidateVisibleRectCache();
		}

		onResize(width, height);
		
		// Reset the permeable value
		_flags.permeable = wasPermeable;

		markRectsDamaged();

		_gadgetEventHandlers->raiseResizeEvent(width, height);

		return true;
	}

	return false;
}

bool Gadget::changeDimensions(s16 x, s16 y, u16 width, u16 height) {
	
	bool wasHidden = _flags.hidden;
	hide();
	
	bool moved = moveTo(x, y);
	bool resized = resize(width, height);
	
	if (!wasHidden) show();
	
	return (resized | moved);
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
	// Abort dragging if not dragging the bottom screen; will only be an issue
	// in SDL code
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

Gadget* Gadget::getChild(const u32 index) const {
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
		gadget->markRectsDamaged();

		// Invalidate all gadgets that collide with the depth-swapped gadget
		for (s32 i = 0; i < _gadgets.size(); i++) {
			if (_gadgets[i]->checkCollision(gadget)) {
				_gadgets[i]->invalidateVisibleRectCache();
			}
		}

		return true;
	}

	return false;
}

bool Gadget::lowerGadgetToBottom(Gadget* gadget) {

	// Locate gadget in the stack
	s32 index = getGadgetIndex(gadget);

	if (index > _decorationCount) {
		gadget->markRectsDamaged();

		// Handle visible region caching
		gadget->invalidateVisibleRectCache();
		invalidateLowerGadgetsVisibleRectCache(gadget);

		_gadgets.erase(index);
		_gadgets.insert(_decorationCount, gadget);

		return true;
	}

	return false;
}

// Append a gadget to the end of the gadget list
void Gadget::addGadget(Gadget* gadget) {

	// Do not add gadgets that already belong to another gadget
	if (gadget->getParent() != NULL) return;

	gadget->setParent(this);

	// Process decorations and standard gadgets differently
	if (gadget->isDecoration()) {
		_gadgets.insert(_decorationCount, gadget);
		_decorationCount++;
	} else {
		_gadgets.push_back(gadget);
	}

	// Should the gadget steal the focus?
	if (gadget->hasFocus()) {
		setFocusedGadget(gadget);
	}

	invalidateVisibleRectCache();
	gadget->markRectsDamaged();
}

// Insert a gadget into the gadget list after the decorations
void Gadget::insertGadget(Gadget* gadget) {

	// Do not insert gadgets that already belong to another gadget
	if (gadget->getParent() != NULL) return;

	gadget->setParent(this);

	// Process decorations and standard gadgets differently
	if (gadget->isDecoration()) {
		_gadgets.insert(0, gadget);
		_decorationCount++;
	} else {
		_gadgets.insert(_decorationCount, gadget);
	}

	invalidateVisibleRectCache();
	gadget->markRectsDamaged();
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
	markRectsDamaged();
	
	if (_parent != NULL) {
		return _parent->removeChild(this);
	}

	return false;
}

bool Gadget::removeChild(Gadget* gadget) {
	
	gadget->markRectsDamaged();

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
		if (_parent != NULL) {
			_parent->invalidateLowerGadgetsVisibleRectCache(this);
		} else {
			invalidateVisibleRectCache();
		}

		_gadgetEventHandlers->raiseShowEvent();
		markRectsDamaged();
		return true;
	}

	return false;
}

bool Gadget::hide() {
	if (!_flags.hidden) {

		markRectsDamaged();

		_flags.hidden = true;

		// Ensure the gadget isn't running modally
		stopModal();

		// Ensure that gadgets behind this do draw over the top
		if (_parent != NULL) {
			_parent->invalidateVisibleRectCache();
		} else {
			invalidateVisibleRectCache();
		}

		_gadgetEventHandlers->raiseHideEvent();

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

	// We interrogate the parent gadget for its screen number rather than just
	// rely on calculatePhysicalScreenNumber() to do this for us because it is
	// possible that our parent is on screen 0 but our y co-ordinate places us
	// on screen 1.  We must be on the same screen as our parent, so we defer
	// to the parent's value.
	if (_parent != NULL) {
		return _parent->getPhysicalScreenNumber();
	} else{
		return calculatePhysicalScreenNumber(getY());
	}
}
