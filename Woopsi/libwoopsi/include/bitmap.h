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
		 * Get a non-const pointer to the internal bitmap.
		 * @return Non-const pointer to the internal bitmap.
		 */
		inline u16* getEditableData() const { return _bitmap; };

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
		inline Bitmap(const Bitmap& bitmap) { };
	};
}

#endif
