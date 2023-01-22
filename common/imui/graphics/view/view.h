//
// Created by Gregor Fabritius on 03.11.22.
//

#pragma once

#include <imgui.h>
#include <functional>
#include <opencv2/core.hpp>
#include "common/imui/graphics/view/view.pb.h"

struct ViewMapper {
  std::function<ImVec2(ImVec2 const&)> toScreen;
  std::function<ImVec2(ImVec2 const&)> toContent;
  std::function<float(float)> lengthToScreen;
};

void applyInteractionsToTransformation(cv::Mat* transform);
ViewMapper makeViewMapper(cv::Mat const& transform);
cv::Mat loadTransformation(view::View const& view);
void storeTransform(cv::Mat const& transform, view::View* view);
