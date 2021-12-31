//
// Created by 43975 on 12/27/2021.
//

#ifndef OBOE_AUDIO_PLAYER_PLAYER_H
#define OBOE_AUDIO_PLAYER_PLAYER_H

#include "cstdint"
#include "array"
#include "chrono"
#include "memory"
#include "atomic"

#include "android/asset_manager.h"
#include "DataSource.h"

class Player{
public:
    /**
     * Construct a new player from the given DataSource. Players can share the same data source.
     * For example, you could play two identical sounds concurrently by creating 2 Players with same data source.
     *
     * @param source
     */
     Player(std::shared_ptr<DataSource> source):mSource(source){};

     void renderAudio(float *targetData, int32_t numFrames);
     void resetPlayHead() {mReadFrameIndex=0;};
     void setPlaying(bool isPlaying) {mIsPlaying=isPlaying; resetPlayHead();};
     void setLooping(bool isLooping) {mIsLooping=isLooping;};

private:
    int32_t mReadFrameIndex = 0;
     std::atomic<bool> mIsPlaying{false};
     std::atomic<bool> mIsLooping{false};
     std::shared_ptr<DataSource> mSource;

     void renderSilence(float *, int32_t);
};

#endif //OBOE_AUDIO_PLAYER_PLAYER_H
