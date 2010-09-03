#include "rectcache.h"

using namespace WoopsiUI;

RectCache::RectCache(const Gadget* gadget) {
	_gadget = gadget;
	_foregroundInvalid = true;
	_backgroundInvalid = true;
}

void RectCache::cache() {
	cacheBackgroundRegions();
}

void RectCache::cacheForegroundRegions() {

	if (_foregroundInvalid) {
		// Use internal region cache to store the non-overlapped rectangles
		// We will use this to clip the gadget
		_foregroundRegions.clear();

		// Create pointer to a vector to store the overlapped rectangles
		// We can discard this later as we don't need it
		WoopsiArray<Rect>* invisibleRects = new WoopsiArray<Rect>();

		// Copy the clipped gadget dimensions into a rect
		Rect rect;
		_gadget->getRectClippedToHierarchy(rect);

		// Do we have a visible region left?
		if ((rect.height > 0) && (rect.width > 0)) {

			// Add rect to list
			_foregroundRegions.push_back(rect);
			
			// Request refresh
			if (_gadget->getParent() != NULL) {
				_gadget->getParent()->getRectCache()->removeOverlappedRects(&_foregroundRegions, invisibleRects, _gadget);
			}
		}

		// Tidy up
		delete invisibleRects;

		_foregroundInvalid = false;
	}
}

void RectCache::cacheBackgroundRegions() {

	// Ensure that foreground is up to date
	cacheForegroundRegions();

	if (_backgroundInvalid) {

		// Cache visible regions not overlapped by children
		_backgroundRegions.clear();

		// Create pointer to a vector to store the overlapped rectangles
		// We can discard this later as we don't need it
		WoopsiArray<Rect>* invisibleRects = new WoopsiArray<Rect>();

		// Copy all foreground regions into the new vector
		for (s32 i = 0; i < _foregroundRegions.size(); i++) {
			_backgroundRegions.push_back(_foregroundRegions[i]);
		}

		// Remove all child rects from the visible vector
		for (s32 i = 0; i < _gadget->getChildCount(); i++) {
			if (_backgroundRegions.size() > 0) {
				_gadget->getChild(i)->getRectCache()->splitRectangles(&_backgroundRegions, invisibleRects, _gadget);
			} else {
				break;
			}
		}

		// Tidy up
		delete invisibleRects;

		_backgroundInvalid = false;
	}
}

// Split rectangles into valid and invalid sub-rectangles
// Used when calculating which portions of a gadget to draw
void RectCache::splitRectangles(WoopsiArray<Rect>* invalidRects, WoopsiArray<Rect>* validRects, const Gadget* sender) const {

	// Check for collisions with any rectangles in the vector
	for (s32 i = 0; i < invalidRects->size(); ++i) {

		// Get rectangle to check
		Rect checkRect = invalidRects->at(i);

		if (_gadget->checkCollision(checkRect)) {
			// Got a collision.  We need to split this rectangle

			// Get clipped dimensions of gadget
			Rect gadgetRect;
			_gadget->getRectClippedToHierarchy(gadgetRect);
			
			invalidRects->erase(i);
			i--;

			// Check for a non-overlapped rect on the left
			if (checkRect.x < gadgetRect.x) {
				Rect left;
				left.x = checkRect.x;
				left.y = checkRect.y;
				left.width = gadgetRect.x - checkRect.x;
				left.height = checkRect.height;
				
				// Insert the rect and make sure we don't check it again
				invalidRects->insert(0, left);
				i++;
				
				// Adjust the dimensions of the checked rect
				checkRect.x = gadgetRect.x;
				checkRect.width -= left.width;
			}
			
			// Check for a non-overlapped rect on the right
			if (checkRect.x + checkRect.width > gadgetRect.x + gadgetRect.width) {
				Rect right;
				right.x = gadgetRect.x + gadgetRect.width;
				right.y = checkRect.y;
				right.width = checkRect.width - (gadgetRect.x + gadgetRect.width - checkRect.x);
				right.height = checkRect.height;
				
				// Insert the rect and make sure we don't check it again
				invalidRects->insert(0, right);
				i++;
				
				// Adjust dimensions of the checked rect
				checkRect.width -= right.width;
			}
			
			// Check for a non-overlapped rect above
			if (checkRect.y < gadgetRect.y) {
				Rect top;
				top.x = checkRect.x;
				top.y = checkRect.y;
				top.width = checkRect.width;
				top.height = gadgetRect.y - checkRect.y;
				
				// Insert the rect and make sure we don't check it again
				invalidRects->insert(0, top);
				i++;
				
				// Adjust the dimensions of the checked rect
				checkRect.y = gadgetRect.y;
				checkRect.height -= top.height;
			}
			
			// Check for a non-overlapped rect below
			if (checkRect.y + checkRect.height > gadgetRect.y + gadgetRect.height) {
				Rect bottom;
				bottom.x = checkRect.x;
				bottom.y = gadgetRect.y + gadgetRect.height;
				bottom.width = checkRect.width;
				bottom.height = checkRect.height - (gadgetRect.y + gadgetRect.height - checkRect.y);
				
				// Insert the rect and make sure we don't check it again
				invalidRects->insert(0, bottom);
				i++;
				
				// Adjust dimensions of the checked rect
				checkRect.height -= bottom.height;
			}
			
			// If we have anything left over, it is the overlapped region
			if (checkRect.hasDimensions()) {
				validRects->push_back(checkRect);
			}
		}
	}
}

// Remove any rectangles that this gadget overlaps from the visible vector
// and add them to the invisible vector
// Called when drawing a gadget to check that no higher gadgets get overwritten
void RectCache::removeOverlappedRects(WoopsiArray<Rect>* visibleRects, WoopsiArray<Rect>* invisibleRects, const Gadget* gadget) const {

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
