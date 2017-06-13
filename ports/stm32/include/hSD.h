/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#ifndef H_SD_H
#define H_SD_H

#include "hTypes.h"
#include "hMutex.h"
#include "hPrintfDev.h"

namespace hFramework
{

class hFileIterator;

/**
 * @brief Provides interface to file object
 */
class hFile
{
public:
	/**
	 * @brief File modes
	 */
	enum FileMode
	{
		//! Specifies read access to the object. Data can be read from the file.
		//! Combine with MODE_WRITE for read-write access.
		MODE_READ = 1,
		//! Specifies write access to the object. Data can be written to the file.
		//! Combine with MODE_READ for read-write access.
		MODE_WRITE = 2,
		//! Opens the file. The function fails if the file is not existing.
		MODE_OPEN_EXISTING = 4,
		//! Opens the file if it is existing. If not, a new file is created.
		//! To append data to the file, use seek method after file open in this method.
		MODE_OPEN_ALWAYS = 8,
		//! Creates a new file. The function fails with hFile::ErrorCodes::ERROR_FILE_ERR if the file is existing.
		MODE_CREATE_NEW = 16,
		//! Creates a new file. If the file is existing, it will be truncated and overwritten.
		MODE_CREATE_ALWAYS = 32,
	};
	/**
	 * @brief Error codes
	 */
	enum ErrorCodes
	{
		ERROR_OK = 0, //!< No error
		ERROR_DISK_ERR, //!< Disk error
		ERROR_FILE_ERR, //!< File error
		ERROR_FILE_NOT_EXISTS_ERR, //!< File not exists error
		ERROR_INVALID_OBJECT, //!< Invalid object error
	};

	hFile(const hFile& prev) = delete;

	hFile(); //!< Create empty instance of a file object.
	~hFile(); //!< Destroy file object

	/**
	 * @brief Close file.
	 * @return Error code of the operation.
	 */
	hFile::ErrorCodes close();

	/**
	 * @brief Advance file position indicator by specified offset.
	 * @param ofs offset in bytes.
	 * @return Error code of the operation.
	 */
	hFile::ErrorCodes seek(uint32_t ofs);

	/**
	 * @brief Sync file object with the underlying persistent storage.
	 * @return Error code of the operation.
	 */
	hFile::ErrorCodes sync();

	/**
	 * @brief Return file size.
	 * @return file size in bytes
	 */
	int32_t size();

	/**
	 * @brief Return file positin indicator.
	 * @return file position indicator in bytes from the beginning.
	 */
	int32_t position();

	/**
	 * @brief Put single character to the file.
	 * @param c a character to be put
	 * @return When the character was written successfuly, it returns number of characters written.
	 * When the function failed due to disk full or any error, an EOF (-1) will be returned.
	 */
	int putch(char c);

	/**
	 * @brief Write a string to the file.
	 * @param str Pointer to the null terminated string to be written. The terminator character will not be written.
	 * @return When the function succeeded, it returns number of characters written.
	 * When the write operation is aborted due to disk full or any error, an EOF (-1) will be returned.
	 */
	int puts(const char* str);

	/**
	 * @brief Write data to the file.
	 *
	 * @param buff pointer to the data to be written
	 * @param len number of bytes to write
	 * @param bytesWritten pointer to the uint16_t variable to return the number of bytes written.
	 * The value is always valid after the function call regardless of the result
	 * @return Error code of the operation.
	 */
	hFile::ErrorCodes write(const char* buff, uint16_t len, uint16_t* bytesWritten);

	/**
	 * @brief Write formatted string to the file.
	 * @param str pointer to null terminated format string
	 * @param ... - optional arguments
	 * @return When the function succeeded, it returns number of characters written.
	 * When the function failed due to disk full or any error, an EOF (-1) will be returned.
	 */
	int printf(const char* str, ...);

	/**
	 * @brief Read data from the file.
	 * @param buff pointer to the buffer to store read data
	 * @param len number of bytes to read
	 * @return Bytes read from the file.
	 */
	uint32_t read(char* buff, int len);

	/**
	 * @brief Read a single character from the file.
	 * @return Character read from the file.
	 */
	char read();
private:
	friend class hFileDescription;
	friend class hSD;
	class hFileImplementation;
	hFileImplementation *impl;
};

inline hFile::FileMode operator|(hFile::FileMode a, hFile::FileMode b)
{
	return (hFile::FileMode)((int)(a) | (int)(b));
}

/**
 * @brief Provides file information such as name, path, type
 */
class hFileDescription
{
public:
	hFileDescription(const hFileDescription& prev) = delete;

	hFileDescription(); //!< Initialize object
	~hFileDescription(); //!< Destroy object

	/**
	 * @brief Get file name.
	 * @param out buffer to store file name
	 */
	void getName(char* out);

	/**
	 * @brief Get file path.
	 * @param out buffer to store file path
	 */
	void getPath(char* out);

	/**
	 * @brief Return if file is a directory.
	 * @return true, if file is a directory, otherwise false.
	 */
	bool isDirectory();

	/**
	 * @brief Return iterator for listing files in this directory
	 * @param out directory iterator
	 * @return Error code of the operation.
	 */
	hFile::ErrorCodes listDirectory(hFileIterator& out);

