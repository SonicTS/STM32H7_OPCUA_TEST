cmd_dts/dt.dtb := cat arch/arm/dts/stm32h7-som-nor.dtb > dts/dt.dtb | ./tools/mkimage -f dts/dummy.its dts/dummy.fit -k "keys" -r -K dts/dt.dtb ; rm -f dts/dummy.fit
