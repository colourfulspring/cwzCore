#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "fatfs/ff.h"

extern FILE *image_file;
extern size_t image_size;

const char *app_name = "fatfs_maker";

// join 2 paths
// return value MUST be free'd when we stop using it!
char *join_paths(const char *p1, const char *p2) {
    size_t p1_len = strlen(p1);
    char *new_str = malloc(p1_len + strlen(p2) + 2);
    if (!new_str) {
        printf("failed to join paths: failed to allocate string!\n");
        abort();
    }
    strcpy(new_str, p1);
    if (p1_len) {
        strcat(new_str, "/");
    }
    strcat(new_str, p2);
    return new_str;
}

// add a folder to the FatFs filesystem
// return value is actually the exit code for this whole program
int add_folder(char *path, const char *image_path, FATFS fs) {
    #if FF_FS_RPATH
    #error FatFs relative path (FF_FS_RPATH) not supported.
    #endif
    struct dirent *ent;
    FIL fp;
    struct stat s;
    FRESULT res = FR_OK;
    DIR *dir = opendir(path);
    if (dir == NULL) {
        printf("can't open directory %s\n", path);
        return 1;
    }
    while ((ent = readdir(dir)) != NULL) {
        if (strstr(ent->d_name, ".") != ent->d_name) {
            if (ent->d_type == DT_DIR) {
                char *new_path = join_paths(path, ent->d_name);
                char *img_dir = join_paths(image_path, ent->d_name);
                res = f_mkdir(img_dir);
                if (res) {
                    printf("failed to make directory \"%s\", FatFs error %d\n", img_dir, res);
                    return 1;
                }
                add_folder(new_path, img_dir, fs);
                free(new_path);
                free(img_dir);
            }
            if (ent->d_type == DT_REG) {
                // add this file to `image_path`
                char *img_file_path = join_paths(image_path, ent->d_name);
                res = f_open(&fp, img_file_path, FA_READ | FA_WRITE | FA_CREATE_NEW);
                if (res) {
                    printf("failed to open file \"%s\", FatFs error %d\n", img_file_path, res);
                    return 1;
                }
                char *sys_file_path = join_paths(path, ent->d_name);
                int fd = open(sys_file_path, O_RDONLY);
                if (!fd) {
                    printf("failed to open file \"%s\", error: %s\n", sys_file_path, strerror(errno));
                    return 1;
                }
                int err = fstat(fd, &s);
                if (err) {
                    perror("fstat error:");
                    return 1;
                }
                void *block = malloc(s.st_blksize);
                if (!block) {
                    printf("failed to allocate space for file reading\n");
                    return 1;
                }
                for (size_t i = 0; i < s.st_size; i += s.st_blksize) {
                    size_t read_size = read(fd, block, s.st_blksize);
                    UINT written_size;
                    res = f_write(&fp, block, read_size, &written_size);
                    if (written_size != read_size) {
                        printf("FatFs write error (%d), out of space?\n", res);
                    }
                }
                free(block);
                res = f_close(&fp);
                if (res) {
                    printf("failed to close file \"%s\", FatFs error %d\n", img_file_path, res);
                    return 1;
                }
                err = close(fd);
                if (err) {
                    printf("error closing file \"%s\", error: %s\n", sys_file_path, strerror(errno));
                    return 1;
                }
                free(img_file_path);
                free(sys_file_path);
            }
        }
    }
    return 0;
}

void create_image(char *image_path, size_t sz, char *folder_pack) {
    struct stat img_stat;
    BYTE work[FF_MAX_SS];
    FILE *img = fopen(image_path, "wb");
    if (!img) {
        printf("failed to open \"%s\": %s\n", image_path, strerror(errno));
        exit(1);
    }
    assert(img);
    int err = stat(image_path, &img_stat);
    if (err) {
        printf("stat error: %s\n", strerror(errno));
        exit(1);
    }
    // we write in the block size of the filesystem for optimization
    void *zeros = malloc(img_stat.st_blksize);
    assert(zeros);
    bzero(zeros, img_stat.st_blksize);
    printf("FS block size: %ld\n", img_stat.st_blksize);
    for (off_t i = 0; i < sz; i += img_stat.st_blksize) {
        if (fwrite(zeros, 1, img_stat.st_blksize, img) != img_stat.st_blksize) {
            printf("error writing to file: %s\n", strerror(errno));
            exit(1);
        }
    }
    free(zeros);
    // repopen the file for r/w
    assert(!fclose(img));
    img = fopen(image_path, "r+");
    if (!img) {
        printf("failed to open \"%s\" as r/w: %s\n", image_path, strerror(errno));
        exit(1);
    }
    // Pass information over to the FatFs module
    image_file = img;
    image_size = sz;
    // Format the filesystem
    FRESULT res = f_mkfs("", NULL, work, sizeof(work));
    if (res) {
        printf("failed to create FAT32 filesystem, FatFs error %d\n", res);
        assert(!fclose(img));
        exit(1);
    }
    FATFS fs;
    res = f_mount(&fs, "", 0);
    if (res) {
        printf("failed to mount filesystem, FatFs error %d\n", res);
        assert(!fclose(img));
        exit(1);
    }
    if (folder_pack) {
        err = add_folder(folder_pack, "", fs);
    }
    assert(!fclose(img));
    if (err) {
        exit(err);
    }
}

int main(int argc, char *argv[]) {
    char *folder_pack = NULL;
    if (argc < 3 || argc > 4) {
        if (argc > 0) {
            app_name = argv[0];
        }
        printf("usage: %s, <image size> <image file name> <folder to pack>\n", app_name);
        exit(1);
    }
    if (argc == 4) {
        folder_pack = argv[3];
    }
    char *image_size_str = argv[1];
    char *image_path = argv[2];
    char *end;
    size_t image_size;
    if (strstr(image_size_str, "0x") == image_size_str) {
        image_size = strtoull(image_size_str + 2, &end, 16);
    } else {
        image_size = strtoull(image_size_str, &end, 10);
    }
    printf("image size: %zu\n", image_size);
    if (folder_pack) {
        printf("folder pack: %s\n", folder_pack);
    }
    create_image(image_path, image_size, folder_pack);
    return 0;
}
