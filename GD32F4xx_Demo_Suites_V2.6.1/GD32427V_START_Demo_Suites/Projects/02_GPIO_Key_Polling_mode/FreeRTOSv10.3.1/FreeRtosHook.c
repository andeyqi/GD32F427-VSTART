/* Kernel includes. */
#include "FreeRTOS.h"

#if ( configUSE_PERF_CNT == 1 )
#include "perf_counter.h"
#endif


extern void vApplicationTickHook(void)
{
#if ( CFG_ENABLE_WDG_MONITOR == 1 )
	extern __no_init volatile char switchintaskname[configMAX_TASK_NAME_LEN];
	static char run_task_name[configMAX_TASK_NAME_LEN] = {0};
	static unsigned int counter = 0;
	extern void show_stack_frame(void);
#endif

#if ( configUSE_PERF_CNT == 1 )
    user_code_insert_to_systick_handler();
#endif

#if ( CFG_ENABLE_WDG_MONITOR == 1 )
    if(memcmp((void *)switchintaskname,(void*)run_task_name,configMAX_TASK_NAME_LEN))
    {
        memcpy((void*)run_task_name,(void*)switchintaskname,configMAX_TASK_NAME_LEN);
		counter = 0;
    }
	else
	{
		counter++;
		if(counter%1000 == 0)
		{
			printf("wdg maybe reset [%s] \r\n",run_task_name);
			show_stack_frame();
		}
	}
#endif
}

extern void vApplicationStartHook(void)
{
#if ( configUSE_PERF_CNT == 1 )
       init_cycle_counter();
#endif
}

extern void vApplicationTaskSwitchInHook(void * name,void *ptTCB)
{
#if ( configUSE_PERF_CNT == 1 )
    __on_context_switch_in((uint32_t *)ptTCB);
#endif
}

extern void vApplicationTaskSwitchOutHook(void *ptTCB)
{
#if ( configUSE_PERF_CNT == 1 )
    __on_context_switch_out((uint32_t *)ptTCB);
#endif
}