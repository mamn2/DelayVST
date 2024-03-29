/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <iostream>

//==============================================================================
DelayPluginAudioProcessor::DelayPluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    
    createNewParams();
    
    //initializing instance variables
    circularBufferLength = 0;
    leftCircularBuffer = nullptr;
    rightCircularBuffer = nullptr;
    bufferWriteHead = 0;
    delayReadHead = 0;
    delayTimeSamples = 0;
    delayTimeSmoothValue = 0;
    feedbackLeft = 0;
    feedbackRight = 0;
    
}

void DelayPluginAudioProcessor::createNewParams() {
    
    addParameter(gainParam = new AudioParameterFloat("gain", "Gain", 0.0f, 1.0f, 0.5f));
    smoothedGainValue = gainParam->get();
    
    addParameter(delayTimeParam = new AudioParameterFloat("delay", "Delay", 0.0f, 1.99f, 1.0f));
    
    addParameter(dryWetParam = new AudioParameterFloat("drywet", "Dry Wet", 0.01f, MAX_DELAY_TIME, 0.5f));
    
    addParameter(feedbackParam = new AudioParameterFloat("feedback", "Feedback", 0.0f, 0.99f, 0.5f));
    
}

//makes sure we delete all buffer values allocated on the heap
DelayPluginAudioProcessor::~DelayPluginAudioProcessor()
{
    if (rightCircularBuffer != nullptr) {
        delete[] rightCircularBuffer;
    }
    if (leftCircularBuffer != nullptr) {
        delete[] leftCircularBuffer;
    }
}

//==============================================================================
const String DelayPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayPluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayPluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayPluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayPluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayPluginAudioProcessor::setCurrentProgram (int index)
{
}

const String DelayPluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayPluginAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DelayPluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
    // # samples = time * sample rate
    delayTimeSamples = delayTimeParam->get() * sampleRate;
    
    circularBufferLength = (int) sampleRate * MAX_DELAY_TIME;
    
    //creates the left buffer based on the length
    if (leftCircularBuffer == nullptr) {
        leftCircularBuffer = new float[circularBufferLength];
    }
    
    //zeroes all the memory in left buffer
    zeromem(leftCircularBuffer, circularBufferLength * sizeof(float));
    
    //creayes the right buffer based on the length
    if (rightCircularBuffer == nullptr) {
        rightCircularBuffer = new float[circularBufferLength];
    }
    
    //zeroes all the memory in right buffer
    zeromem(rightCircularBuffer, circularBufferLength * sizeof(float));
    
    //start writing at the beginning
    bufferWriteHead = 0;
    
    //smooth value should start at the same value as the time
    delayTimeSmoothValue = delayTimeParam->get();
    
}

void DelayPluginAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelayPluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DelayPluginAudioProcessor::applyGainToChannels(int sample) {
    
    //smooths the gain to control the rate at which the gain changes
    const float gainSmoothCoefficient = 0.004f;
    smoothedGainValue = smoothedGainValue - gainSmoothCoefficient *
    (smoothedGainValue - gainParam->get());
    
    //for every sample inside the channel I am multiplying by half
    channelLeft[sample] *= smoothedGainValue;
    channelRight[sample] *= smoothedGainValue;
    
}

void DelayPluginAudioProcessor::applyDelayToChannels(AudioBuffer<float>& buffer, int sample) {
    
    const float delaySmoothValue = 0.001f;
    
    //smooths the delay parameter so that we don't get bad values between shifting of parameter
    delayTimeSmoothValue = delayTimeSmoothValue - delaySmoothValue
                         * (delayTimeSmoothValue - delayTimeParam->get());
    delayTimeSamples = getSampleRate() * delayTimeSmoothValue;
    
    //adds the feedback look. This is a summation that happens with every sample
    leftCircularBuffer[bufferWriteHead] = channelLeft[sample] + feedbackLeft;
    rightCircularBuffer[bufferWriteHead] = channelRight[sample] + feedbackRight;
    
    delayReadHead = bufferWriteHead - delayTimeSamples;
    
    //make sure we aren't going out of bounds of read head
    if (delayReadHead < 0) {
        delayReadHead += circularBufferLength;
    }
    
    int delayReadHeadConcat = (int) delayReadHead;
    //calculates the decimal value in the read head. Ex: if delayReadHead is 555.5, then delayReadHeadFloat = .5, this is the phase value for linear interpolation
    float delayReadHeadPhase = delayReadHead - delayReadHeadConcat;
    int nextBufferValue = delayReadHeadConcat + 1;
    if (nextBufferValue > circularBufferLength) {
        nextBufferValue -= circularBufferLength;
    }
    
    //interpolates the value so that we can access values in the buffer that are not pure integers
    //If an index is a decimal, it will approximate the values from sample below and sample above.
    float delaySampleLeft = interpolatedValue(leftCircularBuffer[delayReadHeadConcat],
                                              leftCircularBuffer[nextBufferValue],
                                              delayReadHeadPhase);
    float delaySampleRight = interpolatedValue(rightCircularBuffer[delayReadHeadConcat],
                                               rightCircularBuffer[nextBufferValue],
                                               delayReadHeadPhase);
    
    //Causes the feedback look by multiplying the delay by a coefficient, diminishing effect over time
    //These are output signals
    feedbackLeft = delaySampleLeft * feedbackParam->get();
    feedbackRight = delaySampleRight * feedbackParam->get();
    
    //Gets the dry sample before adding the delay
    float drySampleLeft = buffer.getSample(0, sample);
    float drySampleRight = buffer.getSample(1, sample);
    
    //Adds the delay on top of the current buffer
    buffer.setSample(0, sample, drySampleLeft * (1 - dryWetParam->get()) +
                     delaySampleLeft * dryWetParam->get());
    buffer.setSample(1, sample, drySampleRight * (1 - dryWetParam->get()) +
                     delaySampleRight * dryWetParam->get());
    
    //Moves on to the next node in the buffer
    bufferWriteHead++;
    
    //Switches buffer back to 0 once it goes beyond the length, ensures we don't go out of bounds
    if (bufferWriteHead >= circularBufferLength) {
        bufferWriteHead = 0;
    }
    
}

void DelayPluginAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    delayTimeSamples = getSampleRate() * delayTimeParam->get();

    channelLeft = buffer.getWritePointer(0);
    channelRight = buffer.getWritePointer(1);

    // ..do something to the data...
    for (int sample = 0; sample < buffer.getNumSamples(); sample++)
    {
        //----------------GAIN----------------//
        
        applyGainToChannels(sample);
        
        //---------------DELAY----------------//
        
        applyDelayToChannels(buffer, sample);
        
    }
}

//==============================================================================
bool DelayPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DelayPluginAudioProcessor::createEditor()
{
    return new DelayPluginAudioProcessorEditor (*this);
}

//==============================================================================
void DelayPluginAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DelayPluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

float DelayPluginAudioProcessor::interpolatedValue(float sampleOne, float sampleTwo, float phase) {
    return (1 - phase) * sampleOne + phase * sampleTwo;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayPluginAudioProcessor();
}
