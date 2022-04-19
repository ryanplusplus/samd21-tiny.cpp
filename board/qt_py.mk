CPU := cortex-m0
LINKER_SCRIPT := lib/asf4/samd21/gcc/gcc/samd21g18a_flash.ld

SVD := lib/svd/ATSAMD21E18A.svd

DEBUG_ADAPTER ?= jlink
JLINK_DEVICE := ATSAMD21E18
BLACK_MAGIC_PORT ?= /dev/ttyACM0
BLACK_MAGIC_POWER_TARGET ?= N

INC_DIRS += \
  src/board/qt_py

DEFINES += \
  __SAMD21E18A__ \
