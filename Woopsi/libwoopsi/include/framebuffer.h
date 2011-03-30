#ifndef _FRAME_BUFFER_H_
#define _FRAME_BUFFER_H_

#include <nds.h>
#include "mutablebitmapbase.h"

namespace WoopsiUI {

	class Graphics;

	/**
	 * Bitmap subclass designed exclusively as a way to wrap the framebuffer
	 * in a bitmap object.  It can then be treated as any other bitmap - it
	 * can be drawn to with setPixel(), blit() and blitFill() methods, and
	 * it can produce a Graphics object for more complex drawing functions.
	 *
	 * The FrameBuffer class automatically switches from using the DS'
	 * framebuffer (more accurately, a 16-bit background) to using an SDL
	 * surface if Woopsi is compiled in SDL mode.
	 */
	class FrameBuffer : public MutableBitmapBase {
	public:

		/**
		 * Copies data from the supplied co-ordinates sequentially into dest.
		 * If the amount to be copied exceeds the available width of the bitmap,
		 * copying will wrap around from the right-hand edge of the bitmap to
		 * the left-hand edge.
		 * The dest parameter must point to an area of memory large enough to
		 * contain the copied data.
		 * @param x The x co-ordinate to copy from.
		 * @param y The y co-ordinate to copy from.
		 * @param size The number of pixels to copy.
		 * @param dest Pointer to the memory that will be copied into.
		 */
		void copy(s16 x, s16 y, u32 size, u16* dest) const;

#ifdef USING_SDL
		// SDL version
		
		/**
		 * Constructor.
		 * @param data Pointer to the raw bitmap data.
		 * @param width The width of the bitmap.
		 * @param height The height of the bitmap.
		 */
		FrameBuffer(SDL_Surface* surface, u16 width, u16 height, u16 yOffset);
		
		/**
		 * Get a pointer to the internal bitmap.
		 * @return Pointer to the internal bitmap.
		 */
		const u16* getData() const;
#else
		// DS version
		
		/**
		 * Constructor.
		 * @param data Pointer to the raw bitmap data.
		 * @param width The width of the bitmap.
		 * @param height The height of the bitmap.
		 */
		FrameBuffer(u16* data, u16 width, u16 height);
		
		/**
		 * Get a pointer to the internal bitmap.
		 * @return Pointer to the internal bitmap.
		 */
		inline const u16* getData() const { return _bitmap; };
#endif

		/**
		 * Destructor.
		 */
		virtual inline ~FrameBuffer() { };
		
		/**
		 * Get the colour of the pixel at the specified co-ordinates
		 * @param x The x co-ordinate of the pixel.
		 * @param y The y co-ordinate of the pixel.
		 * @return The colour of the pixel.
		 */
		const u16 getPixel(s16 x, s16 y) const;

		/**
		 * Set the colour of the specified pixel.
		 * @param x X co-ord of the pixel to set.
		 * @param y Y co-ord of the pixel to set.
		 * @param colour New colour of the pixel.
		 */
		virtual void setPixel(const s16 x, const s16 y, const u16 colour);

		/**
		 * Get a pointer to the internal bitmap data at the specified
		 * co-ordinates.
		 * @param x The x co-ord of the data.
		 * @param y The y co-ord of the data.
		 * @return Pointer to the internal bitmap data.
		 */
		inline const u16* getData(s16 x, s16 y) const;

		/**
		 * Get a new graphics object that can draw to this bitmap.
		 * Object must be deleted when it is no longer needed.
		 * @return A new Graphics object.
		 */
		Graphics* newGraphics();

		/**
		 * Blit data to the specified co-ordinates using the DMA hardware.
		 * @param x The x co-ordinate to blit to.
		 * @param y The y co-ordinate to blit to.
		 * @param data The data to blit.
		 * @param size The number of u16s to blit.
		 */
		void blit(const s16 x, const s16 y, const u16* data, const u32 size);

		/**
		 * Fill at the specified co-ordinates using the DMA hardware.
		 * @param x The x co-ordinate to blit to.
		 * @param y The y co-ordinate to blit to.
		 * @param colour The colour to fill with.
		 * @param size The number of u16s to blit.
		 */
		void blitFill(const s16 x, const s16 y, const u16 colour, const u32 size);

		/**
		 * Get the bitmap's width.
		 * @return The bitmap's width.
		 */
		inline const u16 getWidth() const { return _width; };

		/**
		 * Get the bitmap's height.
		 * @return The bitmap's height.
		 */
		inline const u16 getHeight() const { return _height; };

	protected:
		
#ifdef USING_SDL
		// SDL version
		SDL_Surface* _surface;	/**< Pointer to the SDL surface. */
		u16 _yOffset;			/**< Y offset from top of surface to draw. */
		u16* _dataBuffer;		/**< Buffer used when getData() is called. */

		/**
		 * Write a pixel to the SDL surface.
		 * @param x The x co-ordinate of the pixel to write to.
		 * @param y The y co-ordinate of the pixel to write to.
		 * @param pixel The colour to write.
		 */
		void putSDLPixel(int x, int y, Uint32 pixel);

		/**
		 * Get the colour of pixel on the SDL surface at the given co-ordinates.
		 * @param x The x co-ordinate of the pixel.
		 * @param y The y co-ordinate of the pixel.
		 * @return The pixel colour.
		 */
		Uint32 getSDLPixel(int x, int y);
#else
		// DS version
		u16* _bitmap __attribute__ ((aligned (4)));		/**< Bitmap. */
#endif

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline FrameBuffer(const FrameBuffer& bitmap) { };

		u16 _width;									/**< Width of the bitmap */
		u16 _height;								/**< Height of the bitmap */
	};
}

#endif
