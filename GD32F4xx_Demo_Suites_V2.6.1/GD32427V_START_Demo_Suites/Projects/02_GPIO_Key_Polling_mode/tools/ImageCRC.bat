:: set BIN and HEX file for getting them in the same time
set OUT=%1.out
set BIN=%1.bin
set HEX=%1.hex

:: this is for two ROMs fill and divide calculate the ROMs CRC.
:: fill and calculate CRC for two ROMs 
ielftool --fill="0xFF;__ICFEDIT_region_ROM_start__-__ICFEDIT_region_ROM_end__" --fill="0xFF;__ICFEDIT_region_ROMUL_start__-__ICFEDIT_region_ROMUL_end__" --checksum="__checksum:4,crc32,0xffffffff;__ICFEDIT_region_ROM_start__-__ICFEDIT_region_ROM_end__" --checksum="__checksumUL:4,crc32,0xffffffff;__ICFEDIT_region_ROMUL_start__-__ICFEDIT_region_ROMUL_end__" --verbose %OUT% %OUT%

:: calculate all ROMs CRC
ielftool --fill="0xFF;__ICFEDIT_region_ROM_start__-__ICFEDIT_region_ROMall_end__" --checksum="__checksumall:4,crc32,0xffffffff;__ICFEDIT_region_ROM_start__-__ICFEDIT_region_ROMall_end__" --verbose %OUT% %OUT%

:: get bin file
ielftool --bin --verbose %OUT% %BIN%
:: get hex file
ielftool --ihex --verbose %OUT% %HEX%