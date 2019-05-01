# Final Project

## Delay Plugin

This project contains a delay plugin. It contains 3 parameters to change the features of the delay - Delay Time, Feedback, and Dry/Wet

### Delay Time

Changing the delay time will change the amount of time in between playings of a note. For example if my delay time is 2 seconds and I'm applying the delay to a note A4, A4 will play every 2 seconds. If it change the delay time to 1 second, A4 will play every second.

### Feedback

The feedback parameter changes how loud it will play when it is repeated. This value must be between 0 and .99 in order to effectively apply a delay. So for example, if it is set to .8, then each playing of a note will be 0.8 * (volume_of_previous_playing). Eventually the note will dissipate to a point that you won't be able to hear it anymore.

### Dry/Wet

The Dry/Wet parameter will allow you to change how much you want to apply the distorted signal. If the parameter is completely dry, then none of the delay features will be applied to the signal. If it is completely wet, the processed signal will be fully applied. Any value between will be a mix. 

## Gain Parameter

A parameter separate from the delay values is the gain parameter. You can think of this like volume, except it is applied to input rather than output. So if we have full gain then the amplitude will be as high as it can reach, halfway through will record at half the amplitude, etc.
