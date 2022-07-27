#ifndef READFAT_FAT12_H
#define READFAT_FAT12_H

#ifdef __GNUC__
#define PACK(__Declaration__) __Declaration__ __attribute__((__packed__))
#endif

#ifdef _MSC_VER
#define PACK(__Declaration__) __pragma(pack(push, 1)) __Declaration__ __pragma(pack(pop))
#endif

#define SECTOR_SIZE 512

PACK(struct FAT12 {
    char BS_jmpBoot[3];
    char BS_OEMName[8];
    unsigned short BPB_BytsPerSec;
    unsigned char BPB_SecPerClus;
    unsigned short BPB_ResvdSecCnt;
    unsigned char BPB_NumFATs;
    unsigned short BPB_RootEntCnt;
    unsigned short BPB_TotSec16;
    char BPB_Media;
    unsigned short BPB_FATSz16;
    unsigned short BPB_SecPerTrk;
    unsigned short NumHeads;
    unsigned int BPB_HiddSec;
    unsigned int BPB_TotSec32;
    //    unsigned int BPB_FATSz32;
    //    unsigned short BPB_flags;
    //    unsigned short BPB_FSVer;
    //    unsigned int BPB_RootClus;
    //    unsigned short BPB_FSInfo;
    //    unsigned short BPB_BkBootSec;
    //    char BPB_Reserved[12];
    unsigned char BS_DrvNum;
    unsigned char BS_Reserved1;
    char BS_BootSig;
    unsigned int BS_VolID;
    char BS_VolLAB[11];
    char BS_FilSysType[8];
    char code[448];
    char bootsig[2];
});

PACK(struct dir_entry12 {
    unsigned char name[11]; /* name and extension */
    unsigned char attr;     /* attribute bits */

    //         unsigned char lcase;            /* Case for base and extension */
    //         unsigned char ctime_cs;         /* Creation time, centiseconds (0-199) */
    //         unsigned short ctime;           /* Creation time */
    //         unsigned short cdate;           /* Creation date */
    //         unsigned short adate;           /* Last access date */
    //         unsigned short starthi;         /* High 16 bits of cluster in FAT32 */

    unsigned char rsvd[10]; /* reserved bytes */
    unsigned short time;    /* time */
    unsigned short date;    /* date */
    unsigned short start;   /* first cluster */
    unsigned int size;      /* file size (in bytes) */
});

#endif//READFAT_FAT12_H
