// inode layer interface.

#ifndef inode_h
#define inode_h

#include "stdint.h"

#include "extent_protocol.h" // TODO: delete it

#define DISK_SIZE 1024 * 1024 * 16         // Bytes
#define BLOCK_SIZE 512                     // Bytes
#define BLOCK_NUM (DISK_SIZE / BLOCK_SIZE) // Num

typedef uint32_t blockid_t;

// block layer -----------------------------------------

typedef struct superblock {
    uint32_t size;
    uint32_t nblocks;
    uint32_t ninodes;
} superblock_t;

struct block_manager {
    unsigned char blocks[BLOCK_NUM][BLOCK_SIZE];
    struct superblock sb;
};

void init_block_manager(struct block_manager *bm);
uint32_t alloc_block(struct block_manager *bm);
void free_block(struct block_manager *bm, uint32_t id);
void read_block(struct block_manager *bm, uint32_t id, char *buf);
void write_block(struct block_manager *bm, uint32_t id, const char *buf);

// inode layer -----------------------------------------

#define INODE_NUM 1024

// Inodes per block.
#define IPB 1
//(BLOCK_SIZE / sizeof(struct inode))

// Block containing inode i
#define IBLOCK(i, nblocks) ((nblocks) / BPB + (i) / IPB + 3)

// Bitmap bits per block
#define BPB (BLOCK_SIZE * 8)

// Block containing bit for block b
#define BBLOCK(b) ((b) / BPB + 2)

#define NDIRECT 100
#define NINDIRECT (BLOCK_SIZE / sizeof(unsigned int))
#define MAXFILE (NDIRECT + NINDIRECT)

typedef struct inode {
    short type;
    unsigned int size;
    unsigned int atime;
    unsigned int mtime;
    unsigned int ctime;
    blockid_t blocks[NDIRECT + 1]; // Data block addresses
} inode_t;

struct inode_manager {
    struct block_manager bm;
};

int get_inode(struct inode_manager *im, uint32_t inum, struct inode *ino);
void put_inode(struct inode_manager *im, uint32_t inum, struct inode *ino);

void init_inode_manager(struct inode_manager *im);
uint32_t alloc_inode(struct inode_manager *im, uint32_t type);
void free_inode(struct inode_manager *im, uint32_t inum);
void read_file(struct inode_manager *im, uint32_t inum, char *buf);
void write_file(struct inode_manager *im, uint32_t inum, const char *buf, int size);
void remove_file(struct inode_manager *im, uint32_t inum);
void getattr(struct inode_manager *im, uint32_t inum, struct attr *a);

#endif
