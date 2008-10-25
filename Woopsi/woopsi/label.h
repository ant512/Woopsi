#ifndef _LABEL_H_
#define _LABEL_H_

#include <nds.h>
#include "textwriter.h"
#include "gadget.h"
#include "woopsistring.h"

namespace WoopsiUI {

	/**
	 * Single-line label gadget.  Can align text both vertically and horizontally in
	 * different ways.
	 */
	class Label : public Gadget {
	public:

		/**
		 * Enum of horizontal alignment options.
		 */
		enum TextAlignmentHoriz {
			TEXT_ALIGNMENT_HORIZ_CENTRE = 0,	/**< Centre the text */
			TEXT_ALIGNMENT_HORIZ_LEFT = 1,		/**< Align left */
			TEXT_ALIGNMENT_HORIZ_RIGHT = 2		/**< Align right */
		};

		/**
		 * Enum of vertical alignment options.
		 */
		enum TextAlignmentVert {
			TEXT_ALIGNMENT_VERT_CENTRE = 0,		/**< Align to centre of textbox */
			TEXT_ALIGNMENT_VERT_TOP = 1,		/**< Align to top of textbox */
			TEXT_ALIGNMENT_VERT_BOTTOM = 2		/**< Align to bottom of textbox */
		};

		/**
		 * Constructor for a label containing a string.
		 * @param x The x co-ordinate of the text box, relative to its parent.
		 * @param y The y co-ordinate of the text box, relative to its parent.
		 * @param width The width of the textbox.
		 * @param height The height of the textbox.
		 * @param text Pointer to a string to display in the textbox.
		 * @param font The font to use in this text box.
		 */
		Label(s16 x, s16 y, u16 width, u16 height, const char* text, FontBase* font = NULL);
		
		/**
		 * Constructor for a label containing a single character.
		 * @param x The x co-ordinate of the text box, relative to its parent.
		 * @param y The y co-ordinate of the text box, relative to its parent.
		 * @param width The width of the textbox.
		 * @param height The height of the textbox.
		 * @param letter Single character to display in the textbox.
		 * @param font The font to use in this text box.
		 */
		Label(s16 x, s16 y, u16 width, u16 height, const char letter, FontBase* font = NULL);
		
		/**
		 * Draw the region of the label within the clipping rect. Should not be called
		 * directly.
		 * @param clipRect The clipping rect to limit drawing to.
		 */
		virtual void draw(Rect clipRect);
		
		/**
		 * Draw all visible regions of the label.
		 */
		virtual inline void draw() { Gadget::draw(); };

		/**
		 * Set the horizontal alignment of text within the label.
		 * @param alignment The horizontal position of the text.
		 */
		virtual void setTextAlignmentHoriz(TextAlignmentHoriz alignment);
		
		/**
		 * Set the vertical alignment of text within the label.
		 * @param alignment The vertical position of the text.
		 */
		virtual void setTextAlignmentVert(TextAlignmentVert alignment);
		
		/**
		 * Returns a pointer to the string shown in the label.
		 * @return Pointer to the string.
		 */
		virtual inline const char* getText() const { return _text->getCharArray(); };
		
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
		 * Resize the gadget to the new dimensions.
		 * @param width The new width.
		 * @param height The new height.
		 * @return True if the resize was successful.
		 */
		virtual bool resize(u16 width, u16 height);

		/**
		 * Insert the dimensions that this gadget wants to have into the rect
		 * passed in as a parameter.  All co-ordinates are relative to the gadget's
		 * parent.
		 * @param rect Reference to a rect to populate with data.
		 */
		virtual void getPreferredDimensions(Rect& rect) const;

	protected:
		WoopsiString* _text;					/**< Text that the textbox will display */
		u16 _textX;								/**< X co-ordinate of the text relative to the gadget */
		u16 _textY;								/**< Y co-ordinate of the text relative to the gadget */
		u8 _padding;							/**< Padding around the text in pixels */
		TextAlignmentHoriz _hAlignment;			/**< Horizontal alignment of the text */
		TextAlignmentVert _vAlignment;			/**< Vertical alignment of the text */

		/**
		 * Calculate the position of the string based on its length and the alignment options.
		 */
		virtual void calculateTextPosition();

		/**
		 * Destructor.
		 */
		virtual inline ~Label() {
			delete _text;
			_text = NULL;
		};

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline Label(const Label& label) : Gadget(label) { };
	};
}

#endif
