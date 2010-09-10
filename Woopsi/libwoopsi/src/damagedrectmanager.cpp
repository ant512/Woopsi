#include "damagedrectmanager.h"
#include "gadget.h"

using namespace WoopsiUI;

DamagedRectManager::DamagedRectManager(Gadget* gadget) {
	_gadget = gadget;
}

DamagedRectManager::~DamagedRectManager() {
}

void DamagedRectManager::addDamagedRect(const Rect& rect) {

	WoopsiArray<Rect> newRects(4);
	WoopsiArray<Rect> remainingRects(4);
	Rect intersection;

	newRects.push_back(rect);

	// Ensure that the new rect does not overlap any existing rects - we only
	// want to draw each region once
	for (s32 i = 0; i < _damagedRects.size(); ++i) {
		for (s32 j = 0; j < newRects.size(); ++j) {

			if (_damagedRects[i].splitIntersection(newRects[j], intersection, &remainingRects)) {
				// Intersection contains the part of the new rect that is already known to be damaged
				// and can be discarded.  remainingRects contains the rects that still need to be examined

				newRects.erase(j);
				j--;

				// Insert non-overlapping rects to the front of the array so that they are not
				// examined again for this particular damaged rect
				for (s32 k = 0; k < remainingRects.size(); ++k) {
					newRects.insert(0, remainingRects[k]);
					j++;
				}

				remainingRects.clear();
			}
		}
	}

	// Add any non-overlapping rects into the damaged rect array
	for (s32 i = 0; i < newRects.size(); ++i) {
		_damagedRects.push_back(newRects[i]);
	}
}

void DamagedRectManager::redraw() {
	drawRects(_gadget, &_damagedRects);
}
			
void DamagedRectManager::drawRects(Gadget* gadget, WoopsiArray<Rect>* damagedRects) {
	
	Rect gadgetRect;
	Rect damagedRect;
	Rect intersection;
	
	gadget->getRectClippedToHierarchy(gadgetRect);
	
	WoopsiArray<Rect> remainingRects(4);
	WoopsiArray<Rect> subRects(4);
	
	// Work out which of the damaged rects collide with the current gadget
	for (s32 i = 0; i < damagedRects->size(); ++i) {
		damagedRect = damagedRects->at(i);
		
		// Work out which part of the damaged rect intersects the current gadget
		if (gadgetRect.splitIntersection(damagedRect, intersection, &remainingRects)) {
			damagedRects->erase(i);
			i--;
			
			// Add the non-intersecting parts of the damaged rect back into the
			// list of undrawn rects
			for (s32 j = 0; j < remainingRects.size(); ++j) {
				damagedRects->insert(0, remainingRects[j]);
				i++;
			}
			
			remainingRects.clear();
			
			// Get children to draw all parts of themselves that intersect the
			// intersection we've found.
			subRects.push_back(intersection);
			
			for (s32 j = gadget->getChildCount() - 1; j >= 0; --j) {
				drawRects(gadget->getChild(j), &subRects);
				
				// Abort if all rects have been drawn
				if (subRects.size() == 0) break;
			}
			
			// Children have drawn themselves; anything left in the subRects
			// array must overlap this gadget
			for (s32 j = 0; j < subRects.size(); ++j) {
				gadget->redraw(subRects[j]);
			}
			
			subRects.clear();
		}
	}
}		
