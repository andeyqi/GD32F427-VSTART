#include <stdint.h>
#include <stdbool.h>
#include <cmsis_iar.h>
#include "littleshell.h"
#include "perf_counter.h"
#include "FreeRTOS.h"

#if ( configUSE_PERF_CNT == 1 )

/*============================ MACROS ========================================*/
#ifndef PERF_CNT_COMPENSATION_THRESHOLD
#   define PERF_CNT_COMPENSATION_THRESHOLD          16
#endif

#ifndef PERF_CNT_DELAY_US_COMPENSATION
#   define PERF_CNT_DELAY_US_COMPENSATION           90
#endif


/* IO definitions (access restrictions to peripheral registers) */
/**
    \defgroup CMSIS_glob_defs CMSIS Global Defines

    <strong>IO Type Qualifiers</strong> are used
    \li to specify the access to peripheral variables.
    \li for automatic generation of peripheral register debug information.
*/

#define     __I     volatile const       /*!< Defines 'read only' permissions */
#define     __O     volatile             /*!< Defines 'write only' permissions */
#define     __IO    volatile             /*!< Defines 'read / write' permissions */

/* following defines should be used for structure members */
#define     __IM     volatile const      /*! Defines 'read only' structure member permissions */
#define     __OM     volatile            /*! Defines 'write only' structure member permissions */
#define     __IOM    volatile            /*! Defines 'read / write' structure member permissions */

/* Memory mapping of Core Hardware */
#define SCS_BASE            (0xE000E000UL)                             /*!< System Control Space Base Address */
#define SysTick_BASE        (SCS_BASE +  0x0010UL)                     /*!< SysTick Base Address */
#define SCB_BASE            (SCS_BASE +  0x0D00UL)                    /*!< System Control Block Base Address */

#define SysTick             ((SysTick_Type   *)     SysTick_BASE     ) /*!< SysTick configuration struct */
#define SCB                 ((SCB_Type       *)     SCB_BASE      )   /*!< SCB configuration struct */



/* SysTick Control / Status Register Definitions */
#define SysTick_CTRL_COUNTFLAG_Pos         16U                                            /*!< SysTick CTRL: COUNTFLAG Position */
#define SysTick_CTRL_COUNTFLAG_Msk         (1UL << SysTick_CTRL_COUNTFLAG_Pos)            /*!< SysTick CTRL: COUNTFLAG Mask */

#define SysTick_CTRL_CLKSOURCE_Pos          2U                                            /*!< SysTick CTRL: CLKSOURCE Position */
#define SysTick_CTRL_CLKSOURCE_Msk         (1UL << SysTick_CTRL_CLKSOURCE_Pos)            /*!< SysTick CTRL: CLKSOURCE Mask */

#define SysTick_CTRL_TICKINT_Pos            1U                                            /*!< SysTick CTRL: TICKINT Position */
#define SysTick_CTRL_TICKINT_Msk           (1UL << SysTick_CTRL_TICKINT_Pos)              /*!< SysTick CTRL: TICKINT Mask */

#define SysTick_CTRL_ENABLE_Pos             0U                                            /*!< SysTick CTRL: ENABLE Position */
#define SysTick_CTRL_ENABLE_Msk            (1UL /*<< SysTick_CTRL_ENABLE_Pos*/)           /*!< SysTick CTRL: ENABLE Mask */

/* SysTick Reload Register Definitions */
#define SysTick_LOAD_RELOAD_Pos             0U                                            /*!< SysTick LOAD: RELOAD Position */
#define SysTick_LOAD_RELOAD_Msk            (0xFFFFFFUL /*<< SysTick_LOAD_RELOAD_Pos*/)    /*!< SysTick LOAD: RELOAD Mask */

/* SysTick Current Register Definitions */
#define SysTick_VAL_CURRENT_Pos             0U                                            /*!< SysTick VAL: CURRENT Position */
#define SysTick_VAL_CURRENT_Msk            (0xFFFFFFUL /*<< SysTick_VAL_CURRENT_Pos*/)    /*!< SysTick VAL: CURRENT Mask */

/* SysTick Calibration Register Definitions */
#define SysTick_CALIB_NOREF_Pos            31U                                            /*!< SysTick CALIB: NOREF Position */
#define SysTick_CALIB_NOREF_Msk            (1UL << SysTick_CALIB_NOREF_Pos)               /*!< SysTick CALIB: NOREF Mask */

