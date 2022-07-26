# add_subdirectory(sign)

add_subdirectory(mkfat32)

# 整合成一个总目标
add_custom_target(${TOOLS_TARGET} ALL
    DEPENDS mkfat32
)