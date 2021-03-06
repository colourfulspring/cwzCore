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
show_script_info()

# Variables
set(MAX_SIZE 446)
set(RES_SIZE 4096)
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

    file(SIZE ${loader} _size2)
    if (${_size2} GREATER ${RES_SIZE})
        message(FATAL_ERROR "Size of loader : ${_size2} >> ${RES_SIZE} Bytes !!!")
    else()
        message("Size of loader : ${_size2} Bytes")
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
file(COPY ${kernel} DESTINATION ${_tmp_dir})                                # Copy kernel

if (EXISTS ${_tmp_img})
    file(REMOVE ${_tmp_img})
endif()

# Build temp img
execute_process(COMMAND ${mkfat32} ${IMG_SIZE} ${_tmp_img} ${_tmp_dir} RESULT_VARIABLE _mkfat_res)
if (NOT ${_mkfat_res} EQUAL 0)
    message(FATAL_ERROR "Make fat32 failed with exit code ${_mkfat_res}")
endif()

file(REMOVE_RECURSE ${_tmp_dir})                                            # Remove temp dir

# Fill front with 0
message("Fill with 0 at start")
execute_process(COMMAND ${dd} if=/dev/zero of=${_tmp_img} bs=${MAX_SIZE} count=1 conv=notrunc)
# Copy front of bootblock
message("Copy boot block")
execute_process(COMMAND ${dd} if=${bootblock} of=${_tmp_img} bs=${MAX_SIZE} count=1 conv=notrunc)
# Copy loader
message("Copy loader")
execute_process(COMMAND ${dd} if=${loader} of=${_tmp_img} seek=1 bs=512 count=8 conv=notrunc)

file(RENAME ${_tmp_img} ${output})

message("Build ${output} success!")