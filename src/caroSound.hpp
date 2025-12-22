#include "miniaudio.h"

#include <iostream>
#include <string>
#include <vector>

class AudioSystem {
private:
    ma_engine engine;
    ma_sound_group sfxGroup;
    ma_sound_group bgmGroup;

    bool sfxMuted = false;
    bool bgmMuted = false;

public:
    AudioSystem();
    ~AudioSystem();

    void playSFX(const std::string& filepath);

    ma_sound bgmTrack;
    bool bgmLoaded = false;

    void playBGM(const std::string& filepath);

    void setSFXVolume(float volume);
    void setBGMVolume(float volume);

    void toggleSFX();

    void toggleBGM();
};