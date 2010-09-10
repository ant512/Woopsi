#ifndef _DECORATION_GLYPH_BUTTON_H_
#define _DECORATION_GLYPH_BUTTON_H_

#include <nds.h>
#include "button.h"
#include "gadgetstyle.h"

namespace WoopsiUI {

	/**
	 * Class representing a button that will display a glyph and form part of
	 * another gadget's border decorations.  Examples include the screen
	 * flip/depth buttons.
	 */
	class DecorationGlyphButton : public Button {

	public:
		/**
		 * Constructor.
		 * @param x The x co-ordinate of the button.
		 * @param y The y co-ordinate of the button.
		 * @param width The width of the button.
		 * @param height The height of the button.
		 * @param normalGlyph Character to display when unclicked.
		 * @param clickedGlyph Character to display when clicked.
		 * @param style The style that the button should use.  If this is not
		 * specified, the button will use the values stored in the global
		 * defaultGadgetStyle object.  The button will copy the properties of
		 * the style into its own internal style object.
		 */
		DecorationGlyphButton(s16 x, s16 y, u16 width, u16 height, char normalGlyph, char clickedGlyph, GadgetStyle* style = NULL);

	protected:
		char _normalGlyph;						/**< Character to display when unclicked */
		char _clickedGlyph;						/**< Character to display when clicked */

		/**
		 * Draws the outline of the button.
		 * @param port Graphics port to draw to.
		 */
		virtual void drawOutline(GraphicsPort* port);

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
		 * Marks the button for redraw.
		 */
		virtual void onFocus();

		/**
		 * Destructor.
		 */
		virtual inline ~DecorationGlyphButton() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline DecorationGlyphButton(const DecorationGlyphButton& decorationGlyphButton) : Button(decorationGlyphButton) { };
	};
}

#endif
