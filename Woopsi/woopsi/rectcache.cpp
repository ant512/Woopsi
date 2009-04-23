#include "rectcache.h"

using namespace WoopsiUI;

RectCache::RectCache(const Gadget* gadget) {
	_gadget = gadget;
	_invalid = true;

}

void RectCache::cache() {

	if (_invalid) {
		// Use internal region cache to store the non-overlapped rectangles
		// We will use this to clip the gadget
		_topRegions.clear();

		// Create pointer to a vector to store the overlapped rectangles
		// We can discard this later as we don't need it
		WoopsiArray<Gadget::Rect>* invisibleRects = new WoopsiArray<Gadget::Rect>();

		// Copy the clipped gadget dimensions into a rect
		Gadget::Rect rect;
		_gadget->getRectClippedToHierarchy(rect);

		// Do we have a visible region left?
		if ((rect.height > 0) && (rect.width > 0)) {

			// Add rect to list
			_topRegions.push_back(rect);
			
			// Request refresh
			if (_gadget->getParent() != NULL) {
				_gadget->getParent()->getRectCache()->removeOverlappedRects(&_topRegions, invisibleRects, _gadget);
			}
		}

		invisibleRects->clear();

		// Cache visible regions not overlapped by children
		_endRegions.clear();

		// Copy all visible regions into the new vector
		for (s32 i = 0; i < _topRegions.size(); i++) {
			_endRegions.push_back(_topRegions[i]);
		}

		// Remove all child rects from the visible vector
		for (s32 i = 0; i < _gadget->getChildCount(); i++) {
			if (_endRegions.size() > 0) {
				_gadget->getChild(i)->getRectCache()->splitRectangles(&_endRegions, invisibleRects, _gadget);
			} else {
				break;
			}
		}

		// Tidy up
		delete invisibleRects;

		_invalid = false;
	}
}

// Split rectangles into valid and invalid sub-rectangles
// Used when calculating which portions of a gadget to draw
void RectCache::splitRectangles(WoopsiArray<Gadget::Rect>* invalidRects, WoopsiArray<Gadget::Rect>* validRects, const Gadget* sender) const {

	// Check for collisions with any rectangles in the vector
	for (s32 i = 0; i < invalidRects->size(); i++) {

		// Get rectangle to check
		Gadget::Rect checkRect = invalidRects->at(i);
		s16 splitX[4];
		s16 splitY[4];
		u32 rectXCount = 0;
		u32 rectYCount = 0;
		u32 overlapXRect = 0;
		u32 overlapYRect = 0;

		if (_gadget->checkCollision(checkRect.x, checkRect.y, checkRect.width, checkRect.height)) {
			// Got a collision.  We need to split this rectangle

			// Get clipped dimensions of gadget
			Gadget::Rect gadgetRect;
			_gadget->getRectClippedToHierarchy(gadgetRect);

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
						Gadget::Rect overlapRect;
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
						Gadget::Rect newRect;
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

// Remove any rectangles that this gadget overlaps from the visible vector
// and add them to the invisible vector
// Called when drawing a gadget to check that no higher gadgets get overwritten
void RectCache::removeOverlappedRects(WoopsiArray<Gadget::Rect>* visibleRects, WoopsiArray<Gadget::Rect>* invisibleRects, const Gadget* gadget) const {

	const Gadget* parent = _gadget;
	s32 gadgetIndex = -1;

	while ((gadget != NULL) && (parent != NULL)) {

		// Locate gadget in the list; we add one to the index to
		// ensure that we deal with the next gadget up in the z-order
		gadgetIndex = parent->getGadgetIndex(gadget) + 1;

		// Gadget should never be the bottom item on the screen
		if (gadgetIndex > 0) {

			// Remove any overlapped rectanglesg
			for (s32 i = gadgetIndex; i < parent->getChildCount(); i++) {
				if (visibleRects->size() > 0) {
					parent->getChild(i)->getRectCache()->splitRectangles(visibleRects, invisibleRects, gadget);
				} else {
					break;
				}
			}
		}

		if (visibleRects->size() > 0) {
			gadget = parent;

			if (parent != NULL) {
				parent = parent->getParent();
			}
		} else {
			return;
		}
	}
}
