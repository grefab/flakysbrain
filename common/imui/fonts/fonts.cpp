//
// Created by Gregor Fabritius on 12.11.22.
//

#include "common/imui/fonts/fonts.h"
#include <imgui.h>
#include "common/imui/fonts/IconsMaterialDesign.h"

void setupFonts() {
  ImGuiIO& io = ImGui::GetIO();
  //  io.Fonts->AddFontDefault();
  io.Fonts
    ->AddFontFromFileTTF("common/imui/fonts/Roboto-Regular.ttf",
                         20.0f);
  ImFontConfig font_config;
  font_config.MergeMode = true;
  font_config.GlyphOffset.y = 5;
  static const ImWchar icons_ranges[] = {ICON_MIN_MD, ICON_MAX_MD, 0};
  io.Fonts
    ->AddFontFromFileTTF("common/imui/fonts/MaterialIcons-Regular.ttf",
                         22.0f,
                         &font_config,
                         icons_ranges);
  io.Fonts->Build();
}
