cmd_fs/built-in.o :=  arm-none-eabi-ld.bfd   -r -o fs/built-in.o fs/fs.o fs/fat/built-in.o fs/ubifs/built-in.o fs/fs_internal.o 
