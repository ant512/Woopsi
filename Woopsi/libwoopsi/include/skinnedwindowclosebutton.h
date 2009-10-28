#ifndef _SKINNED_WINDOW_CLOSE_BUTTON_H_
#define _SKINNED_WINDOW_CLOSE_BUTTON_H_

#include <nds.h>
#include "decorationglyphbutton.h"
#include "skin.h"

namespace WoopsiUI {

	class SkinnedWindowCloseButton : public DecorationGlyphButton {

	public:
		SkinnedWindowCloseButton(s16 x, s16 y, const WindowSkin* skin);

		virtual void draw(Rect clipRect);

	protected:
		const WindowSkin* _skin;

		/**
		 * Destructor.
		 */
		virtual inline ~SkinnedWindowCloseButton() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline SkinnedWindowCloseButton(const SkinnedWindowCloseButton& skinnedWindowCloseButton) : DecorationGlyphButton(skinnedWindowCloseButton) { };
	};
}

#endif
