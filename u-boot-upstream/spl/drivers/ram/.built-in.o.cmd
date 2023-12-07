cmd_spl/drivers/ram/built-in.o :=  arm-none-eabi-ld.bfd   -r -o spl/drivers/ram/built-in.o spl/drivers/ram/ram-uclass.o spl/drivers/ram/stm32_sdram.o 
