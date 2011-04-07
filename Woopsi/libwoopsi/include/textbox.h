#ifndef _TEXTBOX_H_
#define _TEXTBOX_H_

#include <nds.h>
#include "label.h"
#include "woopsistring.h"
#include "gadgetstyle.h"
#include "keyboardeventhandler.h"
#include "gadgeteventargs.h"
#include "textboxbase.h"

namespace WoopsiUI {

	class WoopsiTimer;
	class WoopsiKey;

	/**
	 * Single-line textbox gadget.  Can align text both vertically and
	 * horizontally in different ways.  The gadget gains this functionality by
	 * inheriting from the Label class.  However, if the amount of text exceeds
	 * the dimensions of the gadget, the gadget will ignore its horizontal
	 * alignment settings and switch to left-aligned instead.  This ensures that
	 * moving the cursor over the text will scroll through it correctly.
	 */
	class TextBox : public TextBoxBase, public Label, public KeyboardEventHandler {
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
		 * Append new text to the end of the current text displayed in the
		 * label.
		 * @param text String to append.
		 */
		virtual void appendText(const WoopsiString& text);

		/**
		 * Remove all characters from the string from the start index onwards.
		 * @param startIndex Index to remove from.
		 */
		virtual void removeText(const u32 startIndex);

		/**
		 * Remove specified number of characters from the string from the
		 * start index onwards.
		 * @param startIndex Index to remove from.
		 * @param count Number of characters to remove.
		 */
		virtual void removeText(const u32 startIndex, const u32 count);

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
		 * Move the cursor to the text position specified.  0 indicates the
		 * start of the string.  If position is greater than the length of the
		 * string, the cursor is moved to the end of the string.
		 * @param position The new cursor position.
		 */
		virtual void moveCursorToPosition(const s32 position);

		/**
		 * Get the cursor position.  This is the index within the string that
		 * the cursor is currently positioned over.
		 * @return position The cursor position.
		 */
		virtual inline const s32 getCursorPosition() const { return _cursorPos; };

		/**
		 * Handle a keyboard press event.
		 * @param e The event data.
		 */
		virtual void handleKeyboardPressEvent(const KeyboardEventArgs& e);

		/**
		 * Handle a keyboard repeat event.
		 * @param e The event data.
		 */
		virtual void handleKeyboardRepeatEvent(const KeyboardEventArgs& e);

		/**
		 * Handle a key press event.
		 * @param e The event data.
		 */
		virtual void handleKeyPressEvent(const GadgetEventArgs& e);

		/**
		 * Handle a key repeat event.
		 * @param e The event data.
		 */
		virtual void handleKeyRepeatEvent(const GadgetEventArgs& e);

		/**
		 * Check if the textbox opens the keyboard popup when double-clicked.
		 * @return True if the keyboard pops up; false if not.
		 */
		inline bool opensKeyboard() const { return _opensKeyboard; };

		/**
		 * Disables the keyboard popup when double-clicked.
		 */
		inline void disableKeyboardPopup() { _opensKeyboard = false; };

		/**
		 * Enables the keyboard popup when double-clicked.
		 */
		inline void enableKeyboardPopup() { _opensKeyboard = true; };

		/**
		 * Insert the dimensions that this gadget wants to have into the rect
		 * passed in as a parameter.  All co-ordinates are relative to the
		 * gadget's parent.
		 * @param rect Reference to a rect to populate with data.
		 */
		virtual void getPreferredDimensions(Rect& rect) const;

	protected:
		s32 _cursorPos;					/**< Position of the cursor within the string. */
		bool _showCursor;				/**< Set to true to make cursor visible. */
		bool _opensKeyboard;			/**< Set to false to disable keyboard popup when double-clicked. */

		/**
		 * Marks the gadget for redraw.
		 */
		inline void onBlur();

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the redraw() function to draw all visible regions.
		 * @param port The GraphicsPort to draw to.
		 * @see redraw()
		 */
		virtual void drawContents(GraphicsPort* port);

		/**
		 * Moves the cursor without redrawing.
		 * @param position New cursor position.
		 */
		virtual void repositionCursor(const s32 position);

		/**
		 * Move the cursor to the specified co-ordinates.  The co-ordinates
		 * are expected to be the result of a click, and therefore in
		 * world-space rather than gadget-space.
		 */
		void moveCursorToClickLocation(s16 x, s16 y);

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the redraw() function to draw all visible regions.
		 * @param port The GraphicsPort to draw to.
		 * @see redraw()
		 */
		virtual void drawBorder(GraphicsPort* port);

		/**
		 * Moves the cursor to the clicked co-ordinates.
		 * @param x The x co-ordinates of the click.
		 * @param y The y co-ordinates of the click.
		 */
		virtual void onClick(s16 x, s16 y);

		/**
		 * Opens the keyboard on the bottom display.
		 * @param x The x co-ordinates of the click.
		 * @param y The y co-ordinates of the click.
		 */
		virtual void onDoubleClick(s16 x, s16 y);

		/**
		 * Moves the cursor left or right.
		 * @param keyCode The key that was pressed.
		 */
		virtual void onKeyPress(KeyCode keyCode);
		
		/**
		 * Moves the cursor left or right.
		 * @param keyCode The key that repeated.
		 */
		virtual void onKeyRepeat(KeyCode keyCode);

		/**
		 * Handles keyboard key presses and key repeats.
		 * @param key Key that raised the event.
		 */
		virtual void processKey(const WoopsiKey* key);

		/**
		 * Get the x co-ordinate of the cursor in pixels relative
		 * to the left-hand edge of the client rect.
		 * @return The x co-ordinate of the cursor in pixels.
		 */
		virtual const u16 getCursorXPos() const;

		/**
		 * Get the width of the cursor in pixels.
		 * @return The width of the cursor in pixels.
		 */
		virtual const u16 getCursorWidth() const;
		
		/**
		 * Calculate the horizontal position of the string based on its length
		 * and the alignment options.  Alignment options are overridden if the
		 * width of the string exceeds the width of the textbox.
		 */
		virtual void calculateTextPositionHorizontal();

		/**
		 * Marks the region covered by the cursor as damaged for redrawing.
		 */
		void markCursorRectDamaged();

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline TextBox(const TextBox& textbox) : Label(textbox) { };
	};
}

#endif
