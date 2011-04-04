#ifndef _LABEL_H_
#define _LABEL_H_

#include <nds.h>
#include "gadget.h"
#include "gadgetstyle.h"
#include "woopsistring.h"

namespace WoopsiUI {

	/**
	 * Single-line label gadget.  Can align text both vertically and
	 * horizontally in different ways.
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
			TEXT_ALIGNMENT_VERT_CENTRE = 0,	/**< Align to centre of textbox */
			TEXT_ALIGNMENT_VERT_TOP = 1,	/**< Align to top of textbox */
			TEXT_ALIGNMENT_VERT_BOTTOM = 2	/**< Align to bottom of textbox */
		};

		/**
		 * Constructor for a label containing a string.
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
		Label(s16 x, s16 y, u16 width, u16 height, const WoopsiString& text, GadgetStyle* style = NULL);

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
		 * Returns the string shown in the label.
		 * @return The label's text.
		 */
		virtual inline const WoopsiString& getText() const { return _text; };
		
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
		 * Insert text at the specified index.
		 * @param text The text to insert.
		 * @param index Index at which to insert the text.
		 */
		virtual void insertText(const WoopsiString& text, const u32 index);

		/**
		 * Insert the dimensions that this gadget wants to have into the rect
		 * passed in as a parameter.  All co-ordinates are relative to the
		 * gadget's parent.
		 * @param rect Reference to a rect to populate with data.
		 */
		virtual void getPreferredDimensions(Rect& rect) const;

		/**
		 * Sets the font.
		 * @param font A pointer to the font to use.
		 */
		virtual void setFont(FontBase* font);

		/**
		 * Sets this gadget's border state.
		 * @param isBorderless The border state.
		 */
		void setBorderless(bool isBorderless);

	protected:
		WoopsiString _text;						/**< Text that the textbox will display */
		s32 _textX;								/**< X co-ordinate of the text relative to the gadget */
		s32 _textY;								/**< Y co-ordinate of the text relative to the gadget */
		TextAlignmentHoriz _hAlignment;			/**< Horizontal alignment of the text */
		TextAlignmentVert _vAlignment;			/**< Vertical alignment of the text */

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
		 * Resize the gadget to the new dimensions.
		 * @param width The new width.
		 * @param height The new height.
		 */
		virtual void onResize(u16 width, u16 height);
		
		/**
		 * Calculate the vertical position of the string based on the font
		 * height and the alignment options.
		 */
		virtual void calculateTextPositionVertical();
		
		/**
		 * Calculate the position of the string based on its length and the
		 * alignment options.
		 */
		virtual void calculateTextPositionHorizontal();

		/**
		 * Updates the GUI after the text has changed.
		 */
		virtual void onTextChange();

		/**
		 * Marks the horizontal band containing the text as damaged, ready for
		 * redrawing.
		 */
		virtual void markTextRectDamaged();

		/**
		 * Destructor.
		 */
		virtual inline ~Label() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline Label(const Label& label) : Gadget(label) { };
	};
}

#endif
