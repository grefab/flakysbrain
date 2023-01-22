//
// Created by Gregor Fabritius on 19.11.22.
//

#pragma once

#include "common/audio/audio.h"

namespace isotronic {

class AudioTheme {
public:
    static AudioTheme& global() {
        static AudioTheme instance(nullptr);
        return instance;
    }

public:
    explicit AudioTheme(Audio* audio) {
        if (audio == nullptr) {
            this->audio = new Audio();
        } else {
            this->audio = audio;
        }
    };
    void goodbye() { audio->playSound(goodbye_, 32); }
    void hello() { audio->playSound(hello_); }
    void notification() { audio->playSound(notification_); }
    void taskFailed() { audio->playSound(task_failed_); }
    void taskSucceeded() { audio->playSound(task_succeeded_, 80); }

    Audio* audio;

private:
    Audio::Wav goodbye_{"common/audio/theme/goodbye.wav"};
    Audio::Wav hello_{"common/audio/theme/hello.wav"};
    Audio::Wav notification_{"common/audio/theme/notification.wav"};
    Audio::Wav task_failed_{"common/audio/theme/task_failed.wav"};
    Audio::Wav task_succeeded_{"common/audio/theme/task_succeeded.wav"};
};

}  // namespace isotronic
