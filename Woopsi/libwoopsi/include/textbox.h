#ifndef _TEXTBOX_H_
#define _TEXTBOX_H_

#include <nds.h>
#include "label.h"
#include "woopsistring.h"
#include "gadgetstyle.h"
#include "gadgeteventhandler.h"
#include "gadgeteventargs.h"

namespace WoopsiUI {

	class WoopsiTimer;

	/**
	 * Single-line textbox gadget.  Can align text both vertically and horizontally in
	 * different ways.
	 */
	class TextBox : public Label, public GadgetEventHandler {
	public:

		/**
		 * Constructor for a textbox containing a string.
		 * @param x The x co-ordinate of the text box, relative to its parent.
		 * @param y The y co-ordinate of the text box, relative to its parent.
		 * @param width The width of the textbox.
		 * @param height The height of the textbox.
		 * @param text Pointer to a string to display in the textbox.
		 * @param style The style that the gadget should use.  If this is not
		 * specified, the gadget will use the values stored in the global
		 * defaultGadgetStyle object.  The gadget will copy the properties of
		 * the style into its own internal style object.
		 */
		TextBox(s16 x, s16 y, u16 width, u16 height, const WoopsiString& text, GadgetStyle* style = NULL);
		
		/**
		 * Draw the region of the textbox within the clipping rect. Should not be called
		 * directly.
		 * @param clipRect The clipping rect to limit drawing to.
		 */
		virtual void draw(Rect clipRect);

		/**
		 * Shows the cursor.
		 */
		virtual void showCursor();

		/**
		 * Hides the cursor.
		 */
		virtual void hideCursor();

		/**
		 * Set the text displayed in the label.
		 * @param text String to display.
		 */
		virtual void setText(const WoopsiString& text);
		
		/**
		 * Append new text to the end of the current text displayed in the label.
		 * @param text String to append.
		 */
		virtual void appendText(const WoopsiString& text);

		/**
		 * Insert text at the specified index.
		 * @param text The text to insert.
		 * @param index Index at which to insert the text.
		 */
		virtual void insertText(const WoopsiString& text, const u32 index);

		/**
		 * Insert text at the current cursor position.
		 * @param text The text to insert.
		 */
		virtual void insertTextAtCursor(const WoopsiString& text);

		/**
		 * Move the cursor to the text position specified.  0 indicates the start
		 * of the string.  If position is greater than the length of the string,
		 * the cursor is moved to the end of the string.
		 * @param position The new cursor position.
		 */
		virtual void moveCursorToPosition(const u32 position);

		/**
		 * Get the cursor position.  This is the index within the string that
		 * the cursor is currently positioned over.
		 * @return position The cursor position.
		 */
		virtual inline const u32 getCursorPosition() const { return _cursorPos; };

		/**
		 * Insert the properties of the space within this gadget that is available
		 * for children into the rect passed in as a parameter.
		 * All co-ordinates are relative to this gadget.
		 * @param rect Reference to a rect to populate with data.
		 */
		virtual void getClientRect(Rect& rect) const;

		/**
		 * Click this gadget at the supplied co-ordinates.
		 * @param x X co-ordinate of the click.
		 * @param y Y co-ordinate of the click.
		 * @return True if the click was successful.
		 */
		virtual bool click(s16 x, s16 y);

		/**
		 * Send a keypress to the gadget.
		 * @param keyCode The keycode to send to the gadget.
		 * @return True if the keypress was processed.
		 */
		virtual bool keyPress(KeyCode keyCode);

		/**
		 * Send a key release to the gadget.
		 * @param keyCode The keycode to send to the gadget.
		 * @return True if the key release was processed.
		 */
		virtual bool keyRelease(KeyCode keyCode);

		/**
		 * Handle a gadget action event.
		 * @param e The event data.
		 */
		virtual void handleActionEvent(const GadgetEventArgs& e);

	protected:
		u32 _cursorPos;							/**< Position of the cursor within the string. */
		bool _showCursor;						/**< Set to true to make cursor visible. */
		WoopsiTimer* _timer;					/**< Timer for handling dpad repeats. */
		u32 _initialRepeatTime;					/**< Time until held dpad starts to repeat. */
		u32 _secondaryRepeatTime;				/**< Time until dpad already repeating repeats again. */
		KeyCode _heldDirection;					/**< Currently held dpad direction. */

		/**
		 * Get the x co-ordinate of the cursor in pixels.
		 * @return The x co-ordinate of the cursor in pixels.
		 */
		virtual const u16 getCursorXPos() const;

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline TextBox(const TextBox& textbox) : Label(textbox) { };
	};
}

#endif
