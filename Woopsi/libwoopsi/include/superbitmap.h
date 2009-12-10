#ifndef _SUPERBITMAP_H_
#define _SUPERBITMAP_H_

#include <nds.h>
#include "gadget.h"
#include "graphics.h"
#include "gadgetstyle.h"

namespace WoopsiUI {

	class Bitmap;
	class BitmapBase;

	/**
	 * The SuperBitmap class provides a set of 2D drawing tools and a bitmap in RAM to draw on.
	 * The class manifests itself as a gadget that can be scrolled around using the stylus.
	 */
	class SuperBitmap : public Gadget {

	public:

		/**
		 * Constructor.
		 * @param x The x co-ordinate of the gadget.
		 * @param y The y co-ordinate of the gadget.
		 * @param width The width of the gadget.
		 * @param height The height of the gadget.
		 * @param bitmapWidth The desired width of the gadget's bitmap.
		 * @param bitmapHeight The desired height of the gadget's bitmap.
		 * @param isDecoration Specify if the gadget is a decoration or not.  If set to true
		 * the gadget will function as a background image.
		 * @param style The style that the gadget should use.  If this is not
		 * specified, the gadget will use the values stored in the global
		 * defaultGadgetStyle object.  The gadget will copy the properties of
		 * the style into its own internal style object.
		 */
		SuperBitmap(s16 x, s16 y, u16 width, u16 height, u16 bitmapWidth, u16 bitmapHeight, bool isDecoration, GadgetStyle* style = NULL);

		/**
		 * Get a pointer to the internal bitmap.
		 * @return Pointer to the internal bitmap.
		 */
		Bitmap* getBitmap();

		/**
		 * Get a pointer to a Graphics object that can be used to draw
		 * to the internal bitmap.  Note that this should *not* be deleted.
		 * @return Pointer to a graphics object for drawing to the internal
		 * bitmap.
		 */
		Graphics* getGraphics();


		/**
		 * Draws the region of the gadget that falls within the clipping region.  Should not
		 * be called.
		 * @param clipRect The rectangle to clip to.
		 */
		virtual void draw(Rect clipRect);

		/**
		 * Erases the internal bitmap by filling it with the gadget's back colour.
		 */
		virtual void clearBitmap();

		/**
		 * Drag the gadget.
		 * @param x The x co-ordinate of the drag.
		 * @param y The y co-ordinate of the drag.
		 * @param vX The horizontal drag distance.
		 * @param vY The vertical drag distance.
		 */
		virtual bool drag(s16 x, s16 y, s16 vX, s16 vY);

	protected:
		s32 _bitmapX;									/**< X co-ordinate of the bitmap relative to the gadget */
		s32 _bitmapY;									/**< Y co-ordinate of the bitmap relative to the gadget */
		Bitmap* _bitmap;								/**< Bitmap */
		Graphics* _graphics;							/**< Graphics object for drawing to bitmap */

		/**
		 * Destructor.
		 */
		virtual ~SuperBitmap();

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline SuperBitmap(const SuperBitmap& superBitmap) : Gadget(superBitmap) { };
	};
}

#endif
