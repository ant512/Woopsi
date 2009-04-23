#ifndef _RECT_CACHE_H_
#define _RECT_CACHE_H_

#include "woopsiarray.h"
#include "gadget.h"

namespace WoopsiUI {

	class RectCache {
	public:
		RectCache(const Gadget* gadget);
		inline ~RectCache() { };

		void cache();
		inline void invalidate() { _invalid = true; };
		inline WoopsiArray<Gadget::Rect>* getEndRegions() { return &_endRegions; };
		inline WoopsiArray<Gadget::Rect>* getTopRegions() { return &_topRegions; };

		/**
		 * Works out which rectangles in the invalidRectangles list overlap this
		 * gadget, then cuts the rectangles into smaller pieces.  The overlapping
		 * pieces are pushed into validRects, and the non-overlapping pieces are
		 * pushed back into the invalidRects vector.
		 * @param invalidRectangles A vector of regions that need to be tested
		 * for collisions against this gadget; they represent regions that need
		 * to be redrawn.
		 * @param validRects A vector of regions that represents areas of the
		 * display that do not need to be redrawn.
		 * @param sender Pointer to the gadget that initiated the split.
		 */
		void splitRectangles(WoopsiArray<Gadget::Rect>* invalidRectangles, WoopsiArray<Gadget::Rect>* validRects, const Gadget* sender) const;

		/**
		 * Move any rectangles from the visibleRects list that overlap this gadget
		 * into the invisibleRects list.  Used during visible region calculations.
		 * @param visibleRects A vector of regions that are not overlapped.
		 * @param invisibleRects A vector of regions that are overlapped.
		 * @param gadget The gadget that requested the lists.
		 * @see splitRectangles()
		 */
		void removeOverlappedRects(WoopsiArray<Gadget::Rect>* visibleRects, WoopsiArray<Gadget::Rect>* invisibleRects, const Gadget* gadget) const;

	private:
		WoopsiArray<Gadget::Rect> _topRegions;		/**< List of the gadget's visible regions */
		WoopsiArray<Gadget::Rect> _endRegions;		/**< List of the gadget's visible regions with child rects removed */
		const Gadget* _gadget;						/**< Owning gadget */
		bool _invalid;								/**< True if the cache needs refreshing */
	};
}

#endif
