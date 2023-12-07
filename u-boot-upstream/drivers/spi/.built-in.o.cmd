cmd_drivers/spi/built-in.o :=  arm-none-eabi-ld.bfd   -r -o drivers/spi/built-in.o drivers/spi/spi-uclass.o drivers/spi/spi-mem.o drivers/spi/stm32_qspi.o 
