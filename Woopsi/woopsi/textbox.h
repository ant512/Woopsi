#ifndef _TEXTBOX_H_
#define _TEXTBOX_H_

#include <nds.h>
#include "textwriter.h"
#include "label.h"
#include "woopsistring.h"

namespace WoopsiUI {

	/**
	 * Single-line textbox gadget.  Can align text both vertically and horizontally in
	 * different ways.
	 */
	class TextBox : public Label {
	public:

		/**
		 * Constructor for a textbox containing a string.
		 * @param x The x co-ordinate of the text box, relative to its parent.
		 * @param y The y co-ordinate of the text box, relative to its parent.
		 * @param width The width of the textbox.
		 * @param height The height of the textbox.
		 * @param text Pointer to a string to display in the textbox.
		 * @param font The font to use in this text box.
		 */
		TextBox(s16 x, s16 y, u16 width, u16 height, const char* text, FontBase* font = NULL);
		
		/**
		 * Constructor for a textbox containing a single character.
		 * @param x The x co-ordinate of the text box, relative to its parent.
		 * @param y The y co-ordinate of the text box, relative to its parent.
		 * @param width The width of the textbox.
		 * @param height The height of the textbox.
		 * @param letter Single character to display in the textbox.
		 * @param font The font to use in this text box.
		 */
		TextBox(s16 x, s16 y, u16 width, u16 height, const char letter, FontBase* font = NULL);
		
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
		virtual void setText(const char* text);

		/**
		 * Set the text displayed in the label.
		 * @param text Character to display.
		 */
		virtual void setText(const char text);
		
		/**
		 * Append new text to the end of the current text displayed in the label.
		 * @param text String to append.
		 */
		virtual void appendText(const char* text);

		/**
		 * Append new text to the end of the current text displayed in the label.
		 * @param text Char to append.
		 */
		virtual void appendText(const char text);

		/**
		 * Insert text at the specified index.
		 * @param text The text to insert.
		 * @param index Index at which to insert the text.
		 */
		virtual void insertText(const char* text, const u32 index);

		/**
		 * Insert text at the specified index.
		 * @param text Char to insert.
		 * @param index Index at which to insert the char.
		 */
		virtual void insertText(const char text, const u32 index);

		/**
		 * Insert text at the current cursor position.
		 * @param text The text to insert.
		 */
		virtual void insertTextAtCursor(const char* text);

		/**
		 * Insert text at the current cursor position.
		 * @param text Char to insert.
		 */
		virtual void insertTextAtCursor(const char text);

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

	protected:
		u32 _cursorPos;							/**< Position of the cursor within the string */
		bool _showCursor;						/**< Set to true to make cursor visible */

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
