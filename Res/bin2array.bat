cd ..\Res 
srec_cat.exe sine130.bin -Binary -o freqLow.c -C-Array freqLow -INClude
srec_cat.exe sine165.bin -Binary -o freqMid.c -C-Array freqMid -INClude
srec_cat.exe sine200.bin -Binary -o freqHigh.c -C-Array freqHigh -INClude
