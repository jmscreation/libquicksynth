// CPP Implementation

#include <thread>
#include <chrono>

#include "quick_synth.h"

int main() {
    QuickSynth tts;

    tts.TextToSpeech("This is a test!", "kal16");
    tts.TextToSpeech("Additional Text");
    tts.WaitForSynth();
    tts.TextToSpeech("This is amazing", "kal16");
    tts.WaitForSynth();
    tts.TextToSpeech("I am a user");
    tts.WaitForSynth();
    tts.TextToSpeech("I am voice synthesiser", "kal16");
    tts.WaitForSynth();
    tts.TextToSpeech("Synthesis");

    tts.WaitForSynth();
    tts.TextToSpeech("This is the last text to speech test before the program will close.", "kal16");

    tts.WaitForSynth();
    return 0;
}