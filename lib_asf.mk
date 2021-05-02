LIBS := asf $(LIBS)

SYS_INC_DIRS += \
  lib/sam0/cmsis \
  lib/sam0/cmsis/samd21/source \
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

asf_LIB_ROOT := lib/sam0

asf_ASFLAGS := $(ASFLAGS)
asf_CPPFLAGS := $(CPPFLAGS)
asf_CFLAGS := $(CFLAGS)
asf_CXXFLAGS := $(CXXFLAGS)
asf_DEFINES := $(DEFINES)

asf_CPPFLAGS += \
  -Wno-unused-parameter \
  -Wno-expansion-to-defined \
  -Wno-old-style-declaration \

asf_SYS_INC_DIRS := \
  $(SYS_INC_DIRS) \

asf_INC_DIRS := \

asf_SRC_FILES := \
  lib/sam0/drivers/sercom/i2c/i2c_samd21_r21_d10_d11/i2c_master.c \
  lib/sam0/drivers/sercom/sercom.c \
  lib/sam0/drivers/sercom/sercom_interrupt.c \
  lib/sam0/drivers/sercom/spi/spi.c \
  lib/sam0/drivers/wdt/wdt.c \

asf_SRC_DIRS := \
  lib/sam0/drivers/port \
  lib/sam0/drivers/sercom/usart \
  lib/sam0/drivers/system/clock/clock_samd21_r21 \
  lib/sam0/drivers/system/interrupt \
  lib/sam0/drivers/system/pinmux \
