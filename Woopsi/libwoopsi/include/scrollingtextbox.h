#ifndef _SCROLLING_TEXTBOX_H_
#define _SCROLLING_TEXTBOX_H_

#include <nds.h>
#include "multilinetextbox.h"
#include "gadgeteventhandler.h"
#include "gadgetstyle.h"
#include "woopsistring.h"
#include "scrollablebase.h"

namespace WoopsiUI {

	class ScrollbarVertical;

	/**
	 * Gadget containing a MultiLineTextBox and a vertical scrollbar.  Exposed
	 * methods are more or less identical to the methods exposed by the
	 * MultiLineTextBox to ensure that the two are interchangeable.
	 */
	class ScrollingTextBox : public TextBoxBase, public Gadget, public ScrollableBase, public GadgetEventHandler {
	public:

		/**
		 * Constructor.
		 * @param x The x co-ordinate of the text box, relative to its parent.
		 * @param y The y co-ordinate of the text box, relative to its parent.
		 * @param width The width of the textbox.
		 * @param height The height of the textbox.
		 * @param text Pointer to a string to display in the textbox.
		 * @param flags Standard gadget flag options.
		 * @param maxRows The maximum number of rows the textbox can track.  Adding
		 * text beyond this number will cause rows at the start of the text to be
		 * forgotten; text is essentially stored as a queue, and adding to the back
		 * of a full queue causes the front items to be popped off.  Setting this to
		 * 0 will make the textbox track only the visible rows.
		 * @param style The style that the gadget should use.  If this is not
		 * specified, the gadget will use the values stored in the global
		 * defaultGadgetStyle object.  The gadget will copy the properties of
		 * the style into its own internal style object.
		 */
		ScrollingTextBox(s16 x, s16 y, u16 width, u16 height, const WoopsiString& text, u32 flags, s16 maxRows = 0, GadgetStyle* style = NULL);

		/**
		 * Set the horizontal alignment of text within the textbox.
		 * @param alignment The horizontal position of the text.
		 */
		virtual void setTextAlignmentHoriz(MultiLineTextBox::TextAlignmentHoriz alignment);

		/**
		 * Set the vertical alignment of text within the textbox.
		 * @param alignment The vertical position of the text.
		 */
		virtual void setTextAlignmentVert(MultiLineTextBox::TextAlignmentVert alignment);

		/**
		 * Returns the number of "pages" that the text spans.  A page
		 * is defined as the amount of text that can be displayed within
		 * the textbox at one time.
		 * @return The page count.
		 */
		virtual const u16 getPageCount() const;

		/**
		 * Returns the current page.
		 * @return The current page.
		 * @see getPageCount().
		 */
		virtual const u16 getCurrentPage() const;

		/**
		 * Returns a pointer to the Text object that contains the
		 * wrapped text used in the textbox.  It is used as the
		 * pre-processed data source for the textbox, and should
		 * not be altered.
		 * @return Pointer to the Text object.
		 */
		virtual const Text* getText() const;

		/**
		 * Set the text displayed in the textbox.
		 * @param text String to display.
		 */
		virtual void setText(const WoopsiString& text);

		/**
		 * Append new text to the end of the current text
		 * displayed in the textbox.
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
		 * Set the font used in the textbox.
		 * @param font Pointer to the new font.
		 */
		virtual void setFont(FontBase* font);

		/**
		 * Get the length of the text string.
		 * @return The length of the text string.
		 */
		virtual const u32 getTextLength() const;

		/**
		 * Shows the cursor.
		 */
		virtual void showCursor();

		/**
		 * Hides the cursor.
		 */
		virtual void hideCursor();
		
		/**
		 * Move the cursor to the text position specified.  0 indicates the start
		 * of the string.  If position is greater than the length of the string,
		 * the cursor is moved to the end of the string.
		 * @param position The new cursor position.
		 */
		virtual void moveCursorToPosition(const s32 position);

