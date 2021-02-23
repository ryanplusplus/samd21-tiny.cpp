TARGET := target
BUILD_DIR := ./build

CPU := cortex-m0
ARCH := armv6-m
LINKER_CFG := lib/sam0/linker_scripts/samd21/gcc/samd21g18a_flash.ld

SVD := lib/svd/ATSAMD21G18A.svd

DEBUG_ADAPTER ?= openocd
OPENOCD_CFG_DIR := openocd
BLACK_MAGIC_PORT ?= /dev/ttyACM0
BLACK_MAGIC_POWER_TARGET ?= N

DEFINES := \
  __SAMD21G18A__ \
  USART_CALLBACK_MODE=1 \

include tools/defaults.mk

CPPFLAGS += \
  -Wno-expansion-to-defined \

INC_DIRS := \

SYS_INC_DIRS := \
  src \

SRC_FILES := \
  lib/sam0/cmsis/samd21/source/gcc/startup_samd21.c \

SRC_DIRS := \
  src \

include lib_asf.mk
include lib_tiny.mk

include tools/tools.mk
include docs.mk

.PHONY: watch
watch:
	@rerun "$(MAKE) --no-print-directory -f $(firstword $(MAKEFILE_LIST))"
