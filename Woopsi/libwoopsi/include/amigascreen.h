#ifndef _AMIGA_SCREEN_H_
#define _AMIGA_SCREEN_H_

#include <nds.h>
#include "gadgeteventhandler.h"
#include "screen.h"
#include "gadgetstyle.h"
#include "woopsistring.h"

namespace WoopsiUI {

	class DecorationGlyphButton;
	class ScreenFlipButton;

	/**
	 * Class providing a screen with an Amiga-style title bar.  Unlike the AmigaWindow class,
	 * the title bar in this class can be overlapped by child gadgets.  Clicking and dragging
	 * the title bar causes the screen to drop down the DS' display, revealing any screens
	 * below it.  The screen has depth swap and display flip buttons.
	 *
	 * All gadgets should be a child/grandchild/etc of a screen except for other screens
	 * and the Woopsi instance.
	 */
	class AmigaScreen : public Screen, public GadgetEventHandler {

	public:

		/**
		 * Struct describing some basic properties of an AmigaScreen.
		 */
		typedef struct {
			u8 showFlipButton : 1;						/**< True if the flip button is visible. */
			u8 showDepthButton : 1;						/**< True if the depth button is visible. */
		} AmigaScreenFlags;

		/**
		 * Constructor.
		 * @param title The screen's title.
		 * @param showFlipButton True to show the display-flipping button.
		 * @param showDepthButton True to show the z-order sorting button.
		 * @param style The style that the gadget should use.  If this is not
		 * specified, the gadget will use the values stored in the global
		 * defaultGadgetStyle object.  The gadget will copy the properties of
		 * the style into its own internal style object.
		 */
		AmigaScreen(const WoopsiString& title, bool showFlipButton, bool showDepthButton, GadgetStyle* style = NULL);

		/**
		 * Sets this gadget's border state.  Setting to false hides the title bar.
		 * @param isBorderless The border state.
		 */
		virtual void setBorderless(bool isBorderless);

		/**
		 * Handle events fired by decoration gadgets.
		 * @param e Event arguments to process.
		 */
		virtual void handleReleaseEvent(const GadgetEventArgs& e);

		/**
		 * Shows the flip button if it is hidden.  Has no effect
		 * if the screen is borderless.
		 */
		virtual void showFlipButton();

		/**
		 * Shows the depth button if it is hidden.  Has no effect
		 * if the screen is borderless.
		 */
		virtual void showDepthButton();

		/**
		 * Hides the flip button if it is visible.  Has no effect
		 * if the screen is borderless.
		 */
		virtual void hideFlipButton();

		/**
		 * Hides the depth button if it is hidden.  Has no effect
		 * if the screen is borderless.
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
		DecorationGlyphButton* _depthButton;			/**< Pointer to the screen's depth button */
		DecorationGlyphButton* _flipButton;				/**< Pointer to the screen's flip button */
		AmigaScreenFlags _screenFlags;					/**< AmigaScreen-specific flags */

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the redraw() function to draw all visible regions.
		 * @param port The GraphicsPort to draw to.
		 * @see redraw()
		 */
		virtual void drawBorder(GraphicsPort* port);

		/**
		 * Starts the dragging system.
		 * @param x The x co-ordinate of the click.
		 * @param y The y co-ordinate of the click.
		 */
		virtual void onClick(s16 x, s16 y);

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
