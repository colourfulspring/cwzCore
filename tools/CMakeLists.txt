project(tools C)

add_executable(sign sign.c)

# 整合成一个总目标
add_custom_target(${TOOLS_TARGET} ALL
    DEPENDS sign
)