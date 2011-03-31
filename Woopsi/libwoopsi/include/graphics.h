#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <nds.h>
#include "woopsiarray.h"
#include "bitmapbase.h"
#include "mutablebitmapbase.h"
#include "rect.h"
#include "woopsistring.h"

/**
 * Converts separate RGB component values into a single 16-bit value for use
 * with the DS' framebuffer.  All supplied values should be 5 bits wide (ie.
 * between 0 and 31).
 * @param r The red component.
 * @param g The green component.
 * @param b The blue component.
 * @return The 15-bit colour value with the top alpha bit set.
 */
#define woopsiRGB(r, g, b) (((b) << 10) | ((g) << 5) | (r) | (1 << 15))

namespace WoopsiUI {

	class FontBase;

	/**
	 * Class providing bitmap manipulation (drawing, etc) functions.  Functions
	 * are all clipped to the size of the bitmap being drawn to.
	 */
	class Graphics {
	public:

		/**
		 * Constructor.
		 * @param bitmap The bitmap that the port will draw to. 
		 * @param clipRect The clipping region within which the class must draw.
		 */
		Graphics(MutableBitmapBase* bitmap, const Rect& clipRect);

		/**
		 * Destructor.
		 */
		virtual inline ~Graphics() { };

		/**
		 * Sets the clip rect.  Attempts to draw outside of this region
		 * will be clipped out.
		 * @param clipRect The new clipping region.
		 */
		void setClipRect(const Rect& clipRect);

		/**
		 * Get the current clipping region.
		 * @param rect A rect that will be populated with the current
		 * clipping region.
		 */
		void getClipRect(Rect& rect) const;
		
		/**
		 * Get the colour of the pixel at the specified co-ordinates
		 * @param x The x co-ordinate of the pixel.
		 * @param y The y co-ordinate of the pixel.
		 * @return The colour of the pixel.
		 */
		const u16 getPixel(s16 x, s16 y) const;

		/**
		 * Draw a pixel to the internal bitmap.
		 * @param x The x co-ordinate of the pixel.
		 * @param y The y co-ordinate of the pixel.
		 * @param colour The colour of the pixel.
		 */
		virtual void drawPixel(s16 x, s16 y, u16 colour);
		
		/**
		 * Draw a filled rectangle to the internal bitmap.
		 * @param x The x co-ordinate of the rectangle.
		 * @param y The y co-ordinate of the rectangle.
		 * @param width The width of the rectangle.
		 * @param height The height of the rectangle.
		 * @param colour The colour of the rectangle.
		 */
		virtual void drawFilledRect(s16 x, s16 y, u16 width, u16 height, u16 colour);
		
		/**
		 * Draw an unfilled rectangle to the internal bitmap.
		 * @param x The x co-ordinate of the rectangle.
		 * @param y The y co-ordinate of the rectangle.
		 * @param width The width of the rectangle.
		 * @param height The height of the rectangle.
		 * @param colour The colour of the rectangle.
		 */
		virtual void drawRect(s16 x, s16 y, u16 width, u16 height, u16 colour);

		/**
		 * XOR the colour of the pixel at the specified co-ordinates against
		 * the supplied colour.
		 * @param x The x co-ordinate of the pixel.
		 * @param y The y co-ordinate of the pixel.
		 * @param colour The colour to XOR against.
		 */
		virtual void drawXORPixel(s16 x, s16 y, u16 colour);
		
		/**
		 * XOR the colour of a horizontal line of pixels against the
		 * supplied colour.
		 * @param x The x co-ordinate of the line.
		 * @param y The y co-ordinate of the line.
		 * @param width The width of the line.
		 * @param colour The colour to XOR against.
		 */
		virtual void drawXORHorizLine(s16 x, s16 y, u16 width, u16 colour);
		
		/**
		 * XOR the colour of a vertical line of pixels against the
		 * supplied colour.
		 * @param x The x co-ordinate of the line.
		 * @param y The y co-ordinate of the line.
		 * @param height The height of the line.
		 * @param colour The colour to XOR against.
		 */
		virtual void drawXORVertLine(s16 x, s16 y, u16 height, u16 colour);
		
