#ifndef _BITMAP_BUTTON_H_
#define _BITMAP_BUTTON_H_

#include <nds.h>
#include "gadget.h"

namespace WoopsiUI {

	class BitmapBase;

	/**
	 * Class representing a clickable button that contains a bitmap image instead of text.
	 * Displays two bitmaps, one when the button is clicked, and one when it isn't.  The
	 * bitmaps should be the same size.
	 */
	class BitmapButton : public Gadget {

	public:
		/**
		 * Constructor.
		 * @param x The x co-ordinate of the button.
		 * @param y The y co-ordinate of the button.
		 * @param width The width of the button.
		 * @param height The height of the button.
		 * @param bitmapX The x co-ordinate at which the bitmaps will be drawn.
		 * @param bitmapY The y co-ordinate at which the bitmaps will be drawn.
		 * @param bitmapNormal Pointer to the unclicked bitmap.
		 * @param bitmapClicked Pointer to the clicked bitmap.
		 */
		BitmapButton(s16 x, s16 y, u16 width, u16 height, u16 bitmapX, u16 bitmapY, const BitmapBase* bitmapNormal, const BitmapBase* bitmapClicked);

		/**
		 * Insert the dimensions that this gadget wants to have into the rect
		 * passed in as a parameter.  All co-ordinates are relative to the gadget's
		 * parent.
		 * @param rect Reference to a rect to populate with data.
		 */
		virtual void getPreferredDimensions(Rect& rect) const;

	protected:
		const BitmapBase* _bitmapNormal;	/**< Bitmap to display when button is not clicked */
		const BitmapBase* _bitmapClicked;	/**< Bitmap to display when button is clicked */
		u16 _bitmapX;						/**< X co-ordinate of the bitmaps */
		u16 _bitmapY;						/**< Y co-ordinate of the bitmaps */

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
		 * Draws the outline of the button.
		 * @param port Graphics port to draw to.
		 */
		virtual void drawOutline(GraphicsPort* port);

		/**
		 * Redraws the button.
		 * @param x The x co-ordinate of the click.
		 * @param y The y co-ordinate of the click.
		 */
		virtual void onClick(s16 x, s16 y);
		
		/**
		 * Raises an action event and redraws the button.
		 * @param x The x co-ordinate of the stylus.
		 * @param y The y co-ordinate of the stylus.
		 */
		virtual void onRelease(s16 x, s16 y);
		
		/**
		 * Redraws the button.
		 * @param x The x co-ordinate of the stylus.
		 * @param y The y co-ordinate of the stylus.
		 */
		virtual void onReleaseOutside(s16 x, s16 y);

		/**
		 * Destructor.
		 */
		virtual inline ~BitmapButton() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline BitmapButton(const BitmapButton& bitmapButton) : Gadget(bitmapButton) { };
	};
}

#endif
