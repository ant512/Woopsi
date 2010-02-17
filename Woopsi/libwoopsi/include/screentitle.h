#ifndef _SCREEN_TITLE_H_
#define _SCREEN_TITLE_H_

#include <nds.h>
#include "gadget.h"
#include "gadgetstyle.h"

namespace WoopsiUI {

	class Screen;

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
		 * @param screen Pointer to the screen that contains this gadget.
		 * @param style The style that the gadget should use.  If this is not
		 * specified, the gadget will use the values stored in the global
		 * defaultGadgetStyle object.  The gadget will copy the properties of
		 * the style into its own internal style object.
		 */
		ScreenTitle(u16 height, Screen* screen, GadgetStyle* style = NULL);

	protected:
		Screen* _screen;							/**< Pointer to the containing screen */

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the redraw() function to draw all visible regions.
		 * @param port The GraphicsPort to draw to.
		 * @see redraw()
		 */
		virtual void drawContents(GraphicsPort* port);

		/**
		 * Starts the dragging system.
		 * @param x The x co-ordinate of the click.
		 * @param y The y co-ordinate of the click.
		 */
		virtual void onClick(s16 x, s16 y);

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