		/**
		 * XOR the colour of a filled rectangle of pixels against the
		 * supplied colour.
		 * @param x The x co-ordinate of the rectangle.
		 * @param y The y co-ordinate of the rectangle.
		 * @param width The width of the rectangle.
		 * @param height The height of the rectangle.
		 * @param colour The colour to XOR against.
		 */
		virtual void drawFilledXORRect(s16 x, s16 y, u16 width, u16 height, u16 colour);
		
		/**
		 * XOR the colour of an unfilled rectangle of pixels against the
		 * supplied colour.
		 * @param x The x co-ordinate of the rectangle.
		 * @param y The y co-ordinate of the rectangle.
		 * @param width The width of the rectangle.
		 * @param height The height of the rectangle.
		 * @param colour The colour to XOR against.
		 */
		virtual void drawXORRect(s16 x, s16 y, u16 width, u16 height, u16 colour);
		
		/**
		 * Invert the colour of the pixel at the specified co-ordinates.
		 * @param x The x co-ordinate of the pixel.
		 * @param y The y co-ordinate of the pixel.
		 */
		virtual void drawXORPixel(s16 x, s16 y);
		
		/**
		 * Invert the colour of a horizontal line of pixels.
		 * @param x The x co-ordinate of the line.
		 * @param y The y co-ordinate of the line.
		 * @param width The width of the line.
		 */
		virtual void drawXORHorizLine(s16 x, s16 y, u16 width);
		
		/**
		 * Invert the colour of a vertical line of pixels.
		 * @param x The x co-ordinate of the line.
		 * @param y The y co-ordinate of the line.
		 * @param height The height of the line.
		 */
		virtual void drawXORVertLine(s16 x, s16 y, u16 height);
		
		/**
		 * Invert the colour of an unfilled rectangle of pixels.
		 * @param x The x co-ordinate of the rectangle.
		 * @param y The y co-ordinate of the rectangle.
		 * @param width The width of the rectangle.
		 * @param height The height of the rectangle.
		 */
		virtual void drawXORRect(s16 x, s16 y, u16 width, u16 height);
		
		/**
		 * Invert the colour of a filled rectangle of pixels.
		 * @param x The x co-ordinate of the rectangle.
		 * @param y The y co-ordinate of the rectangle.
		 * @param width The width of the rectangle.
		 * @param height The height of the rectangle.
		 */
		virtual void drawFilledXORRect(s16 x, s16 y, u16 width, u16 height);
		
		/**
		 * Draw an external bitmap to the internal bitmap.
		 * @param x The x co-ordinate to draw the bitmap to.
		 * @param y The y co-ordinate to draw the bitmap to.
		 * @param width The width of the bitmap to draw.
		 * @param height The height of the bitmap to draw.
		 * @param bitmap Pointer to the bitmap to draw.
		 * @param bitmapX The x co-ordinate within the supplied bitmap to use as
		 * the origin.
		 * @param bitmapY The y co-ordinate within the supplied bitmap to use as
		 * the origin.
		 */
		virtual void drawBitmap(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16  bitmapY);
		
		/**
		 * Draw a bitmap to the port's bitmap, using the supplied transparent
		 * colour as an invisible colour.  This is considerably slower than the
		 * standard bitmap drawing routine as it plots pixel-by-pixel instead of
		 * using a scanline DMA copy.
		 * @param x The x co-ordinate to draw the bitmap to.
		 * @param y The y co-ordinate to draw the bitmap to.
		 * @param width The width of the bitmap to draw.
		 * @param height The height of the bitmap to draw.
		 * @param bitmap Pointer to the bitmap to draw.
		 * @param bitmapX The x co-ordinate within the supplied bitmap to use as
		 * the origin.
		 * @param bitmapY The y co-ordinate within the supplied bitmap to use as
		 * the origin.
		 * @param transparentColour The transparent colour used in the bitmap.
		 */
		virtual void drawBitmap(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16 bitmapY, u16 transparentColour);

