#ifndef _FRAME_BUFFER_H_
#define _FRAME_BUFFER_H_

#include <nds.h>
#include "mutablebitmapbase.h"

namespace WoopsiUI {

	class Graphics;

	/**
	 * Class wrapper around the framebuffer
	 */
	class FrameBuffer : public MutableBitmapBase {
	public:

		/**
		 * Constructor.
		 * @param data Pointer to the raw bitmap data.
		 * @param width The width of the bitmap.
		 * @param height The height of the bitmap.
		 */
		FrameBuffer(u16* data, u16 width, u16 height);

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
		 * Get a pointer to the internal bitmap.
		 * @return Pointer to the internal bitmap.
		 */
		inline const u16* getData() const { return _bitmap; };
		
		/**
		 * Get a non-const pointer to the internal bitmap.
		 * @return Non-const pointer to the internal bitmap.
		 */
		inline u16* getEditableData() const { return _bitmap; }

		/**
		 * Get a new graphics object that can draw to this bitmap.
		 * Object must be deleted when it is no longer needed.
		 * @return A new Graphics object.
		 */
		Graphics* newGraphics();

	protected:
		u16* _bitmap __attribute__ ((aligned (4)));		/**< Bitmap */

		/**
		 * Copy constructor is protected to prevent usage.
		 */
		inline FrameBuffer(const FrameBuffer& bitmap) { };
	};
}

#endif
