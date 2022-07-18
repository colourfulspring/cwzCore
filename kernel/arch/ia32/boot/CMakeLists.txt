include(KernelTools)

target_include_directories(${KERNEL_TARGET} PRIVATE include)

set(init_src init/boot.asm init/main.c)    
target_sources(${KERNEL_TARGET} PRIVATE ${init_src})

# Get Object File Names
os_target_sources_out_objects(${KERNEL_TARGET} kernel init_objects ${init_src})
set(init_objects ${init_objects} PARENT_SCOPE)

os_objcopy_binary(${KERNEL_TARGET} kernel8.img)