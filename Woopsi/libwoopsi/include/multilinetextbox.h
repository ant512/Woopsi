#ifndef _MULTILINE_TEXTBOX_H_
#define _MULTILINE_TEXTBOX_H_

#include <nds.h>
#include "scrollingpanel.h"
#include "gadgetstyle.h"
#include "woopsistring.h"
#include "text.h"
#include "keyboardeventhandler.h"

namespace WoopsiUI {
	
	class WoopsiTimer;
	class WoopsiKey;

	/**
	 * Textbox that offers multiple lines of text.  Has scrolling
	 * capability and can be dragged using the stylus.  The text
	 * it contains can be changed or added to.  It can remember more
	 * rows of text than it can display, and these additional
	 * rows can be scrolled through.
	 */
	class MultiLineTextBox : public ScrollingPanel, public KeyboardEventHandler  {
	public:

		/**
		 * Enum of horizontal alignment options.
		 */
		enum TextAlignmentHoriz {
			TEXT_ALIGNMENT_HORIZ_CENTRE = 0,		/**< Centre the text */
			TEXT_ALIGNMENT_HORIZ_LEFT = 1,			/**< Align left */
			TEXT_ALIGNMENT_HORIZ_RIGHT = 2			/**< Align right */
		};

		/**
		 * Enum of vertical alignment options.
		 */
		enum TextAlignmentVert {
			TEXT_ALIGNMENT_VERT_CENTRE = 0,			/**< Align to centre of textbox */
			TEXT_ALIGNMENT_VERT_TOP = 1,			/**< Align to top of textbox */
			TEXT_ALIGNMENT_VERT_BOTTOM = 2			/**< Align to bottom of textbox */
		};

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
		MultiLineTextBox(s16 x, s16 y, u16 width, u16 height, const WoopsiString& text, u32 flags, s16 maxRows = 0, GadgetStyle* style = NULL);

		/**
		 * Set the horizontal alignment of text within the textbox.
		 * @param alignment The horizontal position of the text.
		 */
		virtual void setTextAlignmentHoriz(TextAlignmentHoriz alignment);

		/**
		 * Set the vertical alignment of text within the textbox.
		 * @param alignment The vertical position of the text.
		 */
		virtual void setTextAlignmentVert(TextAlignmentVert alignment);

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
		virtual inline const s32 getCursorPosition() const { return _cursorPos; };

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
		 * Handle a keyboard press event.
		 * @param e The event data.
		 */
		virtual void handleKeyboardPressEvent(const KeyboardEventArgs& e);

		/**
		 * Handle a keyboard repeat event.
		 * @param e The event data.
		 */
		virtual void handleKeyboardRepeatEvent(const KeyboardEventArgs& e);

	protected:
		Text* _text;						/**< Text object that manipulates and wraps the raw text string */
		u8 _visibleRows;					/**< Total number of rows that the textbox can display at once */
		s16 _maxRows;						/**< Maximum number of rows that the textbox should buffer */
		u32 _topRow;						/**< Index of the top row of text currently displayed */
		TextAlignmentHoriz _hAlignment;		/**< Horizontal alignment of the text */
		TextAlignmentVert _vAlignment;		/**< Vertical alignment of the text */
		s32 _cursorPos;						/**< Position of the cursor within the string */
		bool _showCursor;					/**< Set to true to make cursor visible */

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
		 * Resize the textbox to the new dimensions.
		 * @param width The new width.
		 * @param height The new height.
		 */
		virtual void onResize(u16 width, u16 height);
		
		/**
		 * Starts the dragging system.
		 * @param x The x co-ordinate of the click.
		 * @param y The y co-ordinate of the click.
		 */
		virtual void onClick(s16 x, s16 y);
		
		/**
		 * Moves the cursor.
		 * @param keyCode The key that was pressed.
		 */
		virtual void onKeyPress(KeyCode keyCode);
		
		/**
		 * Moves the cursor.
		 * @param keyCode The key that repeated.
		 */
		virtual void onKeyRepeat(KeyCode keyCode);

		/**
		 * Handles keyboard key presses and key repeats.
		 * @param key Key that raised the event.
		 */
		virtual void processKey(const WoopsiKey* key);

		/**
		 * Gets the x position of a row of text based on the width of the row and the
		 * type of horizontal alignment currently set.
		 * @param row The index of the row.
		 * @return The x co-ordinate of the row.
		 */
		u8 getRowX(s32 row);

		/**
		 * Gets the y position of the specified row of text based on the type of
		 * vertical alignment currently set.
		 * @param row The row number to find the y co-ordinate of.
		 * @return The y co-ordinate of the specified row of text.
		 */
		s16 getRowY(s32 row);

		/**
		 * Works out the number of visible rows within the textbox.
		 */
		void calculateVisibleRows();

		/**
		 * Clips text output for vertical top-aligned text and calls drawText().
		 * @param port The GraphicsPort to draw to.
		 */
		void drawTextTop(GraphicsPort* port);

		/**
		 * Draws text previously clipped by one of the drawTextXXX functions.
		 * @param port The GraphicsPort to draw to.
		 * @param topRow The index of top row of text to draw.
		 * @param bottomRow The index of the bottom row of text to draw.
		 */
		void drawText(GraphicsPort* port, s32 topRow, s32 bottomRow);

		/**
		 * Draws the cursor.
		 * @param port The GraphicsPort to draw to.
		 */
		void drawCursor(GraphicsPort* port);

		/**
		 * Destructor.
		 */
		inline virtual ~MultiLineTextBox() {
			delete _text;
			_text = NULL;
		}

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline MultiLineTextBox(const MultiLineTextBox& multiLineTextBox) : ScrollingPanel(multiLineTextBox) { };
	};
}

#endif
