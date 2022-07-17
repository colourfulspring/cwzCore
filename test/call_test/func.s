	.type func, @function
	.global func             # 设置 func 函数为全局可见

func:
	callq add
	ret                  # 这个语句和 call 是搭配使用的
