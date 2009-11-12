#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <nds.h>
#include "woopsiarray.h"
#include "bitmapbase.h"
#include "mutablebitmapbase.h"
#include "graphicsunclipped.h"

namespace WoopsiUI {

	class FontBase;

	/**
	 * Class providing bitmap manipulation (drawing, etc) functions.  Functions
	 * are all clipped to the size of the bitmap being drawn to.
	 */
	class Graphics : public GraphicsUnclipped {
	public:

		/**
		 * Constructor.
		 * @param width The width of the bitmap.
		 * @param height The height of the bitmap.
		 */
		Graphics(MutableBitmapBase* bitmap);

		/**
		 * Destructor.
		 */
		virtual inline ~Graphics() { };
		
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
		 * Draw an external bitmap to the internal bitmap.
		 * @param x The x co-ordinate to draw the bitmap to.
		 * @param y The y co-ordinate to draw the bitmap to.
		 * @param width The width of the bitmap to draw.
		 * @param height The height of the bitmap to draw.
		 * @param bitmap Pointer to the bitmap to draw.
		 * @param bitmapX The x co-ordinate within the supplied bitmap to use as the origin.
		 * @param bitmapY The y co-ordinate within the supplied bitmap to use as the origin.
		 * @param bitmapWidth The width of the supplied bitmap.
		 * @param bitmapHeight The height of the supplied bitmap.
		 */
		virtual void drawBitmap(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16  bitmapY);
		
		/**
		 * Draw a bitmap to the port's bitmap, using the supplied transparent colour
		 * as an invisible colour.  This is considerably slower than the standard bitmap
		 * drawing routine as it plots pixel-by-pixel instead of using a scanline DMA copy.
		 * @param x The x co-ordinate to draw the bitmap to.
		 * @param y The y co-ordinate to draw the bitmap to.
		 * @param width The width of the bitmap to draw.
		 * @param height The height of the bitmap to draw.
		 * @param bitmap Pointer to the bitmap to draw.
		 * @param bitmapX The x co-ordinate within the supplied bitmap to use as the origin.
		 * @param bitmapY The y co-ordinate within the supplied bitmap to use as the origin.
		 * @param transparentColour The transparent colour used in the bitmap.
		 */
		virtual void drawBitmap(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16 bitmapY, u16 transparentColour);

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

	protected:
		
		/**
		 * Clip the supplied rectangular dimensions to the size of the internal bitmap.
		 * @param x The x co-ordinate of the rectangle to clip (modified by the function).
		 * @param y The y co-ordinate of the rectangle to clip (modified by the function).
		 * @param width The width of the rectangle to clip (modified by the function).
		 * @param height The height of the rectangle to clip (modified by the function).
		 */
		bool clipBitmapCoordinates(s16* x, s16* y, u16* width, u16* height);

		/**
		 * Remove the last item from the flood fill stack and insert its properties into the
		 * supplied x and y variables.
		 * @param x The x co-ordinate of the pixel from the stack (output by the function).
		 * @param y The y co-ordinate of the pixel from the stack (output by the function).
		 * @param stack Pointer to the stack to pop from.
		 */
		bool popStack(s16* x, s16* y, WoopsiArray<s32>* stack);
		
		/**
		 * Push new co-ordinates onto the stack.
		 * @param x The x co-ordinate of the pixel to add to the stack.
		 * @param y The y co-ordinate of the pixel to add to the stack.
		 * @param stack The stack to add a new value to.
		 */
		void pushStack(s16 x, s16 y, WoopsiArray<s32>* stack);

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline Graphics(const Graphics& graphics) : GraphicsUnclipped(graphics) { };
	};
}

#endif
