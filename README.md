# HRV-Analysis

Use the C compilers to make executables:

gcc PSD.c -lfftw3 -lm -o psd

g++ AsciiToBin.cc -o asc2bin

Place these executables into your "bin" folder

The "asc2bin" command is utilized in the shell script "Asc2Bin.sh". This script converts a data file into binary to 
speed-up loading of data for processing.

The shell script "ApplyPSD.sh" sets up the overlap and number of segment parameters. It is utilized by the shell 
script "RunHRvar.sh"

Asc2Bin.sh WT13-pre-rr.dat
