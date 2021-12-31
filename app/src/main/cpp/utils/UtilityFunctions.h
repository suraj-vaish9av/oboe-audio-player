//
// Created by 43975 on 12/27/2021.
//

#ifndef OBOE_AUDIO_PLAYER_UTILITYFUNCTIONS_H
#define OBOE_AUDIO_PLAYER_UTILITYFUNCTIONS_H

constexpr int64_t kMillisecondsInSecond = 1000;

constexpr int64_t convertFramesToMillis(const int64_t frames, const int sampleRate){
    return static_cast<int64_t>((static_cast<double>(frames)/ sampleRate) * kMillisecondsInSecond);
}

int64_t nowUptimeMillis() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

#endif //OBOE_AUDIO_PLAYER_UTILITYFUNCTIONS_H
