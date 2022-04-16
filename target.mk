include tools/setup.mk

TARGET := target
BUILD_DIR := ./build
BOARD ?= metro_m0

include board/$(BOARD).mk
include tools/defaults.mk

INC_DIRS += \

SYS_INC_DIRS += \

SRC_FILES += \

SRC_DIRS += \
  src \

include lib_asf.mk
include lib/tiny/lib_tiny.mk
include lib/tiny-devices/lib_tiny-devices.mk
include lib_hardware.mk

.PHONY: all
all: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).hex
	@$(SIZE) $<

.PHONY: watch
watch:
	@rerun "$(MAKE) --no-print-directory -f $(firstword $(MAKEFILE_LIST))"

include tools/tools.mk
include docs.mk
