project(kernel)

set(KERNEL_TARGET kernel.img)

# 添加编译宏定义与编译参数
list(APPEND _compile_definitions)
list(APPEND _compile_options -nostdinc -ffreestanding -fPIC -Wall -fno-stack-protector -fno-builtin) # 不查找编译器标准库
list(APPEND _asm_compile_definitions __ASM__)
list(APPEND _asm_compile_options)
list(APPEND _c_compile_definitions)
list(APPEND _c_compile_options)

add_compile_definitions(${_compile_definitions})
add_compile_options(${_compile_options})
add_compile_definitions($<$<COMPILE_LANGUAGE:ASM>:${_asm_compile_definitions}>)
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:${_asm_compile_options}>)
add_compile_definitions($<$<COMPILE_LANGUAGE:C>:${_c_compile_definitions}>)
add_compile_options($<$<COMPILE_LANGUAGE:C>:${_c_compile_options}>)

# 添加公共头文件
include_directories(common)
include_directories(libc/include)

# 驱动
add_subdirectory(drivers)

# 文件系统
add_subdirectory(fs)

# C 库
add_subdirectory(libc)

add_executable(${KERNEL_TARGET}
    $<TARGET_OBJECTS:drivers>
    $<TARGET_OBJECTS:fs>
    $<TARGET_OBJECTS:libc>
)

# 不链接标准库，不检查栈
target_link_options(${KERNEL_TARGET} PUBLIC -no-pie -nostdlib -nostartfiles)