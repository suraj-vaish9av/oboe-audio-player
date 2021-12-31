//
// Created by 43975 on 12/27/2021.
//

#ifndef OBOE_AUDIO_PLAYER_PLAYERCONTROLLER_H
#define OBOE_AUDIO_PLAYER_PLAYERCONTROLLER_H

#include <android/asset_manager.h>
#include <oboe/Oboe.h>
#include "Player.h"
#include "AAssetDataSource.h"
#include "future"

using namespace oboe;

enum class PlayerControllerState{
    Loading,
    Playing,
    FailedToLoad
};

class PlayerController : public AudioStreamDataCallback, AudioStreamErrorCallback{

public:
    explicit PlayerController(AAssetManager&);
    void start(char *fileName);
    void stop();
    void pause();

    bool paused=false;

    // Inherited from oboe::AudioStreamDataCallback
    DataCallbackResult onAudioReady(AudioStream *oboeStream, void* audioData, int32_t numFrames) override ;

    //Inherited from oboe::AudioStreamErrorCallback
    void onErrorAfterClose(AudioStream *oboeStream, Result error) override ;

private:
    AAssetManager& mAssetManager;
    std::shared_ptr<AudioStream> mAudioStream;
    std::atomic<int64_t> mCurrentFrame{0};
    std::atomic<int64_t> mSongPosition{0};
    std::atomic<PlayerControllerState> mControllerState{PlayerControllerState::Loading};
    std::future<void> mLoadingResult;
    std::atomic<int64_t> mLastUpdateTime { 0 };

    char* trackFilename;

    std::unique_ptr<Player> mTrack;

    void load();
    bool openStream();
    bool setupAudioSources();
    void setAudioTrackFilename(char *filename);

};

#endif //OBOE_AUDIO_PLAYER_PLAYERCONTROLLER_H
