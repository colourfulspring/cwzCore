#ifndef _COLOR_H_
#define _COLOR_H_

#include "stdint.h"

/**
 * @brief 颜色定义
 * @note 由于不同平台的颜色实现不同，实际效果有一定差异
 * @todo 标准化
 */

typedef enum {
    /// RGB: 0 0 0
    COLOR_BLACK = 0,
    /// RGB: 0 0 170
    COLOR_BLUE = 1,
    /// RGB: 0 170 0
    COLOR_GREEN = 2,
    /// RGB: 0 170 170
    COLOR_CYAN = 3,
    /// RGB: 170 0 0
    COLOR_RED = 4,
    /// RGB: 170 0 170
    COLOR_PURPLE = 5,
    /// RGB: 170 85 0
    COLOR_BROWN = 6,
    /// RGB: 170 170 170
    COLOR_GREY = 7,
    /// RGB: 85 85 85
    COLOR_DARK_GREY = 8,
    /// RGB: 85 85 255
    COLOR_LIGHT_BLUE = 9,
    /// RGB: 85 255 85
    COLOR_LIGHT_GREEN = 10,
    /// RGB: 85 255 255
    COLOR_LIGHT_CYAN = 11,
    /// RGB: 255 85 85
    COLOR_LIGHT_RED = 12,
    /// RGB: 255 85 255
    COLOR_LIGHT_PURPLE = 13,
    /// RGB: 255 255 85
    COLOR_YELLOW = 14,
    /// RGB: 255 255 255
    COLOR_WHITE = 15,
} color_t;

#endif /* _COLOR_H_ */
