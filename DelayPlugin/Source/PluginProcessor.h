/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#define MAX_DELAY_TIME 2

//==============================================================================
/**
*/
class DelayPluginAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    DelayPluginAudioProcessor();
    ~DelayPluginAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:

    //user input parameters
    AudioParameterFloat* gainParam;
    
    AudioParameterFloat* delayTimeParam;
    
    //dry, wet signal values. See DEVELOPMENT.md if you are unfamiliar with dry/wet
    AudioParameterFloat* dryWetParam;
    
    AudioParameterFloat* feedbackParam;
    
    //gain values
    float smoothedGainValue;
    
    //delay values
    float* leftCircularBuffer;
    float* rightCircularBuffer;
    int bufferWriteHead;
    int circularBufferLength;
    float delayTimeSamples;
    float delayReadHead;
    
    //feedback values
    float feedbackLeft;
    float feedbackRight;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayPluginAudioProcessor)
};
