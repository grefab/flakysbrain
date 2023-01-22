//
// Created by Gregor Fabritius on 12.11.22.
//

#pragma once

#include <OpenGL/gl.h>
#include <opencv2/core/mat.hpp>

GLuint makeTexture();
void uploadMat(cv::Mat const& mat, GLuint image_texture);
