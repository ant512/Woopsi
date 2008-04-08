#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <nds.h>
#include "textbox.h"

using namespace std;

/**
 * Clickable button gadget.  Displays text within the button.
 */
class Button : public Textbox {
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
	Button(s16 x, s16 y, u16 width, u16 height, char* text, FontBase* font = NULL);

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

protected:

	/**
	 * Destructor.
	 */
	virtual inline ~Button() { };
};

#endif
