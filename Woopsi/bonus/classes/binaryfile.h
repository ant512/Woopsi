#ifndef _BINARY_FILE_H_
#define _BINARY_FILE_H_

#include <stdio.h>
#include <nds.h>

namespace WoopsiUI {

	/**
	 * C++ wrapper around the basic C stdio functions.  Intended for
	 * parsing binary data files.  Can deal with files in either big-
	 * or little-endian format.
	 */
	class BinaryFile {
	public:

		/**
		 * Mode in which to open a file.
		 */
		enum FileMode {
			FILE_MODE_READ,				/**< Read-only access */
			FILE_MODE_WRITE,			/**< Write-only access; new data overwrites old */
			FILE_MODE_APPEND			/**< Write-only access; new data is appended to old */
		};

		/**
		 * Endianness of the data file.
		 */
		enum EndianMode {
			ENDIAN_MODE_BIG_ENDIAN,		/**< Data is stored in most significant byte first order */
			ENDIAN_MODE_LITTLE_ENDIAN	/**< Data is stored in least significant byte first order */
		};

		/**
		 * Constructor.  Automatically opens the file.
		 * @param filename Name of the file to open.
		 * @param fileMode Mode in which to open the file.
		 * @param endianMode Endian format of the file.
		 */
		BinaryFile(const char* filename, BinaryFile::FileMode fileMode, BinaryFile::EndianMode endianMode);

		/**
		 * Destructor.  Automatically closes the file.
		 */
		~BinaryFile();

		/**
		 * Read a single unsigned byte.
		 * @return The value.
		 */
		u8 readU8();

		/**
		 * Read two unsigned bytes.
		 * @return The value.
		 */
		u16 readU16();

		/**
		 * Read four unsigned bytes.
		 * @return The value.
		 */
		u32 readU32();

		/**
		 * Read a single signed byte.
		 * @return The value.
		 */
		s8 readS8();

		/**
		 * Read two signed bytes.
		 * @return The value.
		 */
		s16 readS16();

		/**
		 * Read four unsigned bytes.
		 * @return The value.
		 */
		s32 readS32();

		/**
		 * Write a single unsigned byte.
		 * @param value The data to write.
		 */
		void writeU8(u8 value);

		/**
		 * Write two unsigned bytes.
		 * @param value The data to write.
		 */
		void writeU16(u16 value);

		/**
		 * Write four unsigned bytes.
		 * @param value The data to write.
		 */
		void writeU32(u32 value);

		/**
		 * Write a single signed byte.
		 * @param value The data to write.
		 */
		void writeS8(s8 value);

		/**
		 * Write two signed bytes.
		 * @param value The data to write.
		 */
		void writeS16(s16 value);

		/**
		 * Write four signed bytes.
		 * @param value The data to write.
		 */
		void writeS32(s32 value);

		/**
		 * Get the size of the file.
		 * @return The size of the file in bytes.
		 */
		u32 getSize();

		/**
		 * Seek to a new position in the file.
		 * @param position The position to jump to, in bytes.
		 */
		void seek(u32 position);

	private:
		const char* _filename;			/**< The name of the file */
		FileMode _fileMode;				/**< The mode that the file has been opened with */
		EndianMode _endianMode;			/**< The endianness of the data in the file */
		FILE* _file;					/**< Pointer to the open file */

		/**
		 * Read an arbitrary number of bytes from the file with the correct endianness.
		 * @param numBytes The number of bytes to read.  Can be between 1 and 4.
		 * @return The data.
		 */
		u32 readData(u8 numBytes);

		/**
		 * Write an arbitrary number of bytes to the file with the correct endianness.
		 * @param data The data to write.
		 * @param numBytes The number of bytes to write.  Can be between 1 and 4.
		 */
		void writeData(u32 data, u8 numBytes);
	};
}

#endif