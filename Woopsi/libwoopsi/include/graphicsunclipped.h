#ifndef _GRAPHICS_UNCLIPPED_H_
#define _GRAPHICS_UNCLIPPED_H_

#include <nds.h>
#include "woopsiarray.h"
#include "bitmapbase.h"
#include "mutablebitmapbase.h"

namespace WoopsiUI {

	class FontBase;

	/**
	 * Class providing bitmap manipulation (drawing, etc) functions.  Functions
	 * do 
	 */
	class GraphicsUnclipped {
	public:

		/**
		 * Constructor.
		 * @param width The width of the bitmap.
		 * @param height The height of the bitmap.
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
		 * Draw a string to the internal bitmap in a specific colour.
		 * @param x The x co-ordinate of the string.
		 * @param y The y co-ordinate of the string.
		 * @param font The font to draw with.
		 * @param string The string to output.
		 * @param colour The colour of the string.
		 */
		virtual void drawText(s16 x, s16 y, FontBase* font, const char* string, u16 colour);
		
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
		 * Fill a region of the internal bitmap with the specified colour.
		 * @param x The x co-ordinate to use as the starting point of the fill.
		 * @param y The y co-ordinate to use as the starting point of the fill.
		 * @param newColour The colour to fill with.
		 */
		virtual void floodFill(s16 x, s16 y, u16 newColour);

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

	protected:
		MutableBitmapBase* _bitmap;		/**< Bitmap */
		u16* _data;						/**< Bitmap data */
		u16 _width;
		u16 _height;
		
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
		inline GraphicsUnclipped(const GraphicsUnclipped& graphicsUnclipped) { };
	};
}

#endif
