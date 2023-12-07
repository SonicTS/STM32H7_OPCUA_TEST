cmd_spl/drivers/spi/built-in.o :=  arm-none-eabi-ld.bfd   -r -o spl/drivers/spi/built-in.o spl/drivers/spi/spi-uclass.o spl/drivers/spi/spi-mem.o spl/drivers/spi/stm32_qspi.o 
