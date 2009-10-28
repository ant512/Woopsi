#ifndef _WINDOW_BORDER_TOP_H_
#define _WINDOW_BORDER_TOP_H_

#include <nds.h>
#include "gadget.h"

namespace WoopsiUI {

	class Window;

	/**
	 * Class providing a gadget that mimics the appearance of the AmigaOS 3.x
	 * window title bar.  Forms part of the AmigaWindow gadget.
	 */
	class WindowBorderTop : public Gadget {

	public:
		/**
		 * Constructor.
		 * @param x The x co-ordinate of the gadget.
		 * @param width The width of the gadget.
		 * @param height The height of the gadget.
		 * @param window Pointer to the window that contains this border.
		 * @param font The font to display the text with.
		 */
		WindowBorderTop(s16 x, u16 width, u16 height, Window* window, FontBase* font = NULL);

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the draw() function to draw all visible regions.
		 * @param clipRect The clipping region to draw.
		 * @see draw()
		 */
		virtual void draw(Rect clipRect);

		/**
		 * Give the gadget focus.
		 * @return True if the gadget received focus correctly.
		 */
		virtual bool focus();

		/**
		 * Remove focus from the gadget.
		 * @return True if the gadget lost focus correctly.
		 */
		virtual bool blur();

	protected:
		Window* _window;							/**< Pointer to the containing window */

		/**
		 * Destructor.
		 */
		virtual inline ~WindowBorderTop() {	};

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline WindowBorderTop(const WindowBorderTop& windowBorderTop) : Gadget(windowBorderTop) { };
	};
}

#endif