#define SysTick_CALIB_SKEW_Pos             30U                                            /*!< SysTick CALIB: SKEW Position */
#define SysTick_CALIB_SKEW_Msk             (1UL << SysTick_CALIB_SKEW_Pos)                /*!< SysTick CALIB: SKEW Mask */

#define SysTick_CALIB_TENMS_Pos             0U                                            /*!< SysTick CALIB: TENMS Position */
#define SysTick_CALIB_TENMS_Msk            (0xFFFFFFUL /*<< SysTick_CALIB_TENMS_Pos*/)    /*!< SysTick CALIB: TENMS Mask */

/*@} end of group CMSIS_SysTick */




#define SCB_ICSR_PENDSTCLR_Pos             25U                                            /*!< SCB ICSR: PENDSTCLR Position */
#define SCB_ICSR_PENDSTCLR_Msk             (1UL << SCB_ICSR_PENDSTCLR_Pos)                /*!< SCB ICSR: PENDSTCLR Mask */

#define SCB_ICSR_PENDSTSET_Pos             26U                                            /*!< SCB ICSR: PENDSTSET Position */
#define SCB_ICSR_PENDSTSET_Msk             (1UL << SCB_ICSR_PENDSTSET_Pos)                /*!< SCB ICSR: PENDSTSET Mask */


/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/

/*
  \brief  Structure type to access the System Timer (SysTick).
 */
typedef struct
{
  __IOM uint32_t CTRL;                   /*!< Offset: 0x000 (R/W)  SysTick Control and Status Register */
  __IOM uint32_t LOAD;                   /*!< Offset: 0x004 (R/W)  SysTick Reload Value Register */
  __IOM uint32_t VAL;                    /*!< Offset: 0x008 (R/W)  SysTick Current Value Register */
  __IM  uint32_t CALIB;                  /*!< Offset: 0x00C (R/ )  SysTick Calibration Register */
} SysTick_Type;

/*
  \brief  Structure type to access the System Control Block (SCB).
 */
typedef struct
{
  __IM  uint32_t CPUID;                  /*!< Offset: 0x000 (R/ )  CPUID Base Register */
  __IOM uint32_t ICSR;                   /*!< Offset: 0x004 (R/W)  Interrupt Control and State Register */
  __IOM uint32_t VTOR;                   /*!< Offset: 0x008 (R/W)  Vector Table Offset Register */
  __IOM uint32_t AIRCR;                  /*!< Offset: 0x00C (R/W)  Application Interrupt and Reset Control Register */
  __IOM uint32_t SCR;                    /*!< Offset: 0x010 (R/W)  System Control Register */
  __IOM uint32_t CCR;                    /*!< Offset: 0x014 (R/W)  Configuration Control Register */
  __IOM uint8_t  SHP[12U];               /*!< Offset: 0x018 (R/W)  System Handlers Priority Registers (4-7, 8-11, 12-15) */
  __IOM uint32_t SHCSR;                  /*!< Offset: 0x024 (R/W)  System Handler Control and State Register */
  __IOM uint32_t CFSR;                   /*!< Offset: 0x028 (R/W)  Configurable Fault Status Register */
  __IOM uint32_t HFSR;                   /*!< Offset: 0x02C (R/W)  HardFault Status Register */
  __IOM uint32_t DFSR;                   /*!< Offset: 0x030 (R/W)  Debug Fault Status Register */
  __IOM uint32_t MMFAR;                  /*!< Offset: 0x034 (R/W)  MemManage Fault Address Register */
  __IOM uint32_t BFAR;                   /*!< Offset: 0x038 (R/W)  BusFault Address Register */
  __IOM uint32_t AFSR;                   /*!< Offset: 0x03C (R/W)  Auxiliary Fault Status Register */
  __IM  uint32_t PFR[2U];                /*!< Offset: 0x040 (R/ )  Processor Feature Register */
  __IM  uint32_t DFR;                    /*!< Offset: 0x048 (R/ )  Debug Feature Register */
  __IM  uint32_t ADR;                    /*!< Offset: 0x04C (R/ )  Auxiliary Feature Register */
  __IM  uint32_t MMFR[4U];               /*!< Offset: 0x050 (R/ )  Memory Model Feature Register */
  __IM  uint32_t ISAR[5U];               /*!< Offset: 0x060 (R/ )  Instruction Set Attributes Register */
        uint32_t RESERVED0[5U];
  __IOM uint32_t CPACR;                  /*!< Offset: 0x088 (R/W)  Coprocessor Access Control Register */
} SCB_Type;


