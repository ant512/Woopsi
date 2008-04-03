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
class Textbox : public Gadget {
public:

	/**
	 * Enum of horizontal alignment options.
	 */
	enum TextPositionHoriz {
		TEXT_POSITION_HORIZ_CENTRE = 0,		/**< Centre the text */
		TEXT_POSITION_HORIZ_LEFT = 1,		/**< Align left */
		TEXT_POSITION_HORIZ_RIGHT = 2		/**< Align right */
	};

	/**
	 * Enum of vertical alignment options.
	 */
	enum TextPositionVert {
		TEXT_POSITION_VERT_CENTRE = 0,		/**< Align to centre of textbox */
		TEXT_POSITION_VERT_TOP = 1,			/**< Align to top of textbox */
		TEXT_POSITION_VERT_BOTTOM = 2		/**< Align to bottom of textbox */
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
	Textbox(s16 x, s16 y, u16 width, u16 height, const char* text, FontBase* font = NULL);
	
	/**
	 * Constructor for a textbox containing a single character.
	 * @param x The x co-ordinate of the text box, relative to its parent.
	 * @param y The y co-ordinate of the text box, relative to its parent.
	 * @param width The width of the textbox.
	 * @param height The height of the textbox.
	 * @param letter Single character to display in the textbox.
	 * @param font The font to use in this text box.
	 */
	Textbox(s16 x, s16 y, u16 width, u16 height, const char letter, FontBase* font = NULL);
	
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
	 * @param position The horizontal position of the text.
	 */
	virtual void setTextPositionHoriz(TextPositionHoriz position);
	
	/**
	 * Set the vertical alignment of text within the textbox.
	 * @param position The vertical position of the text.
	 */
	virtual void setTextPositionVert(TextPositionVert position);
	
	/**
	 * Returns a pointer to the string shown in the textbox.
	 * @return Pointer to the string.
	 */
	virtual inline const char* getText() const { return _text; };
	
	/**
	 * Set the text displayed in the textbox.
	 * @param text Pointer to the text to display.
	 */
	virtual void setText(const char* text);
	
	/**
	 * Append new text to the end of the current text displayed in the textbox.
	 * @param Pointer to the text to append.
	 */
	virtual void addText(const char* text);

	/**
	 * Click this gadget at the supplied co-ordinates.
	 * @param x X co-ordinate of the click.
	 * @param y Y co-ordinate of the click.
	 * @return True if the click was successful.
	 */
	virtual bool click(s16 x, s16 y);
	
	/**
	 * Release this gadget at the supplied co-ordinates
	 * @param x X co-ordinate of the release.
	 * @param y Y co-ordinate of the release.
	 * @return True if the release was successful.
	 */
	virtual bool release(s16 x, s16 y);

	/**
	 * Resize the gadget to the new dimensions.
	 * @param width The new width.
	 * @param height The new height.
	 * @return True if the resize was successful.
	 */
	virtual bool resize(u16 width, u16 height);


protected:
	char* _text;
	u16 _textX;
	u16 _textY;
	u8 _padding;
	TextPositionHoriz _hPos;
	TextPositionVert _vPos;

	/**
	 * Calculate the position of the string based on its length and the alignment options.
	 */
	void calculateTextPosition();

	/**
	 * Destructor.
	 */
	virtual inline ~Textbox() {
		delete[] _text;
		_text = NULL;
	};
};

#endif
