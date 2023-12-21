#!/bin/bash
# Calculates HRV from input of raw binary file
file=$1
name="${file%.*}"
echo $name
ext="${file##*.}"
echo $ext

size=$(stat -f%z "$name.bin")
echo "Size of file = $size bytes."

# Assume Stream of Floats
SizeOfFloat=4   # in bytes

# Double Parentheses for arithmetic
Ndata=$(( $size / $SizeOfFloat ))
echo $Ndata

python << EOF

import numpy as np
from scipy.signal import detrend

import struct

from subprocess import Popen, PIPE, STDOUT

dt = 0.050	# 50 ms
tSeg = 5*60 # 5 minutes
nElm = int(tSeg/dt)

FLOAT_SIZE = $SizeOfFloat

Ndata = $Ndata

# int() truncates
nSeg = int(round(Ndata/nElm))
print("nSeg: ", nSeg)

vlf_cut = 0.4 # Hz
lf_cut = 1.5

N = 512 # frequencies
W = 2*N # window
dt = 50 #ms
T = W*dt
T /= 1000 # 51.2 s
# frequencies
F = np.arange(N) / T
indVLF = (F<=vlf_cut).nonzero()
indLF = ((F>vlf_cut) & (F<=lf_cut)).nonzero()
indHF = ((F>lf_cut) & (F <= 4.0)).nonzero()

Ptot = np.zeros(nSeg)
Pvlf = np.zeros(nSeg)
Plf = np.zeros(nSeg)
Phf = np.zeros(nSeg)

# Command for calculating PSD
cmd = "ApplyPSD.sh temp"
# temp.bin is a temporary file

with open("$file", "rb") as file:
    for s in range(nSeg): 
        X = np.array( struct.unpack('f'*nElm, file.read(FLOAT_SIZE*nElm)) ) 
        Xd = detrend(X,type='linear') 

        buf = struct.pack( 'f'*nElm, *Xd ) 
        open('temp.bin', 'wb').write(buf)

        p = Popen(cmd, shell=True, stdout=PIPE, close_fds=True) 
        P = np.array( [float(x.decode('utf8')) for x in p.stdout.readlines()] )

        Ptot[s] = P.sum() 
        Pvlf[s] = P[indVLF].sum() 
        Plf[s] = P[indLF].sum() 
        Phf[s] = P[indHF].sum() 

np.savetxt("Ptot.dat", Ptot)
np.savetxt("Pvlf.dat", Pvlf)
np.savetxt("Plf.dat", Plf)
np.savetxt("Phf.dat", Phf)

quit()

EOF

# Clean up
rm temp.bin
