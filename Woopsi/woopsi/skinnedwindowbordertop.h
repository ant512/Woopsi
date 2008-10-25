#ifndef _SKINNED_WINDOW_BORDER_TOP_H_
#define _SKINNED_WINDOW_BORDER_TOP_H_

#include <nds.h>
#include "windowbordertop.h"
#include "skin.h"

namespace WoopsiUI {

	class SkinnedWindowBorderTop : public WindowBorderTop {

	public:
		SkinnedWindowBorderTop(s16 x, u16 width, const char* text, const WindowSkin* skin);

		virtual void draw(Rect clipRect);
		virtual void draw();

	protected:
		const WindowSkin* _skin;

		/**
		 * Destructor.
		 */
		virtual inline ~SkinnedWindowBorderTop() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline SkinnedWindowBorderTop(const SkinnedWindowBorderTop& skinnedWindowBorderTop) : WindowBorderTop(skinnedWindowBorderTop) { };
	};
}

#endif
