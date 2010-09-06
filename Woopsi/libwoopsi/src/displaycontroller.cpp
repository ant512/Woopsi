#include "displaycontroller.h"
#include "gadget.h"

using namespace WoopsiUI;

DisplayController::DisplayController(Gadget* gadget) {
	_gadget = gadget;
}

DisplayController::~DisplayController() {
}

void DisplayController::addDamagedRect(const Rect& rect) {
	_damagedRects.push_back(rect);
}

void DisplayController::redraw(Gadget* gadget) {

	if (_damagedRects.size() == 0) return;

	if (gadget == NULL) gadget = _gadget;

	// Draw all visible child portions first
	for (s32 i = gadget->getChildCount(); i >= 0; --i) {
		if (!gadget->getChild(i)->isDrawingEnabled()) continue;

		redraw(gadget->getChild(i));

		if (_damagedRects.size() == 0) return;
	}

	// Draw visible portions of this gadget
	for (s32 i = 0; i < _damagedRects.size(); ++i) {
		Rect gadgetRect;
		gadget->getRectClippedToHierarchy(gadgetRect);

		if (_damagedRects[i].intersects(gadgetRect)) {
			Rect intersection;
			Rect drawRect = _damagedRects[i];
			WoopsiArray<Rect> tmpRemainingRects;

			_damagedRects.erase(i);
			i--;

			// Locate the part of this damaged rect that intersects the current gadget
			if (gadgetRect.splitIntersection(drawRect, intersection, &tmpRemainingRects)) {

				// Add all rects that don't intersect this gadget to the list that still
				// need to be drawn
				i += tmpRemainingRects.size();
				for (s32 j = 0; j < tmpRemainingRects.size(); ++j) {
					_damagedRects.insert(0, tmpRemainingRects[j]);
				}

				// Order the gadget to redraw this region of itself
				gadget->redraw(intersection);
			}
		}
	}
}
