【Synthesizer】
1、OSC 
	Sine Wave std::sin(x);
	Saw Wave x / juce::MathConstants<float>::pi;
	Square Wave x < 0.0f ? -1.0f : 1.0f;

2、Gain
	juce::dsp::Gain<float> gain;

3、prepareToPlay
          void SynthesizerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
          {
   	 juce::dsp::ProcessSpec spec;
    	spec.maximumBlockSize = samplesPerBlock;
    	spec.numChannels = getTotalNumOutputChannels();
    	spec.sampleRate = sampleRate;

    	osc.prepare(spec);
    	gain.prepare(spec);

    	osc.setFrequency(220.0f);
    	gain.setGainLinear(0.01f);

	// synth
   	synth.setCurrentPlaybackSampleRate(sampleRate);
           }

4、processBlock
           void SynthesizerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
           {
    	juce::ScopedNoDenormals noDenormals;
    	auto totalNumInputChannels  = getTotalNumInputChannels();
    	auto totalNumOutputChannels = getTotalNumOutputChannels();
 
    	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        	buffer.clear (i, 0, buffer.getNumSamples());

    	juce::dsp::AudioBlock<float> audioBlock{ buffer };
  	osc.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));
  	gain.process(juce::dsp::ProcessContextReplacing<float>(audioBlock));

	// synth
    	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    	{
        	       if (auto voice = dynamic_cast<juce::SynthesiserVoice*>(synth.getVoice(i)))
        	       {
            	            // Osc controls
            	            // ADSR
            	            // LFO
        	       }
    	}

    	synth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());
    
    
            }

5、SynthSound
         class SynthSound : public juce::SynthesiserSound
         {
         public:
    	bool appliesToNote(int midiNoteNumber) override { return true; }
    	bool appliesToChannel(int midiChannel) override { return true; }

         };

6、SynthOsc
        class SynthOsc : public juce::SynthesiserVoice
        {
         public:
    	bool canPlaySound (juce::SynthesiserSound* sound) override;
    	void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound* sound, int currentPitchWheelPosition) override;
    	void stopNote (float velocity, bool allowTailOff) override;
    	void controllerMoved (int controllerNumber, int newControllerValue) override;
    	void pitchWheelMoved(int newPitchWheelValue) override;
    	void renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

         private:

        };

7、Synth
	juce::Synthesiser synth;




