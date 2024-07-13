# XXGUI函数，功能说明
  
  # SameLineWithSpacing：并行显示
    参数：spacing 间隔
  # CustomNewLine：换行显示
    参数：lineHeight 间隔
  # HexToColor：十六进制字符串颜色转ImU32
    参数：hexColor 十六进制颜色字符串
  # ConvertToFloat：将百分比字符串转换为浮点数值
    参数：str 百分比，parentSize 具体像素
  # Div：布局头（目前Div嵌套无法触发上下滑动）
    参数：
      id：标识
      gridWidth：占格数，最大12，参考bs的栅格
      height：高度，默认自适应
      bgColor：背景颜色，默认透明
      padding：内间距
      borderRadius：边框圆角
      flags：窗口flag
  # EndDiv：布局结束，与上面是一对出现
  # ResetGrid：重置布局数据
  # Button：按钮
    参数：
      label：按钮显示的文字
      size：按钮的宽高
      buttonColorHex：按钮的颜色
      textColorHex：文字的颜色
      borderRadius：边框圆角
      sameLine：是否不换行
  # Checkbox：复选框
    参数：
      label：复选框显示的文字
      v：复选框值
      checkedBgColor：选中颜色
      uncheckedBgColor：未选中颜色
      checkMarkColor：选中中心的颜色
      textColor：文字颜色
  # RadioButton：单选框
    参数：
      label：显示的文字
      index：单选框值
      currentIndex：点击后赋值的变量
      height：高度
      selectedColor：选中颜色
      unselectedColor：未选中颜色
      textColor：文字颜色
  # Slider：滑块
    参数：
      label：标识
      v：值
      minValue：最小值
      maxValue：最大值
      progressColor：滑块左边颜色
      progressBgColor：滑块右边颜色
      width：宽度
      showValue：是否显示值
  # Tabs：选项卡
    参数：
      tabs：选项集合
      currentIndex：选中下标
      height：高度
      width：宽度
      bgColor：背景颜色
      textColor：文字颜色
      activeColor：选中的底部颜色
      padding：内边距
  # ToggleButton：开关按钮
    参数：
      label：标识
      pToggle：值
      width：宽度
      selectBgColor：选中颜色
      unSelectBgColor：未选中颜色
      showText：是否显示文字
      borderThickness：边框厚度
  # Show：加载信息弹窗
    参数：
      content：显示的内容
      position：显示的位置
      bgColor：背景颜色
      textColor：文字颜色
      borderRadius：圆角
      duration：显示的时长，单位秒
      padding：内边距
  # Render：渲染弹窗信息

  # 弹窗位置枚举描述
      TopLeft：左上
      TopRight：右上
      BottomLeft：左下
      BottomRight：右下
      Center：居中
      Top：顶部
      Bottom：底部
  

