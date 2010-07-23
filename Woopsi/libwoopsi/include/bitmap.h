#ifndef _BITMAP_H_
#define _BITMAP_H_

#include <nds.h>
#include "woopsiarray.h"
#include "mutablebitmapbase.h"

namespace WoopsiUI {

	class Graphics;

	/**
	 * Class providing bitmap manipulation (drawing, etc) functions.  Creates a new
	 * internal bitmap data array and uses it to draw onto.
	 */
	class Bitmap : public MutableBitmapBase {
	public:

		/**
		 * Constructor.
		 * @param width The width of the bitmap.
		 * @param height The height of the bitmap.
		 */
		Bitmap(u16 width, u16 height);

		/**
		 * Destructor.
		 */
		virtual inline ~Bitmap() {
			delete[] _bitmap;
		};
		
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
		void setPixel(const s16 x, const s16 y, const u16 colour);

		/**
		 * Get a pointer to the internal bitmap.
		 * @return Pointer to the internal bitmap.
		 */
		inline const u16* getData() const { return _bitmap; };

		/**
		 * Get a pointer to the internal bitmap data at the specified
		 * co-ordinates.
		 * @param x The x co-ord of the data.
		 * @param y The y co-ord of the data.
		 * @return Pointer to the internal bitmap data.
		 */
		const u16* getData(s16 x, s16 y) const;

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

	protected:
		u16* _bitmap __attribute__ ((aligned (4)));		/**< Bitmap */
		
		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline Bitmap(const Bitmap& bitmap) { };
	};
}

#endif
