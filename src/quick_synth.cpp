#include "quick_synth.h"


cst_val *flite_set_voice_list() {
   flite_voice_list = cons_val(voice_val(register_cmu_us_kal16(NULL)), flite_voice_list);
   flite_voice_list = cons_val(voice_val(register_cmu_us_rms(NULL)), flite_voice_list);
   flite_voice_list = cons_val(voice_val(register_cmu_us_slt(NULL)), flite_voice_list);
   flite_voice_list = val_reverse(flite_voice_list);
   return flite_voice_list;
}


QuickSynth::QuickSynth() {
}

QuickSynth::~QuickSynth() {
    rendering.lock(); // wait for rendering to stop

    for(SoundBuffer*& buf : renders) {
        delete buf;
        buf = nullptr;
    }
}

bool QuickSynth::TextToSpeech(const char* text, const char* voice){
    rendering.lock();

    flite_init();
    flite_voice_list = flite_set_voice_list();
    
    cst_voice *v;
    v = flite_voice_select(voice);

    cst_audio_streaming_info* asi = new_audio_streaming_info();
    asi->asc = QuickSynth::custom_audio_stream_chunk;
    asi->userdata = this;
    
    feat_set(v->features,"streaming_info",audio_streaming_info_val(asi));
    
    float durs = flite_text_to_speech(text, v, "stream");

    return true;
}

void QuickSynth::WaitForSynth() {
    for(SoundBuffer* buf : renders) {
        while(buf->isPlaying()) std::this_thread::sleep_for(std::chrono::milliseconds(2));
    }
}



int QuickSynth::custom_audio_stream_chunk(const cst_wave *w, int start, int size, int last, void *user) {
    QuickSynth* _this = (QuickSynth*)user;
    SoundBuffer* snd;

    if(start == 0){
        snd = new SoundBuffer;
        snd->preallocateSamples(w->num_samples, w->sample_rate, w->num_channels, 2);
        _this->renders.push_back(snd);
    } else {
        snd = _this->renders.back();
    }

    snd->appendSamples(w->samples + start, size);
    
    if(last){
        snd->play(true);
        _this->rendering.unlock(); // finally free the render lock
    }

    return 0;
}