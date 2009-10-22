#include "bitmapio.h"
#include "woopsifuncs.h"

#define BITMAPINFOHEADER 40
#define BITMAP_HEADER_SIZE 14
#define BITMAP_RESOLUTION 0
#define COLOUR_PLANES 1

using namespace WoopsiUI;

Bitmap* BitmapIO::loadBMP(const char* filename) {

	// Open the file
	BinaryFile* file = new BinaryFile(filename, BinaryFile::FILE_MODE_READ, BinaryFile::ENDIAN_MODE_LITTLE_ENDIAN);

	// Load and parse header
	BMPHeader bmpHeader;
	DIBV3Header dibHeader;

	parseBMPHeader(file, bmpHeader);
	parseDIBV3Header(file, dibHeader);

	// Prepare a bitmap to store parsed pixel data
	Bitmap* bitmap = new Bitmap(dibHeader.width, dibHeader.height);

	parsePixelData(file, bmpHeader, dibHeader, bitmap);
	
	delete file;

	return bitmap;
}

void BitmapIO::saveBMP(const char* filename, Bitmap* bitmap) {
	
	// Open the file
	BinaryFile* file = new BinaryFile(filename, BinaryFile::FILE_MODE_WRITE, BinaryFile::ENDIAN_MODE_LITTLE_ENDIAN);
	
	// Image size calculated in bytes - multily by 3 as each pixel uses 3 bytes
	// All images saved as 24-bit
	u32 imageSize = bitmap->getWidth() * bitmap->getHeight() * 3;	// *FIX* cater for padding?
	u32 offset = BITMAPINFOHEADER + BITMAP_HEADER_SIZE;

	// Pixel rows must be aligned to 4-byte boundaries, so calculate total size
	// of padding bytes
	u8 mod = (bitmap->getWidth() * 3) % 4;
	u8 paddingBytes = 0;
	
	if (mod != 0) {
		paddingBytes = 4 - mod;
		imageSize += paddingBytes * bitmap->getHeight();
	}

	// Write the BMP header
	file->writeU8('B');						// Identifier
	file->writeU8('M');
	file->writeU32(imageSize + offset);		// Size
	file->writeU32(0);						// Two reserved shorts
	file->writeU32(offset);					// Offset
	
	// Write the V3 DIB header
	file->writeU32(BITMAPINFOHEADER);		// Header size
	file->writeS32(bitmap->getWidth());		// Width
	file->writeS32(bitmap->getHeight());	// Height
	file->writeU16(COLOUR_PLANES);			// Colour planes
	file->writeU16(24);						// Bits per pixel
	file->writeU32(0);						// Compression method
	file->writeU32(imageSize);				// Image size *FIX* cater for padding?
	file->writeU32(BITMAP_RESOLUTION);		// Horizontal resolution
	file->writeU32(BITMAP_RESOLUTION);		// Vertical resolution
	file->writeU32(0);						// Colours in palette
	file->writeU32(0);						// Important colours used
	
	// Write the pixel data
	for (s16 y = bitmap->getHeight() - 1; y >= 0; --y) {
		for (s16 x = 0; x < bitmap->getWidth(); ++x) {
			
			// Components of each pixel are stored in the following format:
			// b - 1 byte
			// g - 1 byte
			// r - 1 byte

			// Since the DS uses pixels in 5-bit, not 8-bit format, the
			// values extracted need to be shifted up 3 places
			u16 pixel = bitmap->getPixel(x, y);
			u8 b = ((pixel >> 10) & 31) << 3;
			u8 g = ((pixel >> 5) & 31) << 3;
			u8 r = (pixel & 31) << 3;
			
			// Write the components to the file in BGR order
			file->writeU8(b);
			file->writeU8(g);
			file->writeU8(r);
		}
		
		// Reached end of row of pixels - BMP data is aligned to 4-byte boundary, so write
		// padding bytes
		for (u8 i = 0; i < paddingBytes; ++i) {
			file->writeU8(0);
		}
	}
	
	delete file;
}

void BitmapIO::parseBMPHeader(BinaryFile* file, BMPHeader& bmpHeader) {
	bmpHeader.identifier[0] = file->readU8();
	bmpHeader.identifier[1] = file->readU8();
	bmpHeader.size = file->readU32();
	file->readU32();	// Two reserved and unused shorts
	bmpHeader.offset = file->readU32();
}

void BitmapIO::parseDIBV3Header(BinaryFile* file, DIBV3Header& dibHeader) {
	dibHeader.headerSize = file->readU32();
	dibHeader.width = file->readS32();
	dibHeader.height = file->readS32();
	dibHeader.colourPlanes = file->readU16();
	dibHeader.bitsPerPixel = file->readU16();
	dibHeader.compressionMethod = file->readU32();
	dibHeader.imageSize = file->readU32();
	dibHeader.horizontalResolution = file->readU32();
	dibHeader.verticalResolution = file->readU32();
	dibHeader.coloursInPalette = file->readU32();
	dibHeader.importantColoursUsed = file->readU32();
}

void BitmapIO::parsePixelData(BinaryFile* file, BMPHeader& bmpHeader, DIBV3Header& dibHeader, Bitmap* bitmap) {
	switch(dibHeader.bitsPerPixel) {
		case 1:
			
			// Monochrome bitmap
			break;
		case 4:

			// 16-colour bitmap
			break;
		case 8:

			// 256-colour bitmap
			break;
		case 16:

			// 16-bit bitmap
			break;
		case 24:

			// 24-bit bitmap
			parsePixelData24(file, bmpHeader, bitmap);
			break;
	}
}

void BitmapIO::parsePixelData24(BinaryFile* file, BMPHeader& bmpHeader, Bitmap* bitmap) {
	
	// Pixel rows must be aligned to 4-byte boundaries, so calculate total size
	// of padding bytes
	u8 mod = (bitmap->getWidth() * 3) % 4;
	u8 paddingBytes = 0;
	
	if (mod != 0) {
		paddingBytes = 4 - mod;
	}

	// Run through rows backwards as BMP rows are stored in reverse order
	for (s16 y = bitmap->getHeight() - 1; y >= 0; --y) {
		for (s16 x = 0; x < bitmap->getWidth(); ++x) {

			// Components of each pixel are stored in the following format:
			// b - 1 byte
			// g - 1 byte
			// r - 1 byte

			// Since the DS requires pixels in 5-bit, not 8-bit format, the
			// values extracted need to be shifted back 3 places
			u8 b = (file->readU8() >> 3) & 31;
			u8 g = (file->readU8() >> 3) & 31;
			u8 r = (file->readU8() >> 3) & 31;
			
			// Draw the pixel to the bitmap
			bitmap->drawPixel(x, y, woopsiRGB(r, g, b));
		}
		
		// Reached end of row of pixels - BMP data is aligned to 4-byte boundary, so skip
		// any remaining padding bytes
		for (u8 i = 0; i < paddingBytes; ++i) {
			file->readU8();
		}
	}
}