		/**
		 * Draw a bitmap to the port's bitmap in greyscale.  This is
		 * considerably slower than the standard bitmap drawing routine as it
		 * plots pixel-by-pixel instead of using a scanline DMA copy.
		 * @param x The x co-ordinate to draw the bitmap to.
		 * @param y The y co-ordinate to draw the bitmap to.
		 * @param width The width of the bitmap to draw.
		 * @param height The height of the bitmap to draw.
		 * @param bitmap Pointer to the bitmap to draw.
		 * @param bitmapX The x co-ordinate within the supplied bitmap to use as
		 * the origin.
		 * @param bitmapY The y co-ordinate within the supplied bitmap to use as
		 * the origin.
		 */
		virtual void drawBitmapGreyScale(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16  bitmapY);

		/**
		 * Fill a region of the internal bitmap with the specified colour.
		 * @param x The x co-ordinate to use as the starting point of the fill.
		 * @param y The y co-ordinate to use as the starting point of the fill.
		 * @param newColour The colour to fill with.
		 */
		virtual void floodFill(s16 x, s16 y, u16 newColour);

		/**
		 * Copy a rectangular region from the source co-ordinates to the
		 * destination co-ordinates.  Uses the DMA for speed.  Worst-case
		 * scenario uses an off-screen buffer for situations wherein there
		 * is no vertical movement and the source and destination overlap,
		 * in which case two copies are performed for each row (source to
		 * buffer and buffer to destination).  Does not clip.
		 * @param sourceX Source x co-ord.
		 * @param sourceY Source y co-ord.
		 * @param destX Destination x co-ord.
		 * @param destY Destination y co-ord.
		 * @param width Width of the rectangle to copy.
		 * @param height Height of the rectangle to copy.
		 */
		void copy(s16 sourceX, s16 sourceY, s16 destX, s16 destY, u16 width, u16 height);

		/**
		 * Halve the brightness of a specified region.
		 * @param x X co-ord of the region to dim.
		 * @param y Y co-ord of the region to dim.
		 * @param width Width of the region to dim.
		 * @param height Height of the region to dim.
		 */
		void dim(s16 x, s16 y, u16 width, u16 height);

		/**
		 * Draw a line to the internal bitmap.
		 * @param x1 The x co-ordinate of the start point of the line.
		 * @param y1 The y co-ordinate of the start point of the line.
		 * @param x2 The x co-ordinate of the end point of the line.
		 * @param y2 The y co-ordinate of the end point of the line.
		 * @param colour The colour of the line.
		 */
		virtual void drawLine(s16 x1, s16 y1, s16 x2, s16 y2, u16 colour);
		
		/**
		 * Draw an unfilled ellipse to the bitmap.
		 * @param xCentre The x co-ordinate of the ellipse's centre.
		 * @param yCentre The y co-ordinate of the ellipse's centre.
		 * @param horizRadius The size of the ellipse's horizontal radius.
		 * @param vertRadius The size of the ellipse's vertical radius.
		 * @param colour The colour of the ellipse.
		 */
		virtual void drawEllipse(s16 xCentre, s16 yCentre, s16 horizRadius, s16 vertRadius, u16 colour);
		
		/**
		 * Draw a filled ellipse to the bitmap.
		 * @param xCentre The x co-ordinate of the ellipse's centre.
		 * @param yCentre The y co-ordinate of the ellipse's centre.
		 * @param horizRadius The size of the ellipse's horizontal radius.
		 * @param vertRadius The size of the ellipse's vertical radius.
		 * @param colour The colour of the ellipse.
		 */
		virtual void drawFilledEllipse(s16 xCentre, s16 yCentre, s16 horizRadius, s16 vertRadius, u16 colour);

		/**
		 * Convert the region to greyscale.
		 * @param x X co-ord of the region to change.
		 * @param y Y co-ord of the region to change.
		 * @param width Width of the region to change.
		 * @param height Height of the region to change.
		 */
		void greyScale(s16 x, s16 y, u16 width, u16 height);
		
