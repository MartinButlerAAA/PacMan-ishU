rem This batch file runs the .bmp files through a tool that creates a .h header file for the file passed in.
rem The .h file produced is 2 dimensional array of 32-bit integers for 24 bit colour, in the correct format for
rem the WiiU OSScreen pixel function. 
rem
rem see https://github.com/MartinButlerAAA/ProcessBmp for the ProcessBmp tool.
rem
rem This tool can only handle .bmp images. Any other images need to be converted to 24-bit colour .bmp files, which can
rem be done using Microsoft Paint.
rem 
cd .
processBmp AlienA.bmp
processBmp AlienB.bmp
processBmp AlienC.bmp
processBmp PacManA.bmp
processBmp PacManB.bmp
processBmp PacManC.bmp
processBmp Dot.bmp
processBmp BigDot.bmp
processBmp Block.bmp

Pause