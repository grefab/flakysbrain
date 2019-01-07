//
// Created by grefab on 28.12.18.
//

#include "grefab/flakysbrain/gui/gui.h"
#include <GL/gl3w.h>  // Initialize with gl3wInit()
#include <SDL2/SDL.h>
#include <gui.h>
#include <future>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_sdl.h"

gui::gui(brain* b) : brain_(b) {
    std::cout << "Starting GUI" << std::endl;

    gui_thread_ = std::thread([this]() {
        // Setup SDL
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
            std::cerr << "could not initialize sdl2: " << SDL_GetError() << std::endl;
            return;
        }

        // GL 3.0 + GLSL 130
        const char* glsl_version = "#version 130";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

        // Create window with graphics context
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_DisplayMode current;
        SDL_GetCurrentDisplayMode(0, &current);
        SDL_Window* window =
            SDL_CreateWindow("flakysbrain", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
        SDL_GLContext gl_context = SDL_GL_CreateContext(window);
        SDL_GL_SetSwapInterval(1);  // Enable vsync

        // Initialize OpenGL loader
        if (gl3wInit() != 0) {
            std::cerr << "Failed to initialize OpenGL loader!" << std::endl;
            return;
        }

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls

        // Setup Platform/Renderer bindings
        ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
        ImGui_ImplOpenGL3_Init(glsl_version);

        // Setup Style
        ImGui::StyleColorsDark();

        // Main loop
        ImVec4 const clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        while (!close_thread_) {
            // GUI actions
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                ImGui_ImplSDL2_ProcessEvent(&event);
                if (event.type == SDL_QUIT) {
                    close_thread_ = true;
                }
                if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE
                    && event.window.windowID == SDL_GetWindowID(window)) {
                    close_thread_ = true;
                }
            }

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame(window);
            ImGui::NewFrame();

            {
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
            }

            // Rendering
            ImGui::Render();
            SDL_GL_MakeCurrent(window, gl_context);
            glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
            glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            SDL_GL_SwapWindow(window);
        }

        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
    });

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
    wait();
    std::cout << "GUI Closed" << std::endl;
}

void gui::connect_to(std::string address) {}

void gui::wait() {
    if (gui_thread_.joinable()) {
        gui_thread_.join();
    }
    if (collect_thread_.joinable()) {
        collect_thread_.join();
    }
}
