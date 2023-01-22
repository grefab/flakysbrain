//
// Created by Gregor Fabritius on 26.11.22.
//

#pragma once

#include <string>
#include "imgui_impl_glfw.h"

class ImUi {
  public:
  ImUi(int width, int height, std::string const& title);
  virtual ~ImUi();

  bool active();
  void startFrame();
  void finishFrame();

  private:
  GLFWwindow* window_ = nullptr;
};
