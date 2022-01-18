ifeq ($(OPENOCD_CFG_DIR),)
$(error "OPENOCD_CFG_DIR must be defined")
endif

.PHONY: debug-deps
debug-deps: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/openocd.cfg $(BUILD_DIR)/$(TARGET).svd

.PHONY: upload
upload: $(BUILD_DIR)/$(TARGET).hex
	@openocd -f $(OPENOCD_CFG_DIR)/upload.cfg

.PHONY: erase
erase:
	@openocd -f $(OPENOCD_CFG_DIR)/erase.cfg

$(BUILD_DIR)/openocd.cfg: $(OPENOCD_CFG_DIR)/debug.cfg
	@mkdir -p $(dir $@)
	@cp $< $@
