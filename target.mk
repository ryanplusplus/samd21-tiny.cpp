TARGET = tiny
BUILD_DIR ?= ./build

CPU := cortex-m0
ARCH := armv6-m
LINKER_CFG := lib/sam0/linker_scripts/samd21/gcc/samd21g18a_flash.ld

DEFINES := \
  __SAMD21G18A__ \
  DONT_USE_CMSIS_INIT \

SRC_FILES := \
  lib/sam0/cmsis/samd21/source/gcc/startup_samd21.c \

SRC_DIRS := \
  src \

LIB_FILES := \

LIB_DIRS := \
  lib/tiny/src \
  lib/sam0/drivers/system/clock/clock_samd21_r21 \
  lib/sam0/drivers/system/interrupt \

INC_DIRS := \
  lib/sam0/cmsis \
  lib/sam0/cmsis/samd21/include \
  lib/sam0/drivers/system \
  lib/sam0/drivers/system/clock \
  lib/sam0/drivers/system/interrupt \
  lib/sam0/drivers/system/interrupt/system_interrupt_samd21 \
  lib/sam0/drivers/system/pinmux \
  lib/sam0/include \
  lib/tiny/include \

include makefile-worker.mk
