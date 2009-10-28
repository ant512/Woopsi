#ifndef _RECT_CACHE_H_
#define _RECT_CACHE_H_

#include "woopsiarray.h"
#include "gadget.h"

namespace WoopsiUI {

	/**
	 * Maintains a list of foreground (ie. above children) and background (with
	 * child overlapped-rects removed) rectangles representing the visible portions
	 * of a gadget.
	 */
	class RectCache {
	public:

		/**
		 * Constructor.
		 * @param gadget Gadget that contains the rect cache.
		 */
		RectCache(const Gadget* gadget);

		/**
		 * Destructor.
		 */
		inline ~RectCache() { };

		/**
		 * Rebuild the cache if it is invalid.
		 */
		void cache();

		/**
		 * Invalidates the cache.
		 */
		inline void invalidate() {
			_foregroundInvalid = true;
			_backgroundInvalid = true;
		};

		/**
		 * Return the list of background regions.  These are regions that are not overlapped by
		 * child gadgets.
		 * @return The list of background regions.
		 */
		inline WoopsiArray<Gadget::Rect>* getBackgroundRegions() { return &_backgroundRegions; };

		/**
		 * Return the list of foreground regions.  These are regions that represent the entire
		 * visible surface of the gadget, including any regions that are actually overlapped by
		 * child gadgets.
		 * @return The list of foreground regions.
		 */
		inline WoopsiArray<Gadget::Rect>* getForegroundRegions() { return &_foregroundRegions; };

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
		WoopsiArray<Gadget::Rect> _foregroundRegions;		/**< List of the gadget's visible regions */
		WoopsiArray<Gadget::Rect> _backgroundRegions;		/**< List of the gadget's visible regions with child rects removed */
		const Gadget* _gadget;								/**< Owning gadget */
		bool _foregroundInvalid;							/**< True if the foreground cache needs refreshing */
		bool _backgroundInvalid;							/**< True if the background cache needs refreshing */

		/**
		 * Cache the foreground regions.
		 */
		void cacheForegroundRegions();

		/**
		 * Cache the background regions.
		 */
		void cacheBackgroundRegions();

	};
}

#endif
