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
	};
}

#endif
