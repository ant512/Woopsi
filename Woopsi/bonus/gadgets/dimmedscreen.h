#ifndef _DIMMED_SCREEN_H_
#define _DIMMED_SCREEN_H_

#include "screen.h"

using namespace std;

class DimmedScreen : public Screen {
public:
	/**
	 * Constructor.
	 * @param title The title of the screen; not displayed by default.
	 * @param font The font to use with the screen.
	 */
	DimmedScreen(char* title, u32 flags, FontBase* font = NULL) : Screen(title, flags, font) { };

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
				
				// Split into components
				s16 r = colour & 31;
				s16 g = (colour >> 5) & 31;
				s16 b = (colour >> 10) & 31;
			
				// Dim the colour
				if (r > 10) r -= 10; else r = 0;
				if (g > 10) g -= 10; else g = 0;
				if (b > 10) b -= 10; else b = 0;

				// Write back to framebuffer
				*(DrawBg[screen] + (x + (y * SCREEN_WIDTH))) = woopsiRGB(r, g, b);
			}
		}

		_flags.erased = false;
	};
};

#endif
