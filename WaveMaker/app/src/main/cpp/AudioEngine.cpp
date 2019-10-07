//
// Created by nitisha on 2/10/19.
//

#include "AudioEngine.h"
#include <thread>
#include<mutex>
#include<android/log.h>

//double-buffering offers a good tradeoff between latency and protection against glitches
constexpr int32_t kBufferSizeInBursts = 2;

//data callback is defined globally
aaudio_data_callback_result_t dataCallbackResult(
        AAudioStream *audioStream_,
        void *userData,
        void *audioData,
        int32_t numFrames
        ){
    //this function could have been replaced by AAudioStream_write but using a datacallback function is best for low latency

    //userData is used as a pointer to Oscillator object which is passed to function render
    ((Oscillator *) (userData))->render(static_cast<float *>(audioData), numFrames);
    //method returns a value that tells audiostream to keep consuming audiodata
    return AAUDIO_CALLBACK_RESULT_CONTINUE;
}
void errorCallback(AAudioStream *audioStream_,
                   void *userData,
                   aaudio_result_t error){
    if (error == AAUDIO_ERROR_DISCONNECTED){
        //this is to handle sudden deviceid change to handle audio stream disconnected error
        std::function<void(void)> restartFunction = std::bind(&AudioEngine::restart,
                                                              static_cast<AudioEngine *>(userData));
        new std::thread(restartFunction);
    }
}

bool AudioEngine::start() {
    //start method sets up an audio stream, this done by making an object AAudioStream
    //to create an audio stream, we need a stream builder
    AAudioStreamBuilder *streamBuilder;  //creating streambuilder object
    AAudio_createStreamBuilder(&streamBuilder); //passing the address of the object to create function
    AAudioStreamBuilder_setFormat(streamBuilder, AAUDIO_FORMAT_PCM_FLOAT);  //audioformat is set to floating point numbers
    AAudioStreamBuilder_setChannelCount(streamBuilder, 1); //sound is being outputted in mono channel
    AAudioStreamBuilder_setPerformanceMode(streamBuilder, AAUDIO_PERFORMANCE_MODE_LOW_LATENCY); //setting performance to low latency
    //performance mode can be found from AAudioStream.getPerformanceMode()
    //the above function however does not guarantee a low latency performance mode


    AAudioStreamBuilder_setDataCallback(streamBuilder, ::dataCallbackResult, &oscillator_);
    AAudioStreamBuilder_setErrorCallback(streamBuilder, ::errorCallback, this);

    // Opens the stream.
    aaudio_result_t result = AAudioStreamBuilder_openStream(streamBuilder, &audioStream_);
    if (result != AAUDIO_OK) {
        __android_log_print(ANDROID_LOG_ERROR, "AudioEngine", "Error opening stream %s",AAudio_convertResultToText(result));
        return false;
    }

    // Retrieves the sample rate of the stream for our oscillator.
    int32_t sampleRate = AAudioStream_getSampleRate(audioStream_);
    oscillator_.setSampleRate(sampleRate);  //setting the sample rate

    // Sets the buffer size.
    //A burst is a discrete amount of data written during each callback.
    AAudioStream_setBufferSizeInFrames(
            audioStream_, AAudioStream_getFramesPerBurst(audioStream_) * kBufferSizeInBursts);

    // Starts the stream.
    result = AAudioStream_requestStart(audioStream_);
    if (result != AAUDIO_OK) {
        __android_log_print(ANDROID_LOG_ERROR, "AudioEngine", "Error starting stream %s",
                            AAudio_convertResultToText(result));
        return false;
    }

    AAudioStreamBuilder_delete(streamBuilder);  //deleting streambuilder as it is no longer needed
    return true;
}

void AudioEngine::restart(){

    static std::mutex restartingLock;
    if (restartingLock.try_lock()){
        stop();
        start();
        restartingLock.unlock();
    }
}

void AudioEngine::stop() {
    if (audioStream_ != nullptr) {
        AAudioStream_requestStop(audioStream_);
        AAudioStream_close(audioStream_);
    }
}

void AudioEngine::setToneOn(bool isToneOn) {
    oscillator_.setWAveOn(isToneOn);
}