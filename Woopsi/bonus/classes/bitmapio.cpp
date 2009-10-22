#include "bitmapio.h"
#include "woopsifuncs.h"

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

	return bitmap;
}

void BitmapIO::saveBMP(const char* filename, Bitmap* bitmap) {
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

	file->seek(bmpHeader.offset);

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
	}
}
