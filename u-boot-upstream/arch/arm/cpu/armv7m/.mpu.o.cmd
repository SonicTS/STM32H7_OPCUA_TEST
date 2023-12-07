cmd_arch/arm/cpu/armv7m/mpu.o := arm-none-eabi-gcc -Wp,-MD,arch/arm/cpu/armv7m/.mpu.o.d -nostdinc -isystem /usr/lib/gcc/arm-none-eabi/10.3.1/include -Iinclude    -I./arch/arm/include -include ./include/linux/kconfig.h -D__KERNEL__ -D__UBOOT__ -Wall -Wstrict-prototypes -Wno-format-security -fno-builtin -ffreestanding -std=gnu11 -fshort-wchar -fno-strict-aliasing -fno-PIE -Os -fno-stack-protector -fno-delete-null-pointer-checks -fmacro-prefix-map=./= -g -fstack-usage -Wno-format-nonliteral -Werror=date-time -D__ARM__ -Wa,-mimplicit-it=always -mthumb -mthumb-interwork -mabi=aapcs-linux -mword-relocations -fno-pic -mno-unaligned-access -ffunction-sections -fdata-sections -fno-common -ffixed-r9 -msoft-float -pipe -march=armv7-m -D__LINUX_ARM_ARCH__=7 -I./arch/arm/mach-stm32/include    -D"KBUILD_STR(s)=$(pound)s" -D"KBUILD_BASENAME=KBUILD_STR(mpu)"  -D"KBUILD_MODNAME=KBUILD_STR(mpu)" -c -o arch/arm/cpu/armv7m/mpu.o arch/arm/cpu/armv7m/mpu.c

source_arch/arm/cpu/armv7m/mpu.o := arch/arm/cpu/armv7m/mpu.c

deps_arch/arm/cpu/armv7m/mpu.o := \
  include/linux/bitops.h \
    $(wildcard include/config/sandbox.h) \
    $(wildcard include/config/sandbox/bits/per/long.h) \
  arch/arm/include/asm/types.h \
    $(wildcard include/config/arm64.h) \
    $(wildcard include/config/phys/64bit.h) \
    $(wildcard include/config/dma/addr/t/64bit.h) \
  include/asm-generic/int-ll64.h \
  include/asm-generic/bitsperlong.h \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/kasan.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
    $(wildcard include/config/kprobes.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
    $(wildcard include/config/gcov/kernel.h) \
    $(wildcard include/config/arch/use/builtin/bswap.h) \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
  include/linux/posix_types.h \
  include/linux/stddef.h \
  arch/arm/include/asm/posix_types.h \
  /usr/lib/gcc/arm-none-eabi/10.3.1/include/stdbool.h \
  arch/arm/include/asm/bitops.h \
  include/asm-generic/bitops/__ffs.h \
  arch/arm/include/asm/proc-armv/system.h \
    $(wildcard include/config/cpu/sa1100.h) \
    $(wildcard include/config/cpu/sa110.h) \
  include/asm-generic/bitops/__fls.h \
  include/asm-generic/bitops/fls.h \
  include/asm-generic/bitops/fls64.h \
  arch/arm/include/asm/armv7m.h \
  arch/arm/include/asm/armv7_mpu.h \
    $(wildcard include/config/cpu/v7m.h) \
    $(wildcard include/config/cpu/v7r.h) \
  arch/arm/include/asm/io.h \
  arch/arm/include/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/linux/byteorder/swab.h \
  include/linux/byteorder/generic.h \
  arch/arm/include/asm/memory.h \
    $(wildcard include/config/discontigmem.h) \
  arch/arm/include/asm/barriers.h \
  include/asm-generic/io.h \
  include/iotrace.h \
    $(wildcard include/config/io/trace.h) \
    $(wildcard include/config/spl/build.h) \

arch/arm/cpu/armv7m/mpu.o: $(deps_arch/arm/cpu/armv7m/mpu.o)

$(deps_arch/arm/cpu/armv7m/mpu.o):
