#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <unordered_map>
#include <chrono>
#include <vector>
#include <map>
#include <functional>

float screen_x,screen_y;
bool 是否在滑动内容 = false;
ImVec2 MenuSize(1800, 900);
ImVec2 SetMenuSize(700, 550);
ImVec2 BeginPos(0, 0);

TextureInfo J1{};
TextureInfo J2{};

// 不换行并行显示带间距
void SameLineWithSpacing(float spacing = 0.0) {
    ImGui::SameLine(0.0f, spacing);
}

// 换行显示
void CustomNewLine(float lineHeight = -1.0f) {
    ImVec2 cursorPos = ImGui::GetCursorPos();
    float lastItemHeight = ImGui::GetItemRectSize().y;
    float newY = cursorPos.y + lastItemHeight + (lineHeight <= 0.0f ? 0.0f : lineHeight);
    ImGui::SetCursorPos(ImVec2(ImGui::GetStyle().WindowPadding.x, newY));
}

// 辅助函数：将十六进制字符串转换为 ImGui 使用的颜色
ImU32 HexToColor(const std::string &hexColor) {
    if (hexColor.empty()) {
        return IM_COL32(0, 0, 0, 0);  // 透明颜色
    }
    unsigned int hex;
    std::stringstream ss;
    ss << std::hex << hexColor;
    ss >> std::hex >> hex;
    int r, g, b, a;
    if (hexColor.length() == 8) {
        r = (hex >> 24) & 0xFF;
        g = (hex >> 16) & 0xFF;
        b = (hex >> 8) & 0xFF;
        a = hex & 0xFF;
    } else {
        r = (hex >> 16) & 0xFF;
        g = (hex >> 8) & 0xFF;
        b = hex & 0xFF;
        a = 255;  // 不透明
    }
    return IM_COL32(r, g, b, a);
}

// 将ImU32颜色转换为ImVec4
ImVec4 ImU32ToImVec4(ImU32 color) {
    float alpha = ((color >> 24) & 0xFF) / 255.0f;
    float red = ((color >> 16) & 0xFF) / 255.0f;
    float green = ((color >> 8) & 0xFF) / 255.0f;
    float blue = (color & 0xFF) / 255.0f;
    return ImVec4(red, green, blue, alpha);
}

// 辅助函数：将十六进制字符串转换为 ImVec4 使用的颜色
ImVec4 HexToImVec4Color(const std::string &hexColor) {
    if (hexColor.empty()) {
        return ImU32ToImVec4(IM_COL32(0, 0, 0, 0));  // 透明颜色
    }
    unsigned int hex;
    std::stringstream ss;
    ss << std::hex << hexColor;
    ss >> std::hex >> hex;
    int r, g, b, a;
    if (hexColor.length() == 8) {
        r = (hex >> 24) & 0xFF;
        g = (hex >> 16) & 0xFF;
        b = (hex >> 8) & 0xFF;
        a = hex & 0xFF;
    } else {
        r = (hex >> 16) & 0xFF;
        g = (hex >> 8) & 0xFF;
        b = hex & 0xFF;
        a = 255;  // 不透明
    }
    return ImU32ToImVec4(IM_COL32(r, g, b, a));
}

// 辅助函数：将百分比字符串转换为浮点数值
float ConvertToFloat(const std::string &str, float parentSize) {
    if (str.back() == '%') {
        float percentage = std::stof(str.substr(0, str.size() - 1));
        return (percentage / 100.0f) * parentSize;
    } else {
        return std::stof(str);
    }
}

class XXGUI {
public:
    bool setBeginPos = true;
    enum class Position {
        TopLeft,
        TopRight,
        BottomLeft,
        BottomRight,
        Center,
        Top,
        Bottom
    };
    enum class TextAlign {
        Left,
        Center,
        Right
    };
    enum class MarkerPosition {
        Left,
        Right
    };
    enum class AnimationType {
        All,
        FadeInOut,
        FadeDown,
        FadeUp,
        FadeLeft,
        FadeRight
    };
    struct ToggleButtonState {
        float t = 0.0f;
        std::string label;
    };
    struct PopupInfo {
        std::string content;
        Position position;
        ImU32 bgColor;
        ImU32 textColor;
        int borderRadius;
        float duration;
        float padding;
        ImVec2 textSize;
        std::chrono::steady_clock::time_point showTime;
    };
    // 栅格系统全局变量
    struct GridState {
        int currentColSum;
        float parentWidth;
        float parentHeight;
    };
    std::vector <GridState> gridStack;
    std::unordered_map<std::string, float> scrollOffsets;
    // 使用一个全局 map 存储每个动画的 Fade 变量
    std::map<std::string, float> fadeMap;
    // Div标识累计
    int divCounter = 0;
    // 复选框标记累计
    int CheckboxButtonFlowLayoutItemIndex = 0;

    std::vector <ImVec4> colorList = {
            ImVec4(1.0f, 1.0f, 1.0f, 1.0f),
            ImVec4(1.0f, 0.341f, 0.133f, 1.0f),  // #ff5722
            ImVec4(1.0f, 0.722f, 0.0f, 1.0f),    // #ffb800
            ImVec4(0.086f, 0.729f, 0.667f, 1.0f), // #16baaa
            ImVec4(0.117f, 0.62f, 1.0f, 1.0f),    // #1e9fff
            ImVec4(0.635f, 0.2f, 0.776f, 1.0f),   // #a233c6
            ImVec4(1.0f, 0.341f, 0.133f, 1.0f),  // #ff5722
            ImVec4(1.0f, 0.722f, 0.0f, 1.0f),    // #ffb800
            ImVec4(0.086f, 0.729f, 0.667f, 1.0f), // #16baaa
            ImVec4(0.086f, 0.729f, 0.667f, 1.0f), // #16baaa
            ImVec4(0.117f, 0.62f, 1.0f, 1.0f),    // #1e9fff
            ImVec4(0.635f, 0.2f, 0.776f, 1.0f),   // #a233c6
            ImVec4(0.184f, 0.215f, 0.235f, 1.0f) // #2f363c
    };

    void Show(const std::string &content,
              Position position = Position::Top,
              ImU32 bgColor = IM_COL32_WHITE,
              ImU32 textColor = IM_COL32_BLACK,
              int borderRadius = 10,
              float duration = 1.0f,
              float padding = 30.0f,
              ImVec2 textSize = ImVec2(0, 0)) {
        PopupInfo popupInfo;
        popupInfo.content = content;
        popupInfo.position = position;
        popupInfo.bgColor = bgColor;
        popupInfo.textColor = textColor;
        popupInfo.borderRadius = borderRadius;
        popupInfo.duration = duration;
        popupInfo.showTime = std::chrono::steady_clock::now();
        popupInfo.textSize = textSize;
        popupInfo.padding = padding;
        popups.push_back(popupInfo);
    }

    void Render() {
        auto currentTime = std::chrono::steady_clock::now();
        for (auto &popup: popups) {
            if (std::chrono::duration_cast < std::chrono::duration <
                float >> (currentTime - popup.showTime).count() >= popup.duration) {
                popup.showTime = std::chrono::steady_clock::time_point::min(); // 标记为过期
            } else {
                RenderPopup(popup);
            }
        }
        popups.erase(std::remove_if(popups.begin(), popups.end(), [](const PopupInfo &popup) {
            return popup.showTime == std::chrono::steady_clock::time_point::min();
        }), popups.end());
        CheckboxButtonFlowLayoutItemIndex = 0;
    }

