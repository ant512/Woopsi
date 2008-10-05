#ifndef _TEXTBOX_H_
#define _TEXTBOX_H_

#include <nds.h>
#include "textwriter.h"
#include "gadget.h"

using namespace std;

/**
 * Single-line textbox gadget.  Can align text both vertically and horizontally in
 * different ways.
 */
class TextBox : public Gadget {
public:

	/**
	 * Enum of horizontal alignment options.
	 */
	enum TextAlignmentHoriz {
		TEXT_ALIGNMENT_HORIZ_CENTRE = 0,		/**< Centre the text */
		TEXT_ALIGNMENT_HORIZ_LEFT = 1,		/**< Align left */
		TEXT_ALIGNMENT_HORIZ_RIGHT = 2		/**< Align right */
	};

	/**
	 * Enum of vertical alignment options.
	 */
	enum TextAlignmentVert {
		TEXT_ALIGNMENT_VERT_CENTRE = 0,		/**< Align to centre of textbox */
		TEXT_ALIGNMENT_VERT_TOP = 1,			/**< Align to top of textbox */
		TEXT_ALIGNMENT_VERT_BOTTOM = 2		/**< Align to bottom of textbox */
	};

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
	 * Returns a pointer to the string shown in the textbox.
	 * @return Pointer to the string.
	 */
	virtual inline const char* getText() const { return _text; };
	
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
	 * Append new text to the end of the current text displayed in the textbox.
	 * @param text String to append.
	 */
	virtual void appendText(const char* text);

	/**
	 * Append new text to the end of the current text displayed in the textbox.
	 * @param text Char to append.
	 */
	virtual void appendText(const char text);

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

	/**
	 * Move the cursor to the text position specified.  0 indicates the start
	 * of the string.  If position is greater than the length of the string,
	 * the cursor is moved to the end of the string.
	 * @param position The new cursor position.
	 */
	virtual void moveCursorToPosition(const u32 position);

protected:
	char* _text;							/**< String to display in the textbox */
	u16 _textX;								/**< X co-ordinate of the text relative to the gadget */
	u16 _textY;								/**< Y co-ordinate of the text relative to the gadget */
	u8 _padding;							/**< Padding around the text in pixels */
	TextAlignmentHoriz _hAlignment;			/**< Horizontal alignment of the text */
	TextAlignmentVert _vAlignment;			/**< Vertical alignment of the text */
	u32 _cursorPos;							/**< Position of the cursor within the string */

	/**
	 * Calculate the position of the string based on its length and the alignment options.
	 */
	virtual void calculateTextPosition();

	/**
	 * Destructor.
	 */
	virtual inline ~TextBox() {
		delete[] _text;
		_text = NULL;
	};

	/**
	 * Copy constructor is protected to prevent usage.
	 */
	inline TextBox(const TextBox& textbox) : Gadget(textbox) { };
};

#endif
