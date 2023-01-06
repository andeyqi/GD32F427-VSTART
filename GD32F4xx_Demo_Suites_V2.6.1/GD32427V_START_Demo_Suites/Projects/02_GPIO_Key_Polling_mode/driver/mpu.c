#include "mpu_reg.h"
#include "gd32f4xx.h"
#include <stdio.h>
#include "littleshell.h"
#include "utilities.h"
#include "core_cm4.h"

/** @defgroup CORTEX_MPU_Region_Initialization_Structure_definition MPU Region Initialization Structure Definition
  * @{
  */
typedef struct
{
  uint8_t                Enable;                /*!< Specifies the status of the region.
                                                     This parameter can be a value of @ref CORTEX_MPU_Region_Enable                 */
  uint8_t                Number;                /*!< Specifies the number of the region to protect.
                                                     This parameter can be a value of @ref CORTEX_MPU_Region_Number                 */
  uint32_t               BaseAddress;           /*!< Specifies the base address of the region to protect.                           */
  uint8_t                Size;                  /*!< Specifies the size of the region to protect.
                                                     This parameter can be a value of @ref CORTEX_MPU_Region_Size                   */
  uint8_t                SubRegionDisable;      /*!< Specifies the number of the subregion protection to disable.
                                                     This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF    */
  uint8_t                TypeExtField;          /*!< Specifies the TEX field level.
                                                     This parameter can be a value of @ref CORTEX_MPU_TEX_Levels                    */
  uint8_t                AccessPermission;      /*!< Specifies the region access permission type.
                                                     This parameter can be a value of @ref CORTEX_MPU_Region_Permission_Attributes  */
  uint8_t                DisableExec;           /*!< Specifies the instruction access status.
                                                     This parameter can be a value of @ref CORTEX_MPU_Instruction_Access            */
  uint8_t                IsShareable;           /*!< Specifies the shareability status of the protected region.
                                                     This parameter can be a value of @ref CORTEX_MPU_Access_Shareable              */
  uint8_t                IsCacheable;           /*!< Specifies the cacheable status of the region protected.
                                                     This parameter can be a value of @ref CORTEX_MPU_Access_Cacheable              */
  uint8_t                IsBufferable;          /*!< Specifies the bufferable status of the protected region.
                                                     This parameter can be a value of @ref CORTEX_MPU_Access_Bufferable             */
}MPU_Region_InitTypeDef;


/** @defgroup CORTEX_MPU_HFNMI_PRIVDEF_Control CORTEX MPU HFNMI and PRIVILEGED Access control
  * @{
  */
