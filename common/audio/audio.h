//
// Created by Gregor Fabritius on 19.11.22.
//

#pragma once

#include <SDL2/SDL_audio.h>
#include <mutex>
#include <optional>
#include <vector>

class Audio {
public:
    struct Wav {
        explicit Wav(std::string const& filename);
        SDL_AudioSpec audio_spec;
        Uint32 buffer_size = 0;
        Uint8* buffer = nullptr;
    };

public:
    virtual ~Audio();
    Audio();
    static std::optional<Wav> loadSound(std::string const& filename);
    void playSound(Wav const& wav, int volume = SDL_MIX_MAXVOLUME);
    [[nodiscard]] bool isPlaying() const;

private:
    SDL_AudioSpec sdl_audio_spec_;
    SDL_AudioDeviceID sdl_audio_device_id_;
    std::vector<uint8_t> buffer_;
};
