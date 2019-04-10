# Final Project Development

## Clipping

Explored the clipping effect of audio signals. In order to create distortion we need to clip an audio signal past a certain amplitude. 

In my distortion effect I will be using 3 inputs:

* Drive: affects the gain, increasing the drive creates a "fuzzy," or "growling" sound. It works by clipping the signal (pushing it past its max, sheering off the peaks and troughs of the signal wave).
* Blend: affects how much a user wants to blend the distorted sound with the clean sound. Increasing the blend will increase the amount of distorted sound.
* Volume: knob just changes the volume.

Here is my formula for the clipping effect: 
currentsig = (((( (2 / pi) * atan(currentsig) ) * blend) + (cleansig * (1 - blend)) ) / 2 ) * volume) * drive
