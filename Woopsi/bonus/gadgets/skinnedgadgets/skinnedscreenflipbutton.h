#ifndef _SKINNED_SCREEN_FLIP_BUTTON_H_
#define _SKINNED_SCREEN_FLIP_BUTTON_H_

#include <nds.h>
#include "decorationglyphbutton.h"
#include "skin.h"

namespace WoopsiUI {

	class SkinnedScreenFlipButton : public DecorationGlyphButton {

	public:
		SkinnedScreenFlipButton(s16 x, s16 y, const ScreenSkin* skin);

		virtual void draw(Rect clipRect);

	protected:
		const ScreenSkin* _skin;

		/**
		 * Destructor.
		 */
		virtual inline ~SkinnedScreenFlipButton() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline SkinnedScreenFlipButton(const SkinnedScreenFlipButton& skinnedScreenFlipButton) : DecorationGlyphButton(skinnedScreenFlipButton) { };
	};
}

#endif
