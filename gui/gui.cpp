//
// Created by grefab on 28.12.18.
//

#include "gui.h"
#include <SDL2/SDL.h>
#include <iostream>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

gui::gui(brain* b) : brain_(b) {
    std::cout << "Starting GUI" << std::endl;
    auto request_display_data = [this](brain* b, timestamp now) { abs_time_ += now; };

    thread_ = std::thread([this, request_display_data]() {
        SDL_Window* window = nullptr;
        SDL_Surface* screenSurface = nullptr;
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cerr << "could not initialize sdl2: " << SDL_GetError() << std::endl;
            return;
        }
        window = SDL_CreateWindow("hello_sdl2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (!window) {
            std::cerr << "could not create window: " << SDL_GetError() << std::endl;
            return;
        }

        screenSurface = SDL_GetWindowSurface(window);

        while (!close_thread_) {
            SDL_FillRect(screenSurface, nullptr, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
            SDL_UpdateWindowSurface(window);

            brain_->add_maintenance_action(request_display_data);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        SDL_Delay(2000);
        SDL_DestroyWindow(window);
        SDL_Quit();
    });
}

gui::~gui() {
    std::cout << "Closing GUI" << std::endl;
    close_thread_ = true;
    thread_.join();
    std::cout << "GUI Closed" << std::endl;
}
