cmd_spl/u-boot-spl-nodtb.bin := arm-none-eabi-objcopy -j .text -j .secure_text -j .secure_data -j .rodata -j .hash -j .data -j .got -j .got.plt -j .u_boot_list -j .rel.dyn -j .binman_sym_table -j .text_rest -j .dtb.init.rodata  -O binary  spl/u-boot-spl spl/u-boot-spl-nodtb.bin
