#ifndef _WINDOW_BORDER_TOP_H_
#define _WINDOW_BORDER_TOP_H_

#include <nds.h>
#include "gadget.h"
#include "gadgetstyle.h"

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
		 * @param style The style that the gadget should use.  If this is not
		 * specified, the gadget will use the values stored in the global
		 * defaultGadgetStyle object.  The gadget will copy the properties of
		 * the style into its own internal style object.
		 */
		WindowBorderTop(s16 x, u16 width, u16 height, Window* window, GadgetStyle* style = NULL);

	protected:
		Window* _window;							/**< Pointer to the containing window */

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the redraw() function to draw all visible regions.
		 * @param port The GraphicsPort to draw to.
		 * @see redraw()
		 */
		virtual void drawContents(GraphicsPort* port);

		/**
		 * Starts the dragging system.
		 * @param x The x co-ordinate of the click.
		 * @param y The y co-ordinate of the click.
		 */
		virtual void onClick(s16 x, s16 y);

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
