# ImGui-基础参数、函数文档
  ## bool ImGui::Begin：创建父窗口
    参数：const char* id, bool* p_open = NULL, ImGuiWindowFlags flags = 0
    返回值：如果窗口当前可见，则返回 true。
    说明：
      id：唯一标识
      p_open：窗口是否（默认打开）
      flags：窗口属性，如是否无背景，拖拽，滚动，接收点击等
    调用示例：
      ImGui::Begin("窗口", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground);
        // 其他内容
      ImGui::End();
  ## void ImGui::End：结束父窗口，和ImGui::Begin一起成对出现
  ## bool ImGui::BeginChild：创建子窗口，写在父窗口内
    参数：const char* str_id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags
    返回值：如果窗口当前可见，则返回 true。
    说明：
      str_id：唯一标识
      size_arg：窗口大小，ImVec2代表宽高
      flags：窗口属性，如是否无背景，拖拽，滚动，接收点击等
    调用示例：
      ImGui::BeginChild("子窗口", ImVec2(500, 200));
       // 其他内容
      ImGui::EndChild();
  ## void ImGui::EndChild：结束子窗口，跟创建子窗口成对出现
  ## void ImGui::SetCursorPos：设置窗口坐标
    参数：const ImVec2& local_pos
    说明：local_pos：X，Y坐标的位置
    调用示例：
      ImGui::Begin("窗口", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground);
        ImGui::SetCursorPos(ImVec2((screen_x/2)-250, 0));//给子窗口设置以父窗口为开始坐标的固定坐标
        ImGui::BeginChild("子窗口", ImVec2(500, 200));
         // 其他内容
        ImGui::EndChild();
      ImGui::End();
  ## void ImGui::PushStyleColor：设置颜色
    参数：ImGuiCol idx, const ImVec4& col
    说明：
      idx：控件，例如：ImGuiCol_ChildBg，ImGuiCol_Text
      col：颜色，类型是ImVec4-例如：ImVec4(0, 0, 0, 0)透明，ImVec4(1,1,1,1)白色，ImVec4(1.0f, 0.831f, 0.623f, 1.0f)亮黄色
    调用示例：
      ImGui::Begin("窗口", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0,0,0,1));//给子窗口设置黑色背景颜色
        ImGui::BeginChild("子窗口", ImVec2(500, 200));
         // 其他内容
        ImGui::EndChild();
      ImGui::End();
  ## void ImGui::PushStyleVar：设置样式
    参数：ImGuiStyleVar idx, float val
    说明：
      idx：控件，例如：ImGuiStyleVar_ChildRounding
      val：设置控件的具体值
    调用示例：
      ImGui::Begin("窗口", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);//给子窗口设置边框圆角
        ImGui::BeginChild("子窗口", ImVec2(500, 200));
         // 其他内容
        ImGui::EndChild();
      ImGui::End();
  ## void ImGui::SameLine：水平布局
    参数：float offset_from_start_x, float spacing_w
    说明：
      offset_from_start_x：靠窗口左边距离多少个像素，默认为上一个控件坐标后的20个像素点
      spacing_w：如果pos_x==0，则使用默认间距；如果pos_x！=，则不使用间距0
    调用示例：
      {
        ImGui::Text("11111");
        ImGui::SameLine();
        ImGui::Text("2222");//这里的2222会显示在1111后面是水平显示
        ImGui::SameLine(100);//这里会让333显示在窗口开始后的100个像素的位置
        ImGui::Text("333");
      }
  ## void ImGui::NewLine：换行，在这个后面写的控件会换一行显示
    调用示例：
      {
        ImGui::Text("11111");
        ImGui::NewLine();
        ImGui::Text("2222");//这里的2222会显示在1111下面一行
      }
  ## void ImGui::Dummy：换行，可以设置换行的宽高度
    参数：const ImVec2& size
    说明：size：宽高
    调用示例：
      {
        ImGui::Text("11111");
        ImGui::Dummy(100,100);
        ImGui::Text("2222");//这里的2222会显示在1111下面一行,间隔是100像素点
      }
  ## void SetNextWindowPos：设置下一个窗口位置。在Begin（）之前调用
    参数：const ImVec2& pos, ImGuiCond cond = 0, const ImVec2& pivot = ImVec2(0, 0)
    说明：
      pos：窗口的坐标点，例如ImVec2(100,100)
      cond：条件，用于控制何时应用位置设置
      pivot：窗口位置的枢轴点
  ## void SetNextWindowSize：设置下一个窗口大小。将轴设置为0.0f以强制在此轴上进行自动拟合。Begin（）之前的调用
    参数：const ImVec2& pos, ImGuiCond cond = 0
    说明：
      pos：窗口的坐标点，例如ImVec2(100,100)
      cond：条件，用于控制何时应用位置设置
  ## void SetNextWindowSizeConstraints：设置下一个窗口大小限制。在X/Y轴上使用-1、-1可以保留当前大小。尺寸将向下取整。使用回调来应用非琐碎的编程约束。
    参数：const ImVec2& size_min, const ImVec2& size_max, ImGuiSizeCallback custom_callback = NULL, void* custom_callback_data = NULL
    说明：
      size_min：窗口最小大小。
      size_max：窗口最大大小。
      custom_callback：自定义回调函数。
      custom_callback_data：自定义回调函数的数据。
  ## void SetNextWindowContentSize：设置下一个窗口的内容大小（~可滚动的客户端区域，强制执行滚动条的范围）。不包括窗口装饰（标题栏、菜单栏等），也不包括WindowPadding。将轴设置为0.0f以使其保持自动。Begin（）之前的调用
    参数：const ImVec2& size
    说明：内容的目标大小
  ## void SetNextWindowCollapsed：设置下一个窗口的折叠状态。必须在 Begin() 之前调用
    参数：bool collapsed, ImGuiCond cond = 0
    说明：
      collapsed：窗口是否折叠。
      cond：条件，用于控制何时应用折叠状态。
  ## void SetNextWindowFocus：设置下一个窗口为焦点窗口或置于最前。必须在 Begin() 之前调用
  ## void SetNextWindowBgAlpha：设置下一个窗口背景色的透明度。帮助轻松覆盖 ImGuiCol_WindowBg/ChildBg/PopupBg 的 Alpha 组件。也可以使用 ImGuiWindowFlags_NoBackground
    参数：float alpha
    说明：alpha：背景颜色的透明度
  ## void SetWindowPos：设置当前窗口的位置。在 Begin()/End() 之间调用
    参数：const ImVec2& pos, ImGuiCond cond = 0
    说明：
      pos：窗口的目标位置。
      cond：条件，用于控制何时应用位置设置。
  ## void SetWindowSize：设置当前窗口的大小。在 Begin()/End() 之间调用。设置为 ImVec2(0, 0) 以强制自动适应内容
    参数：const ImVec2& size, ImGuiCond cond = 0
    说明：
      size：窗口的目标大小。
      cond：条件，用于控制何时应用大小设置
  ## void SetWindowCollapsed：设置当前窗口的折叠状态
    参数：bool collapsed, ImGuiCond cond = 0
    说明：
      collapsed：窗口是否折叠。
      cond：条件，用于控制何时应用折叠状态。
  ## void SetWindowFocus：设置当前窗口为焦点窗口或置于最前
  ## void SetWindowFontScale：设置窗口的字体缩放比例。[过时] 调整 IO.FontGlobalScale 如果你想缩放所有窗口。这是旧 API！为了正确缩放，更推荐重新加载字体、重建 ImFontAtlas 并调用 style.ScaleAllSizes()
    参数：float scale
    说明：scale：字体缩放比例
  ## void SetWindowPos： 设置指定名称窗口的位置
    参数：const char* name, const ImVec2& pos, ImGuiCond cond = 0
    说明：
      name：窗口名称。
      pos：窗口的目标位置。
      cond：条件，用于控制何时应用位置设置。
  ## void SetWindowSize：设置指定名称窗口的大小。设置某个轴为 0.0f 以强制该轴自动适应内容
    参数：const char* name, const ImVec2& size, ImGuiCond cond = 0
    说明：
      name：窗口名称。
      size：窗口的目标大小。
      cond：条件，用于控制何时应用大小设置。
  ## void SetWindowCollapsed：设置指定名称窗口的折叠状态。
    参数：const char* name, bool collapsed, ImGuiCond cond = 0
    说明：
      name：窗口名称。
      collapsed：窗口是否折叠。
      cond：条件，用于控制何时应用折叠状态。
  ## void SetWindowFocus：  设置指定名称窗口为焦点窗口或置于最前 
    参数：const char* name
    说明：name：窗口名称
  ## void ImGui::Text：文本控件，显示字符
    参数：const char* fmt
    说明：需要显示的字符
  ## bool ImGui::Button：按钮控件
    参数：const char* label, const ImVec2& size = ImVec2(0, 0)
    返回值：如果按钮被按下，则返回 true。
    说明：
      label：按钮显示的文本，也是标识
      size：按钮的大小，ImVec2代表宽高
    调用示例：
      if(ImGui::Button("按钮文本",ImVec2(100,50))){
        //点击后调用
      }
  ## bool ImGui::InputText：文本输入框
    参数：const char label, char buf, size_t buf_size, ImGuiInputTextFlags flags = 0
    返回值：如果文本被修改，则返回 true。
    说明：
      label：输入框前面显示的标签
      buf：存储输入文本的缓冲区,也就是输入框内容变量
      buf_size：缓冲区大小
      flags：输入框的标志，可以控制输入框的行为和外观
  ## bool ImGui::Checkbox：复选框
    参数：const char label, bool v
    返回值：如果复选框的状态被修改，则返回 true。
    说明：
      label：复选框旁边显示的标签
      v：复选框状态
    调用示例：
      bool 复选框值=false;
      if(ImGui::Checkbox("复选框",复选框值)){
        //点击后调用
      }
  ## bool ImGui::SliderFloat：浮点滑块
    参数：const char label, float v, float v_min, float v_max, const char* format = "%.3f", float power = 1.0f
    返回值：如果滑动条的值被修改，则返回 true。
    说明：
      label：滑动条前面显示的标签。
      v：一个 float 变量的指针，用于存储滑动条的值。
      v_min：滑动条的最小值。
      v_max：滑动条的最大值。
      format：显示值的格式化字符串。
      power：滑动条的速度。
    调用示例：
      float 滑块值=0;
      ImGui::SliderFloat("滑块",滑块值,0,10);
