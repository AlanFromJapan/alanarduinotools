Fakes the response of a Hatteker Shaver batter on the "IN+" pin of the battery.

Whole story and details on main site : https://electrogeek.cc/repair%20-%20electric%20shaver%20battery%20cloning.html

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
