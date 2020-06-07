HARDWARE_MANUAL_URL := http://ww1.microchip.com/downloads/en/DeviceDoc/SAM_D21_DA1_Family_DataSheet_DS40001882F.pdf
DATASHEET_URL := http://ww1.microchip.com/downloads/en/DeviceDoc/SAM_D21_DA1_Family_DataSheet_DS40001882F.pdf

hardware_manual:
	xdg-open $(HARDWARE_MANUAL_URL)

.PHONY: datasheet
datasheet:
	xdg-open $(DATASHEET_URL)
