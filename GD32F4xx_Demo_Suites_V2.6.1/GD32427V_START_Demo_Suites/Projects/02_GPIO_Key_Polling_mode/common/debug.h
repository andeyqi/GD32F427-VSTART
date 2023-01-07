#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdio.h>
#include <stdint.h>

extern void trace_word_stream(uint32_t *pwStream, uint16_t hwSize);
extern void trace_hword_stream(uint16_t *phwStream, uint16_t hwSize);
extern void trace_byte_stream(uint8_t *pchStream, uint16_t hwSize);
extern void trace_byte_stream1(uint32_t base,uint8_t *pchStream, uint16_t hwSize);
extern inline struct log_control * logctrl_instance(void);

#endif /*DEBUG_H_*/

