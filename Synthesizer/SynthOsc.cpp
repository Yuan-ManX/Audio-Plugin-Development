/*
  ==============================================================================

    SynthOsc.cpp

  ==============================================================================
*/

#include "SynthOsc.h"


bool SynthOsc::canPlaySound(juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthOsc::startNote(int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition)
{

}

void SynthOsc::stopNote(float velocity, bool allowTailOff)
{

}
void SynthOsc::controllerMoved(int controllerNumber, int newControllerValue)
{

}

void SynthOsc::pitchWheelMoved(int newPitchWheelValue)
{

}

void SynthOsc::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{

}