#ifndef _WINDOW_BORDER_BUTTON_H_
#define _WINDOW_BORDER_BUTTON_H_

#include <nds.h>
#include "decorationglyphbutton.h"

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
		 * @param font The font to use.
		 */
		WindowBorderButton(s16 x, s16 y, u16 width, u16 height, char normalGlyph, char clickedGlyph, FontBase* font = NULL);

		/**
		 * Draws the gadget.
		 */
		virtual inline void draw() { Gadget::draw(); };

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the draw() function to draw all visible regions.
		 * @param clipRect The clipping region to draw.
		 * @see draw()
		 */
		virtual void draw(Rect clipRect);

	protected:
		/**
		 * Destructor.
		 */
		virtual inline ~WindowBorderButton() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline WindowBorderButton(const WindowBorderButton& windowBorderButton) : DecorationGlyphButton(windowBorderButton) { };
	};
}

#endif
