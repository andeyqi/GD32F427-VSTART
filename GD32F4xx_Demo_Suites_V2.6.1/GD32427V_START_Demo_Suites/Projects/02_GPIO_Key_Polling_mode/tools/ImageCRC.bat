:: set BIN and HEX file for getting them in the same time
set OUT=%1.out
set BIN=%1.bin
set HEX=%1.hex
set ASM=%1.asm

ielfdumparm.exe %OUT% -o %ASM%  --code

:: this is for two ROMs fill and divide calculate the ROMs CRC.
:: fill and calculate CRC for two ROMs 
::ielftool --fill="0xFF;__ICFEDIT_region_ROM_start__-__ICFEDIT_region_ROM_end__" --fill="0xFF;__ICFEDIT_region_ROMUL_start__-__ICFEDIT_region_ROMUL_end__" --checksum="__checksum:4,crc32,0xffffffff;__ICFEDIT_region_ROM_start__-__ICFEDIT_region_ROM_end__" --checksum="__checksumUL:4,crc32,0xffffffff;__ICFEDIT_region_ROMUL_start__-__ICFEDIT_region_ROMUL_end__" --verbose %OUT% %OUT%

::ielftool --fill="0xff __ICFEDIT_region_ROM_start__-__ICFEDIT_region_ROM_end__" --checksum="__checksum:4,crc32,0xffffffff;__ICFEDIT_region_ROM_start__-__ICFEDIT_region_ROM_end__" --verbose %OUT% %OUT%


:: calculate ROMs CRC
ielftool.exe  --fill=0xff;__ICFEDIT_region_ROM_start__-__ICFEDIT_region_ROM_end__ --checksum=__checksum:4,crc32;__ICFEDIT_region_ROM_start__-__ICFEDIT_region_ROM_end__ --verbose %OUT% %OUT%

:: get bin file
ielftool --bin --verbose %OUT% %BIN%
:: get hex file
ielftool --ihex --verbose %OUT% %HEX%