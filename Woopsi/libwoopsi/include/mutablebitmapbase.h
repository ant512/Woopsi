#ifndef _MUTABLE_BITMAP_BASE_H_
#define _MUTABLE_BITMAP_BASE_H_

#include <nds.h>
#include "bitmapbase.h"

namespace WoopsiUI {

	/**
	 * Abstract class defining the basic properties of an editable bitmap.
	 */
	class MutableBitmapBase : public BitmapBase {
	public:

		/**
		 * Destructor.
		 */
		virtual inline ~MutableBitmapBase() { };

		/**
		 * Set the colour of the specified pixel.
		 * @param x X co-ord of the pixel to set.
		 * @param y Y co-ord of the pixel to set.
		 * @param colour New colour of the pixel.
		 */
		virtual void setPixel(const s16 x, const s16 y, const u16 colour) = 0;

		/**
		 * Blit data to the specified co-ordinates using the DMA hardware.
		 * @param x The x co-ordinate to blit to.
		 * @param y The y co-ordinate to blit to.
		 * @param data The data to blit.
		 * @param size The number of u16s to blit.
		 */
		virtual void blit(const s16 x, const s16 y, const u16* data, const u32 size) = 0;

		/**
		 * Fill at the specified co-ordinates using the DMA hardware.
		 * @param x The x co-ordinate to blit to.
		 * @param y The y co-ordinate to blit to.
		 * @param colour The colour to fill with.
		 * @param size The number of u16s to blit.
		 */
		virtual void blitFill(const s16 x, const s16 y, const u16 colour, const u32 size) = 0;
	};
}

#endif
