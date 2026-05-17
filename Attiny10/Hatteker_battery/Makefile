
build:
	avra -I /usr/share/avra hatteker_battery_faker.asm

clean:
	rm -f hatteker_battery_faker.obj
	rm -f hatteker_battery_faker.hex
	rm -f hatteker_battery_faker.eep.hex

upload:
	avrdude -p t10 -cavrispmkII  -U flash:w:hatteker_battery_faker.hex:i -B12 