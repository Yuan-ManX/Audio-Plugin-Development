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
    osc.setFrequency (juce::MidiMessage::getMidiNoteInHertz (midiNoteNumber));
    adsr.noteOn();
}

void SynthOsc::stopNote(float velocity, bool allowTailOff)
{
    adsr.noteOff();

    if (!allowTailOff || !adsr.isActive())
    {
        clearCurrentNote();
    }
}
void SynthOsc::controllerMoved(int controllerNumber, int newControllerValue)
{

}

void SynthOsc::pitchWheelMoved(int newPitchWheelValue)
{

}

void SynthOsc::prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels)
{
    adsr.setSampleRate(sampleRate);
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = outputChannels;
    spec.sampleRate = sampleRate;

    osc.prepare(spec);
    gain.prepare(spec);

    // osc.setFrequency(220.0f);
    gain.setGainLinear(0.01f);

    adsrParams.attack = 0.8f;
    adsrParams.decay = 0.8f;
    adsrParams.sustain = 1.0f;
    adsrParams.release = 1.5f;

    adsr.setParameters(adsrParams);

    isPrepared = true;
}

void SynthOsc::renderNextBlock(juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
    jassert (isPrepared);

    if (!isVoiceActive())
        return;

    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();

    juce::dsp::AudioBlock<float> audioBlock{ synthBuffer };
    osc.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
    gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

    adsr.applyEnvelopeToBuffer(synthBuffer, 0, synthBuffer.getNumSamples());

    if (startSample != 0)
    {
        jassertfalse;
    }

    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);

        if (!adsr.isActive())
        {
            clearCurrentNote();
        }
    }
}