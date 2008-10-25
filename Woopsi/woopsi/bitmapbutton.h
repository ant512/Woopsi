#ifndef _BITMAP_BUTTON_H_
#define _BITMAP_BUTTON_H_

#include <nds.h>
#include "gadget.h"

namespace WoopsiUI {

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
		 * @param bitmapWidth The width of the bitmaps.
		 * @param bitmapHeight The height of the bitmaps.
		 * @param bitmapNormal Pointer to the unclicked bitmap.
		 * @param bitmapClicked Pointer to the clicked bitmap.
		 */
		BitmapButton(s16 x, s16 y, u16 width, u16 height, u16 bitmapX, u16 bitmapY, u16 bitmapWidth, u16 bitmapHeight, const u16* bitmapNormal, const u16* bitmapClicked);

		/**
		 * Override the Gadget::draw() method.
		 */
		virtual inline void draw() { Gadget::draw(); };

		/**
		 * Draw the region of the button that falls within the clipRect.
		 * @param clipRect The clipping region to draw within.
		 */
		virtual void draw(Rect clipRect);

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

	protected:
		const u16* _bitmapNormal;			/**< Bitmap to display when button is not clicked */
		const u16* _bitmapClicked;			/**< Bitmap to display when button is clicked */
		u16 _bitmapWidth;					/**< Width of the bitmaps */
		u16 _bitmapHeight;					/**< Height of the bitmaps */
		u16 _bitmapX;						/**< X co-ordinate of the bitmaps */
		u16 _bitmapY;						/**< Y co-ordinate of the bitmaps */

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
