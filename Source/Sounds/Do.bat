rem 
rem This batch file calls the ProcessWav tool that takes an 8-bit 48000 samples per second .wav files and creates a header 
rem file of the data. WavePad from NCH software can be used to save .mp3 files as .wav in the correct format.
rem 
rem The sounds in these .wav files have been downloaded from Scratch (https://scratch.mit.edu).
rem see https://github.com/MartinButlerAAA/ProcessWav for the ProcessWav tool.
rem
rem The tool converts from 8-bit .wav format (which is 0 to 255 with centre at 128) to the Wii U format which is -128 to 127.
rem If the raw data is used without conversion, it gives a horribly distorted sound.

cd .
ProcessWav BoomCloud.wav
ProcessWav Bite.wav
ProcessWav Cheer.wav
ProcessWav Creak1.wav
ProcessWav DanceAround.wav
Pause