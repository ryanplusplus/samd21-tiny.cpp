TARGET := target
BUILD_DIR := ./build

CPU := cortex-m0
ARCH := armv6-m
LINKER_CFG := lib/sam0/linker_scripts/samd21/gcc/samd21g18a_flash.ld

SVD := lib/svd/ATSAMD21G18A.svd

DEBUG_ADAPTER ?= jlink
JLINK_DEVICE := ATSAMD21G18
OPENOCD_CFG_DIR := openocd
BLACK_MAGIC_PORT ?= /dev/ttyACM0
BLACK_MAGIC_POWER_TARGET ?= N

DEFINES := \
  __SAMD21G18A__ \

include tools/defaults.mk

INC_DIRS := \
  lib/sam0/include \

SYS_INC_DIRS := \
  lib/sam0/cmsis \
  lib/sam0/cmsis/samd21/source \
  lib/sam0/cmsis/samd21/include \

SRC_FILES := \

SRC_DIRS := \
  src \

include lib_tiny.mk

include tools/tools.mk
include docs.mk

.PHONY: watch
watch:
	@rerun "$(MAKE) --no-print-directory -f $(firstword $(MAKEFILE_LIST))"
