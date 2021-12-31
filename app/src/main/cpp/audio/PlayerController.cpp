//
// Created by 43975 on 12/27/2021.
//

#include "PlayerController.h"
#include "thread"
#include "../utils/logging.h"
#include "../utils/UtilityFunctions.h"

PlayerController::PlayerController(AAssetManager &assetManager):mAssetManager(assetManager) {
}
/**
 * Initializes stream and player then eventually starting the stream.
 */
void PlayerController::load() {
    if (!openStream()){
        mControllerState=PlayerControllerState::FailedToLoad;
        return;
    }

    if (!setupAudioSources()){
        mControllerState=PlayerControllerState::FailedToLoad;
        return;
    }

    // starting the stream, after this onAudioReady method of DataCallbackResult will be called.
    Result result = mAudioStream->requestStart();
    if (result!=Result::OK){
        LOGE("Failed to start stream. Error: %s",convertToText(result));
        mControllerState=PlayerControllerState::FailedToLoad;
        return;
    }

    mControllerState=PlayerControllerState::Playing;
}

/**
 * sets the audio filename, call the load method.
 * @param fileName : name of the asset audio file.
 */
void PlayerController::start(char *fileName) {
   // LOGD("PlayerController, comp= %d",strcmp(fileName,trackFilename));
    if (paused){
        LOGD("PlayerController, setPlaying true");
        mTrack->setPlaying(true);
    }
    else{
        LOGD("PlayerController, starting Player");
        setAudioTrackFilename(fileName);
        // async returns a future, we must store this future to avoid blocking. It's not sufficient
        // to store this in a local variable as its destructor will block until PlayerController::load completes.
        mLoadingResult =std::async(&PlayerController::load,this);
    }
}

/**
 * stop the audio stream.
 */
void PlayerController::stop() {
    if (mAudioStream){
        mAudioStream->stop();
        mAudioStream->close();
        mAudioStream.reset();
    }
}

void PlayerController::pause() {
    mAudioStream->pause();

    //mTrack->setPlaying(false);
}

/**
 *
 * @param oboeStream pointer to the associated stream
 * @param audioData buffer containing input data or a place to put output data
 * @param numFrames number of frames to be processed
 * @return DataCallbackResult::Continue or DataCallbackResult::Stop
 */
DataCallbackResult PlayerController::onAudioReady(AudioStream *oboeStream, void *audioData, int32_t numFrames) {
    auto *outputBuffer = static_cast<float *>(audioData);
    for (int i=0; i<numFrames; ++i){
        mSongPosition = convertFramesToMillis(mCurrentFrame,mAudioStream->getSampleRate());
        mCurrentFrame++;
        mTrack->renderAudio(outputBuffer+(oboeStream->getChannelCount()*i), 1);
    }
    mLastUpdateTime = nowUptimeMillis();
    return DataCallbackResult::Continue;
}

/**
 * reset the stream, frame, song position, update time. and restart the stream.
 * @param oboeStream: audioStream pointer to the associated stream
 * @param error
 */
void PlayerController::onErrorAfterClose(AudioStream *oboeStream, Result error) {
    if (error == Result::ErrorDisconnected){
        mControllerState = PlayerControllerState::Loading;
        mAudioStream.reset();
        mCurrentFrame=0;
        mSongPosition=0;
        mLastUpdateTime=0;
        start(trackFilename);
    }else{
        LOGE("Stream error: %s",convertToText(error));
    }
}

