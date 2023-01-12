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
	void prepareToPlay(double sampleRate, int samplesPerBlock, int outputChannels);
    	void renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override;

         private:
	 // juce::dsp::Oscillator<float> osc{ [](float x) { return std::sin(x); } };
    	juce::dsp::Oscillator<float> osc{ [](float x) { return x / juce::MathConstants<float>::pi; } };
    	// Sine Wave std::sin(x);
    	// Saw Wave x / juce::MathConstants<float>::pi;
    	// Square Wave x < 0.0f ? -1.0f : 1.0f;

    	juce::dsp::Gain<float> gain;

    	bool isPrepared{ false };


    	juce::ADSR adsr;
    	juce::ADSR::Parameters adsrParams;

        };

7、Synth
	juce::Synthesiser synth;


8、ADSR
	juce::ADSR adsr;
	juce::ADSR::Parameters adsrParams;

9、Editor
          class SynthesizerAudioProcessorEditor  : public juce::AudioProcessorEditor
          {
          public:
   	 SynthesizerAudioProcessorEditor (SynthesizerAudioProcessor&);
    	~SynthesizerAudioProcessorEditor() override;

    	//==============================================================================
    	void paint (juce::Graphics&) override;
    	void resized() override;

          private:
    
    	// params
    	juce::Slider attackSlider;
    	juce::Slider decaySlider;
   	juce::Slider sustainSlider;
    	juce::Slider releaseSlider;
    	juce::ComboBox oscSelector;
 
    	using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    	std::unique_ptr<SliderAttachment> attackAttachment;
    	std::unique_ptr<SliderAttachment> decayAttachment;
    	std::unique_ptr<SliderAttachment> sustainAttachment;
    	std::unique_ptr<SliderAttachment> releaseAttachment;
    	std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscSelAttachment;

    	SynthesizerAudioProcessor& audioProcessor;

    	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SynthesizerAudioProcessorEditor)
           };


          SynthesizerAudioProcessorEditor::SynthesizerAudioProcessorEditor (SynthesizerAudioProcessor& p)
             : AudioProcessorEditor (&p), audioProcessor (p)
          {
    	setSize (600, 400);

   	using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    	attackAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "ATTACK", attackSlider);
    	decayAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "DECAY", decaySlider);
    	sustainAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "SUSTAIN", sustainSlider);
    	releaseAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "RELEASE", releaseSlider);
    	oscSelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(audioProcessor.apvts, 
        	"OSC", oscSelector);
          }



