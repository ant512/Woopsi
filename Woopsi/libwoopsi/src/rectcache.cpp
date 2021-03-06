#include "rectcache.h"
#include "woopsi.h"
#include "damagedrectmanager.h"

using namespace WoopsiUI;

RectCache::RectCache(const Gadget* gadget) {
	_gadget = gadget;
	_foregroundInvalid = true;
	_backgroundInvalid = true;
	
	_foregroundRegions = new WoopsiArray<Rect>(4);
	_backgroundRegions = new WoopsiArray<Rect>(4);
}

void RectCache::markRectsDamaged() const {
	
	// Abort if Woopsi does not exist.  This can occur if Woopsi is shutting
	// down
	if (woopsiApplication == NULL) return;
	
	for (s32 i = 0; i < _foregroundRegions->size(); ++i) {
		woopsiApplication->getDamagedRectManager()->addDamagedRect(_foregroundRegions->at(i));
	}
}

void RectCache::markRectDamaged(const Rect& rect) const {
	
	// Abort if Woopsi does not exist.  This can occur if Woopsi is shutting
	// down
	if (woopsiApplication == NULL) return;
	
	WoopsiArray<Rect> dirtyRects(4);
	WoopsiArray<Rect> remainderRects(4);
	Rect intersect;
	
	dirtyRects.push_back(rect);
	
	// Work out which parts of the dirty rect overlap the visible portions of
	// this gadget - we only want to attempt to redraw the visible portions of
	// the rect that overlap.
	for (s32 i = 0; i < _foregroundRegions->size(); ++i) {
		for (s32 j = 0; j < dirtyRects.size(); ++j) {
			if (_foregroundRegions->at(i).splitIntersection(dirtyRects[j], intersect, &remainderRects)) {
				dirtyRects.erase(j);
				i--;
				
				i += remainderRects.size();
				for (s32 k = 0; k < remainderRects.size(); ++k) {
					dirtyRects.push_back(remainderRects[k]);
				}
				
				woopsiApplication->getDamagedRectManager()->addDamagedRect(intersect);
				
				remainderRects.clear();
			}
		}
	}
}

void RectCache::cache() {
	cacheBackgroundRegions();
}

void RectCache::cacheForegroundRegions() {

	if (_foregroundInvalid) {
		
		// Use internal region cache to store the non-overlapped rectangles
		// We will use this to clip the gadget
		_foregroundRegions->clear();

		// Create pointer to a vector to store the overlapped rectangles
		// We can discard this later as we don't need it
		WoopsiArray<Rect>* invisibleRects = new WoopsiArray<Rect>(4);

		// Copy the clipped gadget dimensions into a rect
		Rect rect;
		_gadget->getRectClippedToHierarchy(rect);

		// Do we have a visible region left?
		if ((rect.height > 0) && (rect.width > 0)) {

			// Add rect to list
			_foregroundRegions->push_back(rect);
			
			// Request refresh
			if (_gadget->getParent() != NULL) {
				_gadget->getParent()->getRectCache()->removeOverlappedRects(_foregroundRegions, invisibleRects, _gadget);
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
		_backgroundRegions->clear();

		// Create pointer to a vector to store the overlapped rectangles
		// We can discard this later as we don't need it
		WoopsiArray<Rect>* invisibleRects = new WoopsiArray<Rect>(4);

		// Copy all foreground regions into the new vector
		for (s32 i = 0; i < _foregroundRegions->size(); i++) {
			_backgroundRegions->push_back(_foregroundRegions->at(i));
		}

		// Remove all child rects from the visible vector
		for (s32 i = 0; i < _gadget->getGadgetCount(); i++) {
			
			// Stop if there are no more regions to split
			if (_backgroundRegions->size() == 0) break;
			
			_gadget->getGadget(i)->getRectCache()->splitRectangles(_backgroundRegions, invisibleRects);
		}

		// Tidy up
		delete invisibleRects;

		_backgroundInvalid = false;
	}
}

// Split rectangles into valid and invalid sub-rectangles
// Used when calculating which portions of a gadget to draw
void RectCache::splitRectangles(WoopsiArray<Rect>* invalidRects, WoopsiArray<Rect>* validRects) const {

	// Bypass if the gadget is hidden - we do not want hidden gadgets to be able
	// to affect the structure of the screen
	if (_gadget->isHidden()) return;
	
	WoopsiArray<Rect> remainderRects(4);
	Rect checkRect;
	Rect intersection;
	Rect gadgetRect;

	// Check for collisions with any rectangles in the vector
	for (s32 i = 0; i < invalidRects->size(); ++i) {

		// Get rectangle to check
		checkRect = invalidRects->at(i);

		_gadget->getRectClippedToHierarchy(gadgetRect);

		if (gadgetRect.splitIntersection(checkRect, intersection, &remainderRects)) {
			invalidRects->erase(i);
			i--;

			i += remainderRects.size();

			for (int j = 0; j < remainderRects.size(); ++j) {
				invalidRects->insert(0, remainderRects[j]);
			}

			validRects->push_back(intersection);

			remainderRects.clear();
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

			// Remove any overlapped rectangles
			for (s32 i = gadgetIndex; i < parent->getGadgetCount(); i++) {
				if (visibleRects->size() > 0) {
					parent->getGadget(i)->getRectCache()->splitRectangles(visibleRects, invisibleRects);
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
