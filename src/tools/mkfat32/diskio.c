#include <stdio.h>
#include "fatfs/ff.h"
#include "fatfs/diskio.h"

FILE *image_file = NULL;
size_t image_size = 0;
#define BLOCK_SIZE 512

DSTATUS disk_status(BYTE pdrv) {
	return 0;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(BYTE pdrv) {
	return 0;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(BYTE pdrv, BYTE *buf, LBA_t sector, UINT count) {
	if (fseek(image_file, sector * BLOCK_SIZE, SEEK_SET)) {
		return RES_ERROR;
	}
	if (fread(buf, BLOCK_SIZE, count, image_file) != count) {
		return RES_ERROR;
	}
	return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write(BYTE pdrv, const BYTE *buf, LBA_t sector, UINT count) {
	if (fseek(image_file, sector * BLOCK_SIZE, SEEK_SET)) {
		return RES_ERROR;
	}
	if (fwrite(buf, BLOCK_SIZE, count, image_file) != count) {
		return RES_ERROR;
	}
	return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buf) {
	switch (cmd) {
		case GET_SECTOR_SIZE:
			*(DWORD *)buf = BLOCK_SIZE;
			break;
		case GET_BLOCK_SIZE:
			*(DWORD *)buf = BLOCK_SIZE;
			break;
		case GET_SECTOR_COUNT:
			*(DWORD *)buf = image_size / BLOCK_SIZE;
			break;
		case CTRL_TRIM:
		case CTRL_SYNC:
			break;
		default:
			return RES_PARERR;
	}
	return RES_OK;
}
