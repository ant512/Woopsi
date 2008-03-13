#include "gadget.h"
#include "graphicsport.h"
#include "woopsi.h"
#include "woopsifuncs.h"

Gadget::Gadget(s16 x, s16 y, u16 width, u16 height, u32 flags, FontBase* font) {
	_x = x;
	_y = y;
	_width = width;
	_height = height;
	_font = font;

	// Do we need to fetch the system font?
	if (_font == NULL) {
		_font = Woopsi::getSystemFont();
	}

	_parent = NULL;

	// Mask flags against bitmasks and logical NOT twice to obtain boolean values
	_flags.borderless = (!(!(flags & GADGET_BORDERLESS)));
	_flags.draggable = (!(!(flags & GADGET_DRAGGABLE)));
	_flags.closeable = (!(!(flags & GADGET_CLOSEABLE)));

	init();
}

Gadget::~Gadget() {
	// Delete children
	for (u8 i = 0; i < _gadgets.size(); i++) {
		delete _gadgets[i];
	}

	// Delete hidden children
	for (u8 i = 0; i < _hiddenGadgets.size(); i++) {
		delete(_hiddenGadgets[i]);
	}

	_gadgets.clear();
	_hiddenGadgets.clear();
}

void Gadget::init() {
	_grabPointX = 0;
	_grabPointY = 0;

	_backColour = woopsiRGB(20, 20, 20);
	_shineColour = woopsiRGB(31, 31, 31);
	_highlightColour = woopsiRGB(12, 17, 23);
	_shadowColour = woopsiRGB(0, 0, 0);
	_fillColour = woopsiRGB(24, 24, 24);
	_darkColour = woopsiRGB(15, 15, 15);

	_flags.clicked = false;
	_flags.dragging = false;
	_flags.active = false;
	_flags.deleted = false;
	_flags.enabled = true;
	_flags.visible = false;
	_flags.decoration = false;
	_flags.permeable = false;
	_flags.raisesEvents = true;
	_flags.erased = true;

	_activeGadget = NULL;
	_clickedGadget = NULL;
	_eventHandler = NULL;

	_decorationCount = 0;

	_visibleRegionCacheInvalid = true;
	
	_outline = OUTLINE_CLICK_DEPENDENT;
	_closeType = CLOSE_TYPE_CLOSE;
}

s16 Gadget::getX() const {
	if (_parent != NULL) {
		return _parent->getX() + _x;
	}

	return _x;
}

s16 Gadget::getY() const {
	if (_parent != NULL) {
		return _parent->getY() + _y;
	}

	return _y;
}

u16 Gadget::getWidth() const {
	return _width;
}

u16 Gadget::getHeight() const {
	return _height;
}

Gadget* Gadget::getParent() {
	return _parent;
}

const bool Gadget::isPermeable() const {
	return _flags.permeable;
}

const bool Gadget::isActive() const {
	return _flags.active;
}

const bool Gadget::isClicked() const {
	return _flags.clicked;
}

const bool Gadget::raisesEvents() const {
	return _flags.raisesEvents;
}

const bool Gadget::isVisible() const {
	if (_parent != NULL) {
		if (_parent->isVisible()) {
			return _flags.visible;
		}
	} else {
		return _flags.visible;
	}

	return false;
}

const bool Gadget::isDeleted() const {
	return _flags.deleted;
}

const bool Gadget::isDecoration() const {
	return _flags.decoration;
}

const bool Gadget::isBorderless() const {
	return _flags.borderless;
}

Gadget* Gadget::getActiveGadget() {
	return _activeGadget;
}

Gadget::CloseType Gadget::getCloseType() {
	return _closeType;
}

const u8 Gadget::getPhysicalScreenNumber() const{
	return calculatePhysicalScreenNumber(getY());
}

void Gadget::setVisible(bool visible) {
	_flags.visible = visible;
}

void Gadget::setBorderless(bool isBorderless) {
	_flags.borderless = isBorderless;

	invalidateVisibleRectCache();
}

