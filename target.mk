TARGET := target
BUILD_DIR := ./build

CPU := cortex-m0
LINKER_CFG := lib/asf4/samd21/gcc/gcc/samd21g18a_flash.ld

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
  lib/asf4/samd21/include \

SYS_INC_DIRS := \
  lib/asf4/samd21/CMSIS/Include \

SRC_FILES := \

SRC_DIRS := \
  src \

include lib_tiny.mk
include lib_hardware.mk

include tools/tools.mk
include docs.mk

.PHONY: watch
watch:
	@rerun "$(MAKE) --no-print-directory -f $(firstword $(MAKEFILE_LIST))"
