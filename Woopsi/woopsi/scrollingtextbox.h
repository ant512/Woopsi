#ifndef _SCROLLING_TEXTBOX_H_
#define _SCROLLING_TEXTBOX_H_

#include <nds.h>
#include "multilinetextbox.h"
#include "eventhandler.h"

namespace WoopsiUI {

	class ScrollbarVertical;

	/**
	 * Gadget containing a MultiLineTextBox and a vertical scrollbar.  Exposed
	 * methods are more or less identical to the methods exposed by the MultiLineTextBox
	 * to ensure that the two are interchangeable.
	 */
	class ScrollingTextBox : public Gadget, public EventHandler {
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
		 * @param font The font to use in this text box.
		 */
		ScrollingTextBox(s16 x, s16 y, u16 width, u16 height, const char* text, u32 flags, s16 maxRows = 0, FontBase* font = NULL);

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
		virtual void setText(const char* text);

		/**
		 * Set the text displayed in the textbox.
		 * @param text Character to display.
		 */
		virtual void setText(const char text);

		/**
		 * Append new text to the end of the current text
		 * displayed in the textbox.
		 * @param text String to append.
		 */
		virtual void appendText(const char* text);

		/**
		 * Append new text to the end of the current text displayed in the textbox.
		 * @param text Char to append.
		 */
		virtual void appendText(const char text);

		/**
		 * Set the font used in the textbox.
		 * @param font Pointer to the new font.
		 */
		virtual void setFont(FontBase* font);

		/**
		 * Resize the textbox to the new dimensions.
		 * @param width The new width.
		 * @param height The new height.
		 * @return True if the resize was successful.
		 */
		virtual bool resize(u16 width, u16 height);

		/**
		 * Handles events raised by its sub-gadgets.
		 */
		virtual bool handleEvent(const EventArgs& e);

		/**
		 * Draw the region of the textbox within the clipping rect.
		 * Should not be called directly.
		 * @param clipRect The clipping rect to limit drawing to.
		 */
		virtual void draw(Rect clipRect);

		/**
		 * Override the Gadget::draw() method.
		 */
		virtual inline void draw() { Gadget::draw(); };

	protected:
		MultiLineTextBox* _textbox;						/**< Pointer to the textbox */
		ScrollbarVertical* _scrollbar;					/**< Pointer to the scrollbar */
		u8 _scrollbarWidth;								/**< Width of the scrollbar */

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