/**
 * set the properties via StreamBuilder
 * Audio Channel:
 * Mono: Mono sound only uses one channel when converting a signal into a sound.
 *      So even if there are multiple speakers the same signal will go to all the speakers.
 *
 * Stereo: Stereo sound uses more than one channel when converting a signal into a sound.
 *         This means each signal sent out is unique.
 *         For example: If you are listening a stereo sound in earphone/headphone then you can
 *         experience different sound signal in left and right speaker of earphone/headphone.
 *         But in case of mono both left and right speaker will produce same sound and there is only on audio channel.
 *
 * Sharing Mode:
 * Exclusive: Exclusive means the stream has exclusive access to the audio device;
 *            the endpoint cannot be used by any other audio stream. It provides the lowest latancy.
 *            Close this kind of stream as soon as you no longer need them.
 *
 * Shared:  Allows oboe stream to share an endpoint. The operating system will mix all the shared streams
 *          assigned to the same endpoint on the audio device.
 *
 *
 * Sample Rate: Sample rate is like frame rate of video.
 *              In order to accurately record and reproduce a frequency we need to sample it twice every cycle.
 *              1khz audio signal takes 1ms to complete a cycle. So we want to record it we need to sample
 *              it twice that speed that is: 2kHz.
 *              A human ear can hear sound from 20hz - 20000hz.
 *              To record frequencies up to 20kHz we need sample rate of at least 40kHz.
 *              For technical reason, we use sample rate slightly higher than that,
 *              most music application use 44100Hz = 44.1kHz
 *              48kHz is commonly used when producing audio for video because relationship between audio samples
 *              and the frame rate of video.
 *
 * Performance Mode:
 * None:        default mode, It uses basic stream that balances power and latency.
 * LowLatency : uses smaller buffers and an optimized data path for reduced latency.
 * PowerSaving: uses larger internal buffers and a data path that trade off latency for lower power.
 *
 * Bit Depth :  Bit Depth is like resolution of video or a frame.
 *              Higher Bit Depth = Greater Dynamic Range
 *              Dynamic Range means that difference between the loudest signal we can record without distorting
 *              and the quietest signal we can record before the audio falls noise floor
 *              4 Bits = 24 dB dynamic range, Extreme noise artifacts: Quiet sounds are lost.
 *              8 Bits = 48 dB dynamic range, Audible noise that changes the sound: less resolution for quiet sound.
 *              16 Bits = 96 dB dynamic range, Low noise: Good resolution of quiet sounds.
 *
 * FormatConversionAllowed:
 *              If true oboe might convert data format for optimal results. For ex: A float stream could not
 *              get a low latency data path. So an I16 stream might be opened and converted to float.
 *
 * SampleRateConversionQuality: Specifies the quality of sample rate conversion performed by Oboe.
 *              None: No conversion by oboe.
 *              Fastest: May not sound great
 *              Low, Medium, High,
 *              Best: high quality conversion may be expensive in terms of CPU.
 *
 * setDataCallback: Pass the AudioStreamDataCallback, we can pass this as we have made PlayerController child of AudioStreamDataCallback
 * setErrorCallback: Pass the AudioStreamErrorCallback, we can pass this as we have made PlayerController child of AudioStreamErrorCallback
 *
 * @return true if audio stream opened successfully.
 */
bool PlayerController::openStream() {
    // create an audio stream
    AudioStreamBuilder builder;
    builder.setFormat(AudioFormat::Float)
            ->setFormatConversionAllowed(true)
            ->setPerformanceMode(PerformanceMode::LowLatency)
            ->setSharingMode(SharingMode::Exclusive)
            ->setSampleRate(32000)
            ->setSampleRateConversionQuality(SampleRateConversionQuality::Medium)
            ->setChannelCount(ChannelCount::Stereo)
            ->setDataCallback(this)
            ->setErrorCallback(this);

    Result result = builder.openStream(mAudioStream);
    if (result!=Result::OK){
        LOGE("Failed to open stream. Error: %s", convertToText(result));
        return false;
    }

    return true;
}
/**
 * get the assets as data source, and pass it to the player
 * @return true if data source is loaded successfully and passed to the player.
 */
bool PlayerController::setupAudioSources() {
    //Set the properties of our audio source(s) to match that of our audio stream
    AudioProperties targetProperties{
            .channelCount = mAudioStream->getChannelCount(),
            .sampleRate = mAudioStream->getSampleRate()
    };

    // Create a data source and player for our track
    std::shared_ptr<AAssetDataSource> trackSource{
        AAssetDataSource::newFromCompressedAsset(mAssetManager,trackFilename,targetProperties)
    };
    if (trackSource== nullptr){
        LOGE("Could not load source data for track: %s",trackFilename);
        return false;
    }

    mTrack = std::make_unique<Player>(trackSource);
    mTrack->setPlaying(true);
    mTrack->setLooping(true);

    return true;
}
/**
 * sets the file name to class variable.
 * @param filename : name of asset audio file
 */
void PlayerController::setAudioTrackFilename(char *filename) {
    trackFilename=filename;
}