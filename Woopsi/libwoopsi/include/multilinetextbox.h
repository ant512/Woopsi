#ifndef _MULTILINE_TEXTBOX_H_
#define _MULTILINE_TEXTBOX_H_

#include <nds.h>
#include "scrollingpanel.h"
#include "gadgetstyle.h"
#include "woopsistring.h"
#include "document.h"
#include "keyboardeventhandler.h"
#include "textboxbase.h"

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
	class MultiLineTextBox : public TextBoxBase, public ScrollingPanel, public KeyboardEventHandler  {
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
		 * @param maxRows The maximum number of rows the textbox can track.  Adding
		 * text beyond this number will cause rows at the start of the text to be
		 * forgotten; text is essentially stored as a queue, and adding to the back
		 * of a full queue causes the front items to be popped off.  Setting this to
		 * 0 will make the textbox track only the visible rows.  Setting this to -1
		 * will make the textbox track all rows.
		 * @param style The style that the gadget should use.  If this is not
		 * specified, the gadget will use the values stored in the global
		 * defaultGadgetStyle object.  The gadget will copy the properties of
		 * the style into its own internal style object.
		 */
		MultiLineTextBox(s16 x, s16 y, u16 width, u16 height, const WoopsiString& text, s16 maxRows = 0, GadgetStyle* style = NULL);

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
		 * Returns a pointer to the Document object that contains the
		 * wrapped text used in the textbox.  It is used as the
		 * pre-processed data source for the textbox, and should
		 * not be altered.
		 * @return Pointer to the Text object.
		 */
		virtual inline const Document* getDocument() const { return _document; };

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
		 * @param source The keyboard that was pressed.
		 * @param key The key that was pressed.
		 */
		virtual void handleKeyboardPressEvent(WoopsiKeyboard& source, const WoopsiKey& key);

		/**
		 * Handle a keyboard repeat event.
		 * @param source The keyboard that was pressed.
		 * @param key The key that was pressed.
		 */
		virtual void handleKeyboardRepeatEvent(WoopsiKeyboard& source, const WoopsiKey& key);

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

	protected:
		Document* _document;			/**< Text object that manipulates and wraps the raw text string. */
		u8 _visibleRows;				/**< Total number of rows that the textbox can display at once. */
		s16 _maxRows;					/**< Maximum number of rows that the textbox should buffer. */
		u32 _topRow;					/**< Index of the top row of text currently displayed. */
		TextAlignmentHoriz _hAlignment;	/**< Horizontal alignment of the text. */
		TextAlignmentVert _vAlignment;	/**< Vertical alignment of the text. */
		s32 _cursorPos;					/**< Position of the cursor within the string. */
		bool _showCursor;				/**< Set to true to make cursor visible. */
		bool _opensKeyboard;			/**< Set to false to disable keyboard popup when double-clicked. */

		/**
		 * Get the co-ordinates of the cursor relative to the text.
		 * @param x Will be populated with the x co-ordinate of the cursor.
		 * @param y Will be populated with the y co-ordinate of the cursor.
		 */
		virtual void getCursorCoordinates(s16& x, s16& y) const;

		/**
		 * Gets the index of the character at the specified x co-ordinate in the
		 * specified row.
		 * @param x X co-ordinate of the character.
		 * @param rowIndex Index of the row containing the character.
 		 * @return The index of the character at the specified co-ordinate.
		 */
		virtual u32 getCharIndexAtCoordinate(s16 x, s32 rowIndex) const;

		/**
		 * Get the index of the character at the specified co-ordinates.
		 * @param x X co-ordinate of the character.
		 * @param y Y co-ordinate of the character.
		 * @return The index of the character at the specified co-ordinates.
		 */
		virtual u32 getCharIndexAtCoordinates(s16 x, s16 y) const;

		/**
		 * Get the row containing the specified Y co-ordinate.
		 * @param y Y co-ordinate to locate.
		 * @return The index of the row containing the specified Y co-ordinate.
		 */
		s32 getRowContainingCoordinate(s16 y) const;

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
		 * Move cursor one character to the left.
		 */
		virtual void moveCursorLeft();

		/**
		 * Move cursor one character to the right.
		 */
		virtual void moveCursorRight();

		/**
		 * Move cursor one row upwards.
		 */
		virtual void moveCursorUp();

		/**
		 * Move cursor one row downwards.
		 */
		virtual void moveCursorDown();

		/**
		 * Ensures that the textbox only contains the maximum allowed
		 * number of rows by culling any excess rows from the top of
		 * the text.
		 * @return True if lines were removed from the text; false if not.
		 */
		virtual bool cullTopLines();

		/**
		 * Ensures that the canvas height is the height of the gadget,
		 * if the gadget exceeds the size of the text, or the height of
		 * the text if the text exceeds the size of the gadget.
		 */
		virtual void limitCanvasHeight();

		/**
		 * Ensures that the canvas cannot scroll beyond its height.
		 */
		virtual void limitCanvasY();

		/**
		 * Jumps to the cursor co-ordinates of the text.
		 */
		void jumpToCursor();

		/**
		 * Jumps to the bottom of the text.
		 */
		void jumpToTextBottom();

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
		 * Opens the keyboard on the bottom display.
		 * @param x The x co-ordinates of the click.
		 * @param y The y co-ordinates of the click.
		 */
		virtual void onDoubleClick(s16 x, s16 y);
		
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
		 * Handles physical button presses.  Moves the cursor
		 * in the direction pressed.
		 * @param keyCode keyCode The key that was pressed.
		 */
		void processPhysicalKey(KeyCode keyCode);

		/**
		 * Handles keyboard key presses and key repeats.
		 * @param key Key that raised the event.
		 */
		virtual void processKey(const WoopsiKey& key);

		/**
		 * Gets the x position of a row of text based on the width of the row and the
		 * type of horizontal alignment currently set.
		 * @param row The index of the row.
		 * @return The x co-ordinate of the row.
		 */
		u8 getRowX(s32 row) const;

		/**
		 * Gets the y position of the specified row of text based on the type of
		 * vertical alignment currently set.
		 * @param row The row number to find the y co-ordinate of.
		 * @return The y co-ordinate of the specified row of text.
		 */
		s16 getRowY(s32 row) const;

		/**
		 * Gets the character under the cursor.
		 * @return The character under the cursor.
		 */
		u32 getCursorCodePoint() const;

		/**
		 * Works out the number of visible rows within the textbox.
		 */
		void calculateVisibleRows();

		/**
		 * Draws text.
		 * @param port The GraphicsPort to draw to.
		 */
		void drawText(GraphicsPort* port);

		/**
		 * Draws the cursor.
		 * @param port The GraphicsPort to draw to.
		 */
		void drawCursor(GraphicsPort* port);

		/**
		 * Draws a single line of text.
		 * @param port The GraphicsPort to draw to.
		 * @param row The index of the row to draw.
		 */
		void drawRow(GraphicsPort* port, s32 row);

		/**
		 * Destructor.
		 */
		inline virtual ~MultiLineTextBox() {
			delete _document;
			_document = NULL;
		}

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline MultiLineTextBox(const MultiLineTextBox& multiLineTextBox) : ScrollingPanel(multiLineTextBox) { };
	};
}

#endif