## void ImGui::Image：图片控件
    参数：ImTextureID user_texture_id, const ImVec2& size, const ImVec2& uv0, const ImVec2& uv1, const ImVec4& tint_col, const ImVec4& border_col
    说明：
      user_texture_id：图片纹理id
      size：图片大小，ImVec2类型传入宽高
      uv0：图像的左上角 UV 坐标。UV 坐标是纹理坐标，范围通常在 [0, 1] 之间。默认值为 (0, 0)，表示纹理的左上角
      uv1：图像的右下角 UV 坐标。默认值为 (1, 1)，表示纹理的右下角
      tint_col：图像的颜色覆盖（Tint Color）。这个参数是一个 ImVec4 类型，表示 RGBA 颜色值。默认值为 (1, 1, 1, 1)，即不改变图像颜色
      border_col：图像边框的颜色。如果不需要边框，可以将其设置为 (0, 0, 0, 0)。这个参数也是一个 ImVec4 类型，表示 RGBA 颜色值。默认值为 (0, 0, 0, 0)，即没有边框
## ImGuiWindowFlags：窗口风格枚举定义描述
    ImGuiWindowFlags_None                   = 0,        // 无特殊标志
    ImGuiWindowFlags_NoTitleBar             = 1 << 0,   // 禁用标题栏
    ImGuiWindowFlags_NoResize               = 1 << 1,   // 禁用用户通过右下角调整窗口大小
    ImGuiWindowFlags_NoMove                 = 1 << 2,   // 禁用用户移动窗口
    ImGuiWindowFlags_NoScrollbar            = 1 << 3,   // 禁用滚动条（窗口仍然可以通过鼠标或程序滚动）
    ImGuiWindowFlags_NoScrollWithMouse      = 1 << 4,   // 禁用用户通过鼠标滚轮垂直滚动。在子窗口中，鼠标滚轮将被转发到父窗口，除非也设置了 NoScrollbar 标志。
    ImGuiWindowFlags_NoCollapse             = 1 << 5,   // 禁用用户通过双击窗口来折叠窗口（窗口菜单按钮）
    ImGuiWindowFlags_AlwaysAutoResize       = 1 << 6,   // 每帧根据内容调整窗口大小
    ImGuiWindowFlags_NoBackground           = 1 << 7,   // 禁用绘制背景颜色（如 WindowBg）和外边框。类似于使用 SetNextWindowBgAlpha(0.0f)
    ImGuiWindowFlags_NoSavedSettings        = 1 << 8,   // 不在 .ini 文件中加载/保存设置
    ImGuiWindowFlags_NoMouseInputs          = 1 << 9,   // 禁用捕获鼠标，悬停测试通过
    ImGuiWindowFlags_MenuBar                = 1 << 10,  // 有菜单栏
    ImGuiWindowFlags_HorizontalScrollbar    = 1 << 11,  // 允许出现水平滚动条（默认关闭）。可以在调用 Begin() 之前使用 SetNextWindowContentSize(ImVec2(width,0.0f)); 指定宽度。
    ImGuiWindowFlags_NoFocusOnAppearing     = 1 << 12,  // 禁用从隐藏状态过渡到可见状态时获取焦点
    ImGuiWindowFlags_NoBringToFrontOnFocus  = 1 << 13,  // 禁用获取焦点时将窗口置于前面（例如点击它或程序上给它焦点）
    ImGuiWindowFlags_AlwaysVerticalScrollbar= 1 << 14,  // 始终显示垂直滚动条（即使 ContentSize.y < Size.y）
    ImGuiWindowFlags_AlwaysHorizontalScrollbar=1<< 15,  // 始终显示水平滚动条（即使 ContentSize.x < Size.x）
    ImGuiWindowFlags_AlwaysUseWindowPadding = 1 << 16,  // 确保没有边框的子窗口使用 style.WindowPadding（默认情况下无边框子窗口会忽略，因为更方便）
    ImGuiWindowFlags_NoNavInputs            = 1 << 18,  // 禁用游戏手柄/键盘在窗口内导航
    ImGuiWindowFlags_NoNavFocus             = 1 << 19,  // 禁用游戏手柄/键盘导航聚焦到此窗口（例如，使用 CTRL+TAB 跳过）
    ImGuiWindowFlags_UnsavedDocument        = 1 << 20,  // 在标题旁边显示一个点。在选项卡/停靠上下文中使用时，点击 X 时会选中选项卡且不会假设关闭（将等待用户停止提交选项卡）。否则按 X 时假设关闭，如果您继续提交，选项卡可能会重新出现在选项卡栏末尾。
    ImGuiWindowFlags_NoNav                  = ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus, // 禁用所有导航
    ImGuiWindowFlags_NoDecoration           = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse, // 禁用所有装饰
    ImGuiWindowFlags_NoInputs               = ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus, // 禁用所有输入

