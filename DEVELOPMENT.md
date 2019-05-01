# Final Project Development

## Audio Code Design

Audio Processing uses a thread that is SEPARATE from the GUI, MIDI input processor, etc. The digital signal processing portion of this program will NEVER reference the GUI because it will cause the program to crash and will send weird signals to the speakers.

### Signal Smoothing

When changing audio signal parameters with a knob, we often get crackling sounds. This is because the parameter is being set at whatever rate you are changing it, in real time. Because of this, we have discontinuities in our signal, and this causes the processor to send a bunch of crap signals to the output wherever there is a discontinuity. In order to avoid this, DSP engineers use signal smoothing. Here's how I did it:

We have a generic formula for smoothing the signal:
Let x = smoothed value, y = target value, and z = scalar (speed)
x = x - z * (z - y)

z-y is the distance from where we are to where we want to be.

For example, if we set our smoothing coefficient to 0.5, our target value to 1, and our scalar to 0.5, we get this equation:
x = .5 - .5 * (.5 - 1.0)

### Dry and wet signals

A dry signal is the unprocessed raw sound that is being produced by the instrument. A wet signal is the sound that is outputted from the processing block. Ideally, we would like to maintain independent control of our dry signal and wet signal.

In order to accomplish this, we will multiply the input by the dry amount, and the output by the wet amount. The dry and wet amount are determined by user input. For example if the user wants to use only the dry signal, i. e the user doesn't want the signal value to change at all, they can set the dry/wet parameter to 0. If they only want to use the processed signal, they can set the dry wet value to 1. They can also set a value in between to get a mix of both the dry signal and the wet signal.

### Interpolation

Interpolation is a technique for inferring a data point between 2 data points. In digital audio, our time domain is not continuous, it is made up of thousands of samples every second. 

When we change our delay parameter, we have to tell the audio processor to replay the signal after a certain amount of time. However, the audio processor doesn't really have a concept of time, and so instead it uses samples. So we have to tell it how many samples are in between 2 playings. But in order to do this we have to convert time (in seconds) to number of samples. Time is represented as a floating point value, and therefore multiplying it by an integer can get you a value that is not an integer. For example if we set our delay time to be .555 seconds, the delay time converted to samples is 24,475.5. However samples are not floating point values, a sample is represented as an integer (obviously), so there is no index in the buffer for this value. 

We fix this using interpolation. In this case we are going to use a simple method called linear interpolation. If the sample it wants us to read from is 24,475.5 for example, we will take the value at 24,275, and the value at 24,476 and draw a line between them. We will then get the point halfway through thatk line (since the decimal is .5). If the decimal was .33, we would take the value 1/3 the way through, and so on. 

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

### Gain

Gain is arguably the most simple. It is like volume, except it refers to input rather than output. In order to change the gain you just multiply the incoming signal by your desired gain value.

### Delay

Delays are used in a very wide range of DSP effects and are quite fundamental to music plugins.

A delay is an audio effect which takes an input signal, stores it, and then plays it back after a pre-determined period of time. The delay may be played back multiple times to create the sound effect of a repeating, decaying, echo. 

The length of the delay is equal to sample rate * delayTime(in seconds). For example if our sample rate is 44.1K and our delay time is 2 seconds, then the length of the delay is 88.2K samples. 

#### Circular Buffer

Delays are stored in a data structure called a circular buffer in order to conserve memory and avoid overloading the RAM. In the case of delays it is safe to overwrite the old data since the delayed signal will already have played back by then. You can read about circular buffers here: https://en.wikipedia.org/wiki/Circular_buffer


## Testing

Testing is fairly difficult with audio files. I have found that the most effective way of testing is by adjusting your parameters on a sine wave. We use a sine wave because it has low harmonics and allows you to hear what is wrong with your plugin far more effectively than say, a piano sound, since piano has a lot of overtones.
