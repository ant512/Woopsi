#include "bitmapio.h"
#include "woopsifuncs.h"
#include "graphics.h"

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
	parseDIBHeader(file, dibHeader);

	// Prepare a bitmap to store parsed pixel data
	Bitmap* bitmap = new Bitmap(dibHeader.width, dibHeader.height);

	parsePixelData(file, bmpHeader, dibHeader, bitmap);
	
	delete file;

	return bitmap;
}

void BitmapIO::writeBMPHeader(BinaryFile* file, u32 imageSize, u32 offset) {
	file->writeU8('B');						// Identifier
	file->writeU8('M');
	file->writeU32(imageSize + offset);		// Size
	file->writeU32(0);						// Two reserved shorts
	file->writeU32(offset);					// Offset
}

void BitmapIO::writeDIBV3Header(BinaryFile* file, s32 width, s32 height, u16 bitsPerPixel, u32 imageSize) {
	file->writeU32(BITMAPINFOHEADER);		// Header size
	file->writeS32(width);					// Width
	file->writeS32(height);					// Height
	file->writeU16(COLOUR_PLANES);			// Colour planes
	file->writeU16(bitsPerPixel);			// Bits per pixel
	file->writeU32(0);						// Compression method
	file->writeU32(imageSize);				// Image size *FIX* cater for padding?
	file->writeU32(BITMAP_RESOLUTION);		// Horizontal resolution
	file->writeU32(BITMAP_RESOLUTION);		// Vertical resolution
	file->writeU32(0);						// Colours in palette
	file->writeU32(0);						// Important colours used
}

void BitmapIO::writePixelData(BinaryFile* file, const Bitmap* bitmap, u8 paddingBytes) {
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
}

void BitmapIO::saveBMP(const char* filename, const Bitmap* bitmap) {
	
	// Open the file
	BinaryFile* file = new BinaryFile(filename, BinaryFile::FILE_MODE_WRITE, BinaryFile::ENDIAN_MODE_LITTLE_ENDIAN);
	
	// Image size calculated in bytes - multily by 3 as each pixel uses 3 bytes
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
	writeBMPHeader(file, imageSize, offset);
	
	// Write the V3 DIB header
	writeDIBV3Header(file, bitmap->getWidth(), bitmap->getHeight(), 24, imageSize);
	
	// Write the pixel data
	writePixelData(file, bitmap, paddingBytes);
	
	delete file;
}

void BitmapIO::parseBMPHeader(BinaryFile* file, BMPHeader& bmpHeader) {
	bmpHeader.identifier[0] = file->readU8();
	bmpHeader.identifier[1] = file->readU8();
	bmpHeader.size = file->readU32();
	file->readU32();	// Two reserved and unused shorts
	bmpHeader.offset = file->readU32();
}

void BitmapIO::parseDIBHeader(BinaryFile* file, DIBV3Header& dibHeader) {
	dibHeader.headerSize = file->readU32();
	
	switch (dibHeader.headerSize) {
		case 12:
		case 64:
			// V1 and V2 unsupported
			break;
		case 40:
			parseDIBV3Header(file, dibHeader);
			break;
		case 108:
			// Treat V4 as V3
			parseDIBV3Header(file, dibHeader);
			break;
		case 124:
			// Treat V5 as V3
			parseDIBV3Header(file, dibHeader);
			break;
	}
}

void BitmapIO::parseDIBV3Header(BinaryFile* file, DIBV3Header& dibHeader) {
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
			parsePixelData16(file, bmpHeader, dibHeader, bitmap);
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

	// Jump to the start of the pixel data
	file->seek(bmpHeader.offset);
	
	// Get graphics context for bitmap
	Graphics* graphics = bitmap->newGraphics();

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
			graphics->drawPixel(x, y, woopsiRGB(r, g, b));
		}
		
		// Reached end of row of pixels - BMP data is aligned to 4-byte boundary, so skip
		// any remaining padding bytes
		for (u8 i = 0; i < paddingBytes; ++i) {
			file->readU8();
		}
	}
	
	// Clean up
	delete graphics;
}

void BitmapIO::parsePixelData16(BinaryFile* file, BMPHeader& bmpHeader, DIBV3Header& dibHeader, Bitmap* bitmap) {
	
	// Pixel rows must be aligned to 4-byte boundaries, so calculate total size
	// of padding bytes
	u8 mod = (bitmap->getWidth() * 2) % 4;
	u8 paddingBytes = 0;
	
	if (mod != 0) {
		paddingBytes = 4 - mod;
	}

	// Bitmasks for extracting components of colour from short
	u32 bMask = 0x001f;		// 5 bits blue
	u32 gMask = 0x03e0;		// 5 bits green
	u32 rMask = 0x7c00;		// 5 bits red

	// If compression method is set to BI_BITFIELDS, the default
	// bitmasks can be overridden with alternative formats
	if (dibHeader.compressionMethod == COMPRESSION_METHOD_BI_BITFIELDS) {
		rMask = file->readU32();
		gMask = file->readU32();
		bMask = file->readU32();

		// Jump to the start of the pixel data if we're not dealing with
		// a V3 header
		if (dibHeader.headerSize > BITMAPINFOHEADER) {
			file->seek(bmpHeader.offset);
		}
	}
		
	// Calculate the number of bits in each colour
	u8 bBits = countSetBits(bMask);
	u8 gBits = countSetBits(gMask);
	u8 rBits = countSetBits(rMask);

	u16 pixel = 0;
	u8 r;
	u8 g;
	u8 b;
	
	// Get graphics context for bitmap
	Graphics* graphics = bitmap->newGraphics();

	// Run through rows backwards as BMP rows are stored in reverse order
	for (s16 y = bitmap->getHeight() - 1; y >= 0; --y) {
		for (s16 x = 0; x < bitmap->getWidth(); ++x) {

			// Read pixel from file
			pixel = file->readU16();
	
			// Extract individual components of colours
			b = pixel & bMask;
			g = (pixel & gMask) >> bBits;
			r = (pixel & rMask) >> (bBits + gBits);

			// Convert to DS 5,5,5 format
			b = convertTo5Bit(b, bBits);
			g = convertTo5Bit(g, gBits);
			r = convertTo5Bit(r, rBits);

			// Draw the pixel to the bitmap
			graphics->drawPixel(x, y, woopsiRGB(r, g, b));
		}
		
		// Reached end of row of pixels - BMP data is aligned to 4-byte boundary, so skip
		// any remaining padding bytes
		for (u8 i = 0; i < paddingBytes; ++i) {
			file->readU8();
		}
	}
	
	// Clean up
	delete graphics;
}

u8 BitmapIO::countSetBits(u32 value) {
	u8 setBits = 0;
	while (value > 0) {
		setBits += (value & 1);
		value >>= 1;
	}
	return setBits;
}

u8 BitmapIO::convertTo5Bit(u8 component, u8 bits) {
	
	// Nothing to do?
	if (bits == 5) return component;

	// Too many bits - shift down
	if (bits > 5) return component >> (bits - 5);

	// Too few bits - shift up
	return component << (5 - bits);
}
