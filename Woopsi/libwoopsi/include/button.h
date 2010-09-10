#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <nds.h>
#include "label.h"
#include "gadgetstyle.h"

namespace WoopsiUI {

	/**
	 * Clickable button gadget.  Displays text within the button.
	 */
	class Button : public Label {
	public:

		/**
		 * Constructor for buttons that display a string.
		 * @param x The x co-ordinate of the button, relative to its parent.
		 * @param y The y co-ordinate of the button, relative to its parent.
		 * @param width The width of the button.
		 * @param height The height of the button.
		 * @param text The text for the button to display.
		 * @param style The style that the button should use.  If this is not
		 * specified, the button will use the values stored in the global
		 * defaultGadgetStyle object.  The button will copy the properties of
		 * the style into its own internal style object.
		 */
		Button(s16 x, s16 y, u16 width, u16 height, const WoopsiString& text, GadgetStyle* style = NULL);

	protected:

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
		 * @param x The x co-ordinate of the click.
		 * @param y The y co-ordinate of the click.
		 */
		virtual void onClick(s16 x, s16 y);
		
		/**
		 * Raises an action event and marks the button for redraw.
		 * @param x The x co-ordinate of the stylus.
		 * @param y The y co-ordinate of the stylus.
		 */
		virtual void onRelease(s16 x, s16 y);
		
		/**
		 * Marks the button for redraw.
		 * @param x The x co-ordinate of the stylus.
		 * @param y The y co-ordinate of the stylus.
		 */
		virtual void onReleaseOutside(s16 x, s16 y);

		/**
		 * Destructor.
		 */
		virtual inline ~Button() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline Button(const Button& button) : Label(button) { };
	};
}

#endif
