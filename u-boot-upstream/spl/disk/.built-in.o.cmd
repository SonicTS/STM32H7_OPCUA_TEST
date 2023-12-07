cmd_spl/disk/built-in.o :=  arm-none-eabi-ld.bfd   -r -o spl/disk/built-in.o spl/disk/part.o spl/disk/part_dos.o spl/disk/part_efi.o 
