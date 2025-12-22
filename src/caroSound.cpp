#include "caroSound.hpp"

AudioSystem::AudioSystem() {
    ma_result result;

    result = ma_engine_init(NULL, &engine);
    if (result != MA_SUCCESS) {
        std::cerr << "Failed to initialize audio engine." << std::endl;
        exit(1);
    }

    result = ma_sound_group_init(&engine, 0, NULL, &sfxGroup);
    if (result != MA_SUCCESS) {
        std::cerr << "Failed to initialize SFX group." << std::endl;
        exit(1);
    }

    result = ma_sound_group_init(&engine, 0, NULL, &bgmGroup);
    if (result != MA_SUCCESS) {
        std::cerr << "Failed to initialize BGM group." << std::endl;
        exit(1);
    }
}

AudioSystem::~AudioSystem() {
    ma_sound_group_uninit(&sfxGroup);
    ma_sound_group_uninit(&bgmGroup);
    ma_engine_uninit(&engine);
}

void AudioSystem::playSFX(const std::string& filepath) {
    ma_engine_play_sound_ex(&engine, filepath.c_str(), &sfxGroup, 0);
}

void AudioSystem::playBGM(const std::string& filepath) {
    if (bgmLoaded) {
        ma_sound_uninit(&bgmTrack);
    }

    ma_result result = ma_sound_init_from_file(
        &engine,
        filepath.c_str(),
        MA_SOUND_FLAG_STREAM,
        &bgmGroup,
        NULL,
        &bgmTrack
    );

    if (result == MA_SUCCESS) {
        ma_sound_set_looping(&bgmTrack, MA_TRUE);
        ma_sound_start(&bgmTrack);
        bgmLoaded = true;
    }
    else {
        std::cerr << "Failed to load BGM: " << filepath << std::endl;
    }
}

void AudioSystem::setSFXVolume(float volume) {
    ma_sound_group_set_volume(&sfxGroup, volume);
}

void AudioSystem::setBGMVolume(float volume) {
    ma_sound_group_set_volume(&bgmGroup, volume);
}

void AudioSystem::toggleSFX() {
    sfxMuted = !sfxMuted;
    ma_node_set_output_bus_volume(&sfxGroup, 0, sfxMuted ? 0.0f : 1.0f);
}

void AudioSystem::toggleBGM() {
    bgmMuted = !bgmMuted;
    ma_node_set_output_bus_volume(&bgmGroup, 0, bgmMuted ? 0.0f : 1.0f);
}