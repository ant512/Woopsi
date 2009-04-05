#ifndef _DIMMED_SCREEN_H_
#define _DIMMED_SCREEN_H_

#include "screen.h"
#include "woopsifuncs.h"

namespace WoopsiUI {

	/**
	 * Hacky class that, rather than drawing to the screen, dims it to half of its intensity.
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
		 * @param font The font to use with the screen.
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

			// Get the current physical screen number
			u8 screen = getPhysicalScreenNumber();

			u16* rowStart = DrawBg[screen] + (clipRect.y * SCREEN_WIDTH);
			u16 colStart = clipRect.x;
			u16 colPos;
			
			// Loop through all pixels within the clip rect
			for (s16 y = 0; y < clipRect.height; y++) {

				// Reset to start pixel row
				colPos = colStart;

				for (s16 x = 0; x < clipRect.width; x++) {
				
					// Get pixel data directly from the framebuffer
					u16 colour = *(rowStart + colPos);
					
					// Halve the intensity of the colour (cheers Jeff)
					colour = ((colour  >> 1) & (15 | (15 << 5) | (15 << 10)));

					// Write back to framebuffer
					*(rowStart + colPos) = 0x8000 | colour;

					// Move to next pixel column
					colPos++;
				}

				// Move to next pixel row
				rowStart += SCREEN_WIDTH;
			}

			_flags.erased = false;
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
