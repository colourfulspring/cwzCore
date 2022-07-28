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
    char BS_jmpBoot[3];             // 0
    char BS_OEMName[8];             // 3
    unsigned short BPB_BytsPerSec;  // 11
    unsigned char BPB_SecPerClus;   // 13
    unsigned short BPB_ResvdSecCnt; // 14
    unsigned char BPB_NumFATs;      // 16
    unsigned short BPB_RootEntCnt;  // 17
    unsigned short BPB_TotSec16;    // 19
    char BPB_Media;                 // 21
    unsigned short BPB_FATSz16;     // 22
    unsigned short BPB_SecPerTrk;   // 24
    unsigned short NumHeads;        // 26
    unsigned int BPB_HiddSec;       // 28
    unsigned int BPB_TotSec32;      // 32
    //    unsigned int BPB_FATSz32;
    //    unsigned short BPB_flags;
    //    unsigned short BPB_FSVer;
    //    unsigned int BPB_RootClus;
    //    unsigned short BPB_FSInfo;
    //    unsigned short BPB_BkBootSec;
    //    char BPB_Reserved[12];
    unsigned char BS_DrvNum;        // 36
    unsigned char BS_Reserved1;     // 37
    char BS_BootSig;                // 38
    unsigned int BS_VolID;          // 39
    char BS_VolLAB[11];             // 43
    char BS_FilSysType[8];          // 54
    char code[448];                 // 62
    char bootsig[2];                // 510
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
