#ifndef _TEXTBOX_BASE_H_
#define _TEXTBOX_BASE_H_

#include <nds.h>
#include "woopsistring.h"
#include "gadgetstyle.h"
#include "keyboardeventhandler.h"
#include "gadgeteventargs.h"

namespace WoopsiUI {

	/**
	 * Defines the interface that textbox-like classes should implement.
	 */
	class TextBoxBase {
	public:
		
		/**
		 * Destructor.
		 */
		virtual TextBoxBase() { };

		/**
		 * Shows the cursor.
		 */
		virtual void showCursor() = 0;

		/**
		 * Hides the cursor.
		 */
		virtual void hideCursor() = 0;

		/**
		 * Set the text displayed in the label.
		 * @param text String to display.
		 */
		virtual void setText(const WoopsiString& text) = 0;
		
		/**
		 * Append new text to the end of the current text displayed in the
		 * label.
		 * @param text String to append.
		 */
		virtual void appendText(const WoopsiString& text) = 0;

		/**
		 * Remove all characters from the string from the start index onwards.
		 * @param startIndex Index to remove from.
		 */
		virtual void removeText(const u32 startIndex) = 0;

		/**
		 * Remove specified number of characters from the string from the
		 * start index onwards.
		 * @param startIndex Index to remove from.
		 * @param count Number of characters to remove.
		 */
		virtual void removeText(const u32 startIndex, const u32 count) = 0;

		/**
		 * Insert text at the specified index.
		 * @param text The text to insert.
		 * @param index Index at which to insert the text.
		 */
		virtual void insertText(const WoopsiString& text, const u32 index) = 0;

		/**
		 * Insert text at the current cursor position.
		 * @param text The text to insert.
		 */
		virtual void insertTextAtCursor(const WoopsiString& text) = 0;

		/**
		 * Move the cursor to the text position specified.  0 indicates the
		 * start of the string.  If position is greater than the length of the
		 * string, the cursor is moved to the end of the string.
		 * @param position The new cursor position.
		 */
		virtual void moveCursorToPosition(const s32 position) = 0;

		/**
		 * Get the cursor position.  This is the index within the string that
		 * the cursor is currently positioned over.
		 * @return position The cursor position.
		 */
		virtual const s32 getCursorPosition() const = 0;

		/**
		 * Check if the textbox opens the keyboard popup when double-clicked.
		 * @return True if the keyboard pops up; false if not.
		 */
		virtual bool opensKeyboard() const = 0;

		/**
		 * Disables the keyboard popup when double-clicked.
		 */
		virtual void disableKeyboardPopup() = 0;

		/**
		 * Enables the keyboard popup when double-clicked.
		 */
		virtual void enableKeyboardPopup() = 0;
	};
}

#endif
