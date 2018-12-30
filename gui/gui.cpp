//
// Created by grefab on 28.12.18.
//

#include "gui.h"
#include <SDL2/SDL.h>
#include <future>
#include <iostream>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

gui::gui(brain* b) : brain_(b) {
    std::cout << "Starting GUI" << std::endl;

    thread_ = std::thread([this]() {
        // Initialization
        SDL_Window* window = nullptr;
        SDL_Renderer* renderer = nullptr;
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "could not initialize sdl2: " << SDL_GetError() << std::endl;
            return;
        }

        if (SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0, &window, &renderer) != 0) {
            std::cerr << "could not create window: " << SDL_GetError() << std::endl;
            return;
        }
        SDL_SetWindowTitle(window, "flakysbrain");

        // Main loop
        while (!close_thread_) {
            std::promise<timestamp> promise;
            auto request_display_data = [this, &renderer, &promise](brain* b, timestamp now) {
                monotonic_now_ += now;

                // Clear screen
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderClear(renderer);

                SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawLine(renderer, 320, 200, 300, 240);
                SDL_RenderDrawLine(renderer, 300, 240, 340, 240);
                SDL_RenderDrawLine(renderer, 340, 240, 320, 200);
                SDL_RenderPresent(renderer);

                // Slow down brain
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                promise.set_value(now);
            };

            brain_->add_maintenance_action(request_display_data);
            auto future = promise.get_future();
            future.wait();
            std::cout << monotonic_now_ << " " << future.get() << std::endl;
        }

        // Shutdown
        if (renderer) {
            SDL_DestroyRenderer(renderer);
        }
        if (window) {
            SDL_DestroyWindow(window);
        }
        SDL_Quit();
    });
}

gui::~gui() {
    std::cout << "Closing GUI" << std::endl;
    close_thread_ = true;
    thread_.join();
    std::cout << "GUI Closed" << std::endl;
}
