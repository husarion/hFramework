/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

#include <stdio.h>
#include <string.h>
#include "hSD.h"
#include "hSystem.h"

#include "myGPIO.h"
#include "hPrintfDev.h"
#include "hSerial.h"
#include "hSystem.h"
#include <hPrintf.h>

#include "ff.h"
#include "diskio.h"

#include "port.h"

const int path_buf_length = 20;

namespace hFramework
{

hFile::ErrorCodes FRESULTToErrorCode(FRESULT err)
{
	if (err == FR_OK)
		return hFile::ERROR_OK;
	if (err == FR_DISK_ERR || err == FR_INT_ERR || err == FR_NOT_READY
	    || err == FR_WRITE_PROTECTED || err == FR_INVALID_DRIVE
	    || err == FR_NOT_ENABLED || err == FR_NO_FILESYSTEM
	    || err == FR_MKFS_ABORTED || err == FR_TIMEOUT || err == FR_LOCKED
	    || err == FR_NOT_ENOUGH_CORE || err == FR_INVALID_PARAMETER)
		return hFile::ERROR_DISK_ERR;
	if (err == FR_INVALID_NAME || err == FR_DENIED || err == FR_EXIST)
		return hFile::ERROR_FILE_ERR;
	if (err == FR_NO_FILE || err == FR_NO_PATH)
		return hFile::ERROR_FILE_NOT_EXISTS_ERR;
	if (err == FR_INVALID_OBJECT)
		return hFile::ERROR_INVALID_OBJECT;
	return hFile::ERROR_OK;
}
uint8_t hFileModeToFatFSMode(hFile::FileMode mode)
{
	uint8_t modeFatFS = 0;
	if (mode & hFile::MODE_CREATE_ALWAYS)
	{
		modeFatFS |= FA_CREATE_ALWAYS;
	}
	if (mode & hFile::MODE_CREATE_NEW)
	{
		modeFatFS |= FA_CREATE_NEW;
	}
	if (mode & hFile::MODE_OPEN_ALWAYS)
	{
		modeFatFS |= FA_OPEN_ALWAYS;
	}
	if (mode & hFile::MODE_OPEN_EXISTING)
	{
		modeFatFS |= FA_OPEN_EXISTING;
	}
	if (mode & hFile::MODE_READ)
	{
		modeFatFS |= FA_READ;
	}
	if (mode & hFile::MODE_WRITE)
	{
		modeFatFS |= FA_WRITE;
	}
	return modeFatFS;
}

void glueFileNameToPath(char* out, const char* fileName, const char* path)
{
	strcpy(out, path);
	uint8_t size = strlen(out);
	if (out[size - 1] != '/')
	{
		out[size] = '/';
		size++;
	}
	strcpy(&out[size], fileName);
}

class hSD::hSDImplementation
{
public:
	uint8_t mounted;
	FATFS FatFs;
};
class hFileIterator::hFileIteratorImplementation
{
public:
	DIR dir;
	char path[path_buf_length];
};
class hFile::hFileImplementation
{
public:
	FIL fil;
};
class hFileDescription::hFileDescriptionImplementation
{
public:
	FILINFO filinfo;
	char path[path_buf_length];
};

hSD::hSD()
{
	this->impl = (hSDImplementation*)(sys.malloc(sizeof(hSDImplementation)));
	this->impl->mounted = 0;
#if BOARD(ROBOCORE) || BOARD(CORE2)
	myGPIO_config(MYGPIO_SD_POWER_ON);

	myGPIO_dir_out(MYGPIO_SD_POWER_ON);
	myGPIO_outType_pp(MYGPIO_SD_POWER_ON);
	myGPIO_pupd_none(MYGPIO_SD_POWER_ON);

	myGPIO_write(MYGPIO_SD_POWER_ON, 1);
#endif
}

hSD::~hSD()
{
	sys.free(this->impl);
}

hFile::ErrorCodes hSD::mount()
{
	if (this->impl->mounted)
		return hFile::ERROR_OK;

#if BOARD(ROBOCORE) || BOARD(CORE2)
	sys.delay(200);
	myGPIO_config(MYGPIO_SD_POWER_ON);

	myGPIO_dir_out(MYGPIO_SD_POWER_ON);
	myGPIO_outType_pp(MYGPIO_SD_POWER_ON);
	myGPIO_pupd_none(MYGPIO_SD_POWER_ON);

	myGPIO_write(MYGPIO_SD_POWER_ON, 1);
	sys.delay(20);
	myGPIO_write(MYGPIO_SD_POWER_ON, 0);
	sys.delay(200);
#endif
	hFile::ErrorCodes err = FRESULTToErrorCode(f_mount(&impl->FatFs, "", 0));
	if (err == hFile::ERROR_OK)
		this->impl->mounted = 1;

	return err;
}
hFile::ErrorCodes hSD::unmount()
{
	FRESULT res;
	res = f_mount(0, "", 0);
#if BOARD(ROBOCORE) || BOARD(CORE2)
	myGPIO_config(MYGPIO_SD_POWER_ON);

	myGPIO_dir_out(MYGPIO_SD_POWER_ON);
	myGPIO_outType_pp(MYGPIO_SD_POWER_ON);
	myGPIO_pupd_none(MYGPIO_SD_POWER_ON);

	myGPIO_write(MYGPIO_SD_POWER_ON, 1);
#endif
	this->impl->mounted = 0;
	return FRESULTToErrorCode(res);
}

hFile::ErrorCodes hSD::mkdir(const char* path)
{
	return FRESULTToErrorCode(f_mkdir(path));
}

hFile::ErrorCodes hSD::chdir(const char* path)
{
	return FRESULTToErrorCode(f_chdir(path));
}

hFile::ErrorCodes hSD::getcwd(char* buffer, uint16_t buffer_size)
{
	return FRESULTToErrorCode(f_getcwd(buffer, buffer_size));
}

hFile::ErrorCodes hSD::remove(const char* path)
{
	return FRESULTToErrorCode(f_unlink(path));
}

hFile::ErrorCodes hSD::rename(const char* path_old, const char* path_new)
{
	return FRESULTToErrorCode(f_rename(path_old, path_new));
}

bool hSD::exists(const char* path)
{
	FILINFO info;
	return f_stat(path, &info) == FR_OK;
}

hFile::ErrorCodes hSD::listDirectory(hFileIterator& dir, const char* path)
{
	strcpy(dir.impl->path, path);
	return FRESULTToErrorCode(f_opendir(&dir.impl->dir, path));
}

hFile::ErrorCodes hSD::openFile(hFile& file, const char* path,
                                hFile::FileMode mode)
{
	uint8_t modeFatFS = hFileModeToFatFSMode(mode);
	return FRESULTToErrorCode(f_open(&file.impl->fil, path, modeFatFS));
}

hFile::hFile()
{
	this->impl = (hFile::hFileImplementation*)(sys.malloc(
	               sizeof(hFile::hFileImplementation)));
}

hFile::~hFile()
{
	sys.free(this->impl);
}

hFile::ErrorCodes hFile::close()
{
	return FRESULTToErrorCode(f_close(&impl->fil));
}

hFile::ErrorCodes hFile::seek(uint32_t ofs)
{
	return FRESULTToErrorCode(f_lseek(&impl->fil, ofs));
}

hFile::ErrorCodes hFile::sync()
{
	return FRESULTToErrorCode(f_sync(&impl->fil));
}

int32_t hFile::size()
{
	return f_size(&impl->fil);
}

int32_t hFile::position()
{
	return f_tell(&impl->fil);
}

int hFile::putch(char c)
{
	return f_putc(c, &impl->fil);
}

int hFile::puts(const char* str)
{
	return f_puts(str, &impl->fil);
}

int hFile::printf(const char* str, ...)
{
	va_list arp;
	va_start(arp, str);
	int x = f_printf2(&impl->fil, str, arp);
	va_end(arp);
	return x;
}

uint32_t hFile::read(char* buff, int len)
{
	uint32_t br;
	f_read(&impl->fil, buff, (UINT)(len), (UINT*)((&br)));
	buff[br] = 0;
	return br;
}

hFile::ErrorCodes hFile::write(const char* buff, uint16_t len,
                               uint16_t* bytesWritten)
{
	return FRESULTToErrorCode(f_write(&impl->fil, buff, (UINT)len, (UINT *)bytesWritten));
}

char hFile::read()
{
	char res;
	uint32_t br;
	if (f_read(&impl->fil, &res, 1, (UINT*)(&br)) == 0)
	{
		return 0;
	}
	else
	{
		return res;
	}
}

hFileDescription::hFileDescription()
{
	this->impl = (hFileDescriptionImplementation*)(sys.malloc(
	               sizeof(hFileDescriptionImplementation)));
}

hFileDescription::~hFileDescription()
{
	sys.free(this->impl);
}

void hFileDescription::getName(char* out)
{
	strcpy(out, this->impl->filinfo.fname);
}

void hFileDescription::getPath(char* out)
{
	strcpy(out, this->impl->path);
}

bool hFileDescription::isDirectory()
{
	return (this->impl->filinfo.fattrib & AM_DIR);
}

hFile::ErrorCodes hFileDescription::listDirectory(hFileIterator& out)
{
	if (this->isDirectory() == 0)
	{
		sys.log("Not a directory");
		return hFile::ErrorCodes::ERROR_INVALID_OBJECT;
	}
	char* buf = (char*)(sys.malloc(30));
	glueFileNameToPath(buf, this->impl->filinfo.fname, this->impl->path);
	strcpy(out.impl->path, buf);
	hFile::ErrorCodes err = FRESULTToErrorCode(f_opendir(&out.impl->dir, buf));
	sys.free(buf);
	return err;
}

hFile::ErrorCodes hFileDescription::openFile(hFile& file,
    hFile::FileMode mode)
{
	char* buf = (char*)(sys.malloc(30));
	// char* fileName = (char*)(sys.malloc(30)); // TODO
	glueFileNameToPath(buf, this->impl->filinfo.fname, this->impl->path);
	FRESULT res = f_open(&file.impl->fil, buf, hFileModeToFatFSMode(mode));
	sys.free(buf);
	return FRESULTToErrorCode(res);
}

hFileIterator::hFileIterator()
{
	this->impl = (hFileIterator::hFileIteratorImplementation*)(sys.malloc(
	               sizeof(hFileIterator::hFileIteratorImplementation)));
}

hFileIterator::~hFileIterator()
{
	sys.free(this->impl);
}

hFile::ErrorCodes hFileIterator::next(hFileDescription& out)
{
	strcpy(out.impl->path, this->impl->path);
	hFile::ErrorCodes err = FRESULTToErrorCode(
	                          f_readdir(&this->impl->dir, &out.impl->filinfo));
	return err;
}

void hFileIterator::rewind()
{
	f_readdir(&this->impl->dir, NULL);
}

void hFileIterator::getPath(char* out)
{
	strcpy(out, this->impl->path);
}

void hSDLog::init(char* fileName)
{
	if (hFile::ERROR_OK != SD.mount())
	{
		sys.log("Cannot mount SD!");
		return;
	}
	this->fileName = fileName;
	if (hFile::ERROR_OK
	    != SD.openFile(this->file, this->fileName,
	                   hFile::MODE_CREATE_ALWAYS | hFile::MODE_WRITE
	                   | hFile::MODE_READ))
	{
		sys.log("Cannot open file!");
		return;
	}
}

}
