LIBS := hardware $(LIBS)

INC_DIRS += \
  src/hardware

hardware_ASFLAGS := $(ASFLAGS)
hardware_CPPFLAGS := $(CPPFLAGS)
hardware_CFLAGS := $(CFLAGS)
hardware_CXXFLAGS := $(CXXFLAGS)
hardware_DEFINES := $(DEFINES)

hardware_INC_DIRS := $(INC_DIRS)

hardware_SYS_INC_DIRS := $(SYS_INC_DIRS)

hardware_SRC_FILES := \

hardware_SRC_DIRS := \
  src/hardware \
