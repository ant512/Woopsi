#ifndef _SKINNED_WINDOW_DEPTH_BUTTON_H_
#define _SKINNED_WINDOW_DEPTH_BUTTON_H_

#include <nds.h>
#include "decorationglyphbutton.h"
#include "skin.h"

namespace WoopsiUI {

	class SkinnedWindowDepthButton : public DecorationGlyphButton {

	public:
		SkinnedWindowDepthButton(s16 x, s16 y, const WindowSkin* skin);

		virtual void draw(Rect clipRect);

	protected:
		const WindowSkin* _skin;

		/**
		 * Destructor.
		 */
		virtual inline ~SkinnedWindowDepthButton() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline SkinnedWindowDepthButton(const SkinnedWindowDepthButton& skinnedWindowDepthButton) : DecorationGlyphButton(skinnedWindowDepthButton) { };
	};
}

#endif
