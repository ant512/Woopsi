#ifndef _AMIGA_SCREEN_H_
#define _AMIGA_SCREEN_H_

#include <nds.h>
#include "eventhandler.h"
#include "screen.h"

namespace WoopsiUI {

	class DecorationGlyphButton;
	class ScreenFlipButton;
	class ScreenTitle;

	/**
	 * Class providing a screen with an Amiga-style title bar.  Unlike the AmigaWindow class,
	 * the title bar in this class can be overlapped by child gadgets.  Clicking and dragging
	 * the title bar causes the screen to drop down the DS' display, revealing any screens
	 * below it.  The screen has depth swap and display flip buttons.
	 *
	 * All gadgets should be a child/grandchild/etc of a screen except for other screens
	 * and the Woopsi instance.
	 */
	class AmigaScreen : public Screen, public EventHandler {

	public:

		/**
		 * Enum listing flags that can be set in the constructor's "screenFlags" parameter.
		 */
		enum AmigaScreenFlagType {
			AMIGA_SCREEN_SHOW_FLIP = 0x0001,			/**< Screen shows flip button */
			AMIGA_SCREEN_SHOW_DEPTH = 0x0002			/**< Screen shows depth button */
		};

		/**
		 * Struct describing some basic properties of an AmigaScreen.
		 */
		typedef struct {
			u8 showFlipButton : 1;						/**< True if the flip button is visible. */
			u8 showDepthButton : 1;						/**< True if the depth button is visible. */
		} AmigaScreenFlags;

		/**
		 * Constructor.
		 */
		AmigaScreen(const char* title, u32 flags, u32 screenFlags, FontBase* font = NULL);

		/**
		 * Sets this gadget's border state.  Setting to false hides the title bar.
		 * @param isBorderless The border state.
		 */
		virtual void setBorderless(bool isBorderless);

		/**
		 * Handle events fired by decoration gadgets.
		 * @param e Event arguments to process.
		 * @return True if the event was processed.
		 */
		virtual bool handleEvent(const EventArgs& e);

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
		ScreenTitle* _screenTitle;						/**< Title of the screen */
		DecorationGlyphButton* _depthButton;			/**< Pointer to the screen's depth button */
		DecorationGlyphButton* _flipButton;				/**< Pointer to the screen's flip button */
		AmigaScreenFlags _screenFlags;					/**< AmigaScreen-specific flags */

		/**
		 * Destructor.
		 */
		virtual inline ~AmigaScreen() { };

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline AmigaScreen(const AmigaScreen& amigaScreen) : Screen(amigaScreen) { };
	};
}

#endif