    // 自定义 Div 函数
    void Div(int gridWidth = 12, const std::string &height = "", const std::string &bgColor = "",
             const std::string &shadowColor = "", float shadowHeight = 100.0f,
             int shadowDirection = 0, float padding = 10.0f,
             int borderRadius = 10, bool log=false,ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar) {
        const std::string id = "Div_" + std::to_string(divCounter++);
        gridWidth = gridWidth > 0 ? gridWidth : 12;
        // 获取父容器宽度
        if (gridStack.empty()) {
            gridStack.push_back({0, 0});
        }
        // 获取父级内边距
        ImVec2 parentPadding = ImGui::GetStyle().WindowPadding;
        GridState &currentGrid = gridStack.back();
        currentGrid.parentHeight = ImGui::GetContentRegionAvail().y;
        if (currentGrid.parentWidth == 0) {
            currentGrid.parentWidth = ImGui::GetContentRegionAvail().x;
        }
        // 处理换行逻辑
        if (currentGrid.currentColSum + gridWidth > 12) {
            currentGrid.currentColSum = 0;
        }
        // 计算宽度
        float width = currentGrid.parentWidth * (gridWidth / 12.0f) - parentPadding.x;
        // 计算高度
        float heightVal = 0;
        if (!height.empty()) {
            float parentHeight = ImGui::GetContentRegionAvail().y;
            heightVal = ConvertToFloat(height, parentHeight);
        } else {
            heightVal = ImGui::GetContentRegionAvail().y;
        }
        // 设置背景颜色和圆角
        ImGui::PushStyleColor(ImGuiCol_ChildBg, HexToColor(bgColor));
        ImGui::PushStyleColor(ImGuiCol_Border, HexToColor(bgColor));
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, borderRadius);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(padding, padding));
        // 创建带滚动条的子窗口，默认不显示边框
        ImGui::BeginChild(id.c_str(), ImVec2(width, heightVal), true, flags);
        // 获取子窗口的左上角屏幕坐标
        ImVec2 childPos = ImGui::GetCursorScreenPos();
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        //顶部内阴影
        ImVec2 gradient_start = ImVec2(childPos.x - parentPadding.x, childPos.y - parentPadding.y);
        ImVec2 gradient_end = ImVec2(childPos.x + width,
                                     childPos.y + (0 == shadowHeight ? heightVal : shadowHeight));
        if (0 == shadowDirection) {
            draw_list->AddRectFilledMultiColor(gradient_start, gradient_end,
                                               HexToColor(shadowColor) & 0x00FFFFFF,
                                               HexToColor(shadowColor),
                                               HexToColor(shadowColor),
                                               HexToColor(shadowColor) & 0x00FFFFFF);
        } else if (1 == shadowDirection) {
            draw_list->AddRectFilledMultiColor(gradient_start, gradient_end,
                                               HexToColor(shadowColor) & 0x00FFFFFF,
                                               HexToColor(shadowColor) & 0x00FFFFFF,
                                               HexToColor(shadowColor),
                                               HexToColor(shadowColor));
        } else if (2 == shadowDirection) {
            draw_list->AddRectFilledMultiColor(gradient_start, gradient_end,
                                               HexToColor(shadowColor),
                                               HexToColor(shadowColor) & 0x00FFFFFF,
                                               HexToColor(shadowColor) & 0x00FFFFFF,
                                               HexToColor(shadowColor));
        } else if (3 == shadowDirection) {
            draw_list->AddRectFilledMultiColor(gradient_start, gradient_end,
                                               HexToColor(shadowColor),
                                               HexToColor(shadowColor),
                                               HexToColor(shadowColor) & 0x00FFFFFF,
                                               HexToColor(shadowColor) & 0x00FFFFFF);
        }
        // 获取当前滚动偏移量
        float &scrollOffset = scrollOffsets[id];
        // 检测内容高度是否超过窗口高度
        float contentHeight = ImGui::GetCursorPosY() + ImGui::GetScrollMaxY();//内容高度
        float windowHeight = heightVal;//窗口高度
        // 只有内容超过窗口高度时才禁用父窗口拖动
        if (contentHeight > 0 && contentHeight > windowHeight && ImGui::IsWindowHovered() &&
            ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            scrollOffset -= ImGui::GetIO().MouseDelta.y;
            scrollOffset = std::max(0.0f, std::min(scrollOffset, ImGui::GetScrollMaxY()));
            ImGui::SetScrollY(scrollOffset);
        } else if (setBeginPos && contentHeight < windowHeight && ImGui::IsWindowHovered() &&
                   ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
            ImVec2 delta = ImGui::GetMouseDragDelta(0, 0.0f);
            BeginPos.x += delta.x;
            BeginPos.y += delta.y;
            ImGui::ResetMouseDragDelta(0);
        }

        // 更新当前列的总和
        currentGrid.currentColSum += gridWidth;
        // 添加新的栅格状态到栈中
        gridStack.push_back({0, width});
    }

    void DivTitle(const std::string &title = "", int gridWidth = 12, const std::string &height = "",
                  const std::string &bgColor = "",
                  float padding = 10.0f, int borderRadius = 10,
                  ImGuiWindowFlags flags = ImGuiWindowFlags_NoScrollbar) {
        const std::string id = title;
        gridWidth = gridWidth > 0 ? gridWidth : 12;
        // 获取父容器宽度
        if (gridStack.empty()) {
            gridStack.push_back({0, 0});
        }
        // 获取父级内边距
        ImVec2 parentPadding = ImGui::GetStyle().WindowPadding;
        GridState &currentGrid = gridStack.back();
        if (currentGrid.parentWidth == 0) {
            currentGrid.parentWidth = ImGui::GetContentRegionAvail().x;
        }
        // 处理换行逻辑
        if (currentGrid.currentColSum + gridWidth > 12) {
            currentGrid.currentColSum = 0;
        }
        // 计算宽度
        float width = currentGrid.parentWidth * (gridWidth / 12.0f) - parentPadding.x * 2;
        // 计算高度
        float heightVal = 0;
        if (!height.empty()) {
            float parentHeight = ImGui::GetContentRegionAvail().y;
            heightVal = ConvertToFloat(height, parentHeight);
        } else {
            heightVal = ImGui::GetContentRegionAvail().y;
        }
        ImGuiWindow *window = ImGui::GetCurrentWindow();
        const ImVec2 pos(window->DC.CursorPos.x,
                         window->DC.CursorPos.y + window->DC.CurrLineTextBaseOffset);
        // 设置背景颜色和圆角
        ImGui::PushStyleColor(ImGuiCol_ChildBg, HexToColor(bgColor));
        ImGui::PushStyleColor(ImGuiCol_Border, HexToColor(bgColor));
        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, borderRadius);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(padding, padding));
        // 创建带滚动条的子窗口，默认不显示边框
        ImGui::BeginChild(id.c_str(), ImVec2(width, heightVal), true, flags);
        CustomText(id.c_str(), ImVec2(width, 50), 10, 0, 1.2f, false);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY());
//         获取当前滚动偏移量
//        float &scrollOffset = scrollOffsets[id];
        // 检测内容高度是否超过窗口高度
//        float contentHeight = ImGui::GetCursorPosY() + ImGui::GetScrollMaxY();//内容高度
//        float windowHeight = heightVal;//窗口高度
        // 只有内容超过窗口高度时才禁用父窗口拖动