#define  MPU_HFNMI_PRIVDEF_NONE      ((uint32_t)0x00000000)
#define  MPU_HARDFAULT_NMI           ((uint32_t)0x00000002)
#define  MPU_PRIVILEGED_DEFAULT      ((uint32_t)0x00000004)
#define  MPU_HFNMI_PRIVDEF           ((uint32_t)0x00000006)
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Region_Enable CORTEX MPU Region Enable
  * @{
  */
#define  MPU_REGION_ENABLE           ((uint8_t)0x01)
#define  MPU_REGION_DISABLE          ((uint8_t)0x00)
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Instruction_Access CORTEX MPU Instruction Access
  * @{
  */
#define  MPU_INSTRUCTION_ACCESS_ENABLE      ((uint8_t)0x00)
#define  MPU_INSTRUCTION_ACCESS_DISABLE     ((uint8_t)0x01)
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Access_Shareable CORTEX MPU Instruction Access Shareable
  * @{
  */
#define  MPU_ACCESS_SHAREABLE        ((uint8_t)0x01)
#define  MPU_ACCESS_NOT_SHAREABLE    ((uint8_t)0x00)
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Access_Cacheable CORTEX MPU Instruction Access Cacheable
  * @{
  */
#define  MPU_ACCESS_CACHEABLE        ((uint8_t)0x01)
#define  MPU_ACCESS_NOT_CACHEABLE    ((uint8_t)0x00)
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Access_Bufferable CORTEX MPU Instruction Access Bufferable
  * @{
  */
#define  MPU_ACCESS_BUFFERABLE       ((uint8_t)0x01)
#define  MPU_ACCESS_NOT_BUFFERABLE   ((uint8_t)0x00)
/**
  * @}
  */

/** @defgroup CORTEX_MPU_TEX_Levels CORTEX MPU TEX Levels
  * @{
  */
#define  MPU_TEX_LEVEL0              ((uint8_t)0x00)
#define  MPU_TEX_LEVEL1              ((uint8_t)0x01)
#define  MPU_TEX_LEVEL2              ((uint8_t)0x02)
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Region_Size CORTEX MPU Region Size
  * @{
  */
#define   MPU_REGION_SIZE_32B        ((uint8_t)0x04)
#define   MPU_REGION_SIZE_64B        ((uint8_t)0x05)
#define   MPU_REGION_SIZE_128B       ((uint8_t)0x06)
#define   MPU_REGION_SIZE_256B       ((uint8_t)0x07)
#define   MPU_REGION_SIZE_512B       ((uint8_t)0x08)
#define   MPU_REGION_SIZE_1KB        ((uint8_t)0x09)
#define   MPU_REGION_SIZE_2KB        ((uint8_t)0x0A)
#define   MPU_REGION_SIZE_4KB        ((uint8_t)0x0B)
#define   MPU_REGION_SIZE_8KB        ((uint8_t)0x0C)
#define   MPU_REGION_SIZE_16KB       ((uint8_t)0x0D)
#define   MPU_REGION_SIZE_32KB       ((uint8_t)0x0E)
#define   MPU_REGION_SIZE_64KB       ((uint8_t)0x0F)
#define   MPU_REGION_SIZE_128KB      ((uint8_t)0x10)
#define   MPU_REGION_SIZE_256KB      ((uint8_t)0x11)
#define   MPU_REGION_SIZE_512KB      ((uint8_t)0x12)
#define   MPU_REGION_SIZE_1MB        ((uint8_t)0x13)
#define   MPU_REGION_SIZE_2MB        ((uint8_t)0x14)
#define   MPU_REGION_SIZE_4MB        ((uint8_t)0x15)
#define   MPU_REGION_SIZE_8MB        ((uint8_t)0x16)
#define   MPU_REGION_SIZE_16MB       ((uint8_t)0x17)
#define   MPU_REGION_SIZE_32MB       ((uint8_t)0x18)
#define   MPU_REGION_SIZE_64MB       ((uint8_t)0x19)
#define   MPU_REGION_SIZE_128MB      ((uint8_t)0x1A)
#define   MPU_REGION_SIZE_256MB      ((uint8_t)0x1B)
#define   MPU_REGION_SIZE_512MB      ((uint8_t)0x1C)
#define   MPU_REGION_SIZE_1GB        ((uint8_t)0x1D)
#define   MPU_REGION_SIZE_2GB        ((uint8_t)0x1E)
#define   MPU_REGION_SIZE_4GB        ((uint8_t)0x1F)
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Region_Permission_Attributes CORTEX MPU Region Permission Attributes
  * @{
  */
#define  MPU_REGION_NO_ACCESS        ((uint8_t)0x00)
#define  MPU_REGION_PRIV_RW          ((uint8_t)0x01)
#define  MPU_REGION_PRIV_RW_URO      ((uint8_t)0x02)
#define  MPU_REGION_FULL_ACCESS      ((uint8_t)0x03)
#define  MPU_REGION_PRIV_RO          ((uint8_t)0x05)
#define  MPU_REGION_PRIV_RO_URO      ((uint8_t)0x06)
/**
  * @}
  */

/** @defgroup CORTEX_MPU_Region_Number CORTEX MPU Region Number
  * @{
  */
#define  MPU_REGION_NUMBER0          ((uint8_t)0x00)
#define  MPU_REGION_NUMBER1          ((uint8_t)0x01)
#define  MPU_REGION_NUMBER2          ((uint8_t)0x02)
#define  MPU_REGION_NUMBER3          ((uint8_t)0x03)
#define  MPU_REGION_NUMBER4          ((uint8_t)0x04)
#define  MPU_REGION_NUMBER5          ((uint8_t)0x05)
#define  MPU_REGION_NUMBER6          ((uint8_t)0x06)
#define  MPU_REGION_NUMBER7          ((uint8_t)0x07)
/**
  * @}
  */

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define EXAMPLE_RAM_ADDRESS_START                (0x20000000UL)
#define EXAMPLE_RAM_SIZE                         MPU_REGION_SIZE_32KB
#define EXAMPLE_PERIPH_ADDRESS_START             (0x40000000)
#define EXAMPLE_PERIPH_SIZE                      MPU_REGION_SIZE_512MB
#define EXAMPLE_FLASH_ADDRESS_START              (0x08000000)
#define EXAMPLE_FLASH_SIZE                       MPU_REGION_SIZE_128KB
#define EXAMPLE_RAM_REGION_NUMBER                MPU_REGION_NUMBER0
#define EXAMPLE_FLASH_REGION_NUMBER              MPU_REGION_NUMBER1
#define EXAMPLE_PERIPH_REGION_NUMBER             MPU_REGION_NUMBER2
#define portMPU_REGION_READ_WRITE                MPU_REGION_FULL_ACCESS
#define portMPU_REGION_PRIVILEGED_READ_ONLY      MPU_REGION_PRIV_RO
#define portMPU_REGION_READ_ONLY                 MPU_REGION_PRIV_RO_URO
#define portMPU_REGION_PRIVILEGED_READ_WRITE     MPU_REGION_PRIV_RW


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define ARRAY_ADDRESS_START    (0x20002000UL)
#define ARRAY_SIZE             MPU_REGION_SIZE_32B
#define ARRAY_REGION_NUMBER    MPU_REGION_NUMBER3


#pragma location=0x20002000
__no_init uint8_t PrivilegedReadOnlyArray[32];

void HAL_MPU_Disable(void)
{
  /* Make sure outstanding transfers are done */
  __DMB();

  /* Disable fault exceptions */
  SCB->SHCSR &= ~SCB_SHCSR_MEMFAULTENA_Msk;

  /* Disable the MPU and clear the control register*/
  MPU->CTRL = 0U;
}


void HAL_MPU_Enable(uint32_t MPU_Control)
{
  /* Enable the MPU */
  MPU->CTRL = MPU_Control | MPU_CTRL_ENABLE_Msk;

  /* Enable fault exceptions */
  SCB->SHCSR |= SCB_SHCSR_MEMFAULTENA_Msk;

  /* Ensure MPU settings take effects */
  __DSB();
  __ISB();
}


/**
  * @brief  Initialize and configure the Region and the memory to be protected.
  * @param  MPU_Init: Pointer to a MPU_Region_InitTypeDef structure that contains
  *                the initialization and configuration information.
  * @retval None
  */
void HAL_MPU_ConfigRegion(MPU_Region_InitTypeDef *MPU_Init)
{
  /* Set the Region number */
  MPU->RNR = MPU_Init->Number;

  if ((MPU_Init->Enable) != RESET)
  {

    MPU->RBAR = MPU_Init->BaseAddress;
    MPU->RASR = ((uint32_t)MPU_Init->DisableExec        << MPU_RASR_XN_Pos)   |
                ((uint32_t)MPU_Init->AccessPermission   << MPU_RASR_AP_Pos)   |
                ((uint32_t)MPU_Init->TypeExtField       << MPU_RASR_TEX_Pos)  |
                ((uint32_t)MPU_Init->IsShareable        << MPU_RASR_S_Pos)    |
                ((uint32_t)MPU_Init->IsCacheable        << MPU_RASR_C_Pos)    |
                ((uint32_t)MPU_Init->IsBufferable       << MPU_RASR_B_Pos)    |
                ((uint32_t)MPU_Init->SubRegionDisable   << MPU_RASR_SRD_Pos)  |
                ((uint32_t)MPU_Init->Size               << MPU_RASR_SIZE_Pos) |
                ((uint32_t)MPU_Init->Enable             << MPU_RASR_ENABLE_Pos);
  }
  else
  {
    MPU->RBAR = 0x00;
    MPU->RASR = 0x00;
  }
}


/**
  * @brief  This function configures the access right using Cortex-M4 MPU regions.
  * @param  None
  * @retval None
  */
void MPU_AccessPermConfig(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;

  /* Configure region for PrivilegedReadOnlyArray as REGION N�3, 32byte and R
     only in privileged mode */
  /* Disable MPU */
  HAL_MPU_Disable();

  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = ARRAY_ADDRESS_START;
  MPU_InitStruct.Size = ARRAY_SIZE;
  MPU_InitStruct.AccessPermission = portMPU_REGION_PRIVILEGED_READ_ONLY;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = ARRAY_REGION_NUMBER;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable MPU (any access not covered by any enabled region will cause a fault) */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

  /* Read from PrivilegedReadOnlyArray. This will not generate error */
  (void)PrivilegedReadOnlyArray[0];

  /* Uncomment the following line to write to PrivilegedReadOnlyArray. This will
     generate error */
   PrivilegedReadOnlyArray[0] = 'e';
}


unsigned int mputest(char argc,char ** argv)
{
    MPU_AccessPermConfig();
    return 1;
}
LTSH_FUNCTION_EXPORT(mputest,"test mpu api");