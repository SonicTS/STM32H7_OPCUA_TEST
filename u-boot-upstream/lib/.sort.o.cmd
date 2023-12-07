cmd_lib/sort.o := arm-none-eabi-gcc -Wp,-MD,lib/.sort.o.d -nostdinc -isystem /usr/lib/gcc/arm-none-eabi/10.3.1/include -Iinclude    -I./arch/arm/include -include ./include/linux/kconfig.h -D__KERNEL__ -D__UBOOT__ -Wall -Wstrict-prototypes -Wno-format-security -fno-builtin -ffreestanding -std=gnu11 -fshort-wchar -fno-strict-aliasing -fno-PIE -Os -fno-stack-protector -fno-delete-null-pointer-checks -fmacro-prefix-map=./= -g -fstack-usage -Wno-format-nonliteral -Werror=date-time -D__ARM__ -Wa,-mimplicit-it=always -mthumb -mthumb-interwork -mabi=aapcs-linux -mword-relocations -fno-pic -mno-unaligned-access -ffunction-sections -fdata-sections -fno-common -ffixed-r9 -msoft-float -pipe -march=armv7-m -D__LINUX_ARM_ARCH__=7 -I./arch/arm/mach-stm32/include    -D"KBUILD_STR(s)=$(pound)s" -D"KBUILD_BASENAME=KBUILD_STR(sort)"  -D"KBUILD_MODNAME=KBUILD_STR(sort)" -c -o lib/sort.o lib/sort.c

source_lib/sort.o := lib/sort.c

deps_lib/sort.o := \
    $(wildcard include/config/have/efficient/unaligned/access.h) \
  include/linux/compat.h \
    $(wildcard include/config/lbdaf.h) \
  include/malloc.h \
    $(wildcard include/config/sys/malloc/simple.h) \
  include/linux/stddef.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
  include/linux/posix_types.h \
  arch/arm/include/asm/posix_types.h \
  arch/arm/include/asm/types.h \
    $(wildcard include/config/arm64.h) \
    $(wildcard include/config/phys/64bit.h) \
    $(wildcard include/config/dma/addr/t/64bit.h) \
  include/asm-generic/int-ll64.h \
  /usr/lib/gcc/arm-none-eabi/10.3.1/include/stdbool.h \
  include/linux/err.h \
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
  include/linux/errno.h \
  include/linux/kernel.h \

lib/sort.o: $(deps_lib/sort.o)

$(deps_lib/sort.o):
