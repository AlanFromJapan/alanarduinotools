avrdude  -c avrispv2 -p attiny2313 -P COM1 -u -q -U flash:w:.\Debug\AsmSNESPad.hex:i 
@REM avrdude  -c stk500v2 -p attiny2313 -P COM1 -u -q -U flash:w:.\Debug\AsmSNESPad.hex:i 
pause
