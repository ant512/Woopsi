#ifndef _SKINNED_SCREEN_H_
#define _SKINNED_SCREEN_H_

#include <nds.h>
#include "screen.h"
#include "skin.h"

namespace WoopsiUI {

	class SkinnedScreenDepthButton;
	class SkinnedScreenFlipButton;
	class SkinnedScreenTitle;

	class SkinnedScreen : public Screen, public GadgetEventHandler {

	public:

		/**
		 * Enum listing flags that can be set in the constructor's "screenFlags" parameter.
		 */
		enum SkinnedScreenFlagType {
			SKINNED_SCREEN_SHOW_FLIP = 0x0001,			/**< Screen shows flip button */
			SKINNED_SCREEN_SHOW_DEPTH = 0x0002			/**< Screen shows depth button */
		};

		/**
		 * Struct describing some basic properties of a SkinnedScreen.
		 */
		typedef struct {
			u8 showFlipButton : 1;						/**< True if the flip button is visible. */
			u8 showDepthButton : 1;						/**< True if the depth button is visible. */
		} SkinnedScreenFlags;

		SkinnedScreen(const char* title, u32 flags, u32 screenFlags, const ScreenSkin* skin);

		virtual void setBorderless(bool isBorderless);
		virtual void handleReleaseEvent(const GadgetEventArgs& e);
		virtual void handleClickEvent(const GadgetEventArgs& e);
		virtual void handleDragEvent(const GadgetEventArgs& e);
		virtual void handleReleaseOutsideEvent(const GadgetEventArgs& e);

		/**
		 * Shows the flip button if it is hidden.  Has no effect
		 * if the screen is borderless.
		 * Will redraw the gadget.
		 */
		virtual void showFlipButton();

		/**
		 * Shows the depth button if it is hidden.  Has no effect
		 * if the screen is borderless.
		 * Will redraw the gadget.
		 */
		virtual void showDepthButton();

		/**
		 * Hides the flip button if it is visible.  Has no effect
		 * if the screen is borderless.
		 * Will redraw the gadget.
		 */
		virtual void hideFlipButton();

		/**
		 * Hides the depth button if it is hidden.  Has no effect
		 * if the screen is borderless.
		 * Will redraw the gadget.
		 */
		virtual void hideDepthButton();

		/**
		 * Does the screen have a flip button?
		 * @return True if the screen has a flip button.
		 */
		virtual inline const bool hasFlipButton() const { return _screenFlags.showFlipButton; };

		/**
		 * Does the screen have a depth button?
		 * @return True if the screen has a depth button.
		 */
		virtual inline const bool hasDepthButton() const { return _screenFlags.showDepthButton; };

	protected:
		SkinnedScreenTitle* _screenTitle;
		SkinnedScreenDepthButton* _depthButton;
		SkinnedScreenFlipButton* _flipButton;
		SkinnedScreenFlags _screenFlags;				/**< SkinnedScreen-specific flags */

		const ScreenSkin* _skin;

		/**
		 * Destructor.
		 */
		virtual inline ~SkinnedScreen() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline SkinnedScreen(const SkinnedScreen& skinnedScreen) : Screen(skinnedScreen) { };
	};
}

#endif