struct __task_cycle_info_t {
    task_cycle_info_t   tInfo;
    int64_t             lLastTimeStamp;
} ;

/*============================ LOCAL VARIABLES ===============================*/
volatile static int32_t s_nCycleCounts = 0;
volatile static int32_t s_nOffset = 0;
volatile static int32_t s_nUnit = 1;
volatile static int64_t s_lSystemClockCounts = 0;


void user_code_insert_to_systick_handler(void)
{
    uint32_t wLoad = SysTick->LOAD + 1;
    s_nCycleCounts += wLoad;
    s_lSystemClockCounts += wLoad;
}


/*! \brief try to start the performance counter
 *! \retval false the LOAD register is too small
 *! \retval true performance counter starts
*/
bool start_cycle_counter(void)
{
    if (SysTick->LOAD < PERF_CNT_COMPENSATION_THRESHOLD)
    {
        return false;
    }

    int32_t  systick_value;
    uint32_t tmep = __get_PRIMASK();
    __disable_irq();
    systick_value = (int32_t)SysTick->VAL;
    s_nCycleCounts = systick_value - (int32_t)SysTick->LOAD;
    __set_PRIMASK(tmep);

    return true;
}


/*! \note this function should only be called when irq is disabled
 *!       hence SysTick-LOAD and (SCB->ICSR & SCB_ICSR_PENDSTSET_Msk)
 *!       won't change.
 */
static inline int32_t check_systick(void)
{
    int32_t  systick_value = (int32_t)SysTick->VAL;
    int32_t nTemp = (int32_t)SysTick->LOAD - systick_value;

    /*! \note Since we cannot stop counting temporarily, there are several
     *!       conditions which we should take into consideration:
     *!       Condition 1: when assigning nTemp with the register value (LOAD-VAL),
     *!           the underflow didn't happen but when we check the PENDSTSET bit,
     *!           the underflow happens, for this condition, we should not
     *!           do any compensation. When this happens, the (LOAD-nTemp) is
     *!           smaller than PERF_CNT_COMPENSATION_THRESHOLD (a small value) as
     *!           long as LOAD is bigger than (or equals to) the
     *!           PERF_CNT_COMPENSATION_THRESHOLD;
     *!       Condition 2: when assigning nTemp with the register value (LOAD-VAL),
     *!           the VAL is zero and underflow happened and the PENDSTSET bit
     *!           is set, for this condition, we should not do any compensation.
     *!           When this happens, the (LOAD-nTemp) is equals to zero.
     *!       Condition 3: when assigning nTemp with the register value (LOAD-VAL),
     *!           the underflow has already happened, hence the PENDSTSET
     *!           is set, for this condition, we should compensate the return
     *!           value. When this happens, the (LOAD-nTemp) is bigger than (or
     *!           equals to) PERF_CNT_COMPENSATION_THRESHOLD.
     *!       The following code implements an equivalent logic.
     */
    if (SCB->ICSR & SCB_ICSR_PENDSTSET_Msk){
        if (((int32_t)SysTick->LOAD - nTemp) >= PERF_CNT_COMPENSATION_THRESHOLD) {
            nTemp += SysTick->LOAD + 1;
        }
    }

    return nTemp;
}

/*! \brief calculate the elapsed cycle count since the last start point
 *!
 *! \note you can have multiple stop_cycle_counter following one start point
 *!
 *! \return the elapsed cycle count.
 */
int32_t stop_cycle_counter(void)
{
    int32_t nTemp = 0;

    uint32_t tmep = __get_PRIMASK();
    __disable_irq();
    nTemp = check_systick() + s_nCycleCounts;
    __set_PRIMASK(tmep);


    return nTemp - s_nOffset;
}

/*! \brief   initialise cycle counter service
 *!          and don't forget to tell the function whether the systick is already
 *!          used by user applications.
 *!          Don't worry, this cycle counter service won't affect your existing
 *!          systick service.
 *! \param bSysTickIsOccupied  A boolean value which indicates whether SysTick
 *!          is already used by user application.
 */
