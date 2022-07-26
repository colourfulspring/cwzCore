#include "inode_manager.h"

static uint32_t compare(const char *str1,
                        const char *str2,
                        int32_t size, uint32_t start) {
    uint32_t i;
    str1 += start;
    str2 += start;
    for (i = start; i < size; ++i) {
        if (*str1 != *str2) {
            break;
        }
        str1++;
        str2++;
    }
    return i;
}

// block layer -----------------------------------------

// The layout of disk should be like this:
// |<-sb->|<-free block bitmap->|<-inode table->|<-data->|
void init_block_manager(struct block_manager *bm) {
    memset(bm->blocks, 0, BLOCK_NUM * BLOCK_SIZE);

    // format the disk
    bm->sb.size = BLOCK_SIZE * BLOCK_NUM;
    bm->sb.nblocks = BLOCK_NUM;
    bm->sb.ninodes = INODE_NUM;
}

void read_block(struct block_manager *bm, uint32_t id, char *buf) {
    memcpy(buf, (char *) bm->blocks[id], BLOCK_SIZE);
}

void write_block(struct block_manager *bm, uint32_t id, const char *buf) {
    memcpy((char *) bm->blocks[id], buf, BLOCK_SIZE);
}

// Allocate a free disk block.
blockid_t alloc_block(struct block_manager *bm) {
    /*
     * your code goes here.
     * note: you should mark the corresponding bit in block bitmap when alloc.
     * you need to think about which block you can start to be allocated.
     */

    uint32_t bitBlocks = (bm->sb.nblocks) / BPB;
    char buf[BLOCK_SIZE], full[BLOCK_SIZE];
    uint32_t i, j, k;

    memset(full, 0xFF, BLOCK_SIZE);

    // Find Vacancy Block
    for (i = 0; i < bitBlocks; ++i) {
        read_block(bm, i + 2, buf);
        // Find Vacancy Byte
        j = compare(buf, full, BLOCK_SIZE, 0);
        if (j != BLOCK_SIZE) {
            break;
        }
    }

    if (i == bitBlocks) {
        printf("\tim: blocks run out\n");
        return 0;
    }

    char ch0 = buf[j];
    // Find Vacancy Bit
    for (k = 0; k <= 7; ++k) {
        if ((~ch0) & 1 << (7 - k)) {
            break;
        }
    }

    buf[j] |= 1 << (7 - k);
    write_block(bm, i + 2, buf);

    uint32_t startBlock = IBLOCK(INODE_NUM, bm->sb.nblocks) + 1;
    blockid_t id = i * BPB + j * 8 + k + startBlock;

    memset(buf, 0, BLOCK_SIZE);
    write_block(bm, id, buf); // Clear Block

    return id;
}

void free_block(struct block_manager *bm, uint32_t id) {
    /*
     * your code goes here.
     * note: you should unmark the corresponding bit in the block bitmap when free.
     */

    uint32_t i, j, k;
    uint32_t startBlock = IBLOCK(INODE_NUM, bm->sb.nblocks) + 1;
    id -= startBlock;

    i = id / BPB;           // Block
    j = (id - i * BPB) / 8; // Byte
    k = id % 8;             // Bit

    char buf[BLOCK_SIZE];
    read_block(bm, i + 2, buf);
    buf[j] &= ~(1 << (7 - k));
    write_block(bm, i + 2, buf);
}

// inode layer -----------------------------------------

void init_inode_manager(struct inode_manager *im) {
    uint32_t root_dir = alloc_inode(im, T_DIR);
    if (root_dir != 1) {
        printf("\tim: error! alloc first inode %d, should be 1\n", root_dir);
        // exit(0);
    }
}

/* Create a new file.
 * Return its inum. */
uint32_t alloc_inode(struct inode_manager *im, uint32_t type) {
    /*
     * your code goes here.
     * note: the normal inode block should begin from the 2nd inode block.
     * the 1st is used for root_dir, see inode_manager::inode_manager().
     */

    struct inode *ino_cache;
    char buf[BLOCK_SIZE];
    int find = 0;
    uint32_t inum = 1;

    for (; inum < INODE_NUM; ++inum) {
        read_block(&im->bm, IBLOCK(inum, im->bm.sb.nblocks), buf);
        ino_cache = (struct inode *) buf + inum % IPB;
        if (ino_cache->type == 0) {
            find = 1;
            break;
        }
    }

    if (!find) {
        printf("\tim: cannot create more inode\n");
        return 0;
    }

    memset(ino_cache, 0, sizeof(struct inode));
    ino_cache->type = type;

    uint32_t time = 0;
    ino_cache->atime = time;
    ino_cache->ctime = time;
    ino_cache->mtime = time;

    // Write back
    put_inode(im, inum, ino_cache);

    return inum;
}

