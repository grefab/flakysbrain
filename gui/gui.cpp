//
// Created by grefab on 28.12.18.
//

#include "gui/gui.h"

#include <future>
#include <iostream>

#include "common/imui/ImUi.h"

gui::gui(brain* b) : brain_(b) {
    collect_thread_ = std::thread([this]() {
        while (!close_thread_) {
            // Request data
            std::promise<timestamp> promise;
            auto request_display_data = [&promise](brain* b, timestamp now) {
                promise.set_value(now);

                // Slow down brain
                //                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            };
            brain_->add_maintenance_action(request_display_data);

            {
                auto now = promise.get_future().get();
                std::lock_guard<std::mutex> lock(display_data_.mutex_);
                display_data_.monotonic_now_ += now;
            }
        }
    });
}

gui::~gui() {
    std::cout << "Closing GUI" << std::endl;
    close_thread_ = true;
    if (collect_thread_.joinable()) {
        collect_thread_.join();
    }
    std::cout << "GUI Closed" << std::endl;
}

void gui::connect_to(std::string const& address) {}

void gui::run() {
    std::cout << "Starting GUI" << std::endl;
    ImUi ui(1920, 1080, "Isotronic Contour Workshop");
    while (ui.active()) {
        ui.startFrame();

        // Display data
        std::lock_guard<std::mutex> lock(display_data_.mutex_);

        ImGui::Begin("Another Window");  // Pass a pointer to our bool variable (the window will have a closing
                                         // button that will clear the bool when clicked)
        ImGui::Text("%s", std::to_string(display_data_.monotonic_now_).c_str());
        if (ImGui::Button("Close Me")) {
            close_thread_ = true;
        }

        ImDrawList* list = ImGui::GetWindowDrawList();

        ImVec2 canvas_pos = ImGui::GetCursorScreenPos();  // ImDrawList API uses screen coordinates!
        ImVec2 lineP1 = {150.0f, 100.0f};
        ImVec2 lineP2 = {150.0f, 300.0f};
        static float thickness = 4.0f;
        static float arrowWidth = 12.0f;
        static float arrowHeight = 18.0f;
        static float lineWidth = 4.0f;

        list->AddCircleFilled({canvas_pos.x + lineP1.x - thickness, canvas_pos.y + lineP1.y + arrowHeight},
                              50,
                              IM_COL32(128, 128, 128, 255),
                              120);

        list->PathLineTo({canvas_pos.x + lineP1.x - thickness, canvas_pos.y + lineP1.y + arrowHeight});   // P1
        list->PathLineTo({canvas_pos.x + lineP1.x - arrowWidth, canvas_pos.y + lineP1.y + arrowHeight});  // P2
        list->PathLineTo({canvas_pos.x + lineP1.x, canvas_pos.y + lineP1.y});                             // P3
        list->PathLineTo({canvas_pos.x + lineP1.x + arrowWidth, canvas_pos.y + lineP1.y + arrowHeight});  // P4
        list->PathLineTo({canvas_pos.x + lineP1.x + thickness, canvas_pos.y + lineP1.y + arrowHeight});   // P5
        list->PathLineTo({canvas_pos.x + lineP2.x + thickness, canvas_pos.y + lineP2.y});                 // P6
        list->PathLineTo({canvas_pos.x + lineP2.x - thickness, canvas_pos.y + lineP2.y});                 // P7
        list->PathStroke(IM_COL32(128, 128, 128, 255), true, lineWidth);

        ImGui::End();

        ui.finishFrame();
    }
}
