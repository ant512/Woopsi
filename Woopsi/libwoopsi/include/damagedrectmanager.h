#ifndef _DAMAGED_RECT_MANAGER_
#define _DAMAGED_RECT_MANAGER_

#include "rect.h"
#include "woopsiarray.h"

namespace WoopsiUI {

	class Gadget;

	/**
	 * Manages damaged rects.  Keeps a list of damaged rects and redraws them
	 * when redraw() is called.
	 */
	class DamagedRectManager {
	public:
		
		/**
		 * Constructor.
		 * @param gadget The top-level gadget.  This should always be the Woopsi
		 * instance.
		 */
		DamagedRectManager(Gadget* gadget);
		
		/**
		 * Destructor.
		 */
		~DamagedRectManager();

		/**
		 * Add a damaged rect to the list.  The method automatically clips and
		 * splits the rect to ensure that only new regions are added to the
		 * list.
		 * @param rect The rect to add to the list.
		 */
		void addDamagedRect(const Rect& rect);
		
		/**
		 * Redraws all damaged rects.
		 */
		void redraw();

	private:
		WoopsiArray<Rect>* _damagedRects;		/**< List of damaged rects. */
		Gadget* _gadget;						/**< The top-level gadget. */
		
		/**
		 * Redraws all damaged rects.
		 * @param gadget The gadget to compare with the damaged region list to
		 * see if it intersects any regions.  If so, those regions are redrawn.
		 * @param damagedRects Damaged region list.
		 */
		void drawRects(Gadget* gadget, WoopsiArray<Rect>* damagedRects);
	};
}

#endif
