#ifndef _CRC32_H_
#define _CRC32_H_

unsigned long fast_crc32(unsigned long sum, unsigned char *p, unsigned long len);
unsigned long slow_crc32(unsigned long sum, unsigned char *p, unsigned long len);
unsigned long set_crc32_init_value(unsigned long start);

#endif
