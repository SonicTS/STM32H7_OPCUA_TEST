set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(tools /usr)

set(CMAKE_SYSROOT ${tools}/bin)
set(CMAKE_C_COMPILER ${tools}/bin/arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER ${tools}/bin/arm-none-eabi-g++)
set(MY_CMAKE_OBJCOPY ${tools}/bin/arm-none-eabi-objcopy)

add_compile_options(
    -mcpu=cortex-m7
    -mthumb
    -mfloat-abi=hard
    -mfpu=fpv5-d16
    -ffixed-r9
    -ffunction-sections
    -fdata-sections
    -fstack-usage
    -Wall
    -fmessage-length=0
    --specs=nano.specs
)

set(CMAKE_C_LINK_FLAGS " \
    -mcpu=cortex-m7 \
    -mthumb \
    -specs=nosys.specs \
    -specs=nano.specs \
    -mfloat-abi=hard \
    -mfpu=fpv5-d16 \
    -Wl,--gc-sections \
    -Wl,-Map=output.map \
"
)

set(CMAKE_C_FLAGS_DEBUG "-O0 -g")
set(CMAKE_C_FLAGS_RELEASE "-Os")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
