# ImGui-参数、函数文档
  ## bool ImGui::Begin：创建父窗口
    参数：const char* name, bool* p_open = NULL, ImGuiWindowFlags flags = 0
    返回值：如果窗口当前可见，则返回 true。
    说明：
      name：名称/唯一标识
      p_open：窗口是否（默认打开）
      flags：窗口属性，如是否无背景，拖拽，滚动，接收点击等

  ## void ImGui::End：结束父窗口，和ImGui::Begin一起成对出现

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









        
