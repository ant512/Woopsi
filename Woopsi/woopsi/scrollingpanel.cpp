#include "scrollingpanel.h"

ScrollingPanel::ScrollingPanel(s16 x, s16 y, u16 width, u16 height, u32 flags, FontBase* font) : Gadget(x, y, width, height, flags, font) {
	_minScrollX = 0;
	_minScrollY = 0;
	_maxScrollX = 0;
	_maxScrollY = 0;
	_scrollX = 0;
	_scrollY = 0;
	_flags.permeable = true;
}

void ScrollingPanel::draw() {
	Gadget::draw();
}

void ScrollingPanel::draw(Rect clipRect) {
	GraphicsPort* port = newInternalGraphicsPort(clipRect);

	// Clear
	port->drawFilledRect(0, 0, _width, _height, _backColour);

	// Draw border
	port->drawBevelledRect(0, 0, _width, _height);
	delete port;
}

ScrollingPanel::~ScrollingPanel() {
}

void ScrollingPanel::scroll(s32 dx, s32 dy) {

	// Prevent scrolling outside boundaries
	if (_scrollX + dx < _minScrollX) {
		dx = _minScrollX - _scrollX;
	} else if (_scrollX + dx > _maxScrollX) {
		dx = _maxScrollX - _scrollX;
	}

	if (_scrollY + dy < _minScrollY) {
		dy = _minScrollY - _scrollY;
	} else if (_scrollY + dy > _maxScrollY) {
		dy = _maxScrollY - _scrollY;
	}

	// Perform scroll if necessary
	if ((dx != 0) || (dy != 0)) {

		// Horizontal scroll
		if (dx < 0) {

			// Scroll left (increasing x scrolls left)
			scrollLeft(dx);
		} else if (dx > 0) {

			// Scroll right (decreasing x scrolls right)
			scrollRight(dx);
		}

		// Vertical scroll
		if (dy > 0) {

			// Scroll down (increasing y scrolls downwards)
			scrollDown(dy);
		} else if (dy < 0) {

			// Scroll up (decreasing y scrolls upwards)
			scrollUp(dy);
		}

		// Scroll all child gadgets
		scrollChildren(dx, dy);
	}
}

void ScrollingPanel::scrollLeft(s32 dx) {

	// Flip dx
	dx = abs(dx);

	cacheVisibleRects();

	// Adjust scroll value			
	_scrollX -= dx;

	u8 screen = getPhysicalScreenNumber();
	u16 yStart = 0;
	u16 yEnd = 0;
	u16 width = 0;
	u16 x = 0;
	u16* copySource;
	u16* rowBuffer;

	// Loop through visible regions
	for (u8 i = 0; i < _visibleRegionCache.size(); i++) {

		// Create clone of rect
		Rect innerRect;
		innerRect.x = _visibleRegionCache[i].x;
		innerRect.y = _visibleRegionCache[i].y;
		innerRect.width = _visibleRegionCache[i].width;
		innerRect.height = _visibleRegionCache[i].height;

		// Clip rect to client region
		if (clipToClientRect(innerRect)) {

			// Ensure that area is large enough to contain the scroll
			if (innerRect.width > dx) {

				yStart = innerRect.y;
				yEnd = innerRect.y + innerRect.height;
				width = innerRect.width - dx;
				x = innerRect.x + dx;

				// Create a buffer in RAM as wide as a single row
				rowBuffer = new u16[width];

				// Loop through rows
				for (u16 y = yStart; y < yEnd; y++) {

					copySource = DrawBg[screen] + x + (y * SCREEN_WIDTH);

					// Copy row to buffer
					DMA_Copy(copySource, rowBuffer, width, DMA_16NOW);

					// Copy row back to screen
					DMA_Copy(rowBuffer, (copySource - dx), width, DMA_16NOW);
				}

				// Delete the row buffer
				delete [] rowBuffer;

				// Draw the newly visible region
				Rect newRegion;
				newRegion.x = innerRect.x + innerRect.width - dx;
				newRegion.y = innerRect.y;
				newRegion.width = dx;
				newRegion.height = innerRect.height;
				draw(newRegion);
			} else {
				// Just redraw region
				draw(innerRect);
			}
		}
	}
}

