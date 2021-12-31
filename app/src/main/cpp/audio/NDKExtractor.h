//
// Created by 43975 on 12/24/2021.
//

#ifndef OBOE_AUDIO_PLAYER_NDKEXTRACTOR_H
#define OBOE_AUDIO_PLAYER_NDKEXTRACTOR_H

#include <cstdint>
#include "AudioProperties.h"
#include "android/asset_manager.h"

/**
 * NDK Media Decoder
 */
class NDKExtractor{
public:
    static int32_t decode(AAsset *asset, uint8_t *targetData, AudioProperties targetProperties);
};

#endif //OBOE_AUDIO_PLAYER_NDKEXTRACTOR_H
