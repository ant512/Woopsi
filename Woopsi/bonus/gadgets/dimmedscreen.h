#ifndef _DIMMED_SCREEN_H_
#define _DIMMED_SCREEN_H_

#include "screen.h"

using namespace std;

/**
 * Hacky class that, rather than drawing to the screen, dims it to half of its intensity.
 * Can be used as a screen for modal windows giving an Ubuntu-style faded background.  All
 * child gadgets of the screen are not dimmed.
 */
class DimmedScreen : public Screen {
public:
	/**
	 * Constructor.
	 * @param font The font to use with the screen.
	 */
	DimmedScreen() : Screen("", 0) { };

	/**
	 * Override the Gadget::draw() method.
	 */
	virtual inline void draw() { Gadget::draw(); };

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
		
		// Loop through all pixels within the clip rect
		for (s16 y = clipRect.y; y < clipRect.y + clipRect.height; y++) {
			for (s16 x = clipRect.x; x < clipRect.x + clipRect.width; x++) {
			
				// Get pixel data directly from the framebuffer
				u16 colour = *(DrawBg[screen] + (x + (y * SCREEN_WIDTH)));
				
				// Halve the intensity of the colour (cheers Jeff)
				colour = ((colour  >> 1) & (15 | (15 << 5) | (15 << 10)));

				// Write back to framebuffer
				*(DrawBg[screen] + (x + (y * SCREEN_WIDTH))) = 0x8000 | colour;
			}
		}

		_flags.erased = false;
	};
};

#endif
