#ifndef _MULTILINE_TEXTBOX_H_
#define _MULTILINE_TEXTBOX_H_

#include <nds.h>
#include "textwriter.h"
#include "gadget.h"
#include "scrollingpanel.h"
#include "fontbase.h"
#include "text.h"
#include "graphicsport.h"

using namespace std;

/**
 * Textbox that offers multiple lines of text.  Has scrolling
 * capability and can be dragged using the stylus.  The text
 * it contains can be changed or added to.  It can remember more
 * rows of text than it can display, and these additional
 * rows can be scrolled through.
 */
class MultiLineTextBox : public ScrollingPanel {
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
	 * of a full queue causes the front items to be popped off.
	 * @param font The font to use in this text box.
	 */
	MultiLineTextBox(s16 x, s16 y, u16 width, u16 height, char* text, u32 flags, s16 maxRows = 0, FontBase* font = NULL);

	/**
	 * Destructor.
	 */
	virtual ~MultiLineTextBox();

	/**
	 * Draw the region of the textbox within the clipping rect.
	 * Should not be called directly.
	 * @param clipRect The clipping rect to limit drawing to.
	 */
	virtual void draw(Rect clipRect);

	/**
	 * Draw all visible regions of the textbox.
	 */
	virtual void draw();

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
	 * Returns a pointer to the raw, unwrapped text used as the
	 * primary data source for the textbox.  Altering this will
	 * have undesired effects.
	 * @return Pointer to the raw text.
	 */
	virtual char* getRawText();

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
	 * @param text Pointer to the text to display.
	 */
	virtual void setText(char* text);

	/**
	 * Append new text to the end of the current text
	 * displayed in the textbox.
	 * @param Pointer to the text to append.
	 */
	virtual void addText(char* text);

	/**
	 * Set the font used in the textbox.
	 * @param Pointer to the new font.
	 */
	virtual void setFont(FontBase* font);

protected:
	char* _rawText;
	Text* _text;
	u8 _visibleRows;
	s16 _maxRows;
	u32 _topRow;
	u8 _padding;
	TextPositionHoriz _hPos;
	TextPositionVert _vPos;

	u8 getRowX(u8 rowPixelWidth);
	s16 getRowY(u8 screenRow, u8 screenRows);
	void calculateTotalVisibleRows();
	void stripTopLines(const u32 lines);
	virtual void setText(char* text, bool raiseEvent);
};

#endif
