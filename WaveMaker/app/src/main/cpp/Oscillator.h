//
// Created by nitisha on 2/10/19.
//

#ifndef WAVEMAKER_OSCILLATOR_H
#define WAVEMAKER_OSCILLATOR_H

#include <atomic>
#include<stdint.h>


class Oscillator {

public:
    void setWAveOn(bool isWaveOn);
    void setSampleRate(int32_t SampleRate);
    void render(float *audioData, int32_t numFrames);
private:
    std::atomic<bool> isWaveOn_{false};
    double phase_ = 0.0;
    double phaseIncrement_ = 0.0;
};


#endif //WAVEMAKER_OSCILLATOR_H
