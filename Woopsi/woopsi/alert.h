// TODO: Resize

#ifndef _ALERT_H_
#define _ALERT_H_

#include <nds.h>
#include "amigawindow.h"

namespace WoopsiUI {

	class Button;
	class MultiLineTextBox;

	/**
	 * Class providing a window containing a textbox and an OK button.  The contents of the textbox and the title
	 * of the window can be set in the constructor.  Designed to show short messages to the user.
	 * The contents of the window will automatically resize to fit the dimensions of the window.
	 */
	class Alert : public AmigaWindow {
	public:

		/**
		 * Constructor.
		 * @param x The x co-ordinate of the window.
		 * @param y The y co-ordinate of the window.
		 * @param width The width of the window.
		 * @param height The height of the window.
		 * @param title The title of the window.
		 * @param text The text to display in the window.
		 * @param font Optional font to use for text output.
		 */
		Alert(s16 x, s16 y, u16 width, u16 height, const char* title, const char* text, FontBase* font = NULL);

		/**
		 * Handles events raised by its sub-gadgets.
		 */
		virtual bool handleEvent(const EventArgs& e);

	protected:
		Button* _button;					/**< Pointer to the OK button */
		MultiLineTextBox* _textBox;			/**< Pointer to the alert message box */

		/**
		 * Destructor.
		 */
		virtual inline ~Alert() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline Alert(const Alert& alert) : AmigaWindow(alert) { };
	};
}

#endif
