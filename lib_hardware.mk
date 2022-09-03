$(call create_lib_with_defaults,hardware)

hardware_DEFINES := $(DEFINES)
hardware_INC_DIRS := $(INC_DIRS)
hardware_SYS_INC_DIRS := $(SYS_INC_DIRS)

INC_DIRS += \
  src/hardware \
  src/hardware/Spi \
  src/hardware/Uart \

hardware_SRC_FILES := \

hardware_SRC_DIRS := \
  src/hardware \
  src/hardware/Spi \
  src/hardware/Uart \