void free_inode(struct inode_manager *im, uint32_t inum) {
    /*
     * your code goes here.
     * note: you need to check if the inode is already a freed one;
     * if not, clear it, and remember to write back to disk.
     */

    struct inode *ino_cache;
    char buf[BLOCK_SIZE];

    read_block(&im->bm, IBLOCK(inum, im->bm.sb.nblocks), buf);
    ino_cache = (struct inode *) buf + inum % IPB;
    if (ino_cache->type == 0) {
        printf("\tim: inode not exist\n");
        return;
    }

    ino_cache->type = 0;

    // Write back
    put_inode(im, inum, ino_cache);
}


/* Return an inode structure by inum, NULL otherwise.
 * Caller should release the memory. */
int get_inode(struct inode_manager *im, uint32_t inum, struct inode *ino) {
    struct inode *ino_disk;
    char buf[BLOCK_SIZE];

    if (inum < 0 || inum >= INODE_NUM) {
        printf("\tim: inum out of range\n");
        return -1;
    }

    read_block(&im->bm, IBLOCK(inum, im->bm.sb.nblocks), buf);

    ino_disk = (struct inode *) buf + inum % IPB;
    if (ino_disk->type == 0) {
        printf("\tim: inode not exist\n");
        return -1;
    }

    *ino = *ino_disk;
    return 0;
}

void put_inode(struct inode_manager *im, uint32_t inum, struct inode *ino) {
    char buf[BLOCK_SIZE];
    struct inode *ino_disk;

    //    printf("\tim: put_inode %d\n", inum);
    if (ino == NULL)
        return;

    read_block(&im->bm, IBLOCK(inum, im->bm.sb.nblocks), buf);
    ino_disk = (struct inode *) buf + inum % IPB;
    *ino_disk = *ino;
    write_block(&im->bm, IBLOCK(inum, im->bm.sb.nblocks), buf);
}

/* Get all the data of a file by inum.
 * Return allocated data, should be freed by caller. */
void read_file(struct inode_manager *im, uint32_t inum, char *buf) {
    /*
     * your code goes here.
     * note: read blocks related to inode number inum,
     * and copy them to buf_out
     */

    char buf0[BLOCK_SIZE];
    read_block(&im->bm, IBLOCK(inum, im->bm.sb.nblocks), buf0);
    struct inode *target = (struct inode *) buf0 + inum % IPB;
    if (target->type == 0) {
        return;
    }

    // Modify Atime
    target->atime = 0;

    uint32_t total = target->size, len = 0;
    char buf1[BLOCK_SIZE];

    char* ptr = buf;

    // Read Direct Blocks
    for (uint32_t i = 0; i < NDIRECT && len < total; ++i) {
        blockid_t id = target->blocks[i];

        if (total - len < BLOCK_SIZE) {
            memset(buf1, 0, BLOCK_SIZE);
            read_block(&im->bm, id, buf1);
            memcpy(ptr, buf1, total - len);

            ptr += total - len;
            len += total - len;
        } else {
            read_block(&im->bm, id, ptr);

            ptr += BLOCK_SIZE;
            len += BLOCK_SIZE;
        }
    }

    // Read Indirect Blocks
    blockid_t id2 = target->blocks[NDIRECT];
    if (id2 != 0) {
        uint32_t block2[NINDIRECT];
        read_block(&im->bm, id2, (char *) block2);

        for (uint32_t i = 0; i < NINDIRECT; ++i) {
            blockid_t id = block2[i];
            if (id == 0) {
                break;
            }

            if (total - len < BLOCK_SIZE) {
                memset(buf1, 0, BLOCK_SIZE);
                read_block(&im->bm, id, buf1);
                memcpy(ptr, buf1, total - len);

                ptr += total - len;
                len += total - len;
            } else {
                read_block(&im->bm, id, ptr);

                ptr += BLOCK_SIZE;
                len += BLOCK_SIZE;
            }
        }
    }

    write_block(&im->bm, IBLOCK(inum, im->bm.sb.nblocks), buf0);
}

