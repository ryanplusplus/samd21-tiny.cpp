ASFLAGS := \
  -mcpu=$(CPU) \
  -mthumb \
  -g2 \

CPPFLAGS := \
  --specs=nano.specs \
  -mcpu=$(CPU) \
  -mthumb \
  -g \
  -g2 \
  -Os \
  -fdata-sections \
  -ffunction-sections \
  -Wall \
  -Wextra \
  -Werror \
  -Wfatal-errors \
  -Wcast-qual \
  -pedantic \
  -nostartfiles \

CFLAGS := \
  -std=c99 \

CXXFLAGS := \
  -fno-rtti \
  -fno-exceptions \
  -fno-unwind-tables \
  -fno-non-call-exceptions \
  -fno-use-cxa-atexit \
  -Weffc++ \
  -std=c++17 \

LDFLAGS := \
  -Wl,--gc-sections \
  -Wl,-Map=$(BUILD_DIR)/$(TARGET).map \
