cmd_arch/arm/dts/stm32h743i-eval.dtb := mkdir -p arch/arm/dts/ ; (cat arch/arm/dts/stm32h743i-eval.dts; echo '$(pound)include "stm32h7-u-boot.dtsi"') > arch/arm/dts/.stm32h743i-eval.dtb.pre.tmp; arm-none-eabi-gcc -E -Wp,-MD,arch/arm/dts/.stm32h743i-eval.dtb.d.pre.tmp -nostdinc -I./arch/arm/dts -I./arch/arm/dts/include -Iinclude -I./include -I./arch/arm/include -include ./include/linux/kconfig.h -D__ASSEMBLY__ -undef -D__DTS__ -x assembler-with-cpp -o arch/arm/dts/.stm32h743i-eval.dtb.dts.tmp arch/arm/dts/.stm32h743i-eval.dtb.pre.tmp ; ./scripts/dtc/dtc -O dtb -o arch/arm/dts/stm32h743i-eval.dtb -b 0 -i arch/arm/dts/ -Wno-unit_address_vs_reg -Wno-simple_bus_reg -Wno-unit_address_format -Wno-pci_bridge -Wno-pci_device_bus_num -Wno-pci_device_reg -Wno-avoid_unnecessary_addr_size -Wno-alias_paths -d arch/arm/dts/.stm32h743i-eval.dtb.d.dtc.tmp arch/arm/dts/.stm32h743i-eval.dtb.dts.tmp ; cat arch/arm/dts/.stm32h743i-eval.dtb.d.pre.tmp arch/arm/dts/.stm32h743i-eval.dtb.d.dtc.tmp > arch/arm/dts/.stm32h743i-eval.dtb.d ; sed -i "s:arch/arm/dts/.stm32h743i-eval.dtb.pre.tmp:arch/arm/dts/stm32h743i-eval.dts:" arch/arm/dts/.stm32h743i-eval.dtb.d

source_arch/arm/dts/stm32h743i-eval.dtb := arch/arm/dts/stm32h743i-eval.dts

deps_arch/arm/dts/stm32h743i-eval.dtb := \
  arch/arm/dts/stm32h743.dtsi \
  arch/arm/dts/skeleton.dtsi \
  arch/arm/dts/armv7-m.dtsi \
  arch/arm/dts/include/dt-bindings/clock/stm32h7-clks.h \
  arch/arm/dts/include/dt-bindings/mfd/stm32h7-rcc.h \
  arch/arm/dts/stm32h743-pinctrl.dtsi \
  arch/arm/dts/include/dt-bindings/pinctrl/stm32h7-pinfunc.h \
  arch/arm/dts/include/dt-bindings/memory/stm32-sdram.h \
  arch/arm/dts/stm32h7-u-boot.dtsi \

arch/arm/dts/stm32h743i-eval.dtb: $(deps_arch/arm/dts/stm32h743i-eval.dtb)

$(deps_arch/arm/dts/stm32h743i-eval.dtb):
