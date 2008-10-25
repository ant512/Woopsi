#ifndef _SKINNED_SCREEN_TITLE_H_
#define _SKINNED_SCREEN_TITLE_H_

#include <nds.h>
#include "screentitle.h"
#include "skin.h"

using namespace WoopsiUI;

namespace WoopsiUI {

	class SkinnedScreenTitle : public ScreenTitle {

	public:
		SkinnedScreenTitle(const char* text, const ScreenSkin* skin);

		/**
		 * Override the Gadget::draw() method.
		 */
		virtual inline void draw() { Gadget::draw(); };

		virtual void draw(Rect clipRect);

	protected:
		const ScreenSkin* _skin;

		/**
		 * Destructor.
		 */
		virtual inline ~SkinnedScreenTitle() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline SkinnedScreenTitle(const SkinnedScreenTitle& skinnedScreenTitle) : ScreenTitle(skinnedScreenTitle) { };
	};
}

#endif