/* alloc/free blocks if needed */
void write_file(struct inode_manager *im, uint32_t inum, const char *buf, int size) {
    /*
     * your code goes here.
     * note: write buf to blocks of inode inum.
     * you need to consider the situation when the size of buf
     * is larger or smaller than the size of original inode
     */

    char buf0[BLOCK_SIZE];
    read_block(&im->bm, IBLOCK(inum, im->bm.sb.nblocks), buf0);
    struct inode *target = (struct inode *) buf0 + inum % IPB;
    if (target->type == 0) {
        return;
    }

    // Modify AMtime
    unsigned int time = 0;
    target->atime = time;
    target->mtime = time;
    target->ctime = time;
    target->size = size;

    int len = 0;
    const char *ptr = buf;
    char buf1[BLOCK_SIZE];

    // Write Direct Blocks
    for (uint32_t i = 0; i < NDIRECT && len < size; ++i) {
        blockid_t id = target->blocks[i];
        if (id == 0) {
            id = alloc_block(&im->bm);
            if (id == 0) {
                return;
            }
            target->blocks[i] = id;
        }

        if (size - len < BLOCK_SIZE) {
            memset(buf1, 0, BLOCK_SIZE);
            memcpy(buf1, ptr, size - len);
            write_block(&im->bm, id, buf1);

            ptr += size - len;
            len += size - len;
        } else {
            write_block(&im->bm, id, ptr);
            ptr += BLOCK_SIZE;
            len += BLOCK_SIZE;
        }
    }

    blockid_t id2 = target->blocks[NDIRECT];
    if (len == size) {
        // Free Direct Blocks
        for (uint32_t i = size; i < NDIRECT; ++i) {
            blockid_t id = target->blocks[i];
            if (id == 0) {
                break;
            }
            free_block(&im->bm, id);
            target->blocks[i] = 0;
        }
        // Free Indirect Blocks
        if (id2 != 0) {
            uint32_t block2[NINDIRECT];
            read_block(&im->bm, id2, (char *) block2);

            for (uint32_t i = 0; i < NINDIRECT; ++i) {
                blockid_t id = block2[i];
                if (id == 0) {
                    break;
                }
                free_block(&im->bm, id);
            }

            free_block(&im->bm, id2);
            target->blocks[NDIRECT] = 0;
        }
    } else {
        // Need Indirect
        uint32_t block2[NINDIRECT];
        if (id2 == 0) {
            // Create Indirect
            id2 = alloc_block(&im->bm);
            if (id2 == 0) {
                return;
            }
            target->blocks[NDIRECT] = id2;

            memset(block2, 0, BLOCK_SIZE);
        } else {
            read_block(&im->bm, id2, (char *) block2);
        }

        uint32_t i = 0;
        while (len < size && i < NINDIRECT) {
            blockid_t id = block2[i];
            if (id == 0) {
                id = alloc_block(&im->bm);
                if (id == 0) {
                    return;
                }
                block2[i] = id;
            }

            if (size - len < BLOCK_SIZE) {
                memset(buf1, 0, BLOCK_SIZE);
                memcpy(buf1, ptr, size - len);
                write_block(&im->bm, id, buf1);

                ptr += size - len;
                len += size - len;
            } else {
                write_block(&im->bm, id, ptr);
                ptr += BLOCK_SIZE;
                len += BLOCK_SIZE;
            }
            ++i;
        }
        while (i < NINDIRECT && block2[i] != 0) {
            free_block(&im->bm, block2[i]);
            block2[i] = 0;
            ++i;
        }
        write_block(&im->bm, id2, (const char *) block2);
    }

    write_block(&im->bm, IBLOCK(inum, im->bm.sb.nblocks), buf0);
}

void getattr(struct inode_manager *im, uint32_t inum, struct attr *a) {
    /*
     * your code goes here.
     * note: get the attributes of inode inum.
     * you can refer to "struct attr" in extent_protocol.h
     */

    struct inode target;
    if (get_inode(im, inum, &target)) {
        return;
    }

    a->type = target.type;
    a->atime = target.atime;
    a->ctime = target.ctime;
    a->mtime = target.mtime;
    a->size = target.size;
}

void remove_file(struct inode_manager *im, uint32_t inum) {
    /*
     * your code goes here
     * note: you need to consider about both the data block and inode of the file
     */

    char buf0[BLOCK_SIZE];
    read_block(&im->bm, IBLOCK(inum, im->bm.sb.nblocks), buf0);
    struct inode *target = (struct inode *) buf0 + inum % IPB;
    if (target->type == 0) {
        return;
    }

    // Write Direct Blocks
    for (uint32_t i = 0; i < NDIRECT; ++i) {
        blockid_t id = target->blocks[i];
        if (id == 0) {
            break;
        }
        free_block(&im->bm, id);
    }

    blockid_t id2 = target->blocks[NDIRECT];
    // Free Indirect Blocks
    if (id2 != 0) {
        uint32_t block2[NINDIRECT];
        read_block(&im->bm, id2, (char *) block2);
        for (uint32_t i = 0; i < NINDIRECT; ++i) {
            blockid_t id = block2[i];
            if (id == 0) {
                break;
            }
            free_block(&im->bm, id);
        }
        free_block(&im->bm, id2);
    }
    memset(target->blocks, 0, (NDIRECT + 1) * sizeof(blockid_t));
    target->type = 0;

    write_block(&im->bm, IBLOCK(inum, im->bm.sb.nblocks), buf0);
}
