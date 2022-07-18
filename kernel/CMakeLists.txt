project(kernel)

# 添加编译宏定义与编译参数
list(APPEND _compile_options)
list(APPEND _asm_compile_options)
list(APPEND _c_compile_options -nostdinc -ffreestanding -fPIC -Wall -fno-stack-protector -fno-builtin) # 不查找编译器标准库

foreach(_option ${_compile_options})
    add_compile_options(${_option})
endforeach()

foreach(_option ${_asm_compile_options})
    add_compile_options($<$<COMPILE_LANGUAGE:ASM>:${_option}>)  
endforeach()

foreach(_option ${_c_compile_options})
    add_compile_options($<$<COMPILE_LANGUAGE:C>:${_option}>)  
endforeach()

# 添加公共头文件
include_directories(common)
include_directories(libc/include)

# 添加目标
add_executable(${KERNEL_TARGET})

# 添加子目录
add_subdirectory(arch)
add_subdirectory(drivers)
add_subdirectory(fs)
add_subdirectory(libc)

# 不链接标准库，不检查栈
target_link_options(${KERNEL_TARGET} PUBLIC -no-pie -nostdlib -nostartfiles)