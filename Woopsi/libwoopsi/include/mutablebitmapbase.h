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
		 * Get a non-const pointer to the internal bitmap.
		 * @return Non-const pointer to the internal bitmap.
		 */
		virtual u16* getEditableData() const = 0;

		/**
		 * Set the colour of the specified pixel.
		 * @param x X co-ord of the pixel to set.
		 * @param y Y co-ord of the pixel to set.
		 * @param colour New colour of the pixel.
		 */
		virtual void setPixel(const s16 x, const s16 y, const u16 colour) = 0;
	};
}

#endif
