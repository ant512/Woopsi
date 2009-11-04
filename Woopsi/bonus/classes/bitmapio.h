#ifndef _BMPIO_H_
#define _BMPIO_H_

#include <nds.h>
#include "binaryfile.h"
#include "bitmap.h"

#ifdef ARM9
#include <fat.h>
#endif

namespace WoopsiUI {

	/**
	 * Provides static methods for loading and saving BMP files.  The class has a number of
	 * (intentional) limitations:
	 * - Compression is not supported.
	 * - Only DIB V3 headers are supported (this is the most common type).
	 */
	class BitmapIO {
	public:
	
		/**
		 * Loads the specified BMP and returns it as a Bitmap object.
		 * @param filename The name of the file to load.
		 * @return A bitmap.
		 */
		static Bitmap* loadBMP(const char* filename);
		
		/**
		 * Save the bitmap to a 24-bit BMP file with the specified name.
		 * @param filename Name of the file.
		 * @param bitmap Bitmap to save.
		 */
		static void saveBMP(const char* filename, const Bitmap* bitmap);
	private:

		/**
		 * Struct containing all information from the main BMP header.
		 */
		struct BMPHeader {
			char identifier[2];
			u32 size;
			u32 offset;
		};

		/**
		 * Struct containing all information from the V3 DIB header.
		 */
		struct DIBV3Header {
			u32 headerSize;
			s32 width;
			s32 height;
			u16 colourPlanes;
			u16 bitsPerPixel;
			u32 compressionMethod;
			u32 imageSize;
			u32 horizontalResolution;
			u32 verticalResolution;
			u32 coloursInPalette;
			u32 importantColoursUsed;
		};

		enum CompressionMethod {
			COMPRESSION_METHOD_BI_RGB = 0,
			COMPRESSION_METHOD_BI_BITFIELDS = 3
		};
		
		static void writeBMPHeader(BinaryFile* file, u32 imageSize, u32 offset);
		
		static void writeDIBV3Header(BinaryFile* file, s32 width, s32 height, u16 bitsPerPixel, u32 imageSize);

		static void writePixelData(BinaryFile* file, const Bitmap* bitmap, u8 paddingBytes);
		
		/**
		 * Parse the BMP header from the supplied file and insert the data into
		 * the supplied BMPHeader struct.
		 * @param file File to parse.
		 * @param bmpHeader Header struct to insert information into.
		 */
		static void parseBMPHeader(BinaryFile* file, BMPHeader& bmpHeader);

		/**
		 * Parse the DIB header contained in the supplied file call the correct
		 * procedure to parse the DIB version.
		 * @param file File to parse.  Should already be positioned at the
		 * start of the DIB data.
		 * @param dibHeader Header struct to insert information into.
		 */
		static void parseDIBHeader(BinaryFile* file, DIBV3Header& dibHeader);
		
		/**
		 * Parse the V3 DIB header contained in the supplied file and insert
		 * the data into the DIBHeader struct.
		 * @param file File to parse.  Should already be positioned at the
		 * start of the DIB data.
		 * @param dibHeader Header struct to insert information into.
		 */
		static void parseDIBV3Header(BinaryFile* file, DIBV3Header& dibHeader);
		
		/**
		 * Parse the pixel data from the supplied file and store it in the
		 * supplied bitmap.
		 * @param file File to parse.  Should already be positioned at the
		 * start of the pixel data.
		 * @param dibHeader Previously-extracted DIB header data.
		 * @param bmpHeader Previously-extracted BMP header data.
		 * @param bitmap Bitmap to store data in.
		 */
		static void parsePixelData(BinaryFile* file, BMPHeader& bmpHeader, DIBV3Header& dibHeader, Bitmap* bitmap);
		
		/**
		 * Parse the 24-bit pixel data from the supplied file and store it in
		 * the supplied bitmap.
		 * @param file File to parse.  Should already be positioned at the
		 * start of the pixel data.
		 * @param bmpHeader Previously-extracted BMP header data.
		 * @param bitmap Bitmap to store data in.
		 */
		static void parsePixelData24(BinaryFile* file, BMPHeader& bmpHeader, Bitmap* bitmap);

		/**
		 * Parse the 24-bit pixel data from the supplied file and store it in
		 * the supplied bitmap.
		 * @param file File to parse.  Should already be positioned at the
		 * start of the pixel data.
		 * @param bmpHeader Previously-extracted BMP header data.
		 * @param bitmap Bitmap to store data in.
		 */
		static void parsePixelData16(BinaryFile* file, BMPHeader& bmpHeader, DIBV3Header& dibHeader, Bitmap* bitmap);

		/**
		 * Count the number of set bits in the supplied value.
		 * @param value The value containing the bits to count.
		 * @return The number of set bits in the value.
		 */
		static u8 countSetBits(u32 value);

		/**
		 * Converts an RGB component from its native bit depth to
		 * the DS's 5 bits.
		 * @param component The RGB component to convert.
		 * @param bits The number of bits in the component.
		 */
		static u8 convertTo5Bit(u8 component, u8 bits);
	};
}

#endif