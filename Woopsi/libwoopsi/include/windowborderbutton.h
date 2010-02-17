#ifndef _WINDOW_BORDER_BUTTON_H_
#define _WINDOW_BORDER_BUTTON_H_

#include <nds.h>
#include "decorationglyphbutton.h"
#include "gadgetstyle.h"

namespace WoopsiUI {

	/**
	 * Class representing a button that will appear in the window border.
	 */
	class WindowBorderButton : public DecorationGlyphButton {
	public:

		/**
		 * Constructor.
		 * @param x The x co-ordinate of the gadget.
		 * @param y The y co-ordinate of the gadget.
		 * @param width The width of the gadget.
		 * @param height The height of the gadget.
		 * @param normalGlyph The glyph to display when the gadget is not clicked.
		 * @param clickedGlyph The glyph to display when the gadget is clicked.
		 * @param style The style that the gadget should use.  If this is not
		 * specified, the gadget will use the values stored in the global
		 * defaultGadgetStyle object.  The gadget will copy the properties of
		 * the style into its own internal style object.
		 */
		WindowBorderButton(s16 x, s16 y, u16 width, u16 height, char normalGlyph, char clickedGlyph, GadgetStyle* style = NULL);

	protected:
		/**
		 * Destructor.
		 */
		virtual inline ~WindowBorderButton() { };

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
		 * Copy constructor is protected to prevent usage.
		 */
		inline WindowBorderButton(const WindowBorderButton& windowBorderButton) : DecorationGlyphButton(windowBorderButton) { };
	};
}

#endif
