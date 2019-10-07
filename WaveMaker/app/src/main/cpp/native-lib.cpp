#include <jni.h>
#include "AudioEngine.h"
#include<android/input.h>
#include <string>

static AudioEngine *audioEngine = new AudioEngine(); //creating a static instance of AudioEngine class

extern "C" {
JNIEXPORT void JNICALL
Java_com_example_wavemaker_MainActivity_touchEvent(JNIEnv *jniEnv, jobject jobject1, jint action) {
    switch (action) {
        case AMOTION_EVENT_ACTION_DOWN:
            audioEngine->setToneOn(true);
            break;
        case AMOTION_EVENT_ACTION_UP:
            audioEngine->setToneOn(false);
            break;
        default:
            break;
    }
}
JNIEXPORT void JNICALL
Java_com_example_wavemaker_MainActivity_startEngine(JNIEnv *env, jobject /* this */) {
    audioEngine->start();
}
JNIEXPORT void JNICALL
Java_com_example_wavemaker_MainActivity_stopEngine(JNIEnv *env, jobject /* this */) {
    audioEngine->stop();

}
}
