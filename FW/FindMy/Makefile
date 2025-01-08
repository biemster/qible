## Makefile

# Prefix for older riscv gcc is  risv-none-embed
# Prefix for newer riscv gcc is  risv-none-elf
# TOOLCHAIN_PREFIX := riscv-none-embed
TOOLCHAIN_PREFIX := ../../../MRS_Toolchain_Linux_x64_V1.92/RISC-V_Embedded_GCC12/bin/riscv-none-elf


APP_C_SRCS += \
  ./src/main.c \
  ./src/broadcaster.c


SDK_BLE_HAL_C_SRCS := \
  ./sdk/BLE/HAL/MCU.c \
  ./sdk/BLE/HAL/RTC.c \
  ./sdk/BLE/HAL/SLEEP.c


SDK_STDPERIPHDRIVER_C_SRCS += \
  ./sdk/StdPeriphDriver/CH59x_adc.c \
  ./sdk/StdPeriphDriver/CH59x_clk.c \
  ./sdk/StdPeriphDriver/CH59x_flash.c \
  ./sdk/StdPeriphDriver/CH59x_gpio.c \
  ./sdk/StdPeriphDriver/CH59x_i2c.c \
  ./sdk/StdPeriphDriver/CH59x_lcd.c \
  ./sdk/StdPeriphDriver/CH59x_pwm.c \
  ./sdk/StdPeriphDriver/CH59x_pwr.c \
  ./sdk/StdPeriphDriver/CH59x_spi0.c \
  ./sdk/StdPeriphDriver/CH59x_sys.c \
  ./sdk/StdPeriphDriver/CH59x_timer0.c \
  ./sdk/StdPeriphDriver/CH59x_timer1.c \
  ./sdk/StdPeriphDriver/CH59x_timer2.c \
  ./sdk/StdPeriphDriver/CH59x_timer3.c \
  ./sdk/StdPeriphDriver/CH59x_uart0.c \
  ./sdk/StdPeriphDriver/CH59x_uart1.c \
  ./sdk/StdPeriphDriver/CH59x_uart2.c \
  ./sdk/StdPeriphDriver/CH59x_uart3.c \
  ./sdk/StdPeriphDriver/CH59x_usbdev.c \
  ./sdk/StdPeriphDriver/CH59x_usbhostBase.c \
  ./sdk/StdPeriphDriver/CH59x_usbhostClass.c

SDK_RVMSIS_C_SRCS += \
  ./sdk/RVMSIS/core_riscv.c

SDK_BLE_LIB_S_UPPER_SRCS += \
  ./sdk/BLE/LIB/ble_task_scheduler.S
SDK_STARTUP_S_UPPER_SRCS += \
  ./sdk/Startup/startup_CH592.S

C_SRCS := \
  $(APP_C_SRCS) \
  $(SDK_BLE_HAL_C_SRCS) \
  $(SDK_STDPERIPHDRIVER_C_SRCS) \
  $(SDK_RVMSIS_C_SRCS)

S_UPPER_SRCS := \
  $(SDK_BLE_LIB_S_UPPER_SRCS) \
  $(SDK_STARTUP_S_UPPER_SRCS)

OBJS := \
  $(foreach src,$(C_SRCS),$(subst ./,obj/,$(patsubst %.c,%.o,$(src)))) \
  $(foreach src,$(S_UPPER_SRCS),$(subst ./,obj/,$(patsubst %.S,%.o,$(src))))

MAKEFILE_DEPS := \
  $(foreach obj,$(OBJS),$(patsubst %.o,%.d,$(obj)))


STDPERIPHDRIVER_LIBS := -L"./sdk/StdPeriphDriver" -lISP592
BLE_LIB_LIBS := -L"./sdk/BLE/LIB" -lCH59xBLE
LIBS := $(STDPERIPHDRIVER_LIBS) $(BLE_LIB_LIBS)

SECONDARY_FLASH := main.hex
SECONDARY_LIST := main.lst
SECONDARY_SIZE := main.siz
SECONDARY_BIN := main.bin

# ARCH is rv32imac on older gcc, rv32imac_zicsr on newer gcc
# ARCH := rv32imac
ARCH := rv32imac_zicsr

CFLAGS_COMMON := \
  -march=$(ARCH) \
  -mabi=ilp32 \
  -mcmodel=medany \
  -msmall-data-limit=8 \
  -mno-save-restore \
  -Os \
  -fmessage-length=0 \
  -fsigned-char \
  -ffunction-sections \
  -fdata-sections
  #-g

.PHONY: all
all: main.elf secondary-outputs

.PHONY: clean
clean:
	-rm $(OBJS)
	-rm $(MAKEFILE_DEPS)
	-rm $(SECONDARY_FLASH)
	-rm $(SECONDARY_LIST)
	-rm $(SECONDARY_BIN)
	-rm main.elf
	-rm main.map
	-rm -r ./obj

.PHONY: secondary-outputs
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE) $(SECONDARY_BIN)

main.elf: $(OBJS)
	${TOOLCHAIN_PREFIX}-gcc \
	    $(CFLAGS_COMMON) \
	    -T "sdk/Ld/Link.ld" \
	    -nostartfiles \
	    -Xlinker \
	    --gc-sections \
	    -Xlinker \
	    --print-memory-usage \
	    -Wl,-Map,"main.map" \
	    -Lobj \
	    --specs=nano.specs \
	    --specs=nosys.specs \
	    -o "main.elf" \
	    $(OBJS) \
	    $(LIBS)

%.hex: %.elf
	@ ${TOOLCHAIN_PREFIX}-objcopy -O ihex "$<"  "$@"

%.bin: %.elf
	$(TOOLCHAIN_PREFIX)-objcopy -O binary $< "$@"

%.lst: %.elf
	@ ${TOOLCHAIN_PREFIX}-objdump \
	    --source \
	    --all-headers \
	    --demangle \
	    --line-numbers \
	    --wide "$<" > "$@"

%.siz: %.elf
	@ ${TOOLCHAIN_PREFIX}-size --format=berkeley "$<"

obj/%.o: ./%.c
	@ mkdir --parents $(dir $@)
	@ ${TOOLCHAIN_PREFIX}-gcc \
	    $(CFLAGS_COMMON) \
	    -I"src/include" \
	    -I"sdk/StdPeriphDriver/inc" \
	    -I"sdk/RVMSIS" \
	    -I"sdk/BLE/LIB" \
	    -I"sdk/BLE/HAL/include" \
	    -std=gnu99 \
	    -MMD \
	    -MP \
	    -MF"$(@:%.o=%.d)" \
	    -MT"$(@)" \
	    -c \
	    -o "$@" "$<"

obj/%.o: ./%.S
	@ mkdir --parents $(dir $@)
	@ ${TOOLCHAIN_PREFIX}-gcc \
	    $(CFLAGS_COMMON) \
	    -x assembler \
	    -MMD \
	    -MP \
	    -MF"$(@:%.o=%.d)" \
	    -MT"$(@)" \
	    -c \
	    -o "$@" "$<"

f: clean all
	chprog main.bin

flash: 
	chprog main.bin