void ScrollingPanel::scrollRight(s32 dx) {

	cacheVisibleRects();

	// Adjust scroll value
	_scrollX += dx;

	u8 screen = getPhysicalScreenNumber();
	u16 yStart = 0;
	u16 yEnd = 0;
	u16 width = 0;
	u16 x = 0;
	u16* copySource;
	u16* rowBuffer;

	// Loop through visible regions
	for (u8 i = 0; i < _visibleRegionCache.size(); i++) {

		// Create clone of rect
		Rect innerRect;
		innerRect.x = _visibleRegionCache[i].x;
		innerRect.y = _visibleRegionCache[i].y;
		innerRect.width = _visibleRegionCache[i].width;
		innerRect.height = _visibleRegionCache[i].height;

		// Clip rect to client region
		if (clipToClientRect(innerRect)) {

			// Ensure that area is large enough to contain the scroll
			if (innerRect.width > dx) {

				yStart = innerRect.y;
				yEnd = innerRect.y + innerRect.height;
				width = innerRect.width - dx;
				x = innerRect.x;

				// Create a buffer in RAM as wide as a single row
				rowBuffer = new u16[width];

				// Loop through rows
				for (u16 y = yStart; y < yEnd; y++) {

					copySource = DrawBg[screen] + x + (y * SCREEN_WIDTH);

					// Copy row to buffer
					DMA_Copy(copySource, rowBuffer, width, DMA_16NOW);

					// Copy row back to screen
					DMA_Copy(rowBuffer, (copySource + dx), width, DMA_16NOW);
				}

				// Delete the row buffer
				delete [] rowBuffer;

				// Draw the newly visible region
				Rect newRegion;
				newRegion.x = innerRect.x;
				newRegion.y = innerRect.y;
				newRegion.width = dx;
				newRegion.height = innerRect.height;
				draw(newRegion);
			} else {
				// Just redraw region
				draw(innerRect);
			}
		}
	}
}

void ScrollingPanel::scrollUp(s32 dy) {

	// Copy rows upwards one by one from the second row downwards

	// Flip dy
	dy = abs(dy);

	cacheVisibleRects();

	// Adjust the scroll values
	_scrollY -= dy;

	u8 screen = getPhysicalScreenNumber();
	u16 yStart = 0;
	u16 yEnd = 0;
	u16 xStart = 0;
	u16* copySource;
	u16 width;

	// Loop through visible regions
	for (u8 i = 0; i < _visibleRegionCache.size(); i++) {

		// Create clone of rect
		Rect innerRect;
		innerRect.x = _visibleRegionCache[i].x;
		innerRect.y = _visibleRegionCache[i].y;
		innerRect.width = _visibleRegionCache[i].width;
		innerRect.height = _visibleRegionCache[i].height;

		// Clip rect to client region
		if (clipToClientRect(innerRect)) {

			// Ensure that area is large enough to contain the scroll
			if (innerRect.height > dy) {

				yStart = innerRect.y + dy;
				yEnd = innerRect.y + innerRect.height;
				xStart = innerRect.x;
				width = innerRect.width;

				copySource = DrawBg[screen] + xStart + (yStart * SCREEN_WIDTH);

				// Loop through rows
				for (u16 y = yStart; y < yEnd; y++) {
					DMA_Copy(copySource, (copySource - (dy * SCREEN_WIDTH)), width, DMA_16NOW);
					copySource += SCREEN_WIDTH;
				}

				// Draw newly visible region
				Rect newRegion;
				newRegion.x = innerRect.x;
				newRegion.y = innerRect.y + innerRect.height - dy;
				newRegion.width = innerRect.width;
				newRegion.height = dy;
				draw(newRegion);
			} else {
				// Just redraw region
				draw(innerRect);
			}
		}
	}
}

