//
// Created by Gregor Fabritius on 19.11.22.
//

#include "common/audio/audio.h"
#include <SDL2/SDL.h>
#include <spdlog/spdlog.h>

std::mutex buffer_mutex;

void sdl_audio_callback(void* userdata, Uint8* stream, int stream_len) {
    std::lock_guard lock(buffer_mutex);

    auto* buffer = static_cast<std::vector<uint8_t>*>(userdata);
    if (buffer->empty()) {
        SDL_memset(stream, 0, stream_len);
        return;
    }
    if (buffer->size() <= stream_len) {
        SDL_memset(stream, 0, stream_len);
        SDL_memcpy(stream, &buffer->front(), buffer->size());
        buffer->clear();
    } else {
        SDL_memcpy(stream, &buffer->front(), stream_len);
        buffer->erase(std::begin(*buffer), std::begin(*buffer) + stream_len);
    }
}

Audio::Audio() {
    int success = SDL_Init(SDL_INIT_AUDIO);
    if (success != 0) {
        spdlog::error("failed to init SDL: {}", SDL_GetError());
        return;
    }

    SDL_AudioSpec desired_spec;
    desired_spec.freq = 44100;
    desired_spec.format = 32784;
    desired_spec.channels = 2;
    desired_spec.silence = 0;
    desired_spec.samples = 4096;
    desired_spec.padding = 0;
    desired_spec.size = 0;
    desired_spec.callback = sdl_audio_callback;
    desired_spec.userdata = &buffer_;
    sdl_audio_device_id_ = SDL_OpenAudioDevice(nullptr, 0, &desired_spec, &sdl_audio_spec_, 0);
    if (sdl_audio_device_id_ == 0) {
        spdlog::error("failed to load audio device: {}", SDL_GetError());
        return;
    }
    SDL_PauseAudioDevice(sdl_audio_device_id_, 0);
}

Audio::~Audio() {
    SDL_PauseAudioDevice(sdl_audio_device_id_, 1);
    SDL_CloseAudioDevice(sdl_audio_device_id_);
    SDL_Quit();
}

std::optional<Audio::Wav> Audio::loadSound(std::string const& filename) {
    Wav wav(filename);
    if (wav.buffer == nullptr) {
        return std::nullopt;
    }
    return wav;
}

void Audio::playSound(Audio::Wav const& wav, int volume) {
    std::lock_guard lock(buffer_mutex);
    if (buffer_.size() < wav.buffer_size) {
        buffer_.resize(wav.buffer_size, 0x00);
    }
    SDL_MixAudioFormat(&buffer_.front(), wav.buffer, 32784, wav.buffer_size, volume);

    //    buffer_.reserve(buffer_.size() + wav.buffer_size);
    //    copy(wav.buffer, wav.buffer + wav.buffer_size, back_inserter(buffer_));
}

bool Audio::isPlaying() const {
    std::lock_guard lock(buffer_mutex);
    return !buffer_.empty();
}

Audio::Wav::Wav(std::string const& filename) {
    auto* spec = SDL_LoadWAV(filename.c_str(), &audio_spec, &buffer, &buffer_size);
    if (spec == nullptr) {
        spdlog::error("failed to load audio file: {}", SDL_GetError());
        buffer = nullptr;
        buffer_size = 0;
    }
}
