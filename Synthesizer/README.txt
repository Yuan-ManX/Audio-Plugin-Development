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
    
            }