void ScrollingPanel::scrollDown(s32 dy) {
	// Copy rows downwards one by one from the penultimate row upwards

	cacheVisibleRects();

	// Adjust the scroll values
	_scrollY += dy;

	u8 screen = getPhysicalScreenNumber();
	u16 yStart = 0;
	s32 yEnd = 0;
	u16 xStart = 0;
	u16* copySource;
	u16 width;

	// Loop through visible regions
	for (u8 i = 0; i < _visibleRegionCache.size(); i++) {

		// Create clone of rect
		Rect innerRect;
		innerRect.x = _visibleRegionCache[i].x;
		innerRect.y = _visibleRegionCache[i].y;
		innerRect.width = _visibleRegionCache[i].width;
		innerRect.height = _visibleRegionCache[i].height;

		// Clip rect to client region
		if (clipToClientRect(innerRect)) {

			// Ensure that area is large enough to contain the scroll
			if (innerRect.height > dy) {

				yStart = innerRect.y + innerRect.height - dy - 1;
				yEnd = innerRect.y;
				xStart = innerRect.x;
				width = innerRect.width;

				copySource = DrawBg[screen] + xStart + (yStart * SCREEN_WIDTH);

				// Loop through rows
				for (u16 y = yStart; y >= yEnd; y--) {
					DMA_Copy(copySource, (copySource + (dy * SCREEN_WIDTH)), width, DMA_16NOW);
					copySource -= SCREEN_WIDTH;
				}

				// Draw newly visible region
				Rect newRegion;
				newRegion.x = innerRect.x;
				newRegion.y = innerRect.y;
				newRegion.width = innerRect.width;
				newRegion.height = dy;
				draw(newRegion);
			} else {
				// Just redraw region
				draw(innerRect);
			}
		}
	}
}

// Reposition child gadgets without redrawing
void ScrollingPanel::scrollChildren(s32 dx, s32 dy) {
	s16 gadgetX = 0;
	s16 gadgetY = 0;
	for (u8 i = 0; i < _gadgets.size(); i++) {
		gadgetX = (_gadgets[i]->getX() - getX()) + dx;
		gadgetY = (_gadgets[i]->getY() - getY()) + dy;

		_gadgets[i]->moveTo(gadgetX, gadgetY);
	}
}

bool ScrollingPanel::click(s16 x, s16 y) {

	if (_flags.enabled) {
		if (checkCollision(x, y)) {
			_clickedGadget = NULL;

			// Work out which gadget was clicked
			for (s16 i = _gadgets.size() - 1; i > -1; i--) {
				if (_gadgets[i]->click(x, y)) {
					break;
				}
			}

			// Did we click a gadget?
			if (_clickedGadget == NULL) {
				// Handle click on window
				Gadget::click(x, y);

				// Start dragging
				setDragging(x, y);
			}

			return true;
		}
	}

	return false;
}

bool ScrollingPanel::release(s16 x, s16 y) {

	if (_clickedGadget != NULL) {

		// Release clicked gadget
		_clickedGadget->release(x, y);

		return true;
	} else if (_flags.clicked) {

		// Handle release on window
		Gadget::release(x, y);

		return true;
	}

	return false;
}

bool ScrollingPanel::drag(s16 x, s16 y, s16 vX, s16 vY) {

	if (_flags.enabled) {

		// Where to process the drag event?
		if (_flags.dragging) {
			// Run drag on this

			// Perform scroll
			scroll(vX, vY);

			raiseDragEvent(x, y, vX, vY);

			return true;
		} else if (_clickedGadget != NULL) {

			// Run drag on children
			return _clickedGadget->drag(x, y, vX, vY);
		}
	}

	return false;
}


