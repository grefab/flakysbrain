//
// Created by Gregor Fabritius on 20.10.22.
//

#pragma once

#include <functional>
#include "common/imui/graphics/view/view.h"
#include "common/imui/graphics/view/view.pb.h"

namespace imui {

void graphicsUI(
  view::View* vw,
  std::function<void(ViewMapper const& view,
                     ImDrawList* draw_list,
                     ImVec2 const& mouse_content_pos)> const& onDraw);
}
