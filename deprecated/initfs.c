#ifndef __x86_64__
#define _FILE_OFFSET_BITS 64
#endif

#define NOT_OS
#define FILE_MAX 20

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "inode_manager.h"

int main(int argc, char *argv[]) {
    struct stat st;
    if (argc < 3) {
        fprintf(stderr, "Usage: [input files] -s <fs size> -o <output>\n");
        return -1;
    }

    char *input_files[FILE_MAX];
    int input_cnt = 0;

    char *output_file = NULL;
    int fs_size = 0;

    // Parse arguments
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-s") && i < argc) {
            // Read size
            int num;
            char *str = argv[i + 1];
            int success = sscanf(str, "%d", &num);
            if (success < 1) {
                fprintf(stderr, "Error read size '%s'\n", str);
                return -1;
            }
            fs_size = num;
            i++;
        } else if (!strcmp(argv[i], "-o") && i < argc) {
            // Read output file
            output_file = argv[i + 1];
            i++;
        } else {
            if (stat(argv[i], &st) != 0) {
                fprintf(stderr, "Error opening file '%s': %s\n", argv[1], strerror(errno));
                return -1;
            }

            // Read input file
            input_files[input_cnt] = argv[i];
            input_cnt++;
        }
    }

    if (fs_size == 0) {
        fprintf(stderr, "Filesystem size not specified\n");
        return -1;
    }

    if (!output_file) {
        fprintf(stderr, "Output file not specified\n");
        return -1;
    }

    if (input_cnt == 0) {
        fprintf(stderr, "Input files not specified\n");
        return -1;
    }

    // Write file
    struct inode_manager im;
    init_inode_manager(&im);

    printf("'%s' size: %lld bytes\n", argv[1], (long long) st.st_size);
    return 0;
}