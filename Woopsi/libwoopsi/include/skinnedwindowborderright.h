#ifndef _SKINNED_WINDOW_BORDER_RIGHT_H_
#define _SKINNED_WINDOW_BORDER_RIGHT_H_

#include <nds.h>
#include "windowborderside.h"
#include "skin.h"

namespace WoopsiUI {

	class SkinnedWindowBorderRight : public WindowBorderSide {
	public:
		SkinnedWindowBorderRight(s16 x, s16 y, u16 height, const WindowSkin* skin);

		virtual void draw(Rect clipRect);

	protected:
		const WindowSkin* _skin;

		/**
		 * Destructor.
		 */
		virtual inline ~SkinnedWindowBorderRight() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline SkinnedWindowBorderRight(const SkinnedWindowBorderRight& skinnedWindowBorderRight) : WindowBorderSide(skinnedWindowBorderRight) { };
	};
}

#endif
