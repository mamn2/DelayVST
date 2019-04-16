# Final Project Development

## Audio Code Design

Audio Processing uses a thread that is SEPARATE from the GUI, MIDI input processor, etc. The digital signal processing portion of this program will NEVER reference the GUI because it will cause the program to crash and will send weird signals to the speakers.

## Cross Platform Audio Development

Different platforms (Mac, Windows, etc.) sometimes use different plug-in formats. For example, Windows uses VST and most cross-platform DAWs like Ableton Live will also use VSTs. Apple invented their own plugin format called AU which is used for GarageBand, etc. JUCE framework allows for cross-platform development of audio plugins so that we don't need to write separate code for the plugins.

## Potential DSP algortithms

### Clipping

In order to create distortion we need to clip an audio signal past a certain amplitude. 

In my distortion effect I can use 3 inputs:

* Drive: affects the gain, increasing the drive creates a "fuzzy," or "growling" sound. It works by clipping the signal (pushing it past its max, sheering off the peaks and troughs of the signal wave).
* Blend: affects how much a user wants to blend the distorted sound with the clean sound. Increasing the blend will increase the amount of distorted sound.
* Volume: knob just changes the volume.

Here is my formula for the clipping effect: 
currentsig = (((( (2 / pi) * atan(currentsig) ) * blend) + (cleansig * (1 - blend)) ) / 2 ) * volume) * drive
