#ifndef _SCROLLING_PANEL_H_
#define _SCROLLING_PANEL_H_

#include <cstdlib>
#include <nds.h>
#include "gadget.h"
#include "scrollablebase.h"
#include "gadgetstyle.h"

namespace WoopsiUI {

	/**
	 * Class containing a scrollable region.  Responds to stylus movement.  Uses hardware-
	 * accelerated copying to reduce the amount of drawing required, where possible.  Can
	 * contain sub-gadgets which will also be scrolled.
	 */
	class ScrollingPanel : public Gadget, public ScrollableBase {
	public:

		/**
		 * Constructor.
		 * @param x The x co-ordinate of the gadget.
		 * @param y The y co-ordinate of the gadget.
		 * @param width The width of the gadget.
		 * @param height The height of the gadget.
		 * @param flags The usual gadget flags.
		 * @param style The style that the gadget should use.  If this is not
		 * specified, the gadget will use the values stored in the global
		 * defaultGadgetStyle object.  The gadget will copy the properties of
		 * the style into its own internal style object.
		 */
		ScrollingPanel(s16 x, s16 y, u16 width, u16 height, u32 flags, GadgetStyle* style = NULL);

		/**
		 * Scroll the panel by the specified amounts.
		 * @param dx The horizontal distance to scroll.
		 * @param dy The vertical distance to scroll.
		 */
		virtual void scroll(s32 dx, s32 dy);
		
		/**
		 * Reposition the panel's scrolling region to the specified co-ordinates.
		 * @param x The new x co-ordinate of the scrolling region.
		 * @param y The new y co-ordinate of the scrolling region.
		 */
		virtual void jump(s32 x, s32 y);

	protected:
		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the redraw() function to draw all visible regions.
		 * @param port The GraphicsPort to draw to.
		 * @see redraw()
		 */
		virtual void drawContents(GraphicsPort* port);

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the redraw() function to draw all visible regions.
		 * @param port The GraphicsPort to draw to.
		 * @see redraw()
		 */
		virtual void drawBorder(GraphicsPort* port);

		/**
		 * Scrolls the panel to match the drag.
		 * @param x The x co-ordinate of the stylus.
		 * @param y The y co-ordinate of the stylus.
		 * @param vX The horizontal drag distance.
		 * @param vY The vertical drag distance.
		 */
		virtual void onDrag(s16 x, s16 y, s16 vX, s16 vY);
		
		/**
		 * Starts the dragging system.
		 * @param x The x co-ordinate of the click.
		 * @param y The y co-ordinate of the click.
		 */
		virtual void onClick(s16 x, s16 y);

		/**
		 * Scroll all child gadgets by the specified amounts.  Actually uses the gadget's
		 * moveTo() function to reposition them.
		 * @param dx The horizontal distance to scroll.
		 * @param dy The vertical distance to scroll.
		 */
		void scrollChildren(s32 dx, s32 dy);
		
		/**
		 * Draws all child gadgets within the supplied clipping region.
		 * @param clipRect The clipping region to draw within.
		 */
		void drawChildren(Rect clipRect);

		/**
		 * Destructor.
		 */
		virtual ~ScrollingPanel() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline ScrollingPanel(const ScrollingPanel& scrollingPanel) : Gadget(scrollingPanel) { };
	};
}

#endif
