# samd21-tiny
Sample project using [`tiny`](https://github.com/ryanplusplus/tiny) with SAMD21G18A.

## Setup
- Install `gcc-arm-none-eabi`
- Install [`openocd`](https://sourceforge.net/p/openocd/code/ci/master/tree/)

## Use
### Compile
```shell
make -f target.mk
```

### Clean
```shell
make -f target.mk clean
```

### Erase via SWD
```shell
make -f target.mk erase
```

### Flash via SWD
```shell
make -f target.mk upload
```

## Resources
- [SAMD21G18A Datasheet](http://ww1.microchip.com/downloads/en/DeviceDoc/SAM_D21_DA1_Family_DataSheet_DS40001882F.pdf)
- [SAMD Arduino Core](https://github.com/arduino/ArduinoCore-samd)