		/**
		 * Draw a string to the internal bitmap.
		 * @param x The x co-ordinate of the string.
		 * @param y The y co-ordinate of the string.
		 * @param font The font to draw with.
		 * @param string The string to output.
		 */
		virtual void drawText(s16 x, s16 y, FontBase* font, const WoopsiString& string);

		/**
		 * Draw a particular length of a string to the bitmap in a specific
		 * colour.
		 * @param x The x co-ordinate of the string.
		 * @param y The y co-ordinate of the string.
		 * @param font The font to draw with.
		 * @param string The string to output.
		 * @param startIndex The start index within the string from which
		 * drawing will commence.
		 * @param length The number of characters to draw.
		 * @param colour The colour of the string.  If this is not specified the
		 * font's default colour will be used.
		 */
		virtual void drawText(s16 x, s16 y, FontBase* font, const WoopsiString& string, s32 startIndex, s32 length, u16 colour = 0);

		/**
		 * Draw a portion of a string on a baseline (chars may have different
		 * tops/heights/depths) to the bitmap.
		 * @param x The x co-ordinate of the string.
		 * @param y The y co-ordinate of the string.
		 * @param font The font to draw with.
		 * @param string The string to output.
		 * @param startIndex The start index within the string from which
		 * drawing will commence.
		 * @param length The number of characters to draw.
		 * @param colour The colour of the string.  If this is not specified the
		 * font's default colour will be used.
		 */		 
        void drawBaselineText(s16 x, s16 y, FontBase* font, const WoopsiString& string, s32 startIndex, s32 length, u16 colour = 0);		
		
		/**
		 * Scroll a region by a specified distance in two dimensions.  Performs
		 * a clipped copy to achieve scroll effect.
		 * @param x X co-ord of the area to scroll.
		 * @param y Y co-ord of the area to scroll.
		 * @param xDistance Horizontal distance to scroll.
		 * @param yDistance Vertical distance to scroll.
		 * @param width Width of the area to scroll.
		 * @param height Height of the area to scroll.
		 * @param revealedRects Populated with rects representing the region
		 * uncovered by the scroll method.  This should be empty when passed,
		 * and the regions should be drawn to once the scroll has finished.
		 */
		virtual void scroll(s16 x, s16 y, s16 xDistance, s16 yDistance, u16 width, u16 height, WoopsiArray<Rect>* revealedRects);

		/**
		 * Draw a bevelled rectangle to the bitmap.
		 * @param x The x co-ordinate of the rectangle.
		 * @param y The y co-ordinate of the rectangle.
		 * @param width The width of the rectangle.
		 * @param height The height of the rectangle.
		 * @param shineColour The colour of the top/left sides.
		 * @param shadowColour The colour of the bottom/right sides.
		 */
		virtual void drawBevelledRect(s16 x, s16 y, u16 width, u16 height, u16 shineColour, u16 shadowColour);

	protected:
		MutableBitmapBase* _bitmap;		/**< Bitmap */
		u16 _width;						/**< Bitmap width */
		u16 _height;					/**< Bitmap height */
		Rect _clipRect;					/**< Clipping rect that the object must draw within. */

		/**
		 * Draw a horizontal line to the internal bitmap.
		 * @param x The x co-ordinate of the line.
		 * @param y The y co-ordinate of the line.
		 * @param width The width of the line.
		 * @param colour The colour of the line.
		 */
		virtual void drawHorizLine(s16 x, s16 y, u16 width, u16 colour);
		
		/**
		 * Draw a vertical line to the internal bitmap.
		 * @param x The x co-ordinate of the line.
		 * @param y The y co-ordinate of the line.
		 * @param height The height of the line.
		 * @param colour The colour of the line.
		 */
		virtual void drawVertLine(s16 x, s16 y, u16 height, u16 colour);

		/**
		 * Draw an unfilled circle to the internal bitmap.  Faster than
		 * drawEllipse() but limited to circles only.
		 * @param x0 The x co-ordinate of the circle.
		 * @param y0 The y co-ordinate of the circle.
		 * @param radius The radius of the circle.
		 * @param colour The colour of the circle.
		 */
		virtual void drawCircle(s16 x0, s16 y0, u16 radius, u16 colour);
		
