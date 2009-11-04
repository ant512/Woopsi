#ifndef _SKINNED_WINDOW_BORDER_BOTTOM_H_
#define _SKINNED_WINDOW_BORDER_BOTTOM_H_

#include <nds.h>
#include "windowborderbottom.h"
#include "skin.h"

namespace WoopsiUI {

	class SkinnedWindowBorderBottom : public WindowBorderBottom {

	public:
		SkinnedWindowBorderBottom(s16 x, s16 y, u16 width, const WindowSkin* skin);

		virtual void draw(Rect clipRect);

	protected:
		const WindowSkin* _skin;

		/**
		 * Destructor.
		 */
		virtual inline ~SkinnedWindowBorderBottom() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline SkinnedWindowBorderBottom(const SkinnedWindowBorderBottom& skinnedWindowBorderBottom) : WindowBorderBottom(skinnedWindowBorderBottom) { };
	};
}

#endif
