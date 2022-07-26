# Usage:
# cmake
# [-D check_size=true]
# -D bootblock=...
# -D loader=...
# -D kernel=...
# -D mkfat32=...
# -D dd=...
# -D output=...
# -P BuildRootfs.cmake

include(${CMAKE_CURRENT_LIST_DIR}/Modules/Basic.cmake)

check_defined(bootblock)
check_defined(loader)
check_defined(kernel)
check_defined(mkfat32)
check_defined(dd)
check_defined(output)

# Message
get_filename_component(SCRIPT_NAME ${CMAKE_CURRENT_LIST_FILE} NAME)
message("Run ${SCRIPT_NAME} Script")


# Variables
set(MAX_SIZE 446)
set(IMG_SIZE 1024000)

# Check existence
check_exists(${bootblock})
check_exists(${loader})
check_exists(${kernel})
check_exists(${mkfat32})
check_exists(${dd})

# Get file size
if (DEFINED check_size)
    file(SIZE ${bootblock} _size1)
    if (${_size1} GREATER ${MAX_SIZE})
        message(FATAL_ERROR "Size of bootblock : ${_size1} >> ${MAX_SIZE} Bytes !!!")
    else()
        message("Size of bootblock : ${_size1} Bytes")
    endif()
endif()

# Set rootfs dir
get_filename_component(_bin_dir ${output} DIRECTORY)
set(_tmp_dir ${_bin_dir}/_rootfs_temp)
set(_tmp_img ${_bin_dir}/_rootfs_temp.img)

if (EXISTS ${_tmp_dir})
    file(REMOVE_RECURSE ${_tmp_dir})
endif()
file(MAKE_DIRECTORY ${_tmp_dir})                                            # Make temp dir
file(COPY ${loader} DESTINATION ${_tmp_dir})                                # Copy loader
file(COPY ${kernel} DESTINATION ${_tmp_dir})                                # Copy kernel

if (EXISTS ${_tmp_img})
    file(REMOVE ${_tmp_img})
endif()
execute_process(COMMAND ${mkfat32} ${IMG_SIZE} ${_tmp_img} ${_tmp_dir})     # Build temp img

file(REMOVE_RECURSE ${_tmp_dir})                                            # Remove temp dir

# file(READ ${bootblock} _boot_content OFFSET 0 LIMIT ${MAX_SIZE})            # Read bootblock
# file(READ ${_tmp_img} _tmp_img_content OFFSET ${MAX_SIZE})                  # Read temp img
# file(WRITE ${output} ${_boot_content})
# file(APPEND ${output} ${_tmp_img_content})
# file(REMOVE ${_tmp_img})                                                    # Remove temp img

# Fill front with 0
execute_process(COMMAND ${dd} if=/dev/zero of=${_tmp_img} bs=${MAX_SIZE} count=1 conv=notrunc)
# Copy front of bootblock
execute_process(COMMAND ${dd} if=${bootblock} of=${_tmp_img} bs=${MAX_SIZE} count=1 conv=notrunc)

file(RENAME ${_tmp_img} ${output})

message("Build ${output} success!")