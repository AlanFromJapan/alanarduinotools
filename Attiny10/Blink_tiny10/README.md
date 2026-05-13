Just a small memo, this is a poor blink example, but it builds and blinks so [x] purpose served!

# Build
Use `avra`: `sudo apt install avra`

Build with `avra -I /usr/share/avra Blink_tiny10.asm`

NB: had to add the include to the .inc file in the file (file was originally made with AVR Studio if memory serves).

# Upload

Command `avrdude -p t10 -cavrispmkII  -U flash:w:Blink_tiny10.hex:i -B12 -v`

## Using my proto board
Remember these, future me:
- Board has to be powered 5v (top left connector)
- power ON (green LED ON)
- RESET button PRESSED
- run the command
- do a force reset (press 1x - 3x the RESET button)

That should work.
