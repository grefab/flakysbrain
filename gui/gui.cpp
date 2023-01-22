//
// Created by grefab on 28.12.18.
//

#include "gui/gui.h"
#include <spdlog/spdlog.h>
#include <future>
#include "common/imgui_helper/imgui_helper.h"
#include "common/imui/ImUi.h"
#include "common/imui/graphics/graphics_window.h"
#include "common/proto/json/JsonHelper.h"

gui::gui(brain* b) : brain_(b) {
  collect_thread_ = std::thread([this]() {
    while (!close_thread_) {
      std::promise<brain_api::Snapshot> promise;
      auto request_display_data = [&promise](brain* b, timestamp now) {
        auto snapshot = b->brain_mass_->makeSnapshot();
        snapshot.set_last_executed(b->last_executed_event_ts_);
        promise.set_value(snapshot);
        // Slow down brain
        //        std::this_thread::sleep_for(std::chrono::milliseconds(10));
      };
      brain_->add_maintenance_action(request_display_data);
      display_data_.set(promise.get_future().get());
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

    auto dd = display_data_.clone();

    // Display data
    ImGui::Begin("Yay Window");
    ImGui::Text("%s", std::to_string(dd.last_executed()).c_str());
    ImGui::Text("%s", protoToJsonString(dd, false).c_str());
    ImGui::End();

    ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32_BLACK);
    ImGui::Begin("Graphics");
    auto drawFn = [&dd](ViewMapper const& view,
                        ImDrawList* draw_list,
                        ImVec2 const& mouse_content_pos) {
      std::unordered_map<uint32_t, brain_api::Neuron> map;
      for (auto const& neuron : dd.neurons()) {
        map[neuron.id()] = neuron;
      }
      for (auto const& neuron : dd.neurons()) {
        uint8_t brightness = std::round(neuron.potential() * 255.0);
        uint32_t color =
          0xff000000 | brightness << 16 | brightness << 8 | brightness;
        for (auto const& connection : neuron.connections()) {
          draw_list->AddLine(view.toScreen(asImVec2(neuron.pos())),
                             view.toScreen(
                               asImVec2(map[connection.to_id()].pos())),
                             0x80ffffff,
                             2);
        }
      }
      for (auto const& neuron : dd.neurons()) {
        uint8_t brightness = std::round(neuron.potential() * 255.0);
        uint32_t color =
          0xff000000 | brightness << 16 | brightness << 8 | brightness;

        draw_list->AddCircle(view.toScreen(asImVec2(neuron.pos())),
                             view.lengthToScreen(10),
                             color,
                             36,
                             2);
      }
    };
    imui::graphicsUI(&view, drawFn);
    ImGui::End();
    ImGui::PopStyleColor();

    ui.finishFrame();
  }
}
