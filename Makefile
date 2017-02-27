
tassimo.o: tassimo.c
	arm-none-eabi-gcc -ffreestanding -nostdlib -nostartfiles -c tassimo.c


dumper.o: dumper.c
	arm-none-eabi-gcc -ffreestanding -nostdlib -nostartfiles -c dumper.c

dumper.elf: dumper.o tassimo.o dumper.ld
	arm-none-eabi-ld -o dumper.elf -T dumper.ld dumper.o tassimo.o -Map dumper.map

dumper.hex: dumper.elf
	arm-none-eabi-objcopy -O ihex dumper.elf dumper.hex



blinky.o: blinky.c
	arm-none-eabi-gcc -ffreestanding -nostdlib -nostartfiles -c blinky.c

blinky.elf: blinky.o tassimo.o dumper.ld
	arm-none-eabi-ld -o blinky.elf -T dumper.ld blinky.o tassimo.o -Map blinky.map

blinky.hex: blinky.elf
	arm-none-eabi-objcopy -O ihex blinky.elf blinky.hex

