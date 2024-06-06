# ImGui-参数、函数文档
  ## bool ImGui::Begin：创建父窗口
    参数：const char* id, bool* p_open = NULL, ImGuiWindowFlags flags = 0
    返回值：如果窗口当前可见，则返回 true。
    说明：
      id：唯一标识
      p_open：窗口是否（默认打开）
      flags：窗口属性，如是否无背景，拖拽，滚动，接收点击等
  ## void ImGui::End：结束父窗口，和ImGui::Begin一起成对出现
  ## bool ImGui::BeginChild：创建子窗口，写在父窗口内
    参数：const char* str_id, const ImVec2& size_arg, bool border, ImGuiWindowFlags extra_flags
    返回值：如果窗口当前可见，则返回 true。
    说明：
      str_id：唯一标识
      size_arg：窗口大小，ImVec2代表宽高
      flags：窗口属性，如是否无背景，拖拽，滚动，接收点击等
  ## void ImGui::EndChild：结束子窗口，跟创建子窗口成对出现
  ## void ImGui::Text：文本控件，显示字符
    参数：const char* fmt
    说明：需要显示的字符

  ## bool ImGui::Button：按钮控件
    参数：const char* label, const ImVec2& size = ImVec2(0, 0)
    返回值：如果按钮被按下，则返回 true。
    说明：
      label：按钮显示的文本，也是标识
      size：按钮的大小，ImVec2代表宽高
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








        
