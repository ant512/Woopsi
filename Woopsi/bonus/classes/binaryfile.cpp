#include "binaryfile.h"

using namespace WoopsiUI;

BinaryFile::BinaryFile(const char* filename, BinaryFile::FileMode fileMode, BinaryFile::EndianMode endianMode) {

	switch(fileMode) {
		case FILE_MODE_APPEND:
			_file = fopen(filename, "ab");
			break;
		case FILE_MODE_READ:
			_file = fopen(filename, "rb");
			break;
		case FILE_MODE_WRITE:
			_file = fopen(filename, "wb");
			break;
	}

	_fileMode = fileMode;
	_endianMode = endianMode;
}

BinaryFile::~BinaryFile() {
	fclose(_file);
}

u8 BinaryFile::readU8() {
	return (u8)readData(1);
}

s8 BinaryFile::readS8() {
	return (s8)readData(1);
}

u16 BinaryFile::readU16() {
	return (u16)readData(2);
}

s16 BinaryFile::readS16() {
	return (s16)readData(2);
}

u32 BinaryFile::readU32() {
	return (u32)readData(4);
}

s32 BinaryFile::readS32() {
	return (s32)readData(4);
}

void BinaryFile::writeU8(u8 value) {
	writeData(value, 1);
}

void BinaryFile::writeS8(s8 value) {
	writeData(value, 1);
}

void BinaryFile::writeU16(u16 value) {
	writeData(value, 2);
}

void BinaryFile::writeS16(s16 value) {
	writeData(value, 2);
}

void BinaryFile::writeU32(u32 value) {
	writeData(value, 4);
}

void BinaryFile::writeS32(s32 value) {
	writeData(value, 4);
}

void BinaryFile::writeData(u32 data, u8 numBytes) {
	switch(_endianMode) {
		case ENDIAN_MODE_BIG_ENDIAN:

			// Write bytes to file in reverse order
			for (s8 i = numBytes - 1; i >= 0; --i) {
				fputc(0xff & (data >> (i * 8)), _file);
			}
			break;
		case ENDIAN_MODE_LITTLE_ENDIAN:

			// Write bytes to file in standard order
			for (u8 i = 0; i < numBytes; ++i) {
				fputc(0xff & (data >> (i * 8)), _file);
			}
			break;
	}
}

u32 BinaryFile::readData(u8 numBytes) {

	u32 output = 0;

	switch(_endianMode) {
		case ENDIAN_MODE_BIG_ENDIAN:
	
			// Extract bytes in reverse order
			for (s8 i = numBytes - 1; i >= 0; --i) {
				output |= (fgetc(_file) << (i * 8));
			}
			break;
		case ENDIAN_MODE_LITTLE_ENDIAN:

			// Extract bytes in standard order
			for (u8 i = 0; i < numBytes; ++i) {
				output |= (fgetc(_file) << (i * 8));
			}
			break;
	}

	return output;
}

u32 BinaryFile::getSize() {
	
	// Get current position so we can reset later
	u32 currentPos = ftell(_file);

	// Get size of file by seeking to the end
	u32 size = 0;
	fseek(_file, 0L, SEEK_END);
	size = ftell(_file);

	// Seek back to previous position
	seek(currentPos);

	return size;
}

void BinaryFile::seek(u32 position) {
	fseek(_file, position, SEEK_SET);
}