void Gadget::setPermeable(bool isPermeable) {
	_flags.permeable = isPermeable;
}

void Gadget::setOutlineType(OutlineType outline) {
	_outline = outline;
}

void Gadget::setCloseType(CloseType closeType) {
	_closeType = closeType;
}

void Gadget::setDraggable(bool isDraggable) {
	_flags.draggable = isDraggable;
}

void Gadget::setActive(bool active) {

	if (_flags.active != active) {
		_flags.active = active;

		if (_flags.active) {
			// Notify parent if this gadget has become active
			if (_parent != NULL) {
				_parent->setActiveGadget(this);
			}
		} else {
			// Notify active child if this gadget has become inactive
			if (_activeGadget != NULL) {
				_activeGadget->blur();
				_activeGadget = NULL;
			}
		}
	}
}

void Gadget::setActiveGadget(Gadget* gadget) {

	if (_activeGadget != gadget) {
		if (_activeGadget != NULL) {
			// Blur the current active gadget
			_activeGadget->blur();
		}
	}

	// Remember the new active gadget
	_activeGadget = gadget;

	// Make this gadget active too
	focus();
}

void Gadget::setClickedGadget(Gadget* gadget) {
	_clickedGadget = gadget;

	// Notify parent
	if (_parent != NULL) {
		_parent->setClickedGadget(this);
	}
}

void Gadget::setEventHandler(EventHandler* eventHandler) {
	_eventHandler = eventHandler;
}

void Gadget::setRaisesEvents(const bool raisesEvents) {
	_flags.raisesEvents = raisesEvents;
}

void Gadget::setBackColour(u16 colour) {
	_backColour = colour;
}

void Gadget::setShineColour(u16 colour) {
	_shineColour = colour;
}

void Gadget::setHighlightColour(u16 colour) {
	_highlightColour = colour;
}

void Gadget::setShadowColour(u16 colour) {
	_shadowColour = colour;
}

void Gadget::setFillColour(u16 colour) {
	_fillColour = colour;
}

void Gadget::setDarkColour(u16 colour) {
	_darkColour = colour;
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

	// Get the clipped rect
	Rect rect;
	getRectClippedToHierarchy(rect);

	if ((x >= rect.x) && (y >= rect.y) && (x < rect.x + rect.width) && (y < rect.y + rect.height)) {
		return true;
	}

	return false;
}

