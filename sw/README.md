# Firmware
 - the air modem is best programmed with iridium_air.hex which has firmware to automatically answer calls.
 - when also using iridium as ground modem, the board is best flashed with transparent firmware iridium_ground.hex. Use the python tool from the [tools](../tools) folder to dial.

## How to build
To build the software you have to run `make` in the `sw` folder. This will produce both the `iridium_air.elf` and the `iridium_ground.elf`.

## How to flash
To flash you either need to enter `make ground_program` or `make air_program` respectively for the ground and the air module. Note that this is using `avrdude` with an `avrispmkII` programmer.
If needed you can set the programmer using the `AVR_PROG=avrispmkII` make define.
