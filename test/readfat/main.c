#include <malloc.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "fat12.h"
#include "fat32.h"

int str_index_of(const char *str, char ch) {
    int i = 0;
    while (str[i] != '\0' && str[i] != ch) {
        i++;
    }
    if (str[i] == '\0') {
        return -1;
    }
    return i;
}

int convert_char(char *c) {
    char ch = *c;
    if (ch >= 'a' && ch <= 'z') {
        ch = (char) (ch - ('a' - 'A'));
    } else if ((ch >= '0' && ch <= '9') || (ch >= 'A' && ch <= 'Z')) {
        // ...
    } else {
        return -1;
    }
    *c = ch;
    return 0;
}

// readfat scene.img loader.elf

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: readfat <image> <loader>\n");
        return 0;
    }

    char *img_file = argv[1];
    char *loader_file = argv[2];

    // convert loader file name
    int filename_len = strlen(loader_file);
    int dot_index = str_index_of(loader_file, '.');
    if (dot_index > 8) {
        fprintf(stderr, "File name too long.\n");
        fflush(stderr);
        return -1;
    } else if (dot_index < 0) {
        dot_index = filename_len;
    } else if (filename_len - (dot_index + 1) > 3) {
        fprintf(stderr, "File suffix too long.\n");
        fflush(stderr);
        return -1;
    }

    char filename[12];
    memset(filename, 0x20, 11);
    filename[11] = '\0';

    // get base name
    for (int i = 0; i < dot_index; ++i) {
        char ch = loader_file[i];
        int res = convert_char(&ch);
        if (res < 0) {
            fprintf(stderr, "Invalid file name.\n");
            fflush(stderr);
            return -1;
        }
        filename[i] = ch;
    }

    // get suffix
    for (int i = dot_index + 1; i < filename_len; ++i) {
        char ch = loader_file[i];
        int res = convert_char(&ch);
        if (res < 0) {
            fprintf(stderr, "Invalid file suffix.\n");
            fflush(stderr);
            return -1;
        }
        filename[i - (dot_index + 1) + 8] = ch;
    }

    // read image
    int err;
    size_t res;

    FILE *f = fopen(img_file, "rb");
    if (!f) {
        fprintf(stderr, "Failed to read %s.\n", img_file);
        fflush(stderr);
        return -1;
    }

    // parse fat12
    err = fseek(f, 0x01BE, SEEK_SET);
    if (err) {
        fprintf(stderr, "fseek error, err num: %d.\n", err);
        fflush(stderr);
        return -1;
    }

    struct Partition part;
    res = fread((char *) &part, sizeof(part), 1, f);
    if (res != 1) {
        fprintf(stderr, "fread error %d.\n", ferror(f));
        fflush(stderr);
        return -1;
    }

    // 0x7e00
    long FAT12_FS_start_offset = (long) part.startlba * SECTOR_SIZE;
    err = fseek(f, FAT12_FS_start_offset, SEEK_SET);
    if (err) {
        fprintf(stderr, "fseek error, err num: %d.\n", err);
        fflush(stderr);
        return -1;
    }

    struct FAT12 fat12_head;
    res = fread((char *) &fat12_head, sizeof(fat12_head), 1, f);
    if (res != 1) {
        fprintf(stderr, "fread error %d.\n", ferror(f));
        fflush(stderr);
        return -1;
    }

    // 0x3c000
    long root_dir_offset = FAT12_FS_start_offset +
                           (long) (fat12_head.BPB_ResvdSecCnt +
                                   fat12_head.BPB_NumFATs * fat12_head.BPB_FATSz16) *
                                   SECTOR_SIZE;
    err = fseek(f, root_dir_offset, SEEK_SET);
    if (err) {
        fprintf(stderr, "fseek error, err num: %d.\n", err);
        fflush(stderr);
        return -1;
    }

    int found = 0;
    struct dir_entry12 file_ent;

    for (int i = 0; i < fat12_head.BPB_RootEntCnt; ++i) {
        res = fread((char *) &file_ent, sizeof(file_ent), 1, f);
        if (res != 1) {
            fprintf(stderr, "fread error %d.\n", ferror(f));
            fflush(stderr);
            return -1;
        }
        if (*((char *) &file_ent) == '\0') {
            break;
        }
        if (strncmp(file_ent.name, filename, 11) == 0) {
            // entry found
            found = 1;
            break;
        }
    }

    if (!found) {
        fprintf(stderr, "file %s not found.\n", loader_file);
        fflush(stderr);
        return -1;
    }

    long cluster_size = fat12_head.BPB_SecPerClus * fat12_head.BPB_BytsPerSec;

    int cluster_tot = ceil((double) file_ent.size / cluster_size);
    uint16_t *clusters = (uint16_t *) malloc(cluster_tot * sizeof(uint16_t));
    int cluster_cnt = 0;
    if (!clusters) {
        fprintf(stderr, "failed to malloc.\n");
        fflush(stderr);
        return -1;
    }
    clusters[cluster_cnt++] = file_ent.start;

    char *buf = (char *) malloc(file_ent.size);
    long buf_offset = 0;
    if (!buf) {
        fprintf(stderr, "failed to malloc.\n");
        fflush(stderr);
        return -1;
    }

    uint16_t r = file_ent.start % 2;
    uint16_t q = file_ent.start - r;

    // 0x8000
    long fat_table_offset = FAT12_FS_start_offset + SECTOR_SIZE;
    err = fseek(f, fat_table_offset + (long) (q * 1.5), SEEK_SET);
    if (err) {
        fprintf(stderr, "fseek error, err num: %d.\n", err);
        fflush(stderr);
        return -1;
    }

    // 0x40000
    long data_offset = root_dir_offset + fat12_head.BPB_RootEntCnt * 32;

    printf("fat table offset: %lx\n", fat_table_offset);
    printf("data offset: %lx\n", data_offset);

    uint8_t item[3];
    uint16_t num;
    while (1) {
        res = fread(item, sizeof(item), 1, f);
        if (res != 1) {
            fprintf(stderr, "fread error %d.\n", ferror(f));
            fflush(stderr);
            return -1;
        }

        if (r == 0) {
            num = ((item[1] & 0x0Fu) << 8u) | item[0];
            if (num == 0xFFF) {
                break;
            }
            printf("cache fat: %x\n", num);
            clusters[cluster_cnt++] = num;
            r = 1 - r;
        }

        num = ((uint32_t) item[2] << 4u) | (uint16_t)(item[1] >> 4u);
        if (num == 0xFFF) {
            break;
        }
        printf("cache fat: %x\n", num);
        clusters[cluster_cnt++] = num;
        r = 1 - r;
    }

    fflush(stdout);
    printf("read\n");

    // Read
    for (int i = 0; i < cluster_cnt; ++i) {
        long file_offset = data_offset + ((int) clusters[i] - 2) * cluster_size;

        err = fseek(f, file_offset, SEEK_SET);
        if (err) {
            fprintf(stderr, "fseek error, err num: %d.\n", err);
            fflush(stderr);
            return -1;
        }
        long cur_size = (long) file_ent.size - buf_offset;
        if (cur_size > cluster_size) {
            cur_size = cluster_size;
        }

        printf("read fat: %x %lx %lx\n", clusters[i], cur_size, file_offset);

        res = fread(buf + buf_offset, cur_size, 1, f);
        if (res != 1) {
            printf("%lld", res);
            fprintf(stderr, "fread error %d.\n", ferror(f));
            fflush(stderr);
            return -1;
        }
        buf_offset += cur_size;
    }
    fclose(f);
    free(clusters);

    printf("write\n");

    // write to another file
    f = fopen(loader_file, "wb");
    res = fwrite(buf, file_ent.size, 1, f);
    if (res != 1) {
        fprintf(stderr, "fread error %d.\n", ferror(f));
        fflush(stderr);
        return -1;
    }
    fclose(f);
    free(buf);

    return 0;
}