bool ScrollingPanel::keyPress(KeyCode keyCode) {

	if (_flags.enabled) {
		// Handle key press on window
		Gadget::keyPress(keyCode);

		if (_activeGadget != NULL) {
			// Run key press on active gadget
			_activeGadget->keyPress(keyCode);
		}

		return true;
	}

	return false;
}

bool ScrollingPanel::keyRelease(KeyCode keyCode) {

	if (_flags.enabled) {
		// Handle key release on window
		Gadget::keyRelease(keyCode);

		if (_activeGadget != NULL) {
			// Run key release on active gadget
			_activeGadget->keyRelease(keyCode);
		}

		return true;
	}

	return false;
}

void ScrollingPanel::lidClosed() {
	// Handle lid closed on window
	Gadget::lidClosed();

	// Run lid closed on all gadgets
	for (u8 i = 0; i < _gadgets.size(); i++) {
		_gadgets[i]->lidClosed();
	}
}

void ScrollingPanel::lidOpened() {
	// Handle lid closed on window
	Gadget::lidOpened();

	// Run lid closed on all gadgets
	for (u8 i = 0; i < _gadgets.size(); i++) {
		_gadgets[i]->lidOpened();
	}
}

bool ScrollingPanel::focus() {

	if (_flags.enabled) {
		if (!_flags.active) {

			// Handle focus gained on scrolling panel
			Gadget::focus();

			return true;
		}
	}
	return false;
}

bool ScrollingPanel::blur() {

	if (_flags.active) {

		// Handle focus lost on scrolling panel
		Gadget::blur();

		// Take focus away from child gadgets
		if (_activeGadget != NULL) {
			_activeGadget->blur();
			_activeGadget = NULL;
		}

		return true;
	}

	return false;
}

// Ensure clipping rect falls within the client region of the gadget
bool ScrollingPanel::clipToClientRect(Rect& clipRect) {

	// Get parent-relative client region
	Rect clientRect;
	getClientRect(clientRect);

	// Adjust rect to screen-relative co-ordinates
	clientRect.x += getX();
	clientRect.y += getY();

	// Get end co-ords of clipping rect
	s32 clipX2 = clipRect.x + clipRect.width - 1;
	s32 clipY2 = clipRect.y + clipRect.height - 1;

	// Clip x to client region
	if (clipRect.x < clientRect.x) {
		clipRect.x = clientRect.x;
	}

	// Clip y to client region
	if (clipRect.y < clientRect.y) {
		clipRect.y = clientRect.y;
	}

	// Clip width to client region
	if (clipX2 > (clientRect.x + clientRect.width) - 1) {
		clipX2 = (clientRect.x + clientRect.width) - 1;
	}

	// Clip height to client region
	if (clipY2 > (clientRect.y + clientRect.height) - 1) {
		clipY2 = (clientRect.y + clientRect.height) - 1;
	}

	// Clip height to screen height
	if ((clipY2 >= SCREEN_HEIGHT) && (clipY2 < TOP_SCREEN_Y_OFFSET)) {
		clipY2 = SCREEN_HEIGHT - 1;
	} else if (clipY2 >= TOP_SCREEN_Y_OFFSET + SCREEN_HEIGHT) {
		clipY2 = TOP_SCREEN_Y_OFFSET + SCREEN_HEIGHT - 1;
	}

	// Do we have a valid region?
	if ((clipX2 <= clipRect.x) || (clipY2 <= clipRect.y)) {
		return false;
	}

	// Region OK - update rect
	clipRect.width = (clipX2 - clipRect.x) + 1;
	clipRect.height = (clipY2 - clipRect.y) + 1;

	// Adjust y value to match screen
	if (clipRect.y > TOP_SCREEN_Y_OFFSET) {
		clipRect.y -= TOP_SCREEN_Y_OFFSET;
	}

	return true;
}