//        if (contentHeight > 0 && contentHeight > windowHeight && ImGui::IsWindowHovered() &&
//            ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
//            scrollOffset -= ImGui::GetIO().MouseDelta.y;
//            scrollOffset = std::max(0.0f, std::min(scrollOffset, ImGui::GetScrollMaxY()));
//            ImGui::SetScrollY(scrollOffset);
//            是否在滑动内容 = true;
//        } else if (setBeginPos & contentHeight < windowHeight && ImGui::IsWindowHovered() &&
//                   ImGui::IsMouseDragging(ImGuiMouseButton_Left)) {
//            ImVec2 delta = ImGui::GetMouseDragDelta(0, 0.0f);
//            BeginPos.x += delta.x;
//            BeginPos.y += delta.y;
//            ImGui::ResetMouseDragDelta(0);
//        }
        // 更新当前列的总和
        currentGrid.currentColSum += gridWidth;
        // 添加新的栅格状态到栈中
        gridStack.push_back({0, width});
    }

    void EndDiv() {
        ImGui::EndChild();
        ImGui::PopStyleColor(2);
        ImGui::PopStyleVar(2);
        // 从栅格状态栈中移除当前状态
        gridStack.pop_back();
        // 确保同一行的元素并排显示
        if (!gridStack.empty()) {
            GridState &currentGrid = gridStack.back();
            if (currentGrid.currentColSum % 12 != 0) {
                SameLineWithSpacing();
            }
        }
    }

    // 重置栅格系统
    void ResetGrid() {
        divCounter = 0;
        gridStack.clear();
        gridStack.push_back({0, 0});
    }

    // 折叠面板
    void CustomCollapsingHeader(const std::string &title, std::function<void()> widgets, bool defaultOpen = false,
                                const std::string &bgColor = "f2f2f2",
                                const std::string &textColor = "000000",
                                float padding = 10.0f, float titleHeight = 50.0f)
    {
        // 通过 Hex 转换颜色
        ImVec4 backgroundColor = HexToImVec4Color(bgColor);
        ImVec4 textColorVec = HexToImVec4Color(textColor);
        // 设置面板背景颜色
        ImGui::PushStyleColor(ImGuiCol_Header, backgroundColor);
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, backgroundColor);
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, backgroundColor);
        ImGui::PushStyleColor(ImGuiCol_Text, textColorVec);
        // 设置面板内边距
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(padding, titleHeight * 0.5f));
        // 使用ImGui的CollapsingHeader，并设置其展开/折叠状态
        if (ImGui::CollapsingHeader(title.c_str(), defaultOpen ? ImGuiTreeNodeFlags_DefaultOpen : 0)) {
            widgets();  // 调用传入的内容显示函数
        }

    }


    // 文本
    void CustomText(const char *text, ImVec2 size = ImVec2(0, 0), int padding = 5, int alignment = 0,
               float fontSize = 1.5f, bool showShadow = false,
               const std::string &bgColor = "", const std::string &textColor = "FFFFFF") {
        ImGui::SetWindowFontScale(fontSize);
        ImGuiWindow *window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return;
        const ImGuiStyle &style = ImGui::GetStyle();
        const ImGuiID id = window->GetID(text);
        const ImVec2 contentMin = window->DC.CursorPos;
        ImVec2 textSizeVec = ImGui::CalcTextSize(text);
        const ImVec2 contentMax = ImVec2(
                contentMin.x + ((0 == size.x && 0 == size.y) ? textSizeVec.x : size.x),
                contentMin.y + ((0 == size.x && 0 == size.y) ? textSizeVec.y : size.y));
        if (showShadow) {
            ImDrawList *drawList = ImGui::GetWindowDrawList();
            ImU32 shadowColor2 = IM_COL32(0, 0, 0, 128);
            float shadowStartY = contentMin.y + size.y - 3;
            float shadowEndY = shadowStartY + 10;
            for (float alpha = 0.5f; alpha >= 0.0f; alpha -= 0.075f) {
                shadowColor2 = IM_COL32(0, 0, 0, static_cast<int>(128 * alpha));
                drawList->AddRectFilledMultiColor(
                        ImVec2(contentMin.x, shadowStartY),
                        ImVec2(contentMin.x + size.x, shadowEndY),
                        shadowColor2, shadowColor2, shadowColor2, shadowColor2
                );
                shadowStartY += 5;
                shadowEndY += 5;
            }
        }
        window->DrawList->AddRectFilled(contentMin, contentMax, HexToColor(bgColor));
        ImVec2 textSize = ImGui::CalcTextSize(text);
        ImVec2 textPos;
        switch (alignment) {
            case 1:
                textPos = ImVec2(contentMax.x - textSize.x - padding,
                                 contentMin.y + (size.y - textSize.y) * 0.5f);
                break;
            case 2:
                textPos = ImVec2(contentMin.x + (size.x - textSize.x) * 0.5f,
                                 contentMin.y + (size.y - textSize.y) * 0.5f);
                break;
            default:
                textPos = ImVec2(contentMin.x + padding,
                                 contentMin.y + (size.y - textSize.y) * 0.5f);
                break;
        }
        window->DrawList->AddText(textPos, HexToColor(textColor), text);
        float textHeightWithPadding = textSize.y + padding * 2;
        ImGui::ItemSize(ImVec2(size.x, textHeightWithPadding), style.FramePadding.y);
        if (!ImGui::ItemAdd(
                ImRect(contentMin, ImVec2(contentMax.x, contentMin.y + textHeightWithPadding)), id))
            return;
        ImGui::SetWindowFontScale(1.0f);
    }

    // 按钮
    bool Button(const char *label = "按钮", ImVec2 size = ImVec2(0, 0),
                const std::string &buttonColorHex = "FFFFFF",
                const std::string &textColorHex = "000000",
                int borderRadius = 5, bool sameLine = true) {
        // 设置按钮样式
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, borderRadius);
        ImGui::PushStyleColor(ImGuiCol_Text, HexToColor(textColorHex));
        ImGui::PushStyleColor(ImGuiCol_Button, HexToColor(buttonColorHex));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexToColor(buttonColorHex)); // 可选：设置按钮悬停颜色
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexToColor(buttonColorHex));  // 可选：设置按钮按下颜色
        // 创建按钮并捕捉返回值
        bool clicked = ImGui::Button(label, size);
        // 恢复原始样式
        ImGui::PopStyleColor(4);
        ImGui::PopStyleVar();
        // 如果需要并排显示，下一个元素在同一行
        if (sameLine) {
            SameLineWithSpacing(10);
        }
        return clicked;
    }

    //带函数的按钮
    bool ButtonCallBack(const char *label = "按钮",
                        ImVec2 size = ImVec2(0, 0),
                        std::function<void()> widgets = []() {},
                        const std::string &buttonColorHex = "FFFFFF",
                        const std::string &textColorHex = "000000",
                        int borderRadius = 5,
                        bool sameLine = true,
                        const std::string &borderColorHex = "000000",
                        float borderSize = 0.05f) // 新增参数
    {
        if (size.x < 0)
            size = ImVec2(ImGui::GetContentRegionAvail().x, size.y);

        // 设置边框样式
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, borderRadius);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, borderSize); // 设置边框宽度
        ImGui::PushStyleColor(ImGuiCol_Border, HexToColor(borderColorHex)); // 设置边框颜色为黑色

        // 设置按钮文本和背景颜色
        ImGui::PushStyleColor(ImGuiCol_Text, HexToColor(textColorHex));
        ImGui::PushStyleColor(ImGuiCol_Button, HexToColor(buttonColorHex));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexToColor(buttonColorHex));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexToColor(buttonColorHex));

        // 创建按钮并捕捉返回值
        bool clicked = ImGui::Button(label, size);

        // 恢复原始样式
        ImGui::PopStyleColor(1); // 包括边框颜色
        ImGui::PopStyleVar(1); // 包括边框宽度和圆角

        // 如果需要并排显示，下一个元素在同一行
        if (sameLine)
            SameLineWithSpacing(10);

        if (clicked)
            widgets();

        return clicked;
    }

    // 复选框函数
    bool Checkbox(const char *label, bool *v, const std::string &checkedBgColor = "FFFFFF",
                  const std::string &uncheckedBgColor = "FFFFFF", const std::string &checkMarkColor = "F9B7CF",
                  const std::string &textColor = "FFFFFF", float borderRadius = 5.0f) {
        ImGuiWindow *window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return false;
        ImGui::PushID(label);
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 parentPadding = ImGui::GetStyle().WindowPadding;
        ImGuiContext &g = *ImGui::GetCurrentContext();
        const ImGuiStyle &style = g.Style;
        const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
        const float square_sz = ImGui::GetFrameHeight();
        const ImRect total_bb(pos, ImVec2(pos.x + square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f) + 10, pos.y + square_sz));

        ImGui::ItemSize(total_bb);                          // 声明控件占位尺寸
        ImGui::ItemAdd(total_bb, window->GetID(label));     // 注册控件
        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(total_bb, window->GetID(label), &hovered, &held);
        if (pressed) {
            *v = !*v;
            ImGui::MarkItemEdited(window->GetID(label));
        }

        const ImRect check_bb(pos, ImVec2(pos.x + square_sz, pos.y + square_sz));
        ImGui::RenderNavHighlight(total_bb, window->GetID(label));
        ImGui::RenderFrame(check_bb.Min, check_bb.Max,
                           *v ? HexToColor(checkedBgColor.empty() ? "ffb800" : checkedBgColor)
                              : HexToColor(uncheckedBgColor.empty() ? "e2e2e2" : uncheckedBgColor),
                           true, borderRadius);
        if (*v) {
            ImRect checkMark(ImVec2(check_bb.Min.x+15,check_bb.Min.y+15), ImVec2(check_bb.Max.x-15,check_bb.Max.y-15));
            ImGui::GetWindowDrawList()->AddRectFilled(checkMark.Min, checkMark.Max, HexToColor(checkMarkColor.empty() ? "FFFFFF" : checkMarkColor), borderRadius);
        }
        if (label_size.x > 0.0f) {
            SameLineWithSpacing(10);
            ImVec2 text_pos = ImVec2(pos.x + square_sz + style.ItemInnerSpacing.x,
                                     pos.y + style.FramePadding.y);
            ImGui::GetWindowDrawList()->AddText(text_pos, HexToColor(
                    textColor.empty() ? "000000" : textColor), label);
        }
        return pressed;
    }

    // 复选框函数按钮样式
    bool CheckboxButtonFlowLayout(const char *label, bool *v,
                                  ImTextureID icon, int itemsPerRow = 3,
                                  const std::string &checkedBgColor = "F9B7CF",
                                  const std::string &uncheckedBgColor = "FFFFFF50",
                                  const std::string &textColor = "6d6d6d",
                                  const std::string &checkedTextColor = "0e0e0e",
                                  float borderRadius = 12.0f) {
        ImGui::PushID(label);
        ImGuiWindow *window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return false;
        ImGuiID id = window->GetID(label);
        const float spacing = ImGui::GetStyle().ItemSpacing.x;
        float contentWidth = ImGui::GetContentRegionAvail().x;
        float cellWidth = (contentWidth - spacing * (itemsPerRow - 1)) / itemsPerRow;
        ImVec2 size = ImVec2(cellWidth, cellWidth);
        if (CheckboxButtonFlowLayoutItemIndex > 0 && CheckboxButtonFlowLayoutItemIndex % itemsPerRow != 0) {
            ImGui::SameLine();
        }
        CheckboxButtonFlowLayoutItemIndex++;
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImRect bb(pos, ImVec2(pos.x + size.x, pos.y + size.y));
        ImGui::ItemSize(bb);
        if (!ImGui::ItemAdd(bb, id))
            return false;
        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(bb, id, &hovered, &held);
        if (pressed) {
            *v = !*v;
            ImGui::MarkItemEdited(id);
        }
        ImU32 bgColor = HexToColor(*v ? checkedBgColor : uncheckedBgColor);
        ImU32 txtColor = HexToColor(*v ? checkedTextColor : textColor);
        // 背景
        ImGui::RenderFrame(bb.Min, bb.Max, bgColor, true, borderRadius);
        // 图标居中
        const float iconSize = size.x / 2;
        if (icon) {
            ImVec2 iconPos = ImVec2(pos.x + (size.x - iconSize) * 0.5f, pos.y + (size.y - iconSize) * 0.5f - 10);
            ImGui::GetWindowDrawList()->AddImage(icon, iconPos, ImVec2(iconPos.x + iconSize, iconPos.y + iconSize - 10));
        }
        // 文本居中但位于图标下 +10
        ImVec2 textSize = ImGui::CalcTextSize(label);
        ImVec2 textPos = ImVec2(
                pos.x + (size.x - textSize.x) * 0.5f,
                pos.y + size.y - textSize.y - 10
        );
        ImGui::GetWindowDrawList()->AddText(textPos, txtColor, label);
        return pressed;
    }

    //表单复选框
    bool CheckboxFrom(const char *label, bool *v, const std::string &width = "",
                      const std::string &backgroundColor = "",
                      const std::string &checkedBgColor = "F9B7CF",
                      const std::string &uncheckedBgColor = "f2f2f2",
                      const std::string &checkMarkColor = "FFFFFF",
                      const std::string &textColor = "FFFFFF", float borderRadius = 10.0f) {
        ImGuiWindow *window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return false;
        ImGui::PushID(label);
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 parentPadding = ImGui::GetStyle().WindowPadding;
        // 计算控件宽度
        float widthVal = 0;
        if (!width.empty()) {
            float parentWidth = ImGui::GetContentRegionAvail().x;
            widthVal = ConvertToFloat(width, parentWidth);
        } else {
            widthVal = ImGui::GetContentRegionAvail().x;
        }
        widthVal -= parentPadding.x;
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        ImGuiContext &g = *ImGui::GetCurrentContext();
        const ImGuiStyle &style = g.Style;
        const float square_sz = 70.0f;
        const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
        // 背景
        draw_list->AddRectFilled(pos, ImVec2(pos.x + widthVal, pos.y + square_sz),
                                 HexToColor(backgroundColor), 0);
        // 文本
        if (label_size.x > 0.0f) {
            ImVec2 text_pos = ImVec2(pos.x + 10, pos.y + 10);
            draw_list->AddText(text_pos, HexToColor(textColor.empty() ? "000000" : textColor), label);
        }
        // 复选框区域
        ImRect total_bb(ImVec2(pos.x + widthVal - square_sz, pos.y), ImVec2(pos.x + widthVal, pos.y + square_sz));
        ImGui::ItemSize(total_bb);                          // 声明控件占位尺寸
        ImGui::ItemAdd(total_bb, window->GetID(label));     // 注册控件
        bool hovered, held;
        bool pressed = ImGui::ButtonBehavior(total_bb, window->GetID(label), &hovered, &held);
        if (pressed) {
            *v = !*v;
            ImGui::MarkItemEdited(window->GetID(label));
        }
        // 绘制复选框
        draw_list->AddRectFilled(total_bb.Min, total_bb.Max, HexToColor(*v ? checkedBgColor : uncheckedBgColor), borderRadius);
        if (*v) {
            ImRect checkMark(ImVec2(total_bb.Min.x+15,total_bb.Min.y+15), ImVec2(total_bb.Max.x-15,total_bb.Max.y-15));
            draw_list->AddRectFilled(checkMark.Min, checkMark.Max, HexToColor(checkMarkColor.empty() ? "FFFFFF" : checkMarkColor), borderRadius);
        }
        // 关键步骤：推进光标，确保后续控件不会重叠，确保滚动区域正常
        ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + square_sz + style.ItemSpacing.y));

        ImGui::PopID();
        return pressed;
    }


    // 单选框
    bool RadioButton(const char *label, int *currentIndex, int index, float height = 60.0f,
                     const std::string &selectedColor = "F9B7CF99",
                     const std::string &unselectedColor = "777777",
                     const std::string &textColor = "393D49") {
        ImGui::PushID(label);
        float padding = 10.0f;
        height = height <= 0 ? 60 : height;
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        float radius = height * 0.5f;
        ImVec2 circleCenter = ImVec2(pos.x + radius + padding, pos.y + radius + padding);
        draw_list->AddCircle(circleCenter, radius, index == *currentIndex ? HexToColor(
                selectedColor.empty() ? "F9B7CF99" : selectedColor) : HexToColor(
                unselectedColor.empty() ? "777777" : unselectedColor), 0, 4.0f);
        if (index == *currentIndex)
            draw_list->AddCircleFilled(circleCenter, radius * 0.5f, HexToColor(
                    selectedColor.empty() ? "F9B7CF99" : selectedColor));
        ImGui::Dummy(ImVec2(height + padding, height + padding));
        ImVec2 textPos = ImVec2(pos.x + height + padding * 2,
                                pos.y + (height - ImGui::CalcTextSize(label).y) * 0.5f + padding);
        draw_list->AddText(textPos, HexToColor(textColor.empty() ? "393D49" : textColor), label);
        ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y));
        ImGui::InvisibleButton(label, ImVec2(height + padding + ImGui::CalcTextSize(label).x,
                                             height + padding * 2));
        bool hovered = ImGui::IsItemHovered();
        bool clicked = ImGui::IsItemClicked();
        if (hovered || clicked)
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        if (clicked)
            *currentIndex = index;
        ImGui::PopID();
        SameLineWithSpacing(10);
        return clicked;
    }

    //单选按钮组
    void RadioGroup(const char *label, int *selectedIndex, std::vector <std::string> options,
                    const std::string &width = "", const std::string &背景颜色 = "FFFFFF50",
                    const std::string &文本颜色 = "393D49",
                    const std::string &选中颜色 = "F9B7CF", float padding = 5.0f,
                    float fontSize = 1.0f, float borderRadius = 10.0f) {
        ImGui::SetWindowFontScale(fontSize);
        ImGui::PushID(label);
        const ImGuiStyle &style = ImGui::GetStyle();
        const ImVec2 startPos = ImGui::GetCursorScreenPos();
        const float buttonSpacing = style.ItemSpacing.x;
        const ImVec2 buttonStartPos = ImVec2(startPos.x + padding,
                                             startPos.y + padding + style.FramePadding.y);
        const ImU32 selectedColor = HexToColor(选中颜色.empty() ? "F9B7CF" : 选中颜色);
        const ImU32 unselectedTextColor = HexToColor(文本颜色);
        const ImU32 selectedTextColor = HexToColor("FAFAFA");
        float widthVal = 0;
        size_t numOptions = options.size();
        if (!width.empty()) {
            float parentWidth = ImGui::GetContentRegionAvail().x;
            widthVal = ConvertToFloat(width, parentWidth) - padding * 2;
        } else {
            widthVal = ImGui::GetContentRegionAvail().x - padding * 2;
        }
        const float buttonWidth = widthVal / numOptions;
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, borderRadius);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, style.ItemSpacing.y));
        float totalWidth = 0.0f;
        for (int i = 0; i < numOptions; ++i) {
            totalWidth += buttonWidth;
        }
        const ImVec2 bgSize(totalWidth + padding * 2.0f,
                            style.FramePadding.y * 2.0f + ImGui::GetTextLineHeight() +
                            padding * 2.0f);
        const ImVec2 bgStartPos = ImVec2(startPos.x, startPos.y);
        ImGui::GetWindowDrawList()->AddRectFilled(bgStartPos, ImVec2(bgStartPos.x + bgSize.x,
                                                                     bgStartPos.y + bgSize.y),
                                                  HexToColor(
                                                          背景颜色.empty() ? "23292e" : 背景颜色),
                                                  style.FrameRounding);
        ImGui::SetCursorScreenPos(ImVec2(bgStartPos.x + padding, bgStartPos.y + padding));
        for (int i = 0; i < numOptions; ++i) {
            if (i > 0) {
                SameLineWithSpacing();
            }
            if (*selectedIndex == i) {
                ImGui::PushStyleColor(ImGuiCol_Button, selectedColor);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, selectedColor);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, selectedColor);
                ImGui::PushStyleColor(ImGuiCol_Text, selectedTextColor);
            } else {
                ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0)); // 透明
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(0, 0, 0, 0)); // 透明
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(0, 0, 0, 0)); // 透明
                ImGui::PushStyleColor(ImGuiCol_Text, unselectedTextColor);
            }
            if (ImGui::Button(options[i].c_str(), ImVec2(buttonWidth, 0.0f))) {
                *selectedIndex = i;
            }
            ImGui::PopStyleColor(4);
        }
        ImGui::PopStyleVar(3);
        ImGui::PopID();
        ImGui::SetWindowFontScale(1.0f);
    }

    //表单单选按钮组
    void RadioGroupFrom(const char *label, int *selectedIndex, std::vector <std::string> options,
                        const std::string &width = "", const std::string &背景颜色 = "eeeeee",
                        const std::string &文本颜色 = "2f363c",
                        const std::string &选中颜色 = "ffffff", float padding = 5.0f,
                        float fontSize = 1.0f, float borderRadius = 10.0f) {
        ImGuiWindow *window = ImGui::GetCurrentWindow();
        ImGui::SetWindowFontScale(fontSize);
        ImGui::PushID(label);
        const ImGuiStyle &style = ImGui::GetStyle();
        ImVec2 startPos = window->DC.CursorPos;
        startPos = ImVec2(startPos.x,startPos.y-10);
        const float buttonSpacing = style.ItemSpacing.x;
        const ImU32 selectedColor = HexToColor(选中颜色);
        const ImU32 unselectedTextColor = HexToColor(文本颜色);
        const ImU32 selectedTextColor = HexToColor(文本颜色);
        const float square_sz = ImGui::GetFrameHeight();
        ImVec2 pos = startPos;
        // 获取父级内边距
        ImVec2 parentPadding = ImGui::GetStyle().WindowPadding;
        // 获取宽度并计算按钮宽度
        float widthVal = 0;
        size_t numOptions = options.size();
        if (!width.empty()) {
            float parentWidth = ImGui::GetContentRegionAvail().x;
            widthVal = ConvertToFloat(width, parentWidth) / 2 - padding * 2;
        } else {
            widthVal = ImGui::GetContentRegionAvail().x / 2 - padding * 2;
        }
        widthVal -= parentPadding.x;
        // 配置样式
        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, borderRadius);
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0.0f, style.ItemSpacing.y));
        ImVec2 bgStartPos = ImVec2(startPos.x, startPos.y);
        // 在左侧显示 label
        ImGui::SetCursorScreenPos(ImVec2(bgStartPos.x + padding, bgStartPos.y + padding * 4 + 10));
        ImGui::Text("%s", label);
        // 获取 label 宽度
        float labelWidth = ImGui::CalcTextSize(label).x;
        // 在label后面留出空间用于按钮
        ImVec2 optionStartPos = ImVec2(bgStartPos.x + padding + labelWidth + padding,
                                       bgStartPos.y + padding);
        const float buttonWidth = widthVal / numOptions;
        // 背景尺寸和位置
        float totalWidth = 0;
        for (int i = 0; i < numOptions; ++i) {
            totalWidth += buttonWidth;
        }
        // 计算背景最大可用宽度
        float availableWidth = ImGui::GetContentRegionAvail().x - labelWidth - padding * 2;
        if (totalWidth > availableWidth) {
            totalWidth = availableWidth;  // 限制背景宽度，防止超出窗口
        }
        // 背景尺寸
        ImVec2 bgSize(totalWidth + padding * 2.0f,
                      style.FramePadding.y * 2.0f + ImGui::GetTextLineHeight() + padding * 2.0f);
        ImGui::SameLine();  // 保持按钮之间的间距
        // 获取窗口的绝对屏幕坐标
        ImVec2 absolutePos = ImGui::GetCursorScreenPos();
        absolutePos.x += widthVal - labelWidth + padding;
        absolutePos.y -= padding * 2;
        // 绘制背景，确保背景的位置固定
        ImGui::GetWindowDrawList()->AddRectFilled(absolutePos, ImVec2(absolutePos.x + bgSize.x,
                                                                      absolutePos.y + bgSize.y),
                                                  HexToColor(背景颜色), borderRadius);
        absolutePos.x += padding;
        absolutePos.y += padding;
        ImGui::SetCursorScreenPos(absolutePos);
        // 在同一行显示选项
        for (int i = 0; i < numOptions; ++i) {
            if (i > 0) {
                ImGui::SameLine();  // 保持按钮之间的间距
            }

            // 设置选中的样式
            if (*selectedIndex == i) {
                ImGui::PushStyleColor(ImGuiCol_Button, selectedColor);
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, selectedColor);
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, selectedColor);
                ImGui::PushStyleColor(ImGuiCol_Text, selectedTextColor);
            } else {
                // 未选中的样式
                ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(0, 0, 0, 0)); // 透明
                ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(0, 0, 0, 0)); // 透明
                ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(0, 0, 0, 0)); // 透明
                ImGui::PushStyleColor(ImGuiCol_Text, unselectedTextColor);
            }

            // 绘制按钮并响应点击事件
            if (ImGui::Button(options[i].c_str(), ImVec2(buttonWidth, 0.0f))) {
                *selectedIndex = i;
            }

            ImGui::PopStyleColor(4); // 恢复样式
        }
        const ImRect total_bb(ImVec2(pos.x - square_sz, pos.y), ImVec2(pos.x, bgSize.y));
        ImGui::ItemSize(total_bb, style.FramePadding.y);
        ImGui::ItemAdd(total_bb, 0);
        // 关键步骤：推进光标，确保后续控件不会重叠，确保滚动区域正常
        ImGui::SetCursorScreenPos(ImVec2(pos.x, pos.y + bgSize.y + 30));
        ImGui::PopStyleVar(3);
        ImGui::PopID();
        ImGui::SetWindowFontScale(1.0f);
    }

    //滑块
    bool Slider(const char *label, float *value, float minValue = 0.0f, float maxValue = 10.0f,
                const std::string &progressColor = "F9B7CF",
                const std::string &progressBgColor = "eeeeee",
                const std::string &width = "", bool showValue = false) {
        ImGui::PushID(label);
        float height = 40.0f;
        float padding = 10;
        float lineRadius = 50;
        // 获取窗口宽度，若未指定宽度则使用默认宽度
        float widthVal = 0;
        if (!width.empty()) {
            float parentWidth = ImGui::GetContentRegionAvail().x;
            widthVal = ConvertToFloat(width, parentWidth) - padding;
        } else {
            widthVal = ImGui::GetContentRegionAvail().x - padding;
        }
        // 获取当前光标位置
        ImVec2 pos = ImGui::GetCursorScreenPos();
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        float radius = height * 0.5f;
        // 计算滑块的位置
        float normalizedValue = (*value - minValue) / (maxValue - minValue);
        float sliderPos = normalizedValue * widthVal;
        ImVec2 sliderStart = ImVec2(pos.x + padding, pos.y);
        ImVec2 sliderEnd = ImVec2(pos.x + widthVal, pos.y + height * 0.25);
        // 绘制背景
        draw_list->AddRectFilled(sliderStart, sliderEnd, HexToColor(progressBgColor), lineRadius);
        // 绘制进度条
        float progressEndX = pos.x + padding + sliderPos;
        draw_list->AddRectFilled(sliderStart, ImVec2(progressEndX, sliderStart.y + height * 0.25),
                                 HexToColor(progressColor), lineRadius);
        // 如果显示当前值，绘制文本
        if (showValue) {
            ImVec2 circleCenter = ImVec2(pos.x + sliderPos, pos.y + height * 0.2);
            char valueStr[32];
            snprintf(valueStr, sizeof(valueStr), "%.2f", *value);
            ImVec2 textSize = ImGui::CalcTextSize(valueStr);
            ImVec2 textBgSize = ImVec2(circleCenter.x - textSize.x * 0.5f, circleCenter.y - radius);
            draw_list->AddRectFilled(ImVec2(textBgSize.x - padding, textBgSize.y - 5),
                                     ImVec2(textBgSize.x + padding + textSize.x,
                                            textBgSize.y + 5 + textSize.y),
                                     HexToColor(progressColor), 5);
            draw_list->AddRectFilled(ImVec2(textBgSize.x - 5, textBgSize.y),
                                     ImVec2(textBgSize.x + 5 + textSize.x,
                                            textBgSize.y + textSize.y), HexToColor("FFFFFF"), 5);
            draw_list->AddText(textBgSize, HexToColor("393D49"), valueStr);
        } else {
            ImVec2 circleCenter = ImVec2(pos.x + sliderPos, pos.y + height * 0.15);
            // 绘制圆形滑块
            draw_list->AddCircleFilled(circleCenter, radius, HexToColor(progressColor));
            draw_list->AddCircleFilled(circleCenter, radius - 5, HexToColor("FFFFFF"));
        }
        // 处理滑块逻辑
        ImGui::InvisibleButton(label, ImVec2(widthVal, height));
        bool hovered = ImGui::IsItemHovered();
        bool active = ImGui::IsItemActive();
        if (hovered || active) {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }
        if (active) {
            float mouseX = ImGui::GetIO().MousePos.x;
            float newValue = minValue + (mouseX - sliderStart.x) / widthVal * (maxValue - minValue);
            *value = newValue < minValue ? minValue : (newValue > maxValue ? maxValue : newValue);
        }
        ImGui::PopID();
        return active;
    }

    //表单滑块
    bool SliderFrom(const char *label, float *value, float minValue = 0.0f, float maxValue = 10.0f,
                    bool rect = false, float addValue = 1.0f,
                    const std::string &backgroundColor = "",
                    const std::string &progressColor = "F9B7CF",
                    const std::string &progressBgColor = "eeeeee",
                    const std::string &textColor = "2f363c",
                    const std::string &buttonTextColor = "ffffff",
                    const std::string &width = "",const char *snprintfm = "%.0f") {
        ImGui::PushID(label);
        float height = 40.0f;
        float padding = 10;
        float lineRadius = 50;
        bool buttonActive = false;

        // 获取父级内边距
        ImVec2 parentPadding = ImGui::GetStyle().WindowPadding;
        // 获取窗口宽度，若未指定宽度则使用默认宽度
        float widthVal = 0;
        char valueStr[32];
        snprintf(valueStr, sizeof(valueStr), snprintfm, *value);
        if (!width.empty()) {
            float parentWidth = ImGui::GetContentRegionAvail().x;
            widthVal = ConvertToFloat(width, parentWidth) - padding;
        } else {
            widthVal = ImGui::GetContentRegionAvail().x;
        }

        widthVal -= parentPadding.x;

        ImGuiContext &g = *ImGui::GetCurrentContext();
        // 获取当前光标位置
        ImVec2 pos = ImGui::GetCursorScreenPos();
        pos.y+=padding;
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        draw_list->AddRectFilled(pos, ImVec2(pos.x + widthVal, pos.y + height),
                                 HexToColor(backgroundColor), 0);

        ImVec2 text_pos = ImVec2(pos.x + padding, pos.y);
        ImGui::GetWindowDrawList()->AddText(text_pos,
                                            HexToColor(textColor.empty() ? "000000" : textColor),
                                            label);

        // 计算文本宽度
        ImGui::PushStyleColor(ImGuiCol_Button, HexToColor(progressColor));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexToColor(progressColor));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexToColor(progressColor));
        ImGui::PushStyleColor(ImGuiCol_Text, HexToColor(buttonTextColor));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10);

        // 设定默认最小文本宽度
        float defaultTextWidth = 70;
        // 计算文本宽度，默认最小150
        ImVec2 textSizeVec = ImGui::CalcTextSize(valueStr);
        float textWidth = textSizeVec.x < defaultTextWidth ? defaultTextWidth : textSizeVec.x;
        float btnWidth = 50; // 按钮宽度
        float totalWidth = btnWidth * 2 + textWidth + padding; // - 按钮 + 数字 + + 按钮 + 内边距
        // 计算右对齐的起始点
        float rightAlignX = pos.x + widthVal - totalWidth;
        // - 按钮
        ImGui::SetCursorScreenPos(ImVec2(rightAlignX, text_pos.y - padding));
        ImGui::SetWindowFontScale(1.2f);
        if (ImGui::Button("-", ImVec2(btnWidth, 50))) {
            *value -= addValue;
            if (*value < minValue) *value = minValue;
            buttonActive = true;
        }
        ImGui::SetWindowFontScale(1.0f);
        // **计算数值文本的真正居中位置**
        float valueTextX = rightAlignX + btnWidth + (textWidth - textSizeVec.x) * 0.5f;
        ImVec2 valueTextPos = ImVec2(valueTextX, text_pos.y);
        ImGui::GetWindowDrawList()->AddText(valueTextPos, HexToColor(textColor.empty() ? "000000" : textColor), valueStr);
        // + 按钮
        ImGui::SetCursorScreenPos(ImVec2(rightAlignX + btnWidth + textWidth + padding, text_pos.y - padding));
        ImGui::SetWindowFontScale(1.2f);
        if (ImGui::Button("+", ImVec2(btnWidth, 50))) {
            *value += addValue;
            if (*value > maxValue) *value = maxValue;
            buttonActive = true;
        }
        ImGui::SetWindowFontScale(1.0f);

        pos = ImVec2(pos.x, pos.y + textSizeVec.y + 10 + padding);
        float radius = height * 0.5f;

        // 计算滑块的位置
        float normalizedValue = (*value - minValue) / (maxValue - minValue);
        float sliderPos = normalizedValue * widthVal;
        ImVec2 sliderStart = ImVec2(pos.x + padding, pos.y);
        ImVec2 sliderEnd = ImVec2(pos.x + widthVal, pos.y + height * 0.25);

        // 绘制背景
        draw_list->AddRectFilled(sliderStart, sliderEnd, HexToColor(progressBgColor), lineRadius);

        // 绘制进度条
        float progressEndX = pos.x + padding + sliderPos;
        draw_list->AddRectFilled(sliderStart, ImVec2(progressEndX, sliderStart.y + height * 0.25),
                                 HexToColor(progressColor), lineRadius);

        // 显示方型
        if (rect) {
            float rectHeight = height * 0.7;
            ImVec2 circleCenter = ImVec2(pos.x + sliderPos - padding, pos.y + rectHeight * 0.4);
            ImVec2 textBgSize = ImVec2(circleCenter.x, circleCenter.y - radius);
            draw_list->AddRectFilled(ImVec2(textBgSize.x - padding, textBgSize.y - 5),
                                     ImVec2(textBgSize.x + padding + rectHeight / 2,
                                            textBgSize.y + 5 + rectHeight),
                                     HexToColor(progressColor), 3);
            draw_list->AddRectFilled(ImVec2(textBgSize.x - 5, textBgSize.y),
                                     ImVec2(textBgSize.x + 5 + rectHeight / 2,
                                            textBgSize.y + rectHeight), HexToColor("FFFFFF"), 3);
        } else {
            ImVec2 circleCenter = ImVec2(pos.x + sliderPos, pos.y + height * 0.15);
            // 绘制圆形滑块
            draw_list->AddCircleFilled(circleCenter, radius, HexToColor(progressColor));
            draw_list->AddCircleFilled(circleCenter, radius - 5, HexToColor("FFFFFF"));
        }

        const ImRect total_bb(pos, ImVec2(pos.x + widthVal, pos.y));
        ImGui::ItemSize(total_bb, 0);
        ImGui::ItemAdd(total_bb, 0);

        // 处理滑块逻辑
        ImGui::InvisibleButton(label, ImVec2(widthVal, height));
        bool hovered = ImGui::IsItemHovered();
        bool active = ImGui::IsItemActive();
        if (hovered || active) {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }

        if (active) {
            float mouseX = ImGui::GetIO().MousePos.x;
            float newValue = minValue + (mouseX - sliderStart.x) / widthVal * (maxValue - minValue);
            *value = newValue < minValue ? minValue : (newValue > maxValue ? maxValue : newValue);
        }

        ImGui::PopID();
        return buttonActive?buttonActive:active;
    }

    //带图标按钮的滑块
    bool SliderImageButtonFrom(const char *label, float *value, float minValue = 0.0f, float maxValue = 10.0f,
                    ImTextureID icon1 = J1.DS,ImTextureID icon2 = J2.DS,
                    bool rect = false, float addValue = 1.0f,
                    const std::string &backgroundColor = "",
                    const std::string &progressColor = "F9B7CF",
                    const std::string &progressBgColor = "eeeeee",
                    const std::string &textColor = "2f363c",
                    const std::string &buttonTextColor = "ffffff",
                    const std::string &width = "",const char *snprintfm = "%.0f") {
        ImGui::PushID(label);
        float height = 40.0f;
        float padding = 10;
        float lineRadius = 50;
        bool buttonActive = false;

        // 获取父级内边距
        ImVec2 parentPadding = ImGui::GetStyle().WindowPadding;
        // 获取窗口宽度，若未指定宽度则使用默认宽度
        float widthVal = 0;
        char valueStr[32];
        snprintf(valueStr, sizeof(valueStr), snprintfm, *value);
        if (!width.empty()) {
            float parentWidth = ImGui::GetContentRegionAvail().x;
            widthVal = ConvertToFloat(width, parentWidth) - padding;
        } else {
            widthVal = ImGui::GetContentRegionAvail().x;
        }

        widthVal -= parentPadding.x;

        ImGuiContext &g = *ImGui::GetCurrentContext();
        // 获取当前光标位置
        ImVec2 pos = ImGui::GetCursorScreenPos();
        pos.y+=padding;
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        draw_list->AddRectFilled(pos, ImVec2(pos.x + widthVal, pos.y + height),
                                 HexToColor(backgroundColor), 0);

        ImVec2 text_pos = ImVec2(pos.x + padding, pos.y);
        ImGui::GetWindowDrawList()->AddText(text_pos,
                                            HexToColor(textColor.empty() ? "000000" : textColor),
                                            label);

        // 计算文本宽度
        ImGui::PushStyleColor(ImGuiCol_Button, HexToColor("00000000"));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, HexToColor("00000000"));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, HexToColor("00000000"));
        ImGui::PushStyleColor(ImGuiCol_Text, HexToColor(buttonTextColor));
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 10);

        // 设定默认最小文本宽度
        float defaultTextWidth = 70;
        // 计算文本宽度，默认最小150
        ImVec2 textSizeVec = ImGui::CalcTextSize(valueStr);
        float textWidth = textSizeVec.x < defaultTextWidth ? defaultTextWidth : textSizeVec.x;
        float btnWidth = 70; // 按钮宽度
        float totalWidth = btnWidth * 2 + textWidth + padding; // - 按钮 + 数字 + + 按钮 + 内边距
        // 计算右对齐的起始点
        float rightAlignX = pos.x + widthVal - totalWidth;
        // - 按钮
        ImGui::SetCursorScreenPos(ImVec2(rightAlignX, text_pos.y - padding));
        if (ImGui::ImageButton("j1",icon1, ImVec2(btnWidth, btnWidth))) {
            *value -= addValue;
            if (*value < minValue) *value = minValue;
            buttonActive = true;
        }
        // **计算数值文本的真正居中位置**
        float valueTextX = rightAlignX + btnWidth + (textWidth - textSizeVec.x) * 0.5f;
        ImVec2 valueTextPos = ImVec2(valueTextX, text_pos.y);
        ImGui::GetWindowDrawList()->AddText(valueTextPos, HexToColor(textColor.empty() ? "000000" : textColor), valueStr);
        // + 按钮
        ImGui::SetCursorScreenPos(ImVec2(rightAlignX + btnWidth + textWidth + padding, text_pos.y - padding));
        if (ImGui::ImageButton("j2",icon2, ImVec2(btnWidth, btnWidth))) {
            *value += addValue;
            if (*value > maxValue) *value = maxValue;
            buttonActive = true;
        }
        ImGui::SetWindowFontScale(1.0f);

        pos = ImVec2(pos.x, pos.y + textSizeVec.y + 10 + padding);
        float radius = height * 0.5f;

        // 计算滑块的位置
        float normalizedValue = (*value - minValue) / (maxValue - minValue);
        float sliderPos = normalizedValue * widthVal;
        ImVec2 sliderStart = ImVec2(pos.x + padding, pos.y);
        ImVec2 sliderEnd = ImVec2(pos.x + widthVal, pos.y + height * 0.25);

        // 绘制背景
        draw_list->AddRectFilled(sliderStart, sliderEnd, HexToColor(progressBgColor), lineRadius);

        // 绘制进度条
        float progressEndX = pos.x + padding + sliderPos;
        draw_list->AddRectFilled(sliderStart, ImVec2(progressEndX, sliderStart.y + height * 0.25),
                                 HexToColor(progressColor), lineRadius);

        // 显示方型
        if (rect) {
            float rectHeight = height * 0.7;
            ImVec2 circleCenter = ImVec2(pos.x + sliderPos - padding, pos.y + rectHeight * 0.4);
            ImVec2 textBgSize = ImVec2(circleCenter.x, circleCenter.y - radius);
            draw_list->AddRectFilled(ImVec2(textBgSize.x - padding, textBgSize.y - 5),
                                     ImVec2(textBgSize.x + padding + rectHeight / 2,
                                            textBgSize.y + 5 + rectHeight),
                                     HexToColor(progressColor), 3);
            draw_list->AddRectFilled(ImVec2(textBgSize.x - 5, textBgSize.y),
                                     ImVec2(textBgSize.x + 5 + rectHeight / 2,
                                            textBgSize.y + rectHeight), HexToColor("FFFFFF"), 3);
        } else {
            ImVec2 circleCenter = ImVec2(pos.x + sliderPos, pos.y + height * 0.15);
            // 绘制圆形滑块
            draw_list->AddCircleFilled(circleCenter, radius, HexToColor(progressColor));
            draw_list->AddCircleFilled(circleCenter, radius - 5, HexToColor("FFFFFF"));
        }

        const ImRect total_bb(pos, ImVec2(pos.x + widthVal, pos.y));
        ImGui::ItemSize(total_bb, 0);
        ImGui::ItemAdd(total_bb, 0);

        // 处理滑块逻辑
        ImGui::InvisibleButton(label, ImVec2(widthVal, height));
        bool hovered = ImGui::IsItemHovered();
        bool active = ImGui::IsItemActive();
        if (hovered || active) {
            ImGui::SetMouseCursor(ImGuiMouseCursor_Hand);
        }

        if (active) {
            float mouseX = ImGui::GetIO().MousePos.x;
            float newValue = minValue + (mouseX - sliderStart.x) / widthVal * (maxValue - minValue);
            *value = newValue < minValue ? minValue : (newValue > maxValue ? maxValue : newValue);
        }

        ImGui::PopID();
        return buttonActive?buttonActive:active;
    }

    //选项卡
    bool Tabs(const std::vector <std::string> &tabs, int *currentIndex, const std::string &height = "",
         const std::string &width = "",
         const std::string &bgColor = "", const std::string &textColor = "000000",
         const std::string &activeColor = "F9B7CF99",
         float padding = 10.0f, bool selectShowTop = false, float textSize = 30.0f,
         float radius = 5) {
        bool valueChanged = false;
        // 计算高度
        float heightVal = 0;
        if (!height.empty()) {
            float parentHeight = ImGui::GetContentRegionAvail().y;
            heightVal = ConvertToFloat(height, parentHeight);
        } else {
            heightVal = ImGui::GetContentRegionAvail().y;
        }
        // 计算宽度度
        float widthVal = 0;
        if (!width.empty()) {
            float parentWidth = ImGui::GetContentRegionAvail().x;
            widthVal = ConvertToFloat(width, parentWidth);
        } else {
            widthVal = ImGui::GetContentRegionAvail().x;
        }
        ImVec2 itemSpacing = ImGui::GetStyle().ItemSpacing;
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, itemSpacing.y));
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
        ImVec2 canvas_size = ImGui::GetContentRegionAvail();
        float tabWidth = (widthVal == 0.0f) ? (canvas_size.x - padding * 2) / tabs.size() :
                         widthVal / tabs.size();
        float tabHeight = (heightVal == 0.0f) ? 0.0f : heightVal;
        canvas_pos = ImVec2(canvas_pos.x, canvas_pos.y);
        for (int i = 0; i < tabs.size(); ++i) {
            ImVec2 tab_pos = ImVec2(canvas_pos.x + i * tabWidth, canvas_pos.y);
            ImVec2 tab_size = ImVec2(tabWidth, tabHeight);
            // 计算自适应高度
            if (heightVal == 0.0f) {
                ImVec2 textSize = ImGui::CalcTextSize(tabs[i].c_str());
                tabHeight = textSize.y + padding * 2;
                tab_size.y = tabHeight;
            }
            // 绘制背景
            if (i == 0) {
                draw_list->AddRectFilled(tab_pos,
                                         ImVec2(tab_pos.x + tabWidth, tab_pos.y + tabHeight),
                                         HexToColor(bgColor), radius, ImDrawFlags_RoundCornersLeft);
            } else if (i == tabs.size() - 1) {
                draw_list->AddRectFilled(tab_pos,
                                         ImVec2(tab_pos.x + tabWidth, tab_pos.y + tabHeight),
                                         HexToColor(bgColor), radius,
                                         ImDrawFlags_RoundCornersRight);
            } else {
                draw_list->AddRectFilled(tab_pos,
                                         ImVec2(tab_pos.x + tabWidth, tab_pos.y + tabHeight),
                                         HexToColor(bgColor));
            }
            // 文本颜色
            ImVec2 textSizeVec = ImGui::CalcTextSize(tabs[i].c_str(), 0, textSize);
            ImVec2 textPos = ImVec2(tab_pos.x + (tabWidth - textSizeVec.x * 0.5f) * 0.5f,
                                    tab_pos.y + (tabHeight - textSizeVec.y) * 0.5f + 10);
            draw_list->AddText(NULL, textSize, textPos, HexToColor(textColor), tabs[i].c_str());
            // 选中标记颜色
            if (i == *currentIndex) {
                if (selectShowTop) {
                    draw_list->AddLine(ImVec2(tab_pos.x, tab_pos.y),
                                       ImVec2(tab_pos.x + tabWidth, tab_pos.y),
                                       HexToColor(activeColor), 7.0f);
                } else {
                    draw_list->AddLine(ImVec2(tab_pos.x, tab_pos.y + tabHeight),
                                       ImVec2(tab_pos.x + tabWidth, tab_pos.y + tabHeight),
                                       HexToColor(activeColor), 7.0f);
                }
            }
            // 处理点击事件
            ImGui::SetCursorScreenPos(tab_pos);
            ImGui::InvisibleButton(tabs[i].c_str(), tab_size);
            if (ImGui::IsItemClicked()) {
                *currentIndex = i;
                valueChanged = true;
            }
        }
        ImGui::PopStyleVar();
        return valueChanged;
    }

    //垂直选项卡
    bool VerticalTabs(const std::vector <std::string> &tabs, int *currentIndex,
                      const std::string &height = "", const std::string &width = "",
                      const std::string &bgColor = "", const std::string &textColor = "FAFAFA",
                      const std::string &activeColor = "F9B7CF",
                      float padding = 10.0f, bool centerVertically = true, float textSize = 50.0f,
                      TextAlign textAlign = TextAlign::Center) {
        bool valueChanged = false;
        float radius = 5;
        // 计算高度
        float heightVal = 0;
        if (!height.empty()) {
            float parentHeight = ImGui::GetContentRegionAvail().y;
            heightVal = ConvertToFloat(height, parentHeight);
        } else {
            heightVal = ImGui::GetContentRegionAvail().y;
        }
        // 计算宽度度
        float widthVal = 0;
        if (!width.empty()) {
            float parentWidth = ImGui::GetContentRegionAvail().x;
            widthVal = ConvertToFloat(width, parentWidth);
        } else {
            widthVal = ImGui::GetContentRegionAvail().x;
        }
        ImVec2 itemSpacing = ImGui::GetStyle().ItemSpacing;
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, itemSpacing.y));
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
        ImVec2 canvas_size = ImGui::GetContentRegionAvail();
        // 获取控件宽度和高度
        float tabWidth = (widthVal == 0.0f) ? canvas_size.x - padding * 2 : widthVal;
        float tabHeight = (heightVal == 0.0f) ? 0.0f : heightVal;
        // 计算所有选项卡的总高度
        float totalHeight = 0.0f;
        for (int i = 0; i < tabs.size(); ++i) {
            ImVec2 textSizeVec = ImGui::CalcTextSize(tabs[i].c_str(), 0, textSize);
            totalHeight += (tabHeight == 0.0f) ? (textSizeVec.y + padding * 2) : tabHeight;
        }
        // 如果垂直居中，调整起始位置
        if (centerVertically && totalHeight < canvas_size.y) {
            canvas_pos.y += (canvas_size.y - totalHeight) * 0.5f;
        }
        for (int i = 0; i < tabs.size(); ++i) {
            ImVec2 tab_pos = ImVec2(canvas_pos.x + padding, canvas_pos.y + i *
                                                                           (tabHeight == 0.0f ? (
                                                                                   ImGui::CalcTextSize(
                                                                                           tabs[i].c_str(),
                                                                                           NULL,
                                                                                           true,
                                                                                           textSize).y +
                                                                                   padding * 2)
                                                                                              : tabHeight));
            ImVec2 tab_size = ImVec2(tabWidth, tabHeight);
            // 绘制背景
            draw_list->AddRectFilled(tab_pos, ImVec2(tab_pos.x + tabWidth, tab_pos.y + tabHeight),
                                     HexToColor(bgColor));
            // 文本颜色
            ImVec2 textSizeVec = ImGui::CalcTextSize(tabs[i].c_str(), 0, textSize);
            ImVec2 textPos;
            MarkerPosition markerPosition = MarkerPosition::Right;
            switch (textAlign) {
                case TextAlign::Left:
                    textPos = ImVec2(tab_pos.x + padding,
                                     tab_pos.y + tabHeight * 0.5f - textSizeVec.y * 0.5);
                    markerPosition = MarkerPosition::Left;
                    break;
                case TextAlign::Right:
                    textPos = ImVec2(tab_pos.x + tabWidth - textSizeVec.x - padding,
                                     tab_pos.y + tabHeight * 0.5f - textSizeVec.y * 0.5);
                    break;
                case TextAlign::Center:
                default:
                    textPos = ImVec2(tab_pos.x + (tabWidth - textSizeVec.x + padding) * 0.5f,
                                     tab_pos.y + tabHeight * 0.5f - textSizeVec.y * 0.5);
                    break;
            }
            // 绘制选中标记及渐变背景
            if (i == *currentIndex) {
                ImVec2 marker_start, marker_end;
                ImVec2 gradient_start = ImVec2(tab_pos.x + tabWidth, tab_pos.y);
                ImVec2 gradient_end = ImVec2(tab_pos.x, tab_pos.y + tabHeight);
                if (markerPosition == MarkerPosition::Right) {
                    marker_start = ImVec2(tab_pos.x + tabWidth, tab_pos.y - 5);
                    marker_end = ImVec2(tab_pos.x + tabWidth, tab_pos.y + tabHeight + 5);
                    draw_list->AddRectFilledMultiColor(gradient_start, gradient_end,
                                                       HexToColor(activeColor),
                                                       HexToColor(activeColor) & 0x00FFFFFF,
                                                       HexToColor(activeColor) & 0x00FFFFFF,
                                                       HexToColor(activeColor));
                } else {
                    marker_start = ImVec2(tab_pos.x, tab_pos.y - 5);
                    marker_end = ImVec2(tab_pos.x, tab_pos.y + tabHeight + 5);
                    draw_list->AddRectFilledMultiColor(gradient_start, gradient_end,
                                                       HexToColor(activeColor) & 0x00FFFFFF,
                                                       HexToColor(activeColor),
                                                       HexToColor(activeColor),
                                                       HexToColor(activeColor) & 0x00FFFFFF);
                }
                draw_list->AddLine(marker_start, marker_end, HexToColor(activeColor), 5.0f);
            }
            draw_list->AddText(NULL, textSize, textPos, HexToColor(textColor), tabs[i].c_str());
            // 处理点击事件
            ImGui::SetCursorScreenPos(tab_pos);
            ImGui::InvisibleButton(tabs[i].c_str(), tab_size);
            if (ImGui::IsItemClicked()) {
                *currentIndex = i;
                valueChanged = true;
            }
        }
        ImGui::PopStyleVar();
        return valueChanged;
    }

    //图标垂直选项卡
    bool IconVerticalTabs(const std::vector <std::string> &tabs, int *currentIndex,
                      const std::string &height = "0", const std::string &width = "",
                      const std::string &bgColor = "", const std::string &textColor = "6d6d6d",
                      const std::string &activeColor = "F9B7CF",
                      float padding = 10.0f, bool centerVertically = false, float textSize = 50.0f) {
        bool valueChanged = false;
        float radius = 5;
        // 计算高度
        float heightVal = 0;
        if (!height.empty()) {
            float parentHeight = ImGui::GetContentRegionAvail().y;
            heightVal = ConvertToFloat(height, parentHeight);
        } else {
            heightVal = ImGui::GetContentRegionAvail().y;
        }
        // 计算宽度度
        // 获取父级内边距
        ImVec2 parentPadding = ImGui::GetStyle().WindowPadding;
        float widthVal = 0;
        if (!width.empty()) {
            float parentWidth = ImGui::GetContentRegionAvail().x;
            widthVal = ConvertToFloat(width, parentWidth);
        } else {
            widthVal = ImGui::GetContentRegionAvail().x;
        }
        widthVal -= parentPadding.x;
        ImVec2 itemSpacing = ImGui::GetStyle().ItemSpacing;
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(1, itemSpacing.y));
        ImDrawList *draw_list = ImGui::GetWindowDrawList();
        ImVec2 canvas_pos = ImGui::GetCursorScreenPos();
        ImVec2 canvas_size = ImGui::GetContentRegionAvail();
        // 获取控件宽度和高度
        float tabWidth = (widthVal == 0.0f) ? canvas_size.x - padding * 2 : widthVal;
        float tabHeight = (heightVal == 0.0f) ? 0.0f : heightVal;
        // 计算所有选项卡的总高度
        float totalHeight = 0.0f;
        for (int i = 0; i < tabs.size(); ++i) {
            ImVec2 textSizeVec = ImGui::CalcTextSize(tabs[i].c_str(), 0, textSize);
            totalHeight += (tabHeight == 0.0f) ? (textSizeVec.y + padding * 2) : tabHeight;
        }
        // 如果垂直居中，调整起始位置
        if (centerVertically && totalHeight < canvas_size.y) {
            canvas_pos.y += (canvas_size.y - totalHeight) * 0.5f;
        }
        for (int i = 0; i < tabs.size(); ++i) {
            ImVec2 tab_pos = ImVec2(canvas_pos.x + padding, canvas_pos.y + i *
                                                                           (tabHeight == 0.0f ? (
                                                                                   ImGui::CalcTextSize(
                                                                                           tabs[i].c_str(),
                                                                                           NULL, true,
                                                                                           textSize).y +
                                                                                   padding * 2) : tabHeight));
            ImVec2 tab_size = ImVec2(tabWidth, tabHeight);
            // 绘制背景
            draw_list->AddRectFilled(tab_pos, ImVec2(tab_pos.x + tabWidth, tab_pos.y + tabHeight), HexToColor(bgColor));
            // 文本颜色
            ImVec2 textSizeVec = ImGui::CalcTextSize(tabs[i].c_str(), 0, textSize);
            ImVec2 textPos;
            textPos = ImVec2(tab_pos.x + tabWidth * 0.5f - textSizeVec.x * 0.5 - padding, tab_pos.y + tabHeight * 0.5f - textSizeVec.y * 0.5);
            draw_list->AddText(NULL, textSize, textPos, HexToColor(i == *currentIndex?activeColor:textColor), tabs[i].c_str());
            // 处理点击事件
            ImGui::SetCursorScreenPos(tab_pos);
            ImGui::InvisibleButton(tabs[i].c_str(), tab_size);
            if (ImGui::IsItemClicked()) {
                *currentIndex = i;
                valueChanged = true;
            }
        }
        ImGui::PopStyleVar();
        return valueChanged;
    }

    //开关
    static std::vector <ToggleButtonState> toggleButtonStates;

    bool ToggleButton(const char *label, bool *pToggle, float width = 90,
                      const std::string &selectBgColor = "F9B7CF99",
                      const std::string &unSelectBgColor = "eeeeee", float borderThickness = 5.0f) {
        static std::map <std::string, ToggleButtonState> toggleButtonStates;
        if (toggleButtonStates.find(label) == toggleButtonStates.end()) {
            toggleButtonStates[label] = {0.0f};
        }
        ToggleButtonState &toggleButtonState = toggleButtonStates[label];
        const float Multiplier = 1.05f;
        const float CornerRadius = 50.0f;
        ImGuiWindow *window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return false;
        ImGuiContext &g = *GImGui;
        const ImGuiStyle &style = g.Style;
        ImVec2 pos = window->DC.CursorPos;
        pos = ImVec2(pos.x + 10, pos.y);
        const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);
        const float height = label_size.y / Multiplier + style.FramePadding.y * Multiplier;
        const float adjustedWidth = width * Multiplier;
        const float adjustedHeight = width / 2;
        const float knobWidth = adjustedWidth / 2.0f;
        const ImGuiID id = window->GetID(label);
        const ImU32 shadowColor = IM_COL32(0, 0, 0, 64);
        const ImVec2 shadowOffset(3.0f, 3.0f);
        const ImVec2 shadowStartPos = ImVec2(pos.x + shadowOffset.x, pos.y + shadowOffset.y);
        const ImVec2 shadowEndPos = ImVec2(shadowStartPos.x + adjustedWidth,
                                           shadowStartPos.y + adjustedHeight);
        ImGui::GetWindowDrawList()->AddRectFilled(shadowStartPos, shadowEndPos, shadowColor,
                                                  CornerRadius);
        const ImVec2 borderStartPos(pos.x - borderThickness, pos.y - borderThickness);
        const ImVec2 borderEndPos(pos.x + adjustedWidth + borderThickness,
                                  pos.y + adjustedHeight + borderThickness);
        const ImU32 borderColor = IM_COL32(250, 250, 250, 255);
        window->DrawList->AddRect(borderStartPos, borderEndPos, borderColor, CornerRadius, 15,
                                  borderThickness);
        const ImVec2 text_pos(pos.x + style.FramePadding.x, pos.y + style.FramePadding.y);
        ImGui::InvisibleButton(label, ImVec2(adjustedWidth, adjustedHeight));
        if (ImGui::IsItemClicked())
            *pToggle = !*pToggle;
        ImU32 bg_color = ImGui::GetColorU32(
                *pToggle ? HexToColor(selectBgColor) : HexToColor(unSelectBgColor));
        window->DrawList->AddRectFilled(ImVec2(pos.x, pos.y),
                                        ImVec2(pos.x + adjustedWidth, pos.y + adjustedHeight),
                                        bg_color, CornerRadius);
        float t = 0.1;
        if (*pToggle && 1 > toggleButtonState.t) {
            toggleButtonState.t += t;
        } else if (!*pToggle && t < toggleButtonState.t) {
            toggleButtonState.t -= t;
        }
        ImVec2 knob_pos = ImVec2(pos.x + toggleButtonState.t * (adjustedWidth - knobWidth), pos.y);
        window->DrawList->AddRectFilled(knob_pos,
                                        ImVec2(knob_pos.x + knobWidth, knob_pos.y + adjustedHeight),
                                        HexToColor("FFFFFF"), CornerRadius);
        SameLineWithSpacing(10);
        return ImGui::IsItemActive();
    }

    // 颜色选择器
    void ColorPicker(const char *label, ImVec4 &selectedColor, int &colorIndex, ImVec2 lineSize = ImVec2(0, 0)) {
        if (lineSize.y == 0)
            lineSize.y = 50.0f;
        if (lineSize.x == 0)
            lineSize.x = ImGui::GetContentRegionAvail().x;
        int numColors = colorList.size() - 1;
        ImGuiWindow *window = ImGui::GetCurrentWindow();
        if (window->SkipItems)
            return;
        ImGuiContext &g = *GImGui;
        const ImGuiStyle &style = g.Style;
        const float colorWidth = lineSize.x / numColors;
        const float colorHeight = lineSize.y;
        const float spacing = 0.0f;
        ImVec2 startPos = window->DC.CursorPos;
        ImVec2 colorBarPos = ImVec2(startPos.x, startPos.y);
        ImVec2 selectedColorBoxMin;
        ImVec2 selectedColorBoxMax;
        ImVec4 selectColor;
        float radius = 5;
        float selectSize = 10;
        for (int i = 0; i < numColors; ++i) {
            ImVec2 colorBoxMin = ImVec2(startPos.x + (colorWidth + spacing) * i, colorBarPos.y);
            ImVec2 colorBoxMax = ImVec2(startPos.x + (colorWidth + spacing) * i + colorWidth, colorBarPos.y + colorHeight);
            ImVec4 color = colorList[i];
            if (i == colorIndex) {
                selectColor = colorList[i];
                selectedColorBoxMin = ImVec2(colorBoxMin.x - selectSize, colorBoxMin.y - selectSize);
                selectedColorBoxMax = ImVec2(colorBoxMax.x + selectSize, colorBoxMax.y + selectSize);
            } else {
                ImGui::RenderFrame(colorBoxMin, colorBoxMax, ImGui::GetColorU32(color), true, style.FrameRounding);
            }
        }
        ImGui::RenderFrame(selectedColorBoxMin, selectedColorBoxMax, ImGui::GetColorU32(selectColor), true, radius);
        if (ImGui::InvisibleButton(label, ImVec2(numColors * (colorWidth + spacing), colorHeight + selectSize))) {
            ImVec2 mousePos = ImGui::GetMousePos();
            float t = (mousePos.x - startPos.x) / (numColors * (colorWidth + spacing));
            int newColorIndex = static_cast<int>(t * numColors);
            if (newColorIndex >= 0 && newColorIndex < numColors) {
                selectedColor = colorList[newColorIndex];
                colorIndex = newColorIndex;
            }
        }
    }

    // 清除指定类型的动画
    void clearFadeMap(std::initializer_list <AnimationType> types = {AnimationType::All}) {
        // 遍历 fadeMap，根据类型清除对应的键
        auto it = fadeMap.begin();
        while (it != fadeMap.end()) {
            bool erase = false;
            for (auto type: types) {
                switch (type) {
                    case AnimationType::All:
                        erase = true;
                        break;
                    case AnimationType::FadeInOut:
                        erase = (it->first.find("AnimationFadeInOut") != std::string::npos);
                        break;
                    case AnimationType::FadeDown:
                        erase = (it->first.find("AnimationFadeDown") != std::string::npos);
                        break;
                    case AnimationType::FadeUp:
                        erase = (it->first.find("AnimationFadeUp") != std::string::npos);
                        break;
                    case AnimationType::FadeLeft:
                        erase = (it->first.find("AnimationFadeLeft") != std::string::npos);
                        break;
                    case AnimationType::FadeRight:
                        erase = (it->first.find("AnimationFadeRight") != std::string::npos);
                        break;
                }
                if (erase) {
                    it = fadeMap.erase(it);
                    break; // 跳出内部循环，继续外部循环
                }
            }
            if (!erase) {
                ++it;
            }
        }
    }

    //淡入淡出 isChecked：true淡入，false淡出
    bool AnimationFadeInOut(std::function<void()> widgets, bool isChecked = true,
                            float duration = 10.0f) {
        static float Fade;
        Fade = ImLerp(Fade, isChecked ? 1.0f : 0.f, ImGui::GetIO().DeltaTime * duration);
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Fade);
        widgets();
        ImGui::PopStyleVar();
        return std::abs(Fade - 1.f) < 0.1f;
    }

    //循环淡入淡出
    bool AnimationFadeInOutLoop(std::function<void()> widgets, float duration = 10.0f) {
        static float Fade = 0.0f;
        static bool isFadingIn = true;
        static float timeElapsed = 0.0f;
        timeElapsed += ImGui::GetIO().DeltaTime;
        if (isFadingIn) {
            Fade = ImLerp(Fade, 1.0f, timeElapsed / duration);
            if (Fade >= 0.99f) {
                isFadingIn = false;
                timeElapsed = 0.0f;
            }
        } else {
            Fade = ImLerp(Fade, 0.0f, timeElapsed / duration);
            if (Fade <= 0.01f) {
                isFadingIn = true;
                timeElapsed = 0.0f;
            }
        }
        ImGui::PushStyleVar(ImGuiStyleVar_Alpha, Fade);
        widgets();
        ImGui::PopStyleVar();
        return std::abs(Fade - 1.f) < 0.1f;
    }

    //向下移动
    bool AnimationFadeDown(std::function<void()> widgets, float initEndY = 50, float endY = 0,
                           float duration = 10.0f) {
        ImGuiID id = ImGui::GetID((void *) &widgets);
        std::string key = std::to_string(id) + "AnimationFadeDown";
        if (fadeMap.find(key) == fadeMap.end()) {
            fadeMap[key] = initEndY;
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - initEndY);
        }
        float &Fade = fadeMap[key];
        Fade = ImLerp(Fade, endY, ImGui::GetIO().DeltaTime * duration);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + Fade);
        widgets();
        return std::abs(Fade - endY) < 1.0f;
    }

    //向上移动
    bool AnimationFadeUp(std::function<void()> widgets, float initEndY = 50, float endY = 0,
                         float duration = 10.0f) {
        ImGuiID id = ImGui::GetID((void *) &widgets);
        std::string key = std::to_string(id) + "AnimationFadeUp";
        if (fadeMap.find(key) == fadeMap.end()) {
            fadeMap[key] = (-initEndY);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + (-initEndY));
        }
        float &Fade = fadeMap[key];
        Fade = ImLerp(Fade, endY, ImGui::GetIO().DeltaTime * duration);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - Fade);
        widgets();
        return std::abs(Fade - endY) < 1.0f;
    }

    //向左移动
    bool AnimationFadeLeft(std::function<void()> widgets, float initEndX = 300, float endX = 0,
                           float duration = 10.0f) {
        ImGuiID id = ImGui::GetID((void *) &widgets);
        std::string key = std::to_string(id) + "AnimationFadeLeft";
        if (fadeMap.find(key) == fadeMap.end()) {
            fadeMap[key] = (-initEndX);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (-initEndX));
        }
        float &Fade = fadeMap[key];
        Fade = ImLerp(Fade, endX, ImGui::GetIO().DeltaTime * duration);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() - Fade);
        widgets();
        return std::abs(Fade - endX) < 1.0f;
    }

    //向右移动
    bool AnimationFadeRight(std::function<void()> widgets, float initEndX = 300, float endX = 0,
                            float duration = 10.0f) {
        ImGuiID id = ImGui::GetID((void *) &widgets);
        std::string key = std::to_string(id) + "AnimationFadeRight";
        if (fadeMap.find(key) == fadeMap.end()) {
            fadeMap[key] = -initEndX;
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() - initEndX);
        }
        float &Fade = fadeMap[key];
        Fade = ImLerp(Fade, endX, ImGui::GetIO().DeltaTime * duration);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + Fade);
        widgets();
        return std::abs(Fade - endX) < 1.0f;
    }

