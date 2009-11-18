#ifndef _GRAPHICS_UNCLIPPED_H_
#define _GRAPHICS_UNCLIPPED_H_

#include <nds.h>
#include "bitmapbase.h"
#include "mutablebitmapbase.h"

namespace WoopsiUI {

	class FontBase;

	/**
	 * Class providing bitmap manipulation (drawing, etc) functions.  Functions
	 * do not clip to the dimensions of the bitmap.
	 */
	class GraphicsUnclipped {
	public:

		/**
		 * Constructor.
		 * @param bitmap The bitmap that will be drawn to.
		 */
		GraphicsUnclipped(MutableBitmapBase* bitmap);

		/**
		 * Destructor.
		 */
		virtual inline ~GraphicsUnclipped() { };
		
		/**
		 * Get the colour of the pixel at the specified co-ordinates
		 * @param x The x co-ordinate of the pixel.
		 * @param y The y co-ordinate of the pixel.
		 * @return The colour of the pixel.
		 */
		virtual const u16 getPixel(s16 x, s16 y) const;

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
		 * Draw an unfilled rectangle to the internal bitmap.
		 * @param x The x co-ordinate of the rectangle.
		 * @param y The y co-ordinate of the rectangle.
		 * @param width The width of the rectangle.
		 * @param height The height of the rectangle.
		 * @param colour The colour of the rectangle.
		 */
		virtual void drawRect(s16 x, s16 y, u16 width, u16 height, u16 colour);
		
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
		 * Draw an unfilled circle to the internal bitmap.
		 * @param x0 The x co-ordinate of the circle.
		 * @param y0 The y co-ordinate of the circle.
		 * @param radius The radius of the circle.
		 * @param colour The colour of the circle.
		 */
		virtual void drawCircle(s16 x0, s16 y0, u16 radius, u16 colour);
		
		/**
		 * Draw a filled circle to the internal bitmap.
		 * @param x0 The x co-ordinate of the circle.
		 * @param y0 The y co-ordinate of the circle.
		 * @param radius The radius of the circle.
		 * @param colour The colour of the circle.
		 */
		virtual void drawFilledCircle(s16 x0, s16 y0, u16 radius, u16 colour);
		
		/**
		 * Draw a string to the internal bitmap.
		 * @param x The x co-ordinate of the string.
		 * @param y The y co-ordinate of the string.
		 * @param font The font to draw with.
		 * @param string The string to output.
		 */
		virtual void drawText(s16 x, s16 y, FontBase* font, const char* string);

		/**
		 * Draw a particular length of a string to the bitmap.
		 * @param x The x co-ordinate of the string.
		 * @param y The y co-ordinate of the string.
		 * @param font The font to draw with.
		 * @param length The number of characters to output.
		 * @param string The string to output.
		 */
		virtual void drawText(s16 x, s16 y, FontBase* font, u16 length, const char* string);
		
		/**
		 * Draw a string to the internal bitmap in a specific colour.
		 * @param x The x co-ordinate of the string.
		 * @param y The y co-ordinate of the string.
		 * @param font The font to draw with.
		 * @param string The string to output.
		 * @param colour The colour of the string.
		 */
		virtual void drawText(s16 x, s16 y, FontBase* font, const char* string, u16 colour);
		
		/**
		 * Draw a particular length of a string to the bitmap in a secific colour.
		 * @param x The x co-ordinate of the string.
		 * @param y The y co-ordinate of the string.
		 * @param font The font to draw with.
		 * @param length The number of characters to output.
		 * @param string The string to output.
		 * @param colour The colour of the string.
		 */
		virtual void drawText(s16 x, s16 y, FontBase* font, u16 length, const char* string, u16 colour);
		
		/**
		 * Draw a single character to the bitmap.
		 * @param x The x co-ordinate of the character.
		 * @param y The y co-ordinate of the character.
		 * @param font The font to draw with.
		 * @param letter The character to output.
		 */
		virtual void drawText(s16 x, s16 y, FontBase* font, char letter);
	
		/**
		 * Draw a single character to the bitmap in a specific colour.
		 * @param x The x co-ordinate of the character.
		 * @param y The y co-ordinate of the character.
		 * @param font The font to draw with.
		 * @param letter The character to output.
		 * @param colour The colour of the character.
		 */
		virtual void drawText(s16 x, s16 y, FontBase* font, char letter, u16 colour);

		/**
		 * Draw an external bitmap to the internal bitmap.
		 * @param x The x co-ordinate to draw the bitmap to.
		 * @param y The y co-ordinate to draw the bitmap to.
		 * @param width The width of the bitmap to draw.
		 * @param height The height of the bitmap to draw.
		 * @param bitmap Pointer to the bitmap to draw.
		 * @param bitmapX The x co-ordinate within the supplied bitmap to use as the origin.
		 * @param bitmapY The y co-ordinate within the supplied bitmap to use as the origin.
		 */
		virtual void drawBitmap(s16 x, s16 y, u16 width, u16 height, const BitmapBase* bitmap, s16 bitmapX, s16 bitmapY);

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
		 * Invert the colour of an filled rectangle of pixels.
		 * @param x The x co-ordinate of the rectangle.
		 * @param y The y co-ordinate of the rectangle.
		 * @param width The width of the rectangle.
		 * @param height The height of the rectangle.
		 */
		virtual void drawFilledXORRect(s16 x, s16 y, u16 width, u16 height);

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
		 * Convert the region to greyscale.
		 * @param x X co-ord of the region to change.
		 * @param y Y co-ord of the region to change.
		 * @param width Width of the region to change.
		 * @param height Height of the region to change.
		 */
		void greyScale(s16 x, s16 y, u16 width, u16 height);

	protected:
		MutableBitmapBase* _bitmap;		/**< Bitmap */
		u16 _width;						/**< Bitmap width */
		u16 _height;					/**< Bitmap height */

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline GraphicsUnclipped(const GraphicsUnclipped& graphicsUnclipped) { };
	};
}

#endif
