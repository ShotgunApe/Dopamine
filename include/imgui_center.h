#ifndef IMGUI_CENTER_H
#define IMGUI_CENTER_H

#include <imgui.h>

// I stole this (https://github.com/ocornut/imgui/discussions/3862)
class centered_control_wrapper_t
{

public:

    explicit centered_control_wrapper_t(bool result) : result_(result) {}

    operator bool() const { return result_; }

private:

    bool result_;

};

class centered_control_t
{

public:

    centered_control_t(ImVec2 window_size, float y = 0.f, float s = 0.f, float su = 0.f) : window_size_(window_size) { y_offset = y; spacing_below = s; spacing_up = su;}

    template<typename func>
    centered_control_wrapper_t operator()(func control) const
    {
        centered_control_wrapper_t ccw = centered_control_wrapper_t(false);

        ImVec2 original_pos = ImGui::GetCursorPos();
        ImGui::SetCursorPos(ImVec2(-10000.0f, -10000.0f));


        control();

        ImVec2 control_size = ImGui::GetItemRectSize();

        ImGui::Dummy(ImVec2(0, spacing_up));

        ImGui::SetCursorPos(ImVec2((window_size_.x - control_size.x) * 0.5f, original_pos.y + y_offset));

        control();

        ImGui::Dummy(ImVec2(0, spacing_below));

        return ccw;
    }

private:

    ImVec2 window_size_;
    float y_offset;
    float spacing_below;
    float spacing_up;
};

/*
 * This macros centers in the better way possible, got other function but this one may work
 * for the 90% of widgets of ImGui so it's ok, because for some reason with 'ImGui::BeginChild'
 * doesn't work properly
 *
 * In the __VA_ARGS__ which is pointed to the '...' as the second parameter, means that you can
 * call the macro with more arguments. It may not look safe but i'll explain briefly below
 *
 * First arg  : function ( No matter the type )
 * Second arg : vertical offset to center on
 * Third arg  : add spacing below the widget
 * Fourth arg : add spacing over the widget
 *
 * Cons : to add only spacing you'll need to still add a y_offset, you can do it with 0.f on the
 * second one, to avoid redundancy I made it like that
 */
#define CENTERED_CONTROL(control, ...) centered_control_t{ImGui::GetWindowSize(), __VA_ARGS__}([&]() { control; })

#endif
