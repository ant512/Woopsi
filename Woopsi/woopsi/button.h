#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <nds.h>
#include "label.h"

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
		 * @param font The font that the button should use.
		 */
		Button(s16 x, s16 y, u16 width, u16 height, const char* text, FontBase* font = NULL);

		/**
		 * Constructor for buttons that display a single char.
		 * @param x The x co-ordinate of the button, relative to its parent.
		 * @param y The y co-ordinate of the button, relative to its parent.
		 * @param width The width of the button.
		 * @param height The height of the button.
		 * @param letter The character for the button to display.
		 * @param font The font that the button should use.
		 */
		Button(s16 x, s16 y, u16 width, u16 height, char letter, FontBase* font = NULL);

		/**
		 * Draw the region of the textbox within the clipping rect. Should not be called
		 * directly.
		 * @param clipRect The clipping rect to limit drawing to.
		 */
		virtual void draw(Rect clipRect);
		
		/**
		 * Draw all visible regions of the textbox.
		 */
		virtual inline void draw() { Gadget::draw(); };

		/**
		 * Click this gadget at the supplied co-ordinates.
		 * @param x X co-ordinate of the click.
		 * @param y Y co-ordinate of the click.
		 * @return True if the click was successful.
		 */
		virtual bool click(s16 x, s16 y);
		
		/**
		 * Release this gadget at the supplied co-ordinates
		 * @param x X co-ordinate of the release.
		 * @param y Y co-ordinate of the release.
		 * @return True if the release was successful.
		 */
		virtual bool release(s16 x, s16 y);

	protected:

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
