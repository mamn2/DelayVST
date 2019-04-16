/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayPluginAudioProcessorEditor::DelayPluginAudioProcessorEditor (DelayPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    auto& params = processor.getParameters();
    AudioParameterFloat* gainParameter = (AudioParameterFloat*) params.getUnchecked(0);
    
    //Setting up gain control
    gainControlSlider.setBounds(0, 0, 100, 100);
    gainControlSlider.setRange(gainParameter->range.start, gainParameter->range.end);
    gainControlSlider.setValue(*gainParameter);
    gainControlSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    gainControlSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
    
    //Lambda function for updating the value of the gain parameter
    gainControlSlider.onValueChange = [this, gainParameter] {
        *gainParameter = gainControlSlider.getValue();
    };
    
    addAndMakeVisible(gainControlSlider);
    
}

DelayPluginAudioProcessorEditor::~DelayPluginAudioProcessorEditor()
{
}

//==============================================================================
void DelayPluginAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void DelayPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