		/**
		 * Draw a filled circle to the internal bitmap.  Faster than
		 * drawFilledEllipse() but limited to circles only.
		 * @param x0 The x co-ordinate of the circle.
		 * @param y0 The y co-ordinate of the circle.
		 * @param radius The radius of the circle.
		 * @param colour The colour of the circle.
		 */
		virtual void drawFilledCircle(s16 x0, s16 y0, u16 radius, u16 colour);

		/**
		 * Clip the supplied rectangular dimensions to the size of the internal
		 * bitmap.
		 * @param x The x co-ordinate of the rectangle to clip (modified by the
		 * function).
		 * @param y The y co-ordinate of the rectangle to clip (modified by the
		 * function).
		 * @param width The width of the rectangle to clip (modified by the
		 * function).
		 * @param height The height of the rectangle to clip (modified by the
		 * function).
		 */
		bool clipBitmapCoordinates(s16* x, s16* y, u16* width, u16* height);

		/**
		 * Remove the last item from the flood fill stack and insert its
		 * properties into the
		 * supplied x and y variables.
		 * @param x The x co-ordinate of the pixel from the stack (output by the
		 * function).
		 * @param y The y co-ordinate of the pixel from the stack (output by the
		 * function).
		 * @param stack The stack to pop from.
		 */
		bool popStack(s16* x, s16* y, WoopsiArray<s32>& stack);
		
		/**
		 * Push new co-ordinates onto the stack.
		 * @param x The x co-ordinate of the pixel to add to the stack.
		 * @param y The y co-ordinate of the pixel to add to the stack.
		 * @param stack The stack to add a new value to.
		 */
		void pushStack(s16 x, s16 y, WoopsiArray<s32>& stack);

		/**
		 * Get the clipping code for the given co-ordinates based on the
		 * given clipping region.
		 * @param x X co-ordinate to clip.
		 * @param y Y co-ordinate to clip.
		 * @param xMin Minimum x value.
		 * @param yMin Minimum y value.
		 * @param xMax Maximum x value.
		 * @param yMax Maximum y value.
		 */
		u8 getClipLineOutCode(s16 x, s16 y, s16 xMin, s16 yMin, s16 xMax, s16 yMax);

		/**
		 * Draws a line.  The parameters must be pre-clipped by the drawLine()
		 * method.
		 * @param x1 The x co-ord of the start of the line.
		 * @param y1 The y co-ord of the start of the line.
		 * @param x2 The x co-ord of the end of the line.
		 * @param y2 The y co-ord of the end of the line.
		 * @param colour The colour of the line.
		 */
		void drawClippedLine(s16 x1, s16 y1, s16 x2, s16 y2, u16 colour);

		/**
		 * Draws a bitmap in greyscale.  The parameters must be pre-clipped by
		 * the drawBitmapGreyScale method.
		 * @param x The x co-ordinate to draw the bitmap to.
		 * @param y The y co-ordinate to draw the bitmap to.
		 * @param width The width of the bitmap to draw.
		 * @param height The height of the bitmap to draw.
		 * @param bitmap Pointer to the bitmap to draw.
		 * @param bitmapX The x co-ordinate within the supplied bitmap to use as
		 * the origin.
		 * @param bitmapY The y co-ordinate within the supplied bitmap to use as
		 * the origin.
		 */
		void drawClippedBitmapGreyScale(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16 bitmapY);

		/**
		 * Clips the supplied co-ordinates so that they fit within the supplied
		 * clipping rectangle.  If the clipped area is empty, the method returns
		 * false to indicate this.  Otherwise, it returns true.
		 * @param x1 The top-left x co-ordinate.
		 * @param y1 The top-left y co-ordinate.
		 * @param x2 The bottom-right x co-ordinate.
		 * @param y2 The bottom-right y co-ordinate.
		 * @param clipRect A rect to clip to.
		 */
		bool clipCoordinates(s16* x1, s16* y1, s16* x2, s16* y2, const Rect& clipRect);

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline Graphics(const Graphics& graphics) { };
	};
}

#endif
