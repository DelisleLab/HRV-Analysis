# HRV-Analysis

The purpose of this project is to create an enviroment for the efficient calculation of HRV measures with a resolution sufficient to resolve relevant fluctuations of heart rate. 

The PSD.c file is from the the Power Spectral Estmation algorithm in Chapter 13 of Numerical Recipes

https://numerical.recipes/

AsciiToBin.cc is a helper function to convert ascii files to binary files to speed up communication with the hard drive.

To generate executabiles, first Download and install FFTW libraries as a prerequisite 
https://www.fftw.org/

Once you have FFTW installed,  Use C compilers to make executables:

gcc PSD.c -lfftw3 -lm -o psd

g++ AsciiToBin.cc -o asc2bin

Place these executables into a "bin" folder on your systems PATH environmental variable.

The "asc2bin" command is utilized in the shell script "Asc2Bin.sh". This script converts a data file into binary to 
speed-up loading of data for processing. For example,

Asc2Bin.sh WT13-pre-rr.dat

Then, you can run the  script "ApplyPSD.sh" which sets up the overlap and number of segment parameters based on the size of floats and the size of the binary file.

ApplyPSD.sh WT13-pre-rr.bin




