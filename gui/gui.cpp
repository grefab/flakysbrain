//
// Created by grefab on 28.12.18.
//

#include "gui/gui.h"
#include <spdlog/spdlog.h>
#include <future>
#include "common/imui/ImUi.h"
#include "common/imui/graphics/graphics_window.h"

gui::gui(brain* b) : brain_(b) {
  collect_thread_ = std::thread([this]() {
    while (!close_thread_) {
      // Request data
      std::promise<timestamp> promise;
      auto request_display_data = [&promise](brain* b, timestamp now) {
        promise.set_value(b->last_executed_event_ts());

        // Slow down brain
        //                std::this_thread::sleep_for(std::chrono::milliseconds(100));
      };
      brain_->add_maintenance_action(request_display_data);

      {
        auto last_executed_event_ts = promise.get_future().get();
        std::lock_guard<std::mutex> lock(display_data_.mutex_);
        display_data_.last_executed_event_ts_ = last_executed_event_ts;
      }
    }
  });
}

gui::~gui() {
  spdlog::info("closing GUI");
  close_thread_ = true;
  if (collect_thread_.joinable()) {
    collect_thread_.join();
  }
  spdlog::info("GUI closed");
}

void gui::connect_to(std::string const& address) {}

void gui::run() {
  spdlog::info("starting GUI");
  ImUi ui(1920, 1080, "flakysbrain");
  view::View view;
  while (ui.active()) {
    ui.startFrame();

    // Display data
    std::lock_guard<std::mutex> lock(display_data_.mutex_);

    ImGui::Begin("Yay Window");
    ImGui::Text("%s",
                std::to_string(display_data_.last_executed_event_ts_).c_str());

    ImGui::End();

    ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32_BLACK);
    ImGui::Begin("Graphics");
    imui::graphicsUI(&view,
                     [](ViewMapper const& view,
                        ImDrawList* draw_list,
                        ImVec2 const& mouse_content_pos) {});
    ImGui::End();
    ImGui::PopStyleColor();

    ui.finishFrame();
  }
}
