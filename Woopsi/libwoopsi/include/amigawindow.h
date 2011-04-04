#ifndef _AMIGA_WINDOW_H_
#define _AMIGA_WINDOW_H_

#include <nds.h>
#include "window.h"
#include "gadgeteventhandler.h"
#include "gadgetstyle.h"
#include "woopsistring.h"

const s32 WINDOW_TITLE_HEIGHT = 13;
const s32 WINDOW_BORDER_SIZE = 4;
const s32 WINDOW_CLOSE_BUTTON_WIDTH = 14;
const s32 WINDOW_DEPTH_BUTTON_WIDTH = 14;

namespace WoopsiUI {

	class WindowBorderButton;

	/**
	 * Class providing a window that mimics the appearance of the windows from
	 * Amiga Workbench 3.x.  It has a title bar and borders around the edge of
	 * the window, along with a depth button and (optional) close button.
	 */
	class AmigaWindow : public Window, public GadgetEventHandler {

	public:

		/**
		 * Struct describing some basic properties of an AmigaWindow.
		 */
		typedef struct {
			u8 showCloseButton : 1;						/**< True if the close button is visible. */
			u8 showDepthButton : 1;						/**< True if the depth button is visible. */
		} AmigaWindowFlags;

		/**
		 * Constructor.
		 * @param x The x co-ordinate of the window, relative to its parent.
		 * @param y The y co-ordinate of the window, relative to its parent.
		 * @param width The width of the window.
		 * @param height The height of the window.
		 * @param title The text that should appear in the title bar.
		 * @param showCloseButton True to show the window close button.
		 * @param showDepthButton True to show the z-order sorting button.
		 * @param style The style that the gadget should use.  If this is not
		 * specified, the gadget will use the values stored in the global
		 * defaultGadgetStyle object.  The gadget will copy the properties of
		 * the style into its own internal style object.
		 */
		AmigaWindow(s16 x, s16 y, u16 width, u16 height, const WoopsiString& title, bool showCloseButton, bool showDepthButton, GadgetStyle* style = NULL);

		/**
		 * Show or hide the Amiga borders.
		 * @param isBorderless True to enable borders, false to disable them.
		 */
		virtual void setBorderless(bool isBorderless);

		/**
		 * Handle events fired by decoration gadgets.
		 * @param e Event arguments to process.
		 */
		virtual void handleReleaseEvent(const GadgetEventArgs& e);

		/**
		 * Shows the close button if it is hidden.  Has no effect
		 * if the window is borderless.
		 */
		virtual void showCloseButton();

		/**
		 * Shows the depth button if it is hidden.  Has no effect
		 * if the window is borderless.
		 */
		virtual void showDepthButton();

		/**
		 * Hides the close button if it is visible.  Has no effect
		 * if the window is borderless.
		 */
		virtual void hideCloseButton();

		/**
		 * Hides the depth button if it is hidden.  Has no effect
		 * if the window is borderless.
		 */
		virtual void hideDepthButton();

		/**
		 * Does the window have a close button?
		 * @return True if the window has a close button.
		 */
		virtual inline const bool hasCloseButton() const { return _windowFlags.showCloseButton; };

		/**
		 * Does the window have a depth button?
		 * @return True if the window has a depth button.
		 */
		virtual inline const bool hasDepthButton() const { return _windowFlags.showDepthButton; };

	protected:
		AmigaWindowFlags _windowFlags;					/**< AmigaWindow-specific flags */

		WindowBorderButton* _closeButton;				/**< Pointer to the close button */
		WindowBorderButton* _depthButton;				/**< Pointer to the depth button */

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the redraw() function to draw all visible regions.
		 * @param port The GraphicsPort to draw to.
		 * @see redraw()
		 */
		virtual void drawBorder(GraphicsPort* port);

		/**
		 * Resize the gadget to the new dimensions.
		 * @param width The new width.
		 * @param height The new height.
		 */
		virtual void onResize(u16 width, u16 height);
		
		/**
		 * Raises the window to the top of the stack and marks its borders for redraw.
		 */
		virtual void onFocus();
		
		/**
		 * Marks the borders for redraw,
		 */
		virtual void onBlur();

		/**
		 * Called when the gadget is clicked.  Override this when creating new
		 * gadgets if the gadget should exhibit additional behaviour when it is
		 * clicked.
		 * @param x The x co-ordinate of the click.
		 * @param y The y co-ordinate of the click.
		 */
		virtual void onClick(s16 x, s16 y);

		/**
		 * Create the window's border.
		 */
		virtual void createBorder();
		
		/**
		 * Mark the visible border regions as dirty for redraw.
		 */
		virtual void markBorderDirty();
		
		/**
		 * Destructor.
		 */
		virtual inline ~AmigaWindow() { }

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline AmigaWindow(const AmigaWindow& amigaWindow) : Window(amigaWindow) { };
	};
}

#endif
