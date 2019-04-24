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
    
    connectSliderToParam(0, 0, 60, 100, 100, gainControlSlider);
    connectSliderToParam(1, 100, 60, 100, 100, delayTimeSlider);
    connectSliderToParam(2, 200, 60, 100, 100, dryWetSlider);
    connectSliderToParam(3, 300, 60, 100, 100, feedbackSlider);

}

DelayPluginAudioProcessorEditor::~DelayPluginAudioProcessorEditor()
{
}

//==============================================================================
void DelayPluginAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::azure);
    g.setFont (30.0f);
    g.drawFittedText ("Mohamed's Drive Plugin", getLocalBounds(), Justification::centredTop, 1);
    
    g.setFont(15.0f);
    g.setColour(Colours::white);
    g.drawText("Gain", 20, 150, 60, 10, Justification::centred, false);
    g.drawText("Delay", 120, 150, 60, 10, Justification::centred, false);
    g.drawText("Dry/Wet", 220, 150, 60, 10, Justification::centred, false);
    g.drawText("Feedback", 320, 150, 60, 10, Justification::centred, false);

}

void DelayPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

void DelayPluginAudioProcessorEditor::connectSliderToParam(int paramNum, int x, int y,
                                                            int width, int height, Slider& slider) {
    
    auto& params = processor.getParameters();
    AudioParameterFloat* thisParam = (AudioParameterFloat*) params.getUnchecked(paramNum);
    
    //Setting up gain control
    slider.setBounds(x, y, width, height);
    slider.setRange(thisParam->range.start, thisParam->range.end);
    slider.setValue(*thisParam);
    slider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    slider.setTextBoxStyle(Slider::TextBoxBelow, true, 0, 0);
    slider.setTextValueSuffix("Gain");
    slider.setTextBoxIsEditable(false);
    
    //Lambda function for updating the value of the gain parameter
    slider.onValueChange = [this, thisParam, &slider] {
        *thisParam = slider.getValue();
    };
    
    //Notifies host that drag gesture has begun
    //Meant mostly for debuggin and cross-platform purposes
    slider.onDragStart = [thisParam] {
        thisParam->beginChangeGesture();
    };
    
    //Notifies host that drag gesture has ended
    slider.onDragEnd = [thisParam] {
        thisParam->endChangeGesture();
    };
    
    
    addAndMakeVisible(slider);
    
}