void init_cycle_counter(void)
{
    extern uint32_t SystemCoreClock;
    start_cycle_counter();
    //s_nSystemClockCounts = s_nCycleCounts;
    s_nOffset = stop_cycle_counter();

    s_nUnit = SystemCoreClock/1000000ul;
}

int64_t get_system_ticks(void)
{
    int64_t lTemp = 0;

    uint32_t tmep = __get_PRIMASK();
    __disable_irq();
    lTemp = check_systick() + s_lSystemClockCounts;
    __set_PRIMASK(tmep);

    return lTemp;
}


void delay_us(int32_t nUs)
{
    int64_t lUs = nUs * s_nUnit;

    if (lUs <= PERF_CNT_DELAY_US_COMPENSATION) {
        return ;
    }

    lUs -= PERF_CNT_DELAY_US_COMPENSATION;

    lUs += get_system_ticks();
    while(get_system_ticks() < lUs);
}


task_cycle_info_t * get_rtos_task_cycle_info(void)
{
    return NULL;
}


task_cycle_info_t * get_rtos_task_cycle_info_by_stack(void * pxStack)
{
    return  &(((struct __task_cycle_info_t *)pxStack)->tInfo);;
}


void __on_context_switch_in(uint32_t *pwStack)
{

    struct __task_cycle_info_t *ptFrame = (struct __task_cycle_info_t *)pwStack;
    uint64_t dwTimeStamp = get_system_ticks();

    if (0 == ptFrame->tInfo.lStart) {
        ptFrame->tInfo.lStart = dwTimeStamp;
    }
    ptFrame->lLastTimeStamp = dwTimeStamp;
    ptFrame->tInfo.wActiveCount++;

}

void __on_context_switch_out(uint32_t *pwStack)
{

    uint64_t dwTimeStamp = get_system_ticks();
    struct __task_cycle_info_t *ptFrame = (struct __task_cycle_info_t *)pwStack;

    ptFrame->tInfo.nUsedRecent = dwTimeStamp - ptFrame->lLastTimeStamp;
    ptFrame->tInfo.lUsedTotal += ptFrame->tInfo.nUsedRecent;

    if(ptFrame->tInfo.nUsedRecent > ptFrame->tInfo.nUsedMax)
        ptFrame->tInfo.nUsedMax = ptFrame->tInfo.nUsedRecent;

    if((ptFrame->tInfo.nUsedMin > ptFrame->tInfo.nUsedRecent) || (ptFrame->tInfo.nUsedMin == 0))
        ptFrame->tInfo.nUsedMin = ptFrame->tInfo.nUsedRecent;
}

void start_task_cycle_counter(void)
{
    struct __task_cycle_info_t * ptInfo =
        (struct __task_cycle_info_t *)get_rtos_task_cycle_info();
    if (NULL == ptInfo) {
        return ;
    }

    uint32_t tmep = __get_PRIMASK();
    __disable_irq();
    ptInfo->lLastTimeStamp = get_system_ticks();
    ptInfo->tInfo.lUsedTotal = 0;
    __set_PRIMASK(tmep);
}

int64_t stop_task_cycle_counter(void)
{
    struct __task_cycle_info_t * ptInfo =
        (struct __task_cycle_info_t *)get_rtos_task_cycle_info();
    if (NULL == ptInfo) {
        return 0;
    }

    int64_t lCycles = 0;

    uint32_t tmep = __get_PRIMASK();
    __disable_irq();
    lCycles = ptInfo->tInfo.lUsedTotal + (get_system_ticks() - ptInfo->lLastTimeStamp);
    __set_PRIMASK(tmep);


    return lCycles;
}

unsigned int perf_test(char argc,char ** argv)
{
    int32_t counter1,counter2;

    //IO_RESET_8155P_Po(true);
    start_cycle_counter();
    delay_us(100);
	//IO_RESET_8155P_Po(false);
    counter1 = stop_cycle_counter();
	//IO_RESET_8155P_Po(true);
    //OS_Sleep(1);
	//IO_RESET_8155P_Po(false);
    counter2 = stop_cycle_counter();
    printf("counter1 %d counter2 %d\r\n",counter1,counter2);
    return 0;
}

LTSH_FUNCTION_EXPORT(perf_test,"test perf coumter");

#endif