## ImGuiCol：控件/其他
    ImGuiCol_Text：普通文本颜色。
    ImGuiCol_TextDisabled：禁用文本的颜色。
    ImGuiCol_WindowBg：普通窗口的背景颜色。
    ImGuiCol_ChildBg：子窗口的背景颜色。
    ImGuiCol_PopupBg：弹出窗口、菜单和工具提示窗口的背景颜色。
    ImGuiCol_Border：边框颜色。
    ImGuiCol_BorderShadow：边框阴影颜色。
    ImGuiCol_FrameBg：复选框、单选按钮、图表、滑块和文本输入框的背景颜色。
    ImGuiCol_FrameBgHovered：复选框等控件在鼠标悬停时的背景颜色。
    ImGuiCol_FrameBgActive：复选框等控件在激活时的背景颜色。
    ImGuiCol_TitleBg：窗口标题栏的背景颜色。
    ImGuiCol_TitleBgActive：活动窗口的标题栏背景颜色。
    ImGuiCol_TitleBgCollapsed：折叠窗口的标题栏背景颜色。
    ImGuiCol_MenuBarBg：菜单栏的背景颜色。
    ImGuiCol_ScrollbarBg：滚动条的背景颜色。
    ImGuiCol_ScrollbarGrab：滚动条抓取条的颜色。
    ImGuiCol_ScrollbarGrabHovered：滚动条抓取条在鼠标悬停时的颜色。
    ImGuiCol_ScrollbarGrabActive：滚动条抓取条在激活时的颜色。
    ImGuiCol_CheckMark：复选框和单选按钮的勾选标记颜色。
    ImGuiCol_SliderGrab：滑块控件的抓取条颜色。
    ImGuiCol_SliderGrabActive：滑块控件在激活时的抓取条颜色。
    ImGuiCol_Button：按钮的颜色。
    ImGuiCol_ButtonHovered：按钮在鼠标悬停时的颜色。
    ImGuiCol_ButtonActive：按钮在激活时的颜色。
    ImGuiCol_Header：折叠标题、树节点、选择项和菜单项的颜色。
    ImGuiCol_HeaderHovered：上述控件在鼠标悬停时的颜色。
    ImGuiCol_HeaderActive：上述控件在激活时的颜色。
    ImGuiCol_Separator：分隔线的颜色。
    ImGuiCol_SeparatorHovered：分隔线在鼠标悬停时的颜色。
    ImGuiCol_SeparatorActive：分隔线在激活时的颜色。
    ImGuiCol_ResizeGrip：窗口调整大小控件的颜色。
    ImGuiCol_ResizeGripHovered：窗口调整大小控件在鼠标悬停时的颜色。
    ImGuiCol_ResizeGripActive：窗口调整大小控件在激活时的颜色。
    ImGuiCol_Tab：选项卡的颜色。
    ImGuiCol_TabHovered：选项卡在鼠标悬停时的颜色。
    ImGuiCol_TabActive：选项卡在激活时的颜色。
    ImGuiCol_TabUnfocused：未聚焦选项卡的颜色。
    ImGuiCol_TabUnfocusedActive：未聚焦但激活的选项卡的颜色。
    ImGuiCol_PlotLines：折线图的颜色。
    ImGuiCol_PlotLinesHovered：折线图在鼠标悬停时的颜色。
    ImGuiCol_PlotHistogram：直方图的颜色。
    ImGuiCol_PlotHistogramHovered：直方图在鼠标悬停时的颜色。
    ImGuiCol_TableHeaderBg：表格头部的背景颜色。
    ImGuiCol_TableBorderStrong：表格外边框和头部边框的颜色（建议使用 Alpha=1.0）。
    ImGuiCol_TableBorderLight：表格内边框的颜色（建议使用 Alpha=1.0）。
    ImGuiCol_TableRowBg：表格偶数行的背景颜色。
    ImGuiCol_TableRowBgAlt：表格奇数行的背景颜色。
    ImGuiCol_TextSelectedBg：选中文本的背景颜色。
    ImGuiCol_DragDropTarget：拖放目标的颜色。
    ImGuiCol_NavHighlight：游戏手柄/键盘高亮显示当前项目的颜色。
    ImGuiCol_NavWindowingHighlight：使用 CTRL+TAB 时高亮显示窗口的颜色。
    ImGuiCol_NavWindowingDimBg：使用 CTRL+TAB 时，在窗口列表背后暗化/着色整个屏幕的颜色。
    ImGuiCol_ModalWindowDimBg：模态窗口激活时，在窗口背后暗化/着色整个屏幕的颜色。
    ImGuiCol_COUNT：颜色计数，用于枚举的总数。
