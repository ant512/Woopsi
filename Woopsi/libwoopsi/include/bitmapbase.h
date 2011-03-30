#ifndef _BITMAP_BASE_H_
#define _BITMAP_BASE_H_

#include <nds.h>

namespace WoopsiUI {

	/**
	 * Abstract class defining the basic properties of a bitmap.  Since
	 * the DS has 16-bit displays (1 alpha bit and 5 bits each for RGB),
	 * all bitmaps must expose their data in the same format.
	 *
	 * However, since this class does not define how the data is stored,
	 * it is possible for a subclass to store the bitmap data in whatever
	 * format necessary (1, 2, 4, 8, 24 or 32-bits, or any format at all)
	 * as long as it translates back to the DS' native 16-bit format when
	 * accessed externally.
	 */
	class BitmapBase {
	public:

		/**
		 * Destructor.
		 */
		virtual inline ~BitmapBase() { };

		/**
		 * Get a pointer to the internal bitmap data.
		 * @return Pointer to the internal bitmap data.
		 */
		virtual const u16* getData() const = 0;

		/**
		 * Get a pointer to the internal bitmap data at the specified
		 * co-ordinates.
		 * @param x The x co-ord of the data.
		 * @param y The y co-ord of the data.
		 * @return Pointer to the internal bitmap data.
		 */
		virtual const u16* getData(s16 x, s16 y) const = 0;

		/**
		 * Get the colour of the pixel at the specified co-ordinates
		 * @param x The x co-ordinate of the pixel.
		 * @param y The y co-ordinate of the pixel.
		 * @return The colour of the pixel.
		 */
		virtual const u16 getPixel(s16 x, s16 y) const = 0;

		/**
		 * Copies data from the supplied co-ordinates sequentially into dest.
		 * If the amount to be copied exceeds the available width of the bitmap,
		 * copying will wrap around from the right-hand edge of the bitmap to
		 * the left-hand edge.
		 * The dest parameter must point to an area of memory large enough to
		 * contain the copied data.
		 * @param x The x co-ordinate to copy from.
		 * @param y The y co-ordinate to copy from.
		 * @param size The number of pixels to copy.
		 * @param dest Pointer to the memory that will be copied into.
		 */
		virtual void copy(s16 x, s16 y, u32 size, u16* dest) const = 0;

		/**
		 * Get the bitmap's width.
		 * @return The bitmap's width.
		 */
		virtual const u16 getWidth() const = 0;

		/**
		 * Get the bitmap's height.
		 * @return The bitmap's height.
		 */
		virtual const u16 getHeight() const = 0;
	};
}

#endif
