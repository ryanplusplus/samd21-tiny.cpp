TARGET := target
BUILD_DIR := ./build

CPU := cortex-m0
ARCH := armv6-m
OPENOCD_CFG_DIR := openocd
LINKER_CFG := lib/sam0/linker_scripts/samd21/gcc/samd21g18a_flash.ld

SVD := lib/svd/ATSAMD21G18A.svd

DEBUG_ADAPTER ?= jlink
BLACK_MAGIC_PORT ?= /dev/ttyACM0
BLACK_MAGIC_POWER_TARGET ?= N

DEFINES := \
  __SAMD21G18A__ \
  DONT_USE_CMSIS_INIT \
  USART_CALLBACK_MODE=1 \

include tools/defaults.mk

CPPFLAGS += \
  -Wno-expansion-to-defined \

SRC_FILES := \
  lib/sam0/cmsis/samd21/source/gcc/startup_samd21.c \

SRC_DIRS := \
  src \

SYS_INC_DIRS := \
  lib/sam0/cmsis \
	lib/sam0/cmsis/samd21/include \
  lib/sam0/drivers/port \
  lib/sam0/drivers/sercom \
  lib/sam0/drivers/sercom/i2c \
  lib/sam0/drivers/sercom/i2c/i2c_samd21_r21_d10_d11 \
  lib/sam0/drivers/sercom/spi \
  lib/sam0/drivers/sercom/usart \
  lib/sam0/drivers/system \
  lib/sam0/drivers/system/clock \
	lib/sam0/drivers/system/interrupt \
  lib/sam0/drivers/system/interrupt/system_interrupt_samd21 \
  lib/sam0/drivers/system/pinmux \
  lib/sam0/drivers/wdt \
  lib/sam0/include \
  src \

INC_DIRS := \
  lib/tiny/include \

include lib_asf.mk
include lib_tiny.mk

include tools/tools.mk
include docs.mk

.PHONY: watch
watch:
	@rerun "$(MAKE) --no-print-directory -f $(firstword $(MAKEFILE_LIST))"