		/**
		 * Get the cursor position.  This is the index within the string that
		 * the cursor is currently positioned over.
		 * @return position The cursor position.
		 */
		virtual const s32 getCursorPosition() const;

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
		 * Handles events raised by its sub-gadgets.
		 * @param e Event arguments.
		 */
		virtual void handleValueChangeEvent(const GadgetEventArgs& e);

		/**
		 * Handles events raised by its sub-gadgets.
		 * @param e Event arguments.
		 */
		virtual void handleScrollEvent(const GadgetEventArgs& e);

		/**
		 * Check if the textbox opens the keyboard popup when double-clicked.
		 * @return True if the keyboard pops up; false if not.
		 */
		bool opensKeyboard() const { return _textbox->opensKeyboard(); };

		/**
		 * Disables the keyboard popup when double-clicked.
		 */
		void disableKeyboardPopup() { _textbox->disableKeyboardPopup(); };

		/**
		 * Enables the keyboard popup when double-clicked.
		 */
		void enableKeyboardPopup() { _textbox->enableKeyboardPopup(); };

		/**
		 * Gets the x co-ordinate of the virtual canvas.
		 * @return The x co-ordinate of the virtual canvas.
		 */
		virtual const s32 getCanvasX() const;
		
		/**
		 * Gets the y co-ordinate of the virtual canvas.
		 * @return The y co-ordinate of the virtual canvas.
		 */
		virtual const s32 getCanvasY() const;

		/**
		 * Gets the width of the virtual canvas.
		 * @return The width of the virtual canvas.
		 */
		virtual const s32 getCanvasWidth() const;
		
		/**
		 * Gets the height of the virtual canvas.
		 * @return The height of the virtual canvas.
		 */
		virtual const s32 getCanvasHeight() const;

		/**
		 * Scroll the panel by the specified amounts.
		 * @param dx The horizontal distance to scroll.
		 * @param dy The vertical distance to scroll.
		 */
		virtual void scroll(s32 dx, s32 dy);
		
		/**
		 * Reposition the panel's scrolling region to the specified co-ordinates.
		 * @param x The new x co-ordinate of the scrolling region.
		 * @param y The new y co-ordinate of the scrolling region.
		 */
		virtual void jump(s32 x, s32 y);

		/**
		 * Set whether or not horizontal scrolling is allowed.
		 * @param allow True to allow horizontal scrolling; false to deny it.
		 */
		virtual void setAllowsVerticalScroll(bool allow);

		/**
		 * Set whether or not horizontal scrolling is allowed.
		 * @param allow True to allow horizontal scrolling; false to deny it.
		 */
		virtual void setAllowsHorizontalScroll(bool allow);

		/**
		 * Sets the width of the virtual canvas.
		 * @param width The width of the virtual canvas.
		 */
		virtual void setCanvasWidth(const s32 width);
		
		/**
		 * Sets the height of the virtual canvas.
		 * @param height The height of the virtual canvas.
		 */
		virtual void setCanvasHeight(const s32 height);

		/**
		 * Returns true if vertical scrolling is allowed.
		 * @return True if vertical scrolling is allowed.
		 */
		virtual bool allowsVerticalScroll() const;

		/**
		 * Returns true if horizontal scrolling is allowed.
		 * @return True if horizontal scrolling is allowed.
		 */
		virtual bool allowsHorizontalScroll() const;

	protected:
		MultiLineTextBox* _textbox;						/**< Pointer to the textbox */
		ScrollbarVertical* _scrollbar;					/**< Pointer to the scrollbar */
		u8 _scrollbarWidth;								/**< Width of the scrollbar */

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the redraw() function to draw all visible regions.
		 * @param port The GraphicsPort to draw to.
		 * @see redraw()
		 */
		virtual void drawContents(GraphicsPort* port);

		/**
		 * Resize the textbox to the new dimensions.
		 * @param width The new width.
		 * @param height The new height.
		 */
		virtual void onResize(u16 width, u16 height);
		
		/**
		 * Destructor.
		 */
		virtual inline ~ScrollingTextBox() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline ScrollingTextBox(const ScrollingTextBox& scrollingTextBox) : Gadget(scrollingTextBox) { };
	};
}

#endif
