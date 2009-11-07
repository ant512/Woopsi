#ifndef _BITMAP_BASE_H_
#define _BITMAP_BASE_H_

#include <nds.h>

namespace WoopsiUI {

	/**
	 * Abstract class defining the basic properties of the bitmap.
	 */
	class BitmapBase {
	public:

		/**
		 * Get a pointer to the internal bitmap data.
		 * @return Pointer to the internal bitmap data.
		 */
		virtual inline const u16* getData() const = 0;

		/**
		 * Get the colour of the pixel at the specified co-ordinates
		 * @param x The x co-ordinate of the pixel.
		 * @param y The y co-ordinate of the pixel.
		 * @return The colour of the pixel.
		 */
		virtual const u16 getPixel(s16 x, s16 y) const = 0;

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
		u16 _width;										/**< Width of the bitmap */
		u16 _height;									/**< Height of the bitmap */
	};
}

#endif
