#ifndef _DIMMED_SCREEN_H_
#define _DIMMED_SCREEN_H_

#include "screen.h"
#include "graphicsport.h"
#include "woopsi.h"

namespace WoopsiUI {

	/**
	 * Screen class that, rather than drawing to the screen, dims it to half of its intensity.
	 * Can be used as a screen for modal windows giving an Ubuntu-style faded background.  All
	 * child gadgets of the screen are not dimmed.
	 * Note that any activity that takes place behind the screen won't actually get drawn.  If,
	 * for example, you open a screen and put an animation on it, then open a DimmedScreen
	 * above it, the animation will appear to freeze.  This is because the bottom screen does
	 * not think it is visible since another screen is open above it.
	 * There are two workarounds for this.  One is to stop all animations/VBL events/etc on
	 * lower gadgets when opening a DimmedScreen so that they *do* actually freeze.  The other
	 * way is to register the DimmedScreen for VBL events and redraw every time a VBL event
	 * is raised, but this is likely to be very slow.
	 * 
	 */
	class DimmedScreen : public Screen {
	public:

		/**
		 * Constructor.
		 */
		DimmedScreen() : Screen("", 0) { };

		/**
		 * Draw the area of this gadget that falls within the clipping region.
		 * Called by the draw() function to draw all visible regions.
		 * @param clipRect The clipping region to draw.
		 * @see draw()
		 */
		virtual void draw(Rect clipRect) {
			
			// Erase the gadget, thus redrawing everything underneath it
			_flags.erased = false;
			disableDrawing();
			woopsiApplication->eraseRect(clipRect);
			enableDrawing();
			_flags.erased = false;
			
			// Dim the screen
			GraphicsPort* port = newInternalGraphicsPort(clipRect);
			port->dim(clipRect.x, clipRect.y, clipRect.width, clipRect.height);
			delete port;
		};
		
		/**
		 * Destructor.
		 */
		virtual inline ~DimmedScreen() { };
		
		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline DimmedScreen(const DimmedScreen& dimmedScreen) : Screen(dimmedScreen) { };
	};
}

#endif
