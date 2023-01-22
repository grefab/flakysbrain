//
// Created by Gregor Fabritius on 20.10.22.
//

#include "common/imui/graphics/graphics_window.h"
#include <imgui.h>
#include <spdlog/spdlog.h>
#include <cmath>
#include "common/imgui_helper/imgui_helper.h"
#include "common/imui/graphics/view/view.h"
#include "common/imui/graphics/view/view.pb.h"

namespace imui {

bool isMouseInWidget();

void drawGridRaster(ViewMapper const& view,
                    float raster_size,
                    uint32_t grid_color,
                    ImDrawList* drawList) {
  float const thickness = 1;

  ImVec2 window_top_left =
    ImGui::GetWindowPos() + ImGui::GetWindowContentRegionMin();
  ImVec2 window_bottom_right =
    ImGui::GetWindowPos() + ImGui::GetWindowContentRegionMax();
  auto tl = view.toContent(window_top_left);
  auto br = view.toContent(window_bottom_right);
  // vertical lines
  for (float x = std::ceil(tl.x / raster_size) * raster_size; x < br.x;
       x += raster_size) {  // NOLINT(cert-flp30-c)
    ImVec2 p1(x, tl.y);
    ImVec2 p2(x, br.y);
    drawList->AddLine(view.toScreen(p1),
                      view.toScreen(p2),
                      grid_color,
                      thickness);
  }
  // horizontal lines
  for (float y = std::ceil(tl.y / raster_size) * raster_size; y < br.y;
       y += raster_size) {  // NOLINT(cert-flp30-c)
    ImVec2 p1(tl.x, y);
    ImVec2 p2(br.x, y);
    drawList->AddLine(view.toScreen(p1),
                      view.toScreen(p2),
                      grid_color,
                      thickness);
  }
}

void drawGrid(ViewMapper const& view, ImDrawList* drawList) {
  int const grid_granularity = 10;
  for (int i = 1; i <= 1000000; i *= grid_granularity) {
    auto screen_length = view.lengthToScreen(i);
    float const active_min_pixel_distance = 6;
    if (screen_length > active_min_pixel_distance) {
      uint32_t grid_color =
        0x00ffffff
        + (static_cast<uint8_t>(0xff
                                * ((screen_length - active_min_pixel_distance)
                                   / view.lengthToScreen(i * 5)))
           << 24);
      drawGridRaster(view, i, grid_color, drawList);
      drawGridRaster(view, i / 2.0f, grid_color, drawList);
    }
  }
}

void drawScale(ViewMapper const& view, ImDrawList* drawList) {
  int const grid_granularity = 10;
  auto [scale_screen_length, scale_content_length] = [&] {
    for (int i = 1; i <= 1000000; i *= grid_granularity) {
      float screen_length = view.lengthToScreen(i);
      if (screen_length > ImGui::GetWindowContentRegionWidth()) {
        return std::make_tuple(view.lengthToScreen((float)i
                                                   / (float)grid_granularity),
                               (float)i / (float)grid_granularity);
      }
    }
    return std::make_tuple(0.0f, 0.0f);
  }();

  ImVec2 window_top_left =
    ImGui::GetWindowPos() + ImGui::GetWindowContentRegionMin();
  ImVec2 center(ImGui::GetWindowContentRegionWidth() / 2.0f, 0);
  ImVec2 left(center.x - scale_screen_length / 2.0f, 0);
  ImVec2 right(center.x + scale_screen_length / 2.0f, 0);
  ImVec2 offset(0, 20);

  uint32_t color = 0xa0ffffff;
  drawList->AddLine(window_top_left + offset + left,
                    window_top_left + offset + right,
                    color,
                    2);
  std::string s = fmt::format("{:.0f}", scale_content_length);
  drawList->AddText(window_top_left + offset
                      - ImVec2(ImGui::CalcTextSize(s.c_str()).x / 2, 20)
                      + center,
                    color,
                    s.c_str());
}

void drawCoordinates(ImVec2 const& mouse_content_pos, ImDrawList* drawList) {
  ImVec2 window_top_left =
    ImGui::GetWindowPos() + ImGui::GetWindowContentRegionMin();
  std::string s =
    fmt::format("x{:.2f} y{:.2f}", mouse_content_pos.x, mouse_content_pos.y);
  uint32_t color = 0xa0ffffff;
  drawList->AddText(window_top_left, color, s.c_str());
}

void graphicsUI(
  view::View* vw,
  std::function<void(ViewMapper const& view,
                     ImDrawList* draw_list,
                     ImVec2 const& mouse_content_pos)> const& onDraw) {
  bool is_mouse_in_widget = isMouseInWidget();

  // take care for view matrix and resulting transformation functions
  cv::Mat transform = loadTransformation(*vw);
  if (is_mouse_in_widget) {
    applyInteractionsToTransformation(&transform);
  }
  auto view = makeViewMapper(transform);
  storeTransform(transform, vw);

  ImGuiIO const& io = ImGui::GetIO();
  ImVec2 mouse_content_pos = view.toContent(io.MousePos);
  ImDrawList* drawList = ImGui::GetWindowDrawList();

  onDraw(view, drawList, mouse_content_pos);

  drawGrid(view, drawList);
  drawScale(view, drawList);
  drawCoordinates(mouse_content_pos, drawList);
}

bool isMouseInWidget() {
  ImGuiIO const& io = ImGui::GetIO();
  ImVec2 window_top_left =
    ImGui::GetWindowPos() + ImGui::GetWindowContentRegionMin();
  ImVec2 window_bottom_right =
    ImGui::GetWindowPos() + ImGui::GetWindowContentRegionMax();
  return io.MousePos.x > window_top_left.x && io.MousePos.y > window_top_left.y
         && io.MousePos.x < window_bottom_right.x
         && io.MousePos.y < window_bottom_right.y;
}

}  // namespace imui
