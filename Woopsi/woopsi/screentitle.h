#ifndef _SCREEN_TITLE_H_
#define _SCREEN_TITLE_H_

#include <nds.h>
#include "gadget.h"

namespace WoopsiUI {

	/**
	 * Title bar for the AmigaScreen class.  Mimics the appearance of
	 * the title bar from Amiga Workbench 3.x.  Shows the screen's name
	 * and offers depth (z-order) and flip (physical display swapping)
	 * buttons.
	 */
	class ScreenTitle : public Gadget {

	public:

		/**
		 * Constructor.
		 * @param height The height of the title bar.
		 * @param text The text to display in the title bar.
		 * @param font The font to use for the title bar.
		 */
		ScreenTitle(u16 height, const char* text, FontBase* font = NULL);

		/**
		 * Override the Gadget::draw() method.
		 */
		virtual inline void draw() { Gadget::draw(); };

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the draw() function to draw all visible regions.
		 * @param clipRect The clipping region to draw.
		 * @see draw()
		 */
		virtual void draw(Rect clipRect);

		/**
		 * Give the gadget focus.
		 * @return True if the gadget received focus correctly.
		 */
		virtual bool focus();

		/**
		 * Remove focus from the gadget.
		 * @return True if the gadget lost focus correctly.
		 */
		virtual bool blur();

	protected:
		const char* _text;							/**< Text to display in the title bar */

		/**
		 * Destructor.
		 */
		virtual inline ~ScreenTitle() {	};

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline ScreenTitle(const ScreenTitle& screenTitle) : Gadget(screenTitle) { };
	};
}

#endif
