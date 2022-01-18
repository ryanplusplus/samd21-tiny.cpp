ifeq ($(JLINK_DEVICE),)
$(error "JLINK_DEVICE must be defined")
endif

.PHONY: suppress-jlink-license-popup
suppress-jlink-license-popup:
	@type lua > /dev/null 2>&1 && lua $(__arm_none_eabi_gcc_tools_path)/script/suppress-jlink-license-popup.lua > /dev/null 2>&1 || true

.PHONY: debug-deps
debug-deps: $(BUILD_DIR)/$(TARGET).elf $(BUILD_DIR)/$(TARGET).svd suppress-jlink-license-popup

.PHONY: upload
upload: $(BUILD_DIR)/upload.jlink suppress-jlink-license-popup
	@JLinkExe -NoGui 1 -device $(JLINK_DEVICE) -if SWD -autoconnect 1 -speed 4000 -CommandFile $<

.PHONY: erase
erase: $(BUILD_DIR)/erase.jlink suppress-jlink-license-popup
	@JLinkExe -NoGui 1 -device $(JLINK_DEVICE) -if SWD -autoconnect 1 -speed 4000 -CommandFile $<

.PHONY: $(BUILD_DIR)/upload.jlink
$(BUILD_DIR)/upload.jlink: $(BUILD_DIR)/$(TARGET).hex
	@mkdir -p $(dir $@)
	@echo r > $@
	@echo h >> $@
	@echo loadfile $< >> $@
	@echo r >> $@
	@echo exit >> $@

.PHONY: $(BUILD_DIR)/erase.jlink
$(BUILD_DIR)/erase.jlink:
	@mkdir -p $(dir $@)
	@echo r > $@
	@echo h >> $@
	@echo erase >> $@
	@echo exit >> $@
