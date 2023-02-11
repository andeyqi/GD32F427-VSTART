#ifndef __PERFORMANCE_COUNTER_H__
#define __PERFORMANCE_COUNTER_H__

#include <stdint.h>
#include <stdbool.h>


typedef struct {
    int64_t    lStart;
    int64_t    lUsedTotal;
    int32_t    nUsedRecent;
    uint32_t   nUsedMax;
    uint32_t   nUsedMin;
    uint32_t   wActiveCount;
} task_cycle_info_t;

#define WORD_OF_CYCEL_INFO  (10)
#define BYTE_OF_CYCEL_INFO  ((WORD_OF_CYCEL_INFO)*4)

extern bool start_cycle_counter(void);
/*! \brief calculate the elapsed cycle count since the last start point
 *! 
 *! \note you can have multiple stop_cycle_counter following one start point
 *!  
 *! \return the elapsed cycle count.
 */ 
extern int32_t stop_cycle_counter(void);
/* Function : delay specified us with the help from systick
 */
extern void delay_us(int32_t iUs);
extern void init_cycle_counter(void);
extern void user_code_insert_to_systick_handler(void);
extern void __on_context_switch_in(uint32_t *pwStack);
extern void __on_context_switch_out(uint32_t *pwStack);
extern task_cycle_info_t * get_rtos_task_cycle_info_by_stack(void * pxStack);
extern int64_t get_system_ticks(void);

#endif /* end of  __PERFORMANCE_COUNTER_H__ */
