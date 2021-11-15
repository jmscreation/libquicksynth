#pragma once

#include "flite.h"
#include "audio.h"

#include <vector>
#include <thread>
#include <chrono>
#include <mutex>

extern "C" { // manually link against voice library
    cst_voice *register_cmu_us_rms(const char *voxdir);
    cst_voice *register_cmu_us_slt(const char *voxdir);
    cst_voice *register_cmu_us_kal16(const char *voxdir);
};

class QuickSynth {
    std::vector<SoundBuffer*> renders;
    std::mutex rendering;
public:
    static int custom_audio_stream_chunk(const cst_wave *w, int start, int size, int last, void *user);

    QuickSynth();
    virtual ~QuickSynth();

    bool TextToSpeech(const char* text, const char* voice=nullptr);
    void WaitForSynth();
};