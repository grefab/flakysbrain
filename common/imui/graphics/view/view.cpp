//
// Created by Gregor Fabritius on 03.11.22.
//

#include "common/imui/graphics/view/view.h"
#include <opencv2/imgproc.hpp>
#include "common/imgui_helper/imgui_helper.h"

ViewMapper makeViewMapper(cv::Mat const& transform) {
    cv::Mat_<float> inv_transform(2, 3);
    cv::invertAffineTransform(transform, inv_transform);
    auto mapContentToScreen = [transform](ImVec2 const& p) -> ImVec2 {
        cv::Mat_<float> p_as_vector(3, 1);
        p_as_vector << p.x, p.y, 1.0;
        cv::Mat p_transformed = transform * p_as_vector;
        return {p_transformed.at<float>(0), p_transformed.at<float>(1)};
    };
    auto mapScreenToContent = [inv_transform](ImVec2 const& p) -> ImVec2 {
        cv::Mat_<float> p_as_vector(3, 1);
        p_as_vector << p.x, p.y, 1.0;
        cv::Mat p_transformed = inv_transform * p_as_vector;
        return {p_transformed.at<float>(0), p_transformed.at<float>(1)};
    };
    auto mapLengthToScreen = [transform](float l) {
        float scale = transform.at<float>(0, 0);
        return l * scale;
    };
    return {mapContentToScreen, mapScreenToContent, mapLengthToScreen};
}

void applyInteractionsToTransformation(cv::Mat* transform) {
    // translate our mouse position into content coordinates by taking care for
    // pan and zoom
    ImGuiIO const& io = ImGui::GetIO();

    const float mouse_threshold_for_pan = 0.0f;
    if (ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan)) {
        transform->at<float>(0, 2) += io.MouseDelta.x;
        transform->at<float>(1, 2) += io.MouseDelta.y;
        return;
    }

    if (ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift)) {
        if (io.MouseWheel != 0) {
            // this zooms to cursor position, inspired by
            // https://medium.com/@benjamin.botto/zooming-at-the-mouse-coordinates-with-affine-transformations-86e7312fd50b
            float scale_factor = 1.0f + io.MouseWheel / 10;

            cv::Mat_<float> world_mat(3, 3);
            world_mat.at<float>(0, 0) = transform->at<float>(0, 0);
            world_mat.at<float>(0, 1) = transform->at<float>(0, 1);
            world_mat.at<float>(0, 2) = transform->at<float>(0, 2);
            world_mat.at<float>(1, 0) = transform->at<float>(1, 0);
            world_mat.at<float>(1, 1) = transform->at<float>(1, 1);
            world_mat.at<float>(1, 2) = transform->at<float>(1, 2);
            world_mat.at<float>(2, 0) = 0;
            world_mat.at<float>(2, 1) = 0;
            world_mat.at<float>(2, 2) = 1;

            ImVec2 zoom_to = io.MousePos;

            cv::Mat_<float> translate_mat(3, 3);
            translate_mat.at<float>(0, 0) = 1;
            translate_mat.at<float>(0, 1) = 0;
            translate_mat.at<float>(0, 2) = -zoom_to.x;
            translate_mat.at<float>(1, 0) = 0;
            translate_mat.at<float>(1, 1) = 1;
            translate_mat.at<float>(1, 2) = -zoom_to.y;
            translate_mat.at<float>(2, 0) = 0;
            translate_mat.at<float>(2, 1) = 0;
            translate_mat.at<float>(2, 2) = 1;

            cv::Mat_<float> scale_mat(3, 3);
            scale_mat.at<float>(0, 0) = 1 * scale_factor;
            scale_mat.at<float>(0, 1) = 0;
            scale_mat.at<float>(0, 2) = 0;
            scale_mat.at<float>(1, 0) = 0;
            scale_mat.at<float>(1, 1) = 1 * scale_factor;
            scale_mat.at<float>(1, 2) = 0;
            scale_mat.at<float>(2, 0) = 0;
            scale_mat.at<float>(2, 1) = 0;
            scale_mat.at<float>(2, 2) = 1;

            cv::Mat_<float> translate_back_mat(3, 3);
            translate_back_mat.at<float>(0, 0) = 1;
            translate_back_mat.at<float>(0, 1) = 0;
            translate_back_mat.at<float>(0, 2) = zoom_to.x;
            translate_back_mat.at<float>(1, 0) = 0;
            translate_back_mat.at<float>(1, 1) = 1;
            translate_back_mat.at<float>(1, 2) = zoom_to.y;
            translate_back_mat.at<float>(2, 0) = 0;
            translate_back_mat.at<float>(2, 1) = 0;
            translate_back_mat.at<float>(2, 2) = 1;

            cv::Mat result_mat = translate_back_mat * scale_mat * translate_mat * world_mat;
            transform->at<float>(0, 0) = result_mat.at<float>(0, 0);
            transform->at<float>(0, 1) = result_mat.at<float>(0, 1);
            transform->at<float>(0, 2) = result_mat.at<float>(0, 2);
            transform->at<float>(1, 0) = result_mat.at<float>(1, 0);
            transform->at<float>(1, 1) = result_mat.at<float>(1, 1);
            transform->at<float>(1, 2) = result_mat.at<float>(1, 2);
        }
    } else {
        transform->at<float>(0, 2) += io.MouseWheelH * 10;
        transform->at<float>(1, 2) += io.MouseWheel * 10;
    }
}

cv::Mat loadTransformation(view::View const& view) {
    cv::Mat_<float> transform(2, 3);
    if (view.transform().size() == 6) {
        transform.at<float>(0, 0) = view.transform(0);
        transform.at<float>(0, 1) = view.transform(1);
        transform.at<float>(0, 2) = view.transform(2);
        transform.at<float>(1, 0) = view.transform(3);
        transform.at<float>(1, 1) = view.transform(4);
        transform.at<float>(1, 2) = view.transform(5);
    } else {
        ImVec2 window_top_left = ImGui::GetWindowContentRegionMin() + ImGui::GetWindowPos();
        transform.at<float>(0, 0) = 1;
        transform.at<float>(0, 1) = 0;
        transform.at<float>(0, 2) = window_top_left.x;
        transform.at<float>(1, 0) = 0;
        transform.at<float>(1, 1) = 1;
        transform.at<float>(1, 2) = window_top_left.y;
    }
    return std::move(transform);
}

void storeTransform(cv::Mat const& transform, view::View* view) {
    while (view->transform().size() < 6) {
        view->add_transform(0);
    }
    view->mutable_transform()->at(0) = transform.at<float>(0, 0);
    view->mutable_transform()->at(1) = transform.at<float>(0, 1);
    view->mutable_transform()->at(2) = transform.at<float>(0, 2);
    view->mutable_transform()->at(3) = transform.at<float>(1, 0);
    view->mutable_transform()->at(4) = transform.at<float>(1, 1);
    view->mutable_transform()->at(5) = transform.at<float>(1, 2);
}
