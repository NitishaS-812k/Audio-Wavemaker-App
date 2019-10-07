//
// Created by nitisha on 2/10/19.
//

#ifndef WAVEMAKER_AUDIOENGINE_H
#define WAVEMAKER_AUDIOENGINE_H

#include "Oscillator.h"
#include <aaudio/AAudio.h>

class AudioEngine {
public:
    bool start();
    void stop();
    void restart();
    void setToneOn(bool isToneOn);
private:
    Oscillator oscillator_;
    AAudioStream *audioStream_;

};


#endif //WAVEMAKER_AUDIOENGINE_H
