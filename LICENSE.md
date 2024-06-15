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
## 调用示例2
    XXGUI xxg;
    static bool checkboxValue1 = true;
    static bool checkboxValue2 = true;
    static bool checkboxValue3 = true;
    static bool checkboxValue4 = true;
    static bool checkboxValue5 = true;
    static bool checkboxValue6 = true;
    static bool checkboxValue11 = false;
    static bool checkboxValue22 = false;
    static bool checkboxValue33 = false;
    static bool checkboxValue44 = false;
    static bool checkboxValue55 = false;
    static bool checkboxValue66 = false;
    static int RadioButton1 = 0;
    static int RadioButton2 = 0;
    static float sliderValue1 = 5;
    static float sliderValue2 = 10;
    static float sliderValue3 = 15;
    static float sliderValue4 = 25;
    static float sliderValue5 = 25;
    static float sliderValue6 = 25;
    static bool ToggleButton1 = true;
    static bool ToggleButton2 = true;
    static bool ToggleButton3 = true;
    static bool ToggleButton4 = true;
    static bool ToggleButton5 = true;
    static bool ToggleButton6 = true;
    static bool ToggleButton7 = false;
    static int tabIndex = 1;
    static int verticalTabsIndex = 0;
    std::vector<std::string> verticalTabs = {"选项5", "选项2", "选项3","选项4"};
    std::vector<std::string> tabs = {"单/复选框", "滑块&颜色", "按钮&开关","内容滑动","动画效果"};
    static ImVec4* selectedColor;
    static int colorIndex=0;
    void ShowMyGUI() {
        xxg.ResetGrid();
        xxg.AnimationFadeInOut([]() {
            xxg.Div("div0");
                xxg.Div("div1", 3, "", "2f363c",0);
                    if(xxg.VerticalTabs(tabs, &tabIndex,80,0,"","FFFFFF", "16b777", 10.0f, true,50)){
                        xxg.clearFadeMap({
                            XXGUI::AnimationType::FadeInOut,
                            XXGUI::AnimationType::FadeDown,
                            XXGUI::AnimationType::FadeLeft,
                            XXGUI::AnimationType::FadeRight
                        });
                    }
                xxg.EndDiv();
                SameLineWithSpacing(10);
                xxg.Div("div2", 9, "", "2f363c",30);
                    if(4!=tabIndex)
                        xxg.AnimationFadeUp([]() {
                            xxg.Tabs(verticalTabs, &verticalTabsIndex,70,0,"","FFFFFF");
                        });
                    if(0==tabIndex){
                        xxg.Checkbox("复选框0", &checkboxValue1,"67C23A","","","F56C6C");
                        xxg.Checkbox("复选框2", &checkboxValue2,"E6A23C","","","67C23A");
                        xxg.Checkbox("复选框3", &checkboxValue3,"F56C6C","","","E6A23C");
                        xxg.Checkbox("复选框4", &checkboxValue4,"ffb800","","","FFFFFF");
                        CustomNewLine();
                        xxg.Checkbox("复选框5", &checkboxValue11,"67C23A","","","F56C6C");
                        xxg.Checkbox("复选框6", &checkboxValue22,"E6A23C","","","67C23A");
                        xxg.Checkbox("复选框7", &checkboxValue33,"F56C6C","","","E6A23C");
                        xxg.Checkbox("复选框8", &checkboxValue44,"ffb800","","","FFFFFF");
                        CustomNewLine();
                        xxg.RadioButton("单选框9", 0, &RadioButton1,0,"","","FFFFFF");
                        xxg.RadioButton("单选框10", 1, &RadioButton1,0,"","","F56C6C");
                        xxg.RadioButton("单选框11", 2, &RadioButton1,0,"","","67C23A");
                        xxg.RadioButton("单选框12", 3, &RadioButton1,0,"","","ffb800");
                        CustomNewLine();
                        xxg.RadioButton("单选框13", 0, &RadioButton2,0,"67C23A","","FFFFFF");
                        xxg.RadioButton("单选框14", 1, &RadioButton2,0,"FFFFFF","","F56C6C");
                        xxg.RadioButton("单选框15", 2, &RadioButton2,0,"F56C6C","","67C23A");
                        xxg.RadioButton("单选框16", 3, &RadioButton2,0,"a233c6","","ffb800");
                    }else if(1==tabIndex){
                        CustomNewLine();
                        xxg.Slider("滑块名称1", &sliderValue1);
                        xxg.Slider("滑块名称2", &sliderValue2,0,20,"1e9fff","eeeeee",0,true);
                        xxg.Slider("滑块名称3", &sliderValue3,0,20,"67C23A","eeeeee",0);
                        xxg.Slider("滑块名称4", &sliderValue4,0,100,"ff5722","eeeeee",0,true);
                        xxg.Slider("滑块名称5", &sliderValue5,0,50,"F56C6C","a233c6",0);
                        xxg.Slider("滑块名称6", &sliderValue6,0,100,"a233c6","ff5722",0,true);
                        CustomNewLine();
                        //xxg.ColorPicker("颜色",ImVec2(0,0),selectedColor,colorIndex);
                    }else if(2==tabIndex){
                        CustomNewLine();
                        xxg.ToggleButton("开关1", &ToggleButton1);
                        SameLineWithSpacing(30);
                        xxg.ToggleButton("开关2", &ToggleButton2,90,"1e9fff");
                        SameLineWithSpacing(30);
                        xxg.ToggleButton("开关3", &ToggleButton3,90,"67C23A");
                        SameLineWithSpacing(30);
                        xxg.ToggleButton("开关4", &ToggleButton4,90,"ff5722");
                        SameLineWithSpacing(30);
                        xxg.ToggleButton("开关5", &ToggleButton5,90,"F56C6C");
                        SameLineWithSpacing(30);
                        xxg.ToggleButton("开关6", &ToggleButton6,90,"a233c6");
                        CustomNewLine(70);
                        if (xxg.Button("左上弹窗", ImVec2(170, 80),"16baaa","FFFFFF")) {
                            xxg.Show("左上弹窗",XXGUI::Position::TopLeft);
                        }
                        if (xxg.Button("顶部弹窗", ImVec2(170, 80),"16b777","FFFFFF")) {
                            xxg.Show("顶部弹窗",XXGUI::Position::Top);
                        }
                        if (xxg.Button("右上弹窗", ImVec2(170, 80),"1e9fff","FFFFFF")) {
                            xxg.Show("右上弹窗",XXGUI::Position::TopRight);
                        }
                        if (xxg.Button("左下弹窗", ImVec2(170, 80),"ff5722","FFFFFF")) {
                            xxg.Show("左下弹窗",XXGUI::Position::BottomLeft);
                        }
                        CustomNewLine(70);
                        if (xxg.Button("底部弹窗", ImVec2(170, 80),"ffb800","FFFFFF")) {
                        xxg.Show("底部弹窗",XXGUI::Position::Bottom);
                        }
                        if (xxg.Button("右下弹窗", ImVec2(170, 80),"67C23A","FFFFFF",50)) {
                            xxg.Show("右下弹窗",XXGUI::Position::BottomRight);
                        }
                        if (xxg.Button("居中弹窗", ImVec2(170, 80),"FFFFFF","67C23A",50)) {
                            xxg.Show("居中弹窗",XXGUI::Position::Center);
                        }
                        if (xxg.Button("退出程序", ImVec2(170, 80),"ff5722","FFFFFF",50)) {
                            exit(0);
                        }
                    }else if(3==tabIndex){
                        for (int i = 0; i < 50; ++i) {
                            ImGui::Text(("可以按住内容上下滑动" + std::to_string(i)).c_str());
                        }
                    }else if(4==tabIndex){
                        xxg.AnimationFadeInOut([]() {
                            xxg.AnimationFadeDown([]() {
                                xxg.Button("淡入", ImVec2(170, 80),"16baaa","FFFFFF");
                            },30);
                        });
                        xxg.AnimationFadeInOut([]() {
                            xxg.ToggleButton("开关71", &ToggleButton1);
                        },30);
                        xxg.AnimationFadeRight([]() {
                            xxg.RadioButton("单选框13动画", 0, &RadioButton2,0,"67C23A","","FFFFFF");
                        },50);
                        CustomNewLine();
                        xxg.AnimationFadeInOut([]() {
                            xxg.Checkbox("复选框1111", &checkboxValue1,"67C23A","","","F56C6C");
                        },50);
                        xxg.AnimationFadeLeft([]() {
                            xxg.Checkbox("复选框1d111222", &checkboxValue2,"E6A23C","","","67C23A");
                        },50);
                        CustomNewLine();
                        xxg.AnimationFadeDown([]() {
                            xxg.Slider("滑块名称1333", &sliderValue1);
                        },50);
                    }
                xxg.EndDiv();
            xxg.EndDiv();
        });
        xxg.Render();
    }
    
    void menu(){
        ImGui::SetNextWindowPos(BeginPos);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::SetNextWindowSize(MenuSize);
        ImGui::Begin("窗口", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);
           ShowMyGUI();
        ImGui::End();
        ImGui::PopStyleVar();
    }
