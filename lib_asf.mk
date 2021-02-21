LIBS := asf $(LIBS)

asf_LIB_ROOT := lib/sam0

asf_ASFLAGS := $(ASFLAGS)
asf_CPPFLAGS := $(CPPFLAGS)
asf_CFLAGS := $(CFLAGS)
asf_CXXFLAGS := $(CXXFLAGS)

asf_CPPFLAGS += \
  -Wno-unused-parameter \
  -Wno-expansion-to-defined \
  -Wno-old-style-declaration \

asf_SYS_INC_DIRS := \
  $(SYS_INC_DIRS) \

asf_INC_DIRS := \

asf_LIB_FILES := \
  lib/sam0/drivers/sercom/i2c/i2c_samd21_r21_d10_d11/i2c_master.c \
  lib/sam0/drivers/sercom/sercom.c \
  lib/sam0/drivers/sercom/sercom_interrupt.c \
  lib/sam0/drivers/sercom/spi/spi.c \
  lib/sam0/drivers/wdt/wdt.c \

asf_LIB_DIRS := \
  lib/sam0/drivers/port \
  lib/sam0/drivers/sercom/usart \
  lib/sam0/drivers/system/clock/clock_samd21_r21 \
  lib/sam0/drivers/system/interrupt \
  lib/sam0/drivers/system/pinmux \
