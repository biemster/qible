ch592 FindMy firmware
=======================

Based on [rgoulter/ch592-ble-hid-keyboard-example](https://github.com/rgoulter/ch592-ble-hid-keyboard-example) and [Suniasuta makefile template](https://github.com/Suniasuta/CH592_Makefile_Template)

## Installation
The firmware can be compiled with the xpack riscv compiler: https://github.com/xpack-dev-tools/riscv-none-elf-gcc-xpack

The `.bin` can be flashed to the CH592 using [chprog](https://github.com/wagiminator/MCU-Flash-Tools). (Enter the CH592 bootloader by holding down BOOT when connecting it using USB).\
The SDK for CH592 from the openwch EVT is vendored under ``sdk/``. (Encoding has been changed from gbk to utf-8). 

``make`` to compile \
``make clean`` to clean/remove the compiled stuff \
``make flash`` to flash \

## Further preparation
I assume here you are already familiar with the FindMy scripts from `biemster/FindMy`. If you're not, please refer to that repository for further info.
1. Generate a `.keys` file with `FindMy/generate_keys.py`
2. Add the key to the firmware using the `prep_fw.py` script like this (adv-interval is optional, the broadcasting interval in seconds [allowed values are 2,3,5,10,20,30]):
```bash
./prep_fw.py --keyfile=<path to .keys file> --adv-interval=5
```
3. Flash the new `FindMy_XXXXXXX.bin` file using `chprog FindMy_XXXXXXX`

## Retrieving the location of the module
This is explained in more detail in the `FindMy` repository, but boils down to running the `request_reports.py` script in the same
directory as the `.keys` file generated here.
