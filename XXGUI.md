# XXGUI函数，功能说明
  
  # SameLineWithSpacing：并行显示
    参数：spacing 间隔
  # CustomNewLine：换行显示
    参数：lineHeight 间隔
  # HexToColor：十六进制字符串颜色转ImU32
    参数：hexColor 十六进制颜色字符串
  # ConvertToFloat：将百分比字符串转换为浮点数值
    参数：str 百分比，parentSize 具体像素
  # Div：布局头
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
  # Checkbox：复选框
  # RadioButton：单选框
  # Slider：滑块
  # Tabs：选项卡
  # ToggleButton：开关按钮
  # Show：加载信息弹窗
  # Render：渲染弹窗信息
  
  ## 调用示例：
      #include "XXGUI.h"
      XXGUI xxg;
      static bool checkboxValue1 = false;
      static bool checkboxValue2 = true;
      static bool checkboxValue3 = false;
      static bool checkboxValue4 = true;
      static bool checkboxValue5 = true;
      static bool checkboxValue6 = false;
      static bool checkboxValue7 = true;
      static bool checkboxValue8 = false;
      static int selectedIndex1 = 0;
      static int selectedIndex2 = 1;
      static int selectedIndex3 = 2;
      static int selectedIndex4 = 1;
      static float sliderValue1 = 5.0f;
      static float sliderValue2 = 6.0f;
      std::vector<std::string> tabs = {"标签1", "标签2", "标签3"};
      void ShowMyGUI() {
          xxg.ResetGrid();
          xxg.Div("div0"); 
              xxg.Div("div1", 6, "300px", "cecece");
                  xxg.Div("div1-1", 6); 
                      xxg.Checkbox("默认未选中", &checkboxValue1);
                      CustomNewLine(1);
                      xxg.Checkbox("自定义未选中", &checkboxValue3, "1e9fff");
                      CustomNewLine(1);
                      xxg.Checkbox("自定义选中2", &checkboxValue5, "ff5722");
                      CustomNewLine(1);
                      xxg.Checkbox("自定义选中3", &checkboxValue7, "2f363c");
                  xxg.EndDiv();
                  xxg.Div("div1-2", 6);
                      xxg.Checkbox("默认选中", &checkboxValue2);
                      CustomNewLine(1);
                      xxg.Checkbox("自定义选中", &checkboxValue4, "1e9fff");
                      CustomNewLine(1);
                      xxg.Checkbox("自定义未选中2", &checkboxValue6, "ff5722");
                      CustomNewLine(1);
                      xxg.Checkbox("自定未义选中3", &checkboxValue8, "2f363c");
                  xxg.EndDiv();
              xxg.EndDiv();
              xxg.Div("div2", 6, "300px","e2e2e2");
                  xxg.RadioButton("选项1", 0, &selectedIndex1);
                  xxg.RadioButton("选项2", 1, &selectedIndex1);
                  xxg.RadioButton("选项3", 2, &selectedIndex1);
                  CustomNewLine(1);
                  xxg.RadioButton("选项1", 0, &selectedIndex2,60,"1e9fff");
                  xxg.RadioButton("选项2", 1, &selectedIndex2,60,"1e9fff");
                  xxg.RadioButton("选项3", 2, &selectedIndex2,60,"1e9fff");
                  CustomNewLine(1);
                  xxg.RadioButton("选项1", 0, &selectedIndex3,60,"ff5722");
                  xxg.RadioButton("选项2", 1, &selectedIndex3,60,"ff5722");
                  xxg.RadioButton("选项3", 2, &selectedIndex3,60,"ff5722");
                  CustomNewLine(1);
                  xxg.RadioButton("选项1", 0, &selectedIndex4,60,"2f363c");
                  xxg.RadioButton("选项2", 1, &selectedIndex4,60,"2f363c");
                  xxg.RadioButton("选项3", 2, &selectedIndex4,60,"2f363c");
                  CustomNewLine(1);
              xxg.EndDiv();
              xxg.Div("div7", 7, "", "d2d2d2");
                  if (xxg.Button("左上弹窗", ImVec2(160, 100),"16baaa","FFFFFF")) {
                      xxg.Show("左上弹窗",XXGUI::Position::TopLeft);
                  }
                  if (xxg.Button("顶部弹窗", ImVec2(160, 100),"16b777","FFFFFF")) {
                      xxg.Show("顶部弹窗",XXGUI::Position::Top);
                  }
                  if (xxg.Button("右上弹窗", ImVec2(160, 100),"1e9fff","FFFFFF")) {
                      xxg.Show("右上弹窗",XXGUI::Position::TopRight);
                  }
                  ImGui::NewLine();
                  if (xxg.Button("左下弹窗", ImVec2(160, 100),"ff5722","FFFFFF")) {
                      xxg.Show("左下弹窗",XXGUI::Position::BottomLeft);
                  }
                  if (xxg.Button("底部弹窗", ImVec2(160, 100),"ffb800","FFFFFF")) {
                      xxg.Show("底部弹窗",XXGUI::Position::Bottom);
                  }
                  if (xxg.Button("右下弹窗", ImVec2(160, 100),"2f363c","FFFFFF")) {
                      xxg.Show("右下弹窗",XXGUI::Position::BottomRight);
                  }
                  ImGui::NewLine();
                  if (xxg.Button("居中弹窗", ImVec2(250, 100),"FFFFFF","000000",50)) {
                      xxg.Show("居中弹窗",XXGUI::Position::Center);
                  }
                  if (xxg.Button("退出程序", ImVec2(250, 100),"ff5722","FFFFFF",50)) {
                      exit(0);
                  }
                  ImGui::NewLine();
                  ImGui::NewLine();
                  xxg.Slider("滑块名称1", &sliderValue1);
                  xxg.Slider("滑块名称2", &sliderValue2,0,20,"1e9fff","eeeeee",0,true);
                  xxg.Slider("滑块名称3", &sliderValue1,0,20,"2f363c","eeeeee",0);
                  xxg.Slider("滑块名称4", &sliderValue2,0,10,"ff5722","eeeeee",0,true);
              xxg.EndDiv();
              xxg.Div("div9", 5, "", "e74c3c");
                  xxg.Tabs(tabs, &selectedIndex1);
                  xxg.Tabs(tabs, &selectedIndex2,0,0,"2f363c","FFFFFF");
                  xxg.ToggleButton("开关", &checkboxValue1);
                  xxg.ToggleButton("开关2", &checkboxValue2,90,"1e9fff");
                  xxg.ToggleButton("开关3", &checkboxValue3,90,"ff5722");
                  ImGui::NewLine();
                  ImGui::Text("Div 9 内容");
                  for (int i = 0; i < 50; ++i) {
                      ImGui::Text(("Div 9 内容可以上下滑动 " + std::to_string(i)).c_str());
                  }
              xxg.EndDiv();
          xxg.EndDiv();
          xxg.Render();
      }
      
      void menu(){
          ImGui::SetNextWindowPos(ImVec2(10,600));
          ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
          ImGui::SetNextWindowSize(MenuSize);
          ImGui::Begin("窗口", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
             ShowMyGUI();
          ImGui::End();
          ImGui::PopStyleVar();
      }
