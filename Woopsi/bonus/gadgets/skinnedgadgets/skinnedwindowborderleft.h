#ifndef _SKINNED_WINDOW_BORDER_LEFT_H_
#define _SKINNED_WINDOW_BORDER_LEFT_H_

#include <nds.h>
#include "windowborderside.h"
#include "skin.h"

namespace WoopsiUI {

	class SkinnedWindowBorderLeft : public WindowBorderSide {
	public:
		SkinnedWindowBorderLeft(s16 x, s16 y, u16 height, const WindowSkin* skin);

		virtual void draw(Rect clipRect);

	protected:
		const WindowSkin* _skin;

		/**
		 * Destructor.
		 */
		virtual inline ~SkinnedWindowBorderLeft() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline SkinnedWindowBorderLeft(const SkinnedWindowBorderLeft& skinnedWindowBorderLeft) : WindowBorderSide(skinnedWindowBorderLeft) { };
	};
}

#endif