	/**
	 * @brief Open file with specified mode.
	 * @param file reference to file object
	 * @param mode open mode
	 * @return Error code of the operation.
	 */
	hFile::ErrorCodes openFile(hFile& file, hFile::FileMode mode);

private:
	friend class hFileIterator;
	class hFileDescriptionImplementation;
	hFileDescriptionImplementation *impl;
};

/**
 * @brief Provides file iterator used in listing directory entries.
 */
class hFileIterator
{
public:
	hFileIterator(const hFileIterator& prev) = delete;

	hFileIterator(); //!< Initialize iterator object.
	~hFileIterator(); //!< Destroy iterator object.

	/**
	 * @brief Get current path.
	 * @param out buffer to store current path
	 */
	void getPath(char* out);

	/**
	 * @brief Advance iterator position to the next entry.
	 * @param out reference to the file descriptor
	 * @return Error code of the operation.
	 */
	hFile::ErrorCodes next(hFileDescription & out);

	/**
	 * @brief Move iterator position to the beginning of the directory.
	 */
	void rewind();

private:
	class hFileIteratorImplementation;
	hFileIteratorImplementation * impl;
	friend class hFileDescription;
	friend class hSD;
};

/**
 * @brief Provides access to the SD card.
 */
class hSD
{
public:
	hSD(); //!< Initialize SD card object.
	~hSD(); //!< Destroy SD card object.

	/**
	 * @brief Attach SD card to the system.
	 * @return Error code of the operation.
	 */
	hFile::ErrorCodes mount();

	/**
	 * @brief Detach SD card from the system.
	 * @return Error code of the operation.
	 */
	hFile::ErrorCodes unmount();

	/**
	 * @brief Create a new directory.
	 * @param path pointer to the null-terminated string that specifies the directory name to create.
	 * @return Error code of the operation.
	 */
	hFile::ErrorCodes mkdir(const char* path);

	/**
	 * @brief List files and directories.
	 * @param dir directory iterator
	 * @param path Path to the directory to be listed
	 * @return Error code of the operation.
	 */
	hFile::ErrorCodes listDirectory(hFileIterator& dir, const char* path); // todo

	/**
	 * @brief Open file with specified mode.
	 * @param file reference to file object
	 * @param path path to be opened
	 * @param mode open mode
	 * @return Error code of the operation.
	 */
	hFile::ErrorCodes openFile(hFile &file, const char* path, hFile::FileMode mode); // todo

	/**
	 * @brief Change the current directory of a drive.
	 * @param path pointer to the null-terminated string that specifies a directory to go
	 * @return Error code of the operation.
	 */
	hFile::ErrorCodes chdir(const char* path);

	/**
	 * @brief Return the current directory
	 * @param buffer pointer to the buffer to receive the current directory string.
	 * @param buffer_size size of the buffer
	 * @return Error code of the operation.
	 */
	hFile::ErrorCodes getcwd(char* buffer, uint16_t buffer_size);

	/**
	 * @brief Remove a file or sub-directory.
	 * @param path pointer to the null-terminated string that specifies an object to be removed
	 * @return Error code of the operation.
	 */
	hFile::ErrorCodes remove(const char * path);

	/**
	 * @brief Rename a file or sub-directory.
	 * @param path_old pointer to a null-terminated string that specifies an existing file or sub-directory to be renamed
	 * @param path_new pointer to a null-terminated string that specifies the new object name.
	 * The drive number specified in this string is ignored and one determined by path_old is used instead.
	 * @return Error code of the operation.
	 */
	hFile::ErrorCodes rename(const char* path_old, const char* path_new);

	/**
	 * @brief Checks if a file or directory exists.
	 * @param path pointer to the null-terminated string that specifies an object to be checked.
	 * @return true, if object exists, otherwise false.
	 */
	bool exists(const char* path);
	//FRESULT getFreeSpace (uint32_t& total, uint32_t& free);
private:
	class hSDImplementation;
	hSDImplementation * impl;

	hSD(const hSD&) = delete;
};

/**
 * @brief Provides SD card based log device
 */
class hSDLog : public hPrintfDev
{
	char *fileName;
	hFile file;
	void open();
	void close();

public:
	hSDLog() { }

	/**
	 * @brief Initialize SD card log file
	 * @param fileName log file name
	 */
	void init(char* fileName);

#ifdef DOXYGEN
	/**
	 * @brief Write formatted string to the file.
	 * @param str pointer to null terminated format string
	 * @param ... - optional arguments
	 * @return When the function succeeded, it returns number of characters written.
	 * When the function failed due to disk full or any error, an EOF (-1) will be returned.
	 */
	int printf(const char* str, ...);
#endif

	/**
	 * @brief Write formatted string to the file.
	 * @param str pointer to null terminated format string
	 * @param arg arguments as a va_list
	 * @return When the function succeeded, it returns number of characters written.
	 * When the function failed due to disk full or any error, an EOF (-1) will be returned.
	 */
	int vprintf(const char* format, va_list arg);

	// hStreamDev
	int write(const void* data, int len, int timeout)
	{
		return 0;
	}
	int read(void* data, int len, int timeout)
	{
		return 0;
	}

	hSDLog(const hSDLog&) = delete;
};

}

#endif
