ifeq ($(DEBUG_TOOL),black_magic)
ifeq ($(BLACK_MAGIC_POWER_TARGET),Y)
BLACK_MAGIC_TPWR := 'enable'
else
BLACK_MAGIC_TPWR := 'disable'
endif

.PHONY: upload
upload: $(BUILD_DIR)/$(TARGET).elf
	@$(GDB) \
    --nx \
    --batch \
    -ex 'target extended-remote $(BLACK_MAGIC_PORT)' \
    -ex 'monitor tpwr $(BLACK_MAGIC_TPWR)' \
    -ex 'monitor swdp_scan' \
    -ex 'attach 1' \
    -ex 'monitor unlock_flash' \
    -ex 'monitor unlock_bootprot' \
    -ex 'load' \
    -ex 'compare-sections' \
    -ex 'kill' \
    $<

.PHONY: erase
erase:
	@$(GDB) \
    -nx \
    --batch \
    -ex 'target extended-remote $(BLACK_MAGIC_PORT)' \
    -ex 'monitor tpwr $(BLACK_MAGIC_TPWR)' \
    -ex 'monitor swdp_scan' \
    -ex 'attach 1' \
    -ex 'monitor unlock_flash' \
    -ex 'monitor unlock_bootprot' \
    -ex 'monitor erase_mass'
endif
