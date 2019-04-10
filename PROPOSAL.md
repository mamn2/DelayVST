# Project Proposal
For my final project, I will create a VST audio plugin using the JUCE framework. The VST will take in an audio track, and adjust the sound according to the user input. The project will have two main components: the GUI, and audio processing. 

## GUI
The GUI (Graphical User Interface) will be based on some features provided by the JUCE framework. It will look very similar to classic analog audio devices. For example, if I am using a slider, I can design it as a knob similar to volume knobs on a sound device.

Since this plugin is in an AU format (or VST for Windows users), the graphical interface will be accessed through a Digital Audio Workstation (DAW). For purposes of testing, I will try to use multiple DAWs for demonstration such as GarageBand, Pro Tools, etc.

I will be changing the audio signals using multiple processing techniques which will be separated using tabs. For example one tab may include reverberation, another may include distortion, etc.

## Audio Processing
The audio processing is based on mathematical manipulation of the sine wave provided by the audio signal. This will not be provided by the framework. Instead, the algorithms will be developed based on acoustic research of audio signals. Some types of audio processing that I may implement include: reverberation, distortion, timbre, etc. 

# JUCE
For this project, I will be using the JUCE framework instead of openFrameworks. The reason for this is that the JUCE library is specifically designed to support music applications and therefore provides GUI options that are more catered to music products. It will allow me to display audio data graphically so that the user understands exactly how they are changing the sound. It also supports MIDI input, which will be crucial for any extensions I do on this project beyond the scope of this class.

While my experience in JUCE is rather limited, the documentation is excellent and I have familiarized myself with many classes and the features they provide. I am also somewhat familiar with common audio processing techniques. Since it is crucial for me to learn this framework for the work that I am interested in, I am willing to put an immense amount of effort into this project to overcome the lack of support for the JUCE framework as it is unlikely that the course staff is familar with the tool.

