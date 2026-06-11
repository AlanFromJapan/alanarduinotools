Fakes the response of a Hatteker Shaver batter on the "IN+" pin of the battery. It waits for the signal on PB2 using interrupt INT0, and generates a reply pulse on PB0.

Whole story and details on main site : https://electrogeek.cc/repair%20-%20electric%20shaver%20battery%20cloning.html

# Pinout
```
                   +-\/-+
     <Output>  PB0 |o   | PB3  
               GND |    | VCC
               PB1 |____| PB2  >INT0 input<
```


# Build
Use `avra`: `sudo apt install avra`

Build with `avra -I /usr/share/avra hatteker_battery_faker.asm`


# Upload

Command `avrdude -p t10 -cavrispmkII  -U flash:w:hatteker_battery_faker.hex:i -B12 -v`

## Using my proto board
Remember these, future me:
- Board has to be powered 5v (top left connector)
- power ON (green LED ON)
- RESET button PRESSED
- run the command
- do a force reset (press 1x - 3x the RESET button)

That should work.
