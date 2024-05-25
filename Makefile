CC=gcc
AS=as

kernel.o: src/kernel.c
	$(TARGET)-$(CC) -c src/kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

boot.o: src/boot.s
	$(TARGET)-$(AS) src/boot.s -o boot.o

kerberos.bin: boot.o kernel.o src/linker.ld
	$(TARGET)-$(CC) -T src/linker.ld -o kerberos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

.DEFAULT_GOAL := kerberos.bin

clean:
	rm -rf *.o *.bin