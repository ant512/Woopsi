#ifndef _BITMAP_WRAPPER_H_
#define _BITMAP_WRAPPER_H_

#include <nds.h>
#include "bitmapbase.h"

namespace WoopsiUI {

	/**
	 * Class designed to wrap raw u16* data within a bitmap object.  This
	 * enables the data to be accessed just like any other bitmap object.
	 * Since raw u16* data should be const, the bitmap is read-only.
	 */
	class BitmapWrapper : public BitmapBase {
	public:

		/**
		 * Constructor.
		 * @param data Pointer to the raw bitmap data.
		 * @param width The width of the bitmap.
		 * @param height The height of the bitmap.
		 */
		BitmapWrapper(const u16* data, u16 width, u16 height);

		/**
		 * Destructor.
		 */
		virtual inline ~BitmapWrapper() { };
		
		/**
		 * Get the colour of the pixel at the specified co-ordinates
		 * @param x The x co-ordinate of the pixel.
		 * @param y The y co-ordinate of the pixel.
		 * @return The colour of the pixel.
		 */
		const u16 getPixel(s16 x, s16 y) const;

		/**
		 * Get a pointer to the internal bitmap.
		 * @return Pointer to the internal bitmap.
		 */
		inline const u16* getData() const { return _bitmap; };

		/**
		 * Get a pointer to the internal bitmap data at the specified
		 * co-ordinates.
		 * @param x The x co-ord of the data.
		 * @param y The y co-ord of the data.
		 * @return Pointer to the internal bitmap data.
		 */
		const u16* getData(s16 x, s16 y) const;

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
		void copy(s16 x, s16 y, u32 size, u16* dest) const;

		/**
		 * Get the bitmap's width.
		 * @return The bitmap's width.
		 */
		inline const u16 getWidth() const { return _width; };

		/**
		 * Get the bitmap's height.
		 * @return The bitmap's height.
		 */
		inline const u16 getHeight() const { return _height; };

	protected:
		const u16* _bitmap __attribute__ ((aligned (4)));	/**< Bitmap */
		u16 _width;											/**< Width of the bitmap */
		u16 _height;										/**< Height of the bitmap */

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline BitmapWrapper(const BitmapWrapper& bitmap) { };
	};
}

#endif
