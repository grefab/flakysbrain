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
        SDL_Window* window = NULL;
        SDL_Surface* screenSurface = NULL;
        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            fprintf(stderr, "could not initialize sdl2: %s\n", SDL_GetError());
            return 1;
        }
        window = SDL_CreateWindow("hello_sdl2", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            fprintf(stderr, "could not create window: %s\n", SDL_GetError());
            return 1;
        }

        screenSurface = SDL_GetWindowSurface(window);

        while (!close_thread_) {
            SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
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