## ImGuiStyleVar：样式枚举
    ImGuiStyleVar_Alpha：全局透明度，范围为 [0.0, 1.0]。
    ImGuiStyleVar_DisabledAlpha：禁用控件的透明度，范围为 [0.0, 1.0]。
    ImGuiStyleVar_WindowPadding：窗口内边距（x, y）。
    ImGuiStyleVar_WindowRounding：窗口圆角半径。
    ImGuiStyleVar_WindowBorderSize：窗口边框大小。
    ImGuiStyleVar_WindowMinSize：窗口最小大小（宽度和高度）。
    ImGuiStyleVar_WindowTitleAlign：窗口标题对齐方式（x, y），(0.0, 0.0) 表示左对齐，(1.0, 1.0) 表示右对齐。
    ImGuiStyleVar_ChildRounding：子窗口圆角半径。
    ImGuiStyleVar_ChildBorderSize：子窗口边框大小。
    ImGuiStyleVar_PopupRounding：弹出窗口圆角半径。
    ImGuiStyleVar_PopupBorderSize：弹出窗口边框大小。
    ImGuiStyleVar_FramePadding：帧内边距（控件与其内容之间的填充），（x, y）。
    ImGuiStyleVar_FrameRounding：帧圆角半径。
    ImGuiStyleVar_FrameBorderSize：帧边框大小。
    ImGuiStyleVar_ItemSpacing：控件之间的间距，（x, y）。
    ImGuiStyleVar_ItemInnerSpacing：控件内部元素之间的间距，（x, y）。
    ImGuiStyleVar_IndentSpacing：缩进的间距。
    ImGuiStyleVar_CellPadding：表格单元格内边距，（x, y）。
    ImGuiStyleVar_ScrollbarSize：滚动条大小。
    ImGuiStyleVar_ScrollbarRounding：滚动条圆角半径。
    ImGuiStyleVar_GrabMinSize：拖动控件的最小大小。
    ImGuiStyleVar_GrabRounding：拖动控件的圆角半径。
    ImGuiStyleVar_TabRounding：标签圆角半径。
    ImGuiStyleVar_ButtonTextAlign：按钮文字对齐方式，（x, y）。
    ImGuiStyleVar_SelectableTextAlign：可选择项的文字对齐方式，（x, y）。
    ImGuiStyleVar_COUNT：枚举值的数量，用于内部计算。






        
