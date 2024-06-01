# KerberOS

KerberOS is a work-in-progress Operating System that is built from the articles provided on the [OSDev Wiki](https://wiki.osdev.org). 

## Multiboot Standard
The KerberOS kernel is multiboot1 compliant. 

## Development Environment
A cross-compiler toolchain and debugging tools have been set up for the `i686-elf` platform using a [Docker container](https://hub.docker.com/r/joshwyant/gcc-cross/) that was linked on the [OSDev Wiki article](https://osdev.org/GCC_Cross-Compiler) for setting up a cross-compiler toolchain. Additionally, the following packages were added to the Docker image:
- GDB 14.2 accessible as `i686-elf-gdb`
- `grub-pc-bin`
- `xorriso`

The recommended way to access the Development Environment is through VSCode using the Dev Containers extension. The `.devcontainer/devcontainer.json` file details the configuration for use with VSCode. 

## Running the Kernel
Use the command `./qemu.sh` to run the kernel on QEMU within the specified Docker container. If you are having issues connected to the xserver on MacOS, install XQuartz. Under the XQuartz menu, select Preferences
Go to the security tab and ensure "Allow connections from network clients" is checked. This will allow the Docker container to connect to the xserver on your Mac.