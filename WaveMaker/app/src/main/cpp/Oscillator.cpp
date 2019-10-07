//
// Created by nitisha on 2/10/19.
//

#include "Oscillator.h"
#include <math.h>

#define TWO_PI (3.14159 * 2)
#define AMP 0.3   //defining Amplitude
#define FREQ 1000.0  //defining Frequency

void Oscillator::setSampleRate(int32_t sampleRate) {
    phaseIncrement_ = (TWO_PI * FREQ) / (double) sampleRate;  //calculates phase increment using frequency and sample rate
}

void Oscillator::setWAveOn(bool isWaveOn) {
    isWaveOn_.store(isWaveOn);    //setter method to determine if the wave is on or silent
}

void Oscillator::render(float *audioData, int32_t numFrames) {
    //this function puts floating point sine values into audio data whenever it is called

    if (!isWaveOn_.load()) phase_ = 0;  //if wave is silent set phase to 0

    for (int i = 0; i < numFrames; i++) {

        if (isWaveOn_.load()) {

            // Calculates the next sample value for the sine wave.
            audioData[i] = (float) (sin(phase_) * AMP);

            // Increments the phase, handling wrap around.
            phase_ += phaseIncrement_;
            if (phase_ > TWO_PI) phase_ -= TWO_PI;

        } else {
            // Outputs silence by setting sample value to zero.
            audioData[i] = 0;
        }
    }
}
