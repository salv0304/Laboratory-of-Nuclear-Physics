# SiPM gain analysis

The gain is obtained by measuring the mean distance between the peaks in a histogram and converting ADC units into charge. 
In the repo you will find the root macro gain_all.txt, a simple script conversione.py to convert fit parameters into the breakdown voltage of the SiPM, and the data files.

## How to use the macro gain_all.C

-Ensure the input file is in .txt format and contains the data to be analyzed.

-Place the file in the same directory as the macro or provide the correct path.

-Open the macro and adjust the parameters as needed to:

  -Select the appropriate peaks.
  
  -Configure the Gaussian fit for accurate analysis.

  After you run the macro, the terminal will prompt you to input the gain of the digitizer (in dB). The digitizer gain (in dB) is specified in the name of the .txt file provided.
