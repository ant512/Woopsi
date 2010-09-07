#ifndef _STICKY_BUTTON_H_
#define _STICKY_BUTTON_H_

#include <nds.h>
#include "button.h"
#include "gadgetstyle.h"

namespace WoopsiUI {

	/**
	 * Button that sticks down when clicked.
	 */
	class StickyButton : public Button {
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
		StickyButton(s16 x, s16 y, u16 width, u16 height, const WoopsiString& text, GadgetStyle* style = NULL);

		/**
		 * Sets the key's stuck down state.  If this is true, the key has a inwards-bevelled
		 * border when drawn.  If it is false, the key has an outwards-bevelled border.
		 * @param isStuckDown The new stuck down state.
		 */
		void setStuckDown(bool isStuckDown);

	protected:
		bool _isStuckDown;					/**< True if the key is stuck down (ie. is Ctrl key and is active) */

		/**
		 * Draws the outline of the button.
		 * @param port Graphics port to draw to.
		 */
		virtual void drawOutline(GraphicsPort* port);

		/**
		 * Destructor.
		 */
		virtual inline ~StickyButton() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline StickyButton(const StickyButton& button) : Button(button) { };
	};
}

#endif
