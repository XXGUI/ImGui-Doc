# ImGui-参数、函数文档
  bool ImGui::Begin：创建父窗口
    参数：const char* name, bool* p_open = NULL, ImGuiWindowFlags flags = 0
    说明：
      name：名称/唯一标识
      p_open：窗口是否（默认打开）
      flags：窗口属性，如是否无背景，拖拽，滚动，接收点击等

  void ImGui::End：结束父窗口，和ImGui::Begin一起成对出现

  void ImGui::Text：文本控件，显示字符
    参数：const char* fmt
    说明：需要显示的字符

  bool ImGui::Button：按钮控件
    参数：const char* label, const ImVec2& size = ImVec2(0, 0)
    说明：
      label：按钮显示的文本，也是标识
      size：按钮的大小，ImVec2代表宽高