private:
    std::vector <PopupInfo> popups;

    void RenderPopup(const PopupInfo &popupInfo) {
        ImVec2 windowSize = ImGui::CalcTextSize(popupInfo.content.c_str());
        ImVec2 windowSizeWithPadding = ImVec2(windowSize.x + popupInfo.padding * 2,
                                              windowSize.y + popupInfo.padding * 2);
        ImVec2 windowPos = CalculateWindowPosition(popupInfo.position, popupInfo.padding,
                                                   windowSize);
        ImGui::SetNextWindowPos(windowPos);
        ImGui::SetNextWindowSize(windowSizeWithPadding);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, popupInfo.bgColor);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, popupInfo.borderRadius);
        ImGui::Begin(("Layer" + popupInfo.content).c_str(), nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar |
                     ImGuiWindowFlags_NoResize);
        ImVec2 textPos = ImVec2(windowPos.x + (windowSizeWithPadding.x - windowSize.x) / 2,
                                windowPos.y + (windowSizeWithPadding.y - windowSize.y) / 2);
        ImGui::GetWindowDrawList()->AddText(textPos, popupInfo.textColor,
                                            popupInfo.content.c_str());
        ImGui::End();
        ImGui::PopStyleVar();
    }

    float showPadding = 50;

    ImVec2 CalculateWindowPosition(Position position, float padding, const ImVec2 &windowSize) {
        ImVec2 displaySize = ImVec2(screen_x, screen_y);
        ImVec2 windowPos;
        switch (position) {
            case Position::TopLeft:
                windowPos = ImVec2(showPadding, showPadding);
                break;
            case Position::TopRight:
                windowPos = ImVec2(displaySize.x - windowSize.x - showPadding, showPadding);
                break;
            case Position::BottomLeft:
                windowPos = ImVec2(showPadding, displaySize.y - windowSize.y * 2 - showPadding);
                break;
            case Position::BottomRight:
                windowPos = ImVec2(displaySize.x - windowSize.x - showPadding,
                                   displaySize.y - windowSize.y * 2 - showPadding);
                break;
            case Position::Center:
                windowPos = ImVec2((displaySize.x - windowSize.x - padding * 2) * 0.5f,
                                   (displaySize.y - windowSize.y) * 0.5f);
                break;
            case Position::Top:
                windowPos = ImVec2((displaySize.x - windowSize.x - padding * 2) * 0.5f,
                                   showPadding);
                break;
            case Position::Bottom:
                windowPos = ImVec2((displaySize.x - windowSize.x - padding * 2) * 0.5f,
                                   displaySize.y - windowSize.y * 2 - showPadding);
                break;
            default:
                break;
        }
        return windowPos;
    }
};
