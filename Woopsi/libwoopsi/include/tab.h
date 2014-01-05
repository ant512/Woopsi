#ifndef _TAB_H_
#define _TAB_H_

#include <nds.h>
#include "stickybutton.h"
#include "gadgetstyle.h"

namespace WoopsiUI {

	/**
	 * Tab gadget.  Should be used only as part of a tab group.
	 */
	class Tab : public StickyButton {
	public:

		/**
		 * Constructor for a tab.
		 * @param x The x co-ordinate of the tab, relative to its parent.
		 * @param y The y co-ordinate of the tab, relative to its parent.
		 * @param width The width of the tab.
		 * @param height The height of the tab.
		 * @param text Pointer to a string to display in the tab.
		 * @param style The style that the gadget should use.  If this is not
		 * specified, the gadget will use the values stored in the global
		 * defaultGadgetStyle object.  The gadget will copy the properties of
		 * the style into its own internal style object.
		 */
		Tab(s16 x, s16 y, u16 width, u16 height, const WoopsiString& text, GadgetStyle* style = NULL);
		
		void activate();
		void deactivate();

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
		 * @param port The GraphicsPort to draw to.
		 */
		virtual void drawBorder(GraphicsPort* port);
		
		/**
		 * Draws the outline of the tab.
		 * @param port Graphics port to draw to.
		 */
		virtual void drawOutline(GraphicsPort* port);
		
		/**
		 * Tab sticks down when clicked.
		 * @param x The x co-ordinate of the click.
		 * @param y The y co-ordinate of the click.
		 */
		virtual void onClick(s16 x, s16 y);

		/**
		 * Destructor.
		 */
		virtual inline ~Tab() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline Tab(const Tab& tab) : StickyButton(tab) { };
	};
}

#endif