// Check for collisions with another rectangle
bool Gadget::checkCollision(s16 x, s16 y, u16 width, u16 height) {
	// Get the clipped rect
	Rect rect;
	getRectClippedToHierarchy(rect);

	if ((x + width >= rect.x) && (y + height >= rect.y) && (x < rect.x + rect.width) && (y < rect.y + rect.height)) {
		return true;
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
	if ((_eventHandler != NULL) && (_flags.raisesEvents)) {

		EventArgs e;
		e.type = EVENT_CLICK;
		e.eventX = x;
		e.eventY = y;
		e.keyCode = KEY_CODE_NONE;
		e.gadget = this;

		_eventHandler->handleEvent(e);
	}
}

void Gadget::raiseReleaseEvent(s16 x, s16 y) {
	if ((_eventHandler != NULL) && (_flags.raisesEvents)) {

		EventArgs e;
		e.type = EVENT_RELEASE;
		e.eventX = x;
		e.eventY = y;
		e.keyCode = KEY_CODE_NONE;
		e.gadget = this;

		_eventHandler->handleEvent(e);
	}
}

void Gadget::raiseDragEvent(s16 x, s16 y, s16 vX, s16 vY) {
	if ((_eventHandler != NULL) && (_flags.raisesEvents)) {

		EventArgs e;
		e.type = EVENT_DRAG;
		e.eventX = x;
		e.eventY = y;
		e.keyCode = KEY_CODE_NONE;
		e.gadget = this;

		_eventHandler->handleEvent(e);
	}
}

void Gadget::raiseVBLEvent() {
	if ((_eventHandler != NULL) && (_flags.raisesEvents)) {

		EventArgs e;
		e.type = EVENT_VBL;
		e.eventX = 0;
		e.eventY = 0;
		e.keyCode = KEY_CODE_NONE;
		e.gadget = this;

		_eventHandler->handleEvent(e);
	}
}

void Gadget::raiseKeyPressEvent(KeyCode keyCode) {
	if ((_eventHandler != NULL) && (_flags.raisesEvents)) {

		EventArgs e;
		e.type = EVENT_KEY_PRESS;
		e.eventX = 0;
		e.eventY = 0;
		e.keyCode = keyCode;
		e.gadget = this;

		_eventHandler->handleEvent(e);
	}
}

void Gadget::raiseKeyReleaseEvent(KeyCode keyCode) {
	if ((_eventHandler != NULL) && (_flags.raisesEvents)) {

		EventArgs e;
		e.type = EVENT_KEY_RELEASE;
		e.eventX = 0;
		e.eventY = 0;
		e.keyCode = keyCode;
		e.gadget = this;

		_eventHandler->handleEvent(e);
	}
}

void Gadget::raiseLidClosedEvent() {
	if ((_eventHandler != NULL) && (_flags.raisesEvents)) {

		EventArgs e;
		e.type = EVENT_LID_CLOSED;
		e.eventX = 0;
		e.eventY = 0;
		e.keyCode = KEY_CODE_NONE;
		e.gadget = this;

		_eventHandler->handleEvent(e);
	}
}

void Gadget::raiseLidOpenedEvent() {
	if ((_eventHandler != NULL) && (_flags.raisesEvents)) {

		EventArgs e;
		e.type = EVENT_LID_OPENED;
		e.eventX = 0;
		e.eventY = 0;
		e.keyCode = KEY_CODE_NONE;
		e.gadget = this;

		_eventHandler->handleEvent(e);
	}
}

void Gadget::raiseFocusEvent() {
	if ((_eventHandler != NULL) && (_flags.raisesEvents)) {

		EventArgs e;
		e.type = EVENT_FOCUS;
		e.eventX = 0;
		e.eventY = 0;
		e.keyCode = KEY_CODE_NONE;
		e.gadget = this;

		_eventHandler->handleEvent(e);
	}
}

void Gadget::raiseBlurEvent() {
	if ((_eventHandler != NULL) && (_flags.raisesEvents)) {

		EventArgs e;
		e.type = EVENT_BLUR;
		e.eventX = 0;
		e.eventY = 0;
		e.keyCode = KEY_CODE_NONE;
		e.gadget = this;

		_eventHandler->handleEvent(e);
	}
}

void Gadget::raiseCloseEvent() {
	if ((_eventHandler != NULL) && (_flags.raisesEvents)) {

		EventArgs e;
		e.type = EVENT_CLOSE;
		e.eventX = 0;
		e.eventY = 0;
		e.keyCode = KEY_CODE_NONE;
		e.gadget = this;

		_eventHandler->handleEvent(e);
	}
}

void Gadget::raiseHideEvent() {
	if ((_eventHandler != NULL) && (_flags.raisesEvents)) {

		EventArgs e;
		e.type = EVENT_HIDE;
		e.eventX = 0;
		e.eventY = 0;
		e.keyCode = KEY_CODE_NONE;
		e.gadget = this;

		_eventHandler->handleEvent(e);
	}
}

void Gadget::raiseShowEvent() {
	if ((_eventHandler != NULL) && (_flags.raisesEvents)) {

		EventArgs e;
		e.type = EVENT_SHOW;
		e.eventX = 0;
		e.eventY = 0;
		e.keyCode = KEY_CODE_NONE;
		e.gadget = this;

		_eventHandler->handleEvent(e);
	}
}

void Gadget::raiseEnableEvent() {
	if ((_eventHandler != NULL) && (_flags.raisesEvents)) {

		EventArgs e;
		e.type = EVENT_ENABLE;
		e.eventX = 0;
		e.eventY = 0;
		e.keyCode = KEY_CODE_NONE;
		e.gadget = this;

		_eventHandler->handleEvent(e);
	}
}

void Gadget::raiseDisableEvent() {
	if ((_eventHandler != NULL) && (_flags.raisesEvents)) {

		EventArgs e;
		e.type = EVENT_DISABLE;
		e.eventX = 0;
		e.eventY = 0;
		e.keyCode = KEY_CODE_NONE;
		e.gadget = this;

		_eventHandler->handleEvent(e);
	}
}

void Gadget::raiseValueChangeEvent() {
	if ((_eventHandler != NULL) && (_flags.raisesEvents)) {

		EventArgs e;
		e.type = EVENT_VALUE_CHANGE;
		e.eventX = 0;
		e.eventY = 0;
		e.keyCode = KEY_CODE_NONE;
		e.gadget = this;

		_eventHandler->handleEvent(e);
	}
}

void Gadget::raiseResizeEvent(u16 width, u16 height) {
	if ((_eventHandler != NULL) && (_flags.raisesEvents)) {

		EventArgs e;
		e.type = EVENT_RESIZE;
		e.eventX = 0;
		e.eventY = 0;
		e.keyCode = KEY_CODE_NONE;
		e.gadget = this;

		_eventHandler->handleEvent(e);
	}
}

void Gadget::raiseMoveEvent(s16 x, s16 y) {
	if ((_eventHandler != NULL) && (_flags.raisesEvents)) {

		EventArgs e;
		e.type = EVENT_MOVE;
		e.eventX = x;
		e.eventY = y;
		e.keyCode = KEY_CODE_NONE;
		e.gadget = this;

		_eventHandler->handleEvent(e);
	}
}

void Gadget::drawChildren() {
	for (u8 i = 0; i < _gadgets.size(); i++) {
		_gadgets[i]->draw();
	}
}

void Gadget::draw(Rect clipRect) {
}

void Gadget::draw() {
	if (isVisible()) {
		cacheVisibleRects();

		// Create pointer to a vector to store the overlapped rectangles
		// We can discard this later as we don't need it
		DynamicArray<Rect>* invisibleRects = new DynamicArray<Rect>();

		// Create a pointer to store rects that are not overlapped by
		// children
		DynamicArray<Rect>* visibleRects = new DynamicArray<Rect>();

		if (invisibleRects != NULL) {

			// Copy all visible regions into the new vector
			for (u16 i = 0; i < _visibleRegionCache.size(); i++) {
				visibleRects->push_back(_visibleRegionCache[i]);
			}

			// Remove all child rects from the visible vector
			for (u8 i = 0; i < _gadgets.size(); i++) {
				if (visibleRects->size() > 0) {
					_gadgets[i]->splitRectangles(visibleRects, invisibleRects, this);
				} else {
					break;
				}
			}

			// Tidy up
			delete invisibleRects;
		}

		// Draw all visible rectangles
		for (u8 i = 0; i < visibleRects->size(); i++) {
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
void Gadget::redrawDirty(DynamicArray<Rect>* invalidRects, Gadget* sender) {

	if (isVisible()) {

		// Draw any children first
		redrawDirtyChildren(invalidRects, sender);

		DynamicArray<Rect>* overlappingRects = new DynamicArray<Rect>();

		if (overlappingRects != NULL) {

			// Remove any non-overlapping rectangles from dirty vector and add to
			// overlapping vector
			splitRectangles(invalidRects, overlappingRects, sender);

			// Draw overlapped rects
			for (u8 i = 0; i < overlappingRects->size(); i++) {
				draw(overlappingRects->at(i));
			}

			delete overlappingRects;
		}
	}
}

void Gadget::redrawDirtyChildren(DynamicArray<Rect>* invalidRects, Gadget* sender) {

	for (s16 i = _gadgets.size() - 1; i > -1 ; i--) {
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
void Gadget::removeOverlappedRects(DynamicArray<Rect>* visibleRects, DynamicArray<Rect>* invisibleRects, Gadget* gadget) {

	// Locate gadget in the list
	u8 gadgetIndex = 0;

	for (u8 i = 0; i < _gadgets.size(); i++) {
		if (_gadgets[i] == gadget) {
			// Found gadget - remember the index of the next gadget
			gadgetIndex = i + 1;
			break;
		}
	}

	// Gadget should never be the bottom item on the screen
	if (gadgetIndex > 0) {

		// Remove any overlapped rectangles
		for (u8 i = gadgetIndex; i < _gadgets.size(); i++) {
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
void Gadget::splitRectangles(DynamicArray<Rect>* invalidRects, DynamicArray<Rect>* validRects, Gadget* sender) {

	// Check for collisions with any rectangles in the vector
	for (u16 i = 0; i < invalidRects->size(); i++) {

		// Get rectangle to check
		Rect checkRect = invalidRects->at(i);
		s16 splitX[4];
		s16 splitY[4];
		u8 rectXCount = 0;
		u8 rectYCount = 0;
		u8 overlapXRect = 0;
		u8 overlapYRect = 0;

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
			invalidRects->erase(invalidRects->begin() + i);

			// Force the loop to re-examine the new rectangle at this index
			i--;

			// Add the new rectangles (not the overlap; that's the one we need to draw)
			for (u8 xRects = 0; xRects < rectXCount; xRects++) {
				for (u8 yRects = 0; yRects < rectYCount; yRects++) {

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
								invalidRects->insert(invalidRects->begin(), newRect);
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
	s16 gadgetIndex = getGadgetIndex(gadget);

	// Order all lower gadgets to redraw themselves based on the erased gadget's
	// visible rect cache
	for (s16 i = gadgetIndex - 1; i > -1; i--) {
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
		_flags.visible = false;

		erase();

		if (_parent != NULL) {
			_parent->closeChild(this);
		}
	}
}

// Erases the gadget from the display and prevents it from being redrawn
bool Gadget::hide() {

	if (_flags.visible) {

		raiseHideEvent();

		erase();

		_flags.visible = false;

		if (_parent != NULL) {
			_parent->hideChild(this);
		}

		return true;
	}

	return false;
}

// Re-enables drawing and draws the gadget to the display
bool Gadget::show() {

	if (!_flags.visible) {

		raiseShowEvent();

		_flags.visible = true;

		if (_parent != NULL) {
			_parent->moveHiddenToChildList(this);
		}

		draw();

		return true;
	}
	return false;
}

// Add a gadget to the deletion queue ready for later processing
void Gadget::moveChildToDeleteQueue(Gadget* gadget) {
	// Locate gadget in main vector
	for (u8 i = 0; i < _gadgets.size(); i++) {
		if (_gadgets[i] == gadget) {

			// Add gadget to Woopsi's delete vector
			Woopsi::addToDeleteQueue(gadget);

			// Divorce child from parent
			gadget->setParent(NULL);

			// Remove gadget from main vector
			_gadgets.erase(_gadgets.begin() + i);

			break;
		}
	}
}

// Add a gadget to the hidden list ready for later processing
bool Gadget::moveChildToHiddenList(Gadget* gadget) {

	// Locate gadget in main vector
	for (u8 i = 0; i < _gadgets.size(); i++) {
		if (_gadgets[i] == gadget) {

			// Add gadget to hidden vector
			_hiddenGadgets.push_back(gadget);

			// Remove gadget from main vector
			_gadgets.erase(_gadgets.begin() + i);

			return true;
		}
	}

	return false;
}

// Move a hidden gadget back to the child list
bool Gadget::moveHiddenToChildList(Gadget* gadget) {

	// Locate gadget in hidden vector
	for (u8 i = 0; i < _hiddenGadgets.size(); i++) {
		if (_hiddenGadgets[i] == gadget) {

			// Process decorations and gadgets differently
			if (gadget->isDecoration()) {

				// Add gadget to end of decoration list
				_gadgets.insert(_gadgets.begin() + _decorationCount, gadget);
				_decorationCount++;
			} else {

				// Add gadget to end of main vector
				_gadgets.push_back(gadget);
			}

			// Remove gadget from hidden vector
			_hiddenGadgets.erase(_hiddenGadgets.begin() + i);

			return true;
		}
	}

	return false;
}

// Hide a child
void Gadget::hideChild(Gadget* gadget) {
	if (gadget != NULL) {

		// Ensure gadget knows it is being closed
		if (gadget->isVisible()) {
			gadget->hide();
		}

		// Do we need to make another gadget active?
		if (_activeGadget == gadget) {

			// Try to choose highest gadget
			if (_gadgets.size() > 1) {
				for (s16 i = _gadgets.size() - 1; i > -1; i--) {
					if (_gadgets[i] != gadget) {
						_activeGadget = _gadgets[i];
					}
				}
			} else {
				_activeGadget = NULL;
			}
		}

		// Unset clicked gadget if necessary
		if (_clickedGadget == gadget) {
			_clickedGadget = NULL;
		}

		// Decrease decoration count if necessary
		if (gadget->isDecoration()) {
			_decorationCount--;
		}

		// Where should the focus go?
		if (_activeGadget != NULL) {
			// Send focus to the new active gadget
			_activeGadget->focus();
		} else {
			// Give focus to this
			setActiveGadget(NULL);
		}

		moveChildToHiddenList(gadget);
	}
}

// Close a child
void Gadget::closeChild(Gadget* gadget) {
	if (gadget != NULL) {

		// Ensure gadget knows it is being closed
		if (!gadget->isDeleted()) {
			gadget->close();
		}

		// Do we need to make another gadget active?
		if (_activeGadget == gadget) {

			// Try to choose highest gadget
			if (_gadgets.size() > 1) {
				for (s16 i = _gadgets.size() - 1; i > -1; i--) {
					if (_gadgets[i] != gadget) {
						_activeGadget = _gadgets[i];
					}
				}
			} else {
				_activeGadget = NULL;
			}
		}

		// Unset clicked gadget if necessary
		if (_clickedGadget == gadget) {
			_clickedGadget = NULL;
		}

		// Decrease decoration count if necessary
		if (gadget->isDecoration()) {
			_decorationCount--;
		}

		// Where should the focus go?
		if (_activeGadget != NULL) {
			// Send focus to the new active gadget
			_activeGadget->focus();
		} else {
			// Give focus to this
			setActiveGadget(NULL);
		}

		moveChildToDeleteQueue(gadget);
	}
}

bool Gadget::swapGadgetDepth(Gadget* gadget) {
	
	// Can we swap?
	if (_gadgets.size() > 1) {

		u8 gadgetSource = 0;
		u8 gadgetDest = 0;

		// Locate the gadget in the vector
		for (u8 i = _decorationCount; i < _gadgets.size(); i++) {
			if (gadget == _gadgets[i]) {
				gadgetSource = i;
				break;
			}
		}

		// Raise or lower?
		if (gadgetSource < _gadgets.size() - 1) {
			// Raise up one
			gadgetDest = gadgetSource + 1;
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

		_x = x;
		_y = y;

		draw();

		raiseMoveEvent(x, y);

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

bool Gadget::click(s16 x, s16 y) {

	if (_flags.enabled) {
		if (checkCollision(x, y)) {
			_flags.clicked = true;

			// Take focus away from child gadgets
			if (_activeGadget != NULL) {
				_activeGadget->blur();
				_activeGadget = NULL;
			}

			// Give focus to this gadget
			if (!_flags.active) {
				focus();
			}

			// Tell parent that the clicked gadget has changed
			if (_parent != NULL) {
				_parent->setClickedGadget(this);
			}

			raiseClickEvent(x, y);

			return true;
		}
	}

	return false;
}

bool Gadget::release(s16 x, s16 y) {

	if (_flags.clicked) {
		_flags.clicked = false;
		_flags.dragging = false;

		// Tell the parent that the gadget has been released
		if (_parent != NULL) {
			_parent->setClickedGadget(NULL);
		}

		raiseReleaseEvent(x, y);

		return true;
	}

	return false;
}

bool Gadget::drag(s16 x, s16 y, s16 vX, s16 vY) {
	if (_flags.enabled) {
		raiseDragEvent(x, y, vX, vY);

		return true;
	}

	return false;
}

bool Gadget::vbl() {
	if (_flags.enabled) {

		raiseVBLEvent();

		return true;
	}

	return false;
}

bool Gadget::keyPress(KeyCode keyCode) {
	if (_flags.enabled) {
		
		raiseKeyPressEvent(keyCode);

		return true;
	}

	return false;
}

bool Gadget::keyRelease(KeyCode keyCode) {
	if (_flags.enabled) {

		raiseKeyReleaseEvent(keyCode);

		return true;
	}

	return false;
}

void Gadget::lidClosed() {
	raiseLidClosedEvent();
}

void Gadget::lidOpened() {
	raiseLidOpenedEvent();
}

bool Gadget::focus() {
	if (_flags.enabled) {
		if (!_flags.active) {
			setActive(true);

			raiseFocusEvent();

			return true;
		}
	}

	return false;
}

bool Gadget::blur() {

	if (_flags.active) {

		setActive(false);

		raiseBlurEvent();

		return true;
	}

	return false;
}

bool Gadget::raiseToTop() {
	if (_parent != NULL) {
		return _parent->raiseGadgetToTop(this);
	}

	return false;
}

bool Gadget::lowerToBottom() {
	if (_parent != NULL) {
		return _parent->lowerGadgetToBottom(this);
	}

	return false;
}

s16 Gadget::getGadgetIndex(Gadget* gadget) {
	for (u8 i = 0; i < _gadgets.size(); i++) {
		if (_gadgets[i] == gadget) {
			return i;
		}
	}

	return -1;
}

bool Gadget::raiseGadgetToTop(Gadget* gadget) {

	// Locate gadget in the stack
	s16 index = getGadgetIndex(gadget);

	if (index > -1) {
		if ((u16)index < _gadgets.size() - 1) {

			_gadgets.erase(_gadgets.begin() + index);
			_gadgets.push_back(gadget);

			// Handle visible region caching
			gadget->invalidateVisibleRectCache();
			invalidateLowerGadgetsVisibleRectCache(gadget);

			gadget->draw();

			return true;
		}
	}

	return false;
}

bool Gadget::lowerGadgetToBottom(Gadget* gadget) {

	// Locate gadget in the stack
	s16 index = getGadgetIndex(gadget);

	if (index > 0) {
		gadget->erase();

		// Handle visible region caching
		gadget->invalidateVisibleRectCache();
		invalidateLowerGadgetsVisibleRectCache(gadget);

		_gadgets.erase(_gadgets.begin() + index);
		_gadgets.insert(_gadgets.begin() + _decorationCount, gadget);

		gadget->draw();

		return true;
	}

	return false;
}

void Gadget::setParent(Gadget* gadget) {
	_parent = gadget;
}

// Append a gadget to the end of the gadget list
// Do not use this unless you know what you are doing
void Gadget::addGadget(Gadget* gadget) {
	if (gadget->getParent() == NULL) {
		gadget->setParent(this);

		// Process decorations and standard gadgets differently
		if (gadget->isDecoration()) {
			_gadgets.insert(_gadgets.begin() + _decorationCount, gadget);

			// Increase the decoration count
			_decorationCount++;
		} else {
			_gadgets.push_back(gadget);
		}

		gadget->setVisible(true);

		invalidateVisibleRectCache();
	}
}

// Insert a gadget into the gadget list after the decorations
// Do not use this unless you know what you are doing
void Gadget::insertGadget(Gadget* gadget) {
	if (gadget->getParent() == NULL) {
		gadget->setParent(this);

		// Process decorations and standard gadgets differently
		if (gadget->isDecoration()) {
			_gadgets.insert(_gadgets.begin(), gadget);

			// Increate the decoration count
			_decorationCount++;
		} else {
			_gadgets.insert(_gadgets.begin() + _decorationCount, gadget);
		}

		gadget->setVisible(true);

		invalidateVisibleRectCache();
	}
}

// Invalidate any gadgets below the supplied index
void Gadget::invalidateLowerGadgetsVisibleRectCache(Gadget* gadget) {

	// Find the gadget
	u8 gadgetIndex = getGadgetIndex(gadget);	

	// Invalidate lower gadgets
	for (s16 i = gadgetIndex - 1; i > -1; i--) {
		if (_gadgets[i]->checkCollision(gadget)) {
			_gadgets[i]->invalidateVisibleRectCache();
		}
	}
}

// Get the ID of this gadget
u32 Gadget::getRefcon() {
	return _refcon;
}

// Set the ID of this gadget and return the old ID
u32 Gadget::setRefcon(u32 refcon) {
	u32 old = _refcon;
	_refcon = refcon;

	return old;
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

	if (_visibleRegionCacheInvalid) {
		// Use internal region cache to store the non-overlapped rectangles
		// We will use this to clip the gadget
		_visibleRegionCache.clear();

		// Create pointer to a vector to store the overlapped rectangles
		// We can discard this later as we don't need it
		DynamicArray<Rect>* invisibleRects = new DynamicArray<Rect>();

		if (invisibleRects != NULL) {

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
		}

		_visibleRegionCacheInvalid = false;
	}
}

void Gadget::invalidateVisibleRectCache() {
	_visibleRegionCacheInvalid = true;

	// Invalidate child cache
	for (u8 i = 0; i < _gadgets.size(); i++) {
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
	//return new GraphicsPort(this, 0, 0, _width, _height, bitmap, SCREEN_WIDTH, SCREEN_HEIGHT);
}

// Return the client graphics port for a specific clipping rect
GraphicsPort* Gadget::newGraphicsPort(Rect clipRect) {

	Rect rect;
	getClientRect(rect);

	u16* bitmap = DrawBg[getPhysicalScreenNumber()];

	// Ensure visible region cache is up to date
	cacheVisibleRects();

	return new GraphicsPort(this, rect.x, rect.y, rect.width, rect.height, bitmap, SCREEN_WIDTH, SCREEN_HEIGHT, &clipRect);
	//return new GraphicsPort(this, 0, 0, _width, _height, bitmap, SCREEN_WIDTH, SCREEN_HEIGHT, &clipRect);
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
DynamicArray<Gadget::Rect>* Gadget::getVisibleRectCache() {
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

FontBase* Gadget::getFont() {
	return _font;
}

void Gadget::setFont(FontBase* font) {
	_font = font;
}

void Gadget::unregisterChildrenFromVBL() {

	// Unregister children
	for (u8 i = 0; i < _gadgets.size(); i++) {
		Woopsi::unregisterFromVBL(_gadgets[i]);
	}

	// Unregister hidden children
	for (u8 i = 0; i < _hiddenGadgets.size(); i++) {
		Woopsi::unregisterFromVBL(_hiddenGadgets[i]);
	}
}

const u16 Gadget::getBackColour() const {
	return _backColour;
}

const u16 Gadget::getShineColour() const {
	return _shineColour;
}

const u16 Gadget::getHighlightColour() const {
	return _highlightColour;
}

const u16 Gadget::getShadowColour() const {
	return _shadowColour;
}

const u16 Gadget::getFillColour() const {
	return _fillColour;
}

const u16 Gadget::getDarkColour() const {
	return _darkColour;
}

const Gadget::OutlineType Gadget::getOutlineType() const {
	return _outline;
}
