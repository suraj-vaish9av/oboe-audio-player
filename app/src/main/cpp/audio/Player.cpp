//
// Created by 43975 on 12/27/2021.
//
#include "Player.h"
#include "../utils/logging.h"

void Player::renderAudio(float *targetData, int32_t numFrames) {
    const AudioProperties properties = mSource->getProperties();

    if (mIsPlaying){
        int64_t framesToRenderFromData = numFrames;
        int64_t totalSourceFrames = mSource->getSize() / properties.channelCount;
        const float *data = mSource->getData();

        // check whether we're about to reach the end of the recording
        if (!mIsLooping && mReadFrameIndex+numFrames >= totalSourceFrames){
            framesToRenderFromData = totalSourceFrames-mReadFrameIndex;
            mIsPlaying= false;
        }

        for (int i = 0; i < framesToRenderFromData; ++i) {
            for (int j=0; j<properties.channelCount; ++j){
                targetData[(i*properties.channelCount)+j] = data[(mReadFrameIndex*properties.channelCount)+j];
            }

            // Increment and handle wraparound
            if (++mReadFrameIndex >= totalSourceFrames) mReadFrameIndex=0;
        }

        if (framesToRenderFromData < numFrames){
            // fill the rest of the buffer with silence
            renderSilence(&targetData[framesToRenderFromData], numFrames*properties.channelCount);
        }
    }else{
        renderSilence(targetData,numFrames*properties.channelCount);
    }
}

void Player::renderSilence(float *start, int32_t numSamples) {
    for (int i = 0; i < numSamples; ++i) {
        start[i]=0;
    }
}
