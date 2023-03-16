/*!
    \file    main.c
    \brief   key polling mode demo

    \version 2022-04-26, V2.0.0, demo for GD32F4xx
*/

/*
    Copyright (c) 2022, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors
       may be used to endorse or promote products derived from this software without
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
OF SUCH DAMAGE.
*/

#include "gd32f4xx.h"
#include "systick.h"
#include <stdio.h>
#include <string.h>
#include "littleshell.h"
#include "FreeRTOS.h"
#include "task.h"
#include "utilities.h"
#include "uart.h"
#include "drv_gpio.h"

#define START_TASK_PRIO        1
#define START_STK_SIZE         128
TaskHandle_t StartTask_Handler;
void start_task(void *pvParameters);

#define TASK1_TASK_PRIO        2
#define TASK1_STK_SIZE         128
TaskHandle_t Task1Task_Handler;
void start_task1(void *pvParameters);

#define SHELL_TASK_PRIO        2
#define SHELL_STK_SIZE         256
TaskHandle_t ShellTask_Handler;


void start_task(void *pvParameters)
{
    gd32_pin_mode(38,PIN_MODE_OUTPUT);
    while(1)
    {
        gd32_pin_write(38,1);
        vTaskDelay(500);
        gd32_pin_write(38,0);
        vTaskDelay(500);
    }

}


void start_task1(void *pvParameters)
{
    gd32_pin_mode(0,PIN_MODE_INPUT);
    int value_old = 0,value_new = 0;

    value_old = gd32_pin_read(0);
    while(1)
    {
        value_new = gd32_pin_read(0);
        if(value_old != value_new)
        {
            printf("key %d \r\n",value_new);
            value_old = value_new;
        }
        vTaskDelay(50);
    }
}

void show_board_info(void)
{
    printf("\r\n");
    printf("GD32F427V-START %s %s.\r\n",__DATE__,__TIME__);
    printf("\r\n");
}

/*!
    \brief      main function
    \param[in]  none
    \param[out] none
    \retval     none
*/
int main(void)
{
    /* configure systick */
    //systick_config();

    /* enable the LEDs GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOC);
    rcu_periph_clock_enable(RCU_CRC);

    /* configure LED1 GPIO port */
    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_6);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    /* reset LED1 GPIO pin */
    gpio_bit_reset(GPIOC, GPIO_PIN_6);

    uart_init();

    (void)show_board_info();


    xTaskCreate((TaskFunction_t )start_task,
                (const char*    )"task1",
                (uint16_t       )START_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )START_TASK_PRIO,
                (TaskHandle_t*  )&StartTask_Handler);

    xTaskCreate((TaskFunction_t )start_task1,
                (const char*    )"task2",
                (uint16_t       )TASK1_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )TASK1_TASK_PRIO,
                (TaskHandle_t*  )&Task1Task_Handler);

    xTaskCreate((TaskFunction_t )littleshell_main_entry,
                (const char*    )"shell",
                (uint16_t       )SHELL_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )SHELL_TASK_PRIO,
                (TaskHandle_t*  )&Task1Task_Handler);
    vTaskStartScheduler();
}


