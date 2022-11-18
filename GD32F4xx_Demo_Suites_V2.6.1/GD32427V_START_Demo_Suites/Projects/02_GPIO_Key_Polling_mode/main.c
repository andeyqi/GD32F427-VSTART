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


#define START_TASK_PRIO        1
#define START_STK_SIZE         128  
TaskHandle_t StartTask_Handler;
void start_task(void *pvParameters);

#define TASK1_TASK_PRIO        2
#define TASK1_STK_SIZE         128  
TaskHandle_t Task1Task_Handler;
void start_task1(void *pvParameters);

void start_task(void *pvParameters)
{
	while(1)
	{
		printf("I am task2\r\n" );
		vTaskDelay(1000);
	}

}


void start_task1(void *pvParameters)
{
	while(1)
	{
		printf("I am task1\r\n" );
		vTaskDelay(1000);
	}
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

    /* configure LED1 GPIO port */
    gpio_mode_set(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO_PIN_6);
    gpio_output_options_set(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
    /* reset LED1 GPIO pin */
    gpio_bit_reset(GPIOC, GPIO_PIN_6);
	
    /* enable GPIO clock */
    rcu_periph_clock_enable(RCU_GPIOB);

    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);

    /* configure USART0 TX as alternate function push-pull */
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_6);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, GPIO_PIN_6);

    /* configure USART0 RX as alternate function push-pull */
    gpio_mode_set(GPIOB, GPIO_MODE_AF, GPIO_PUPD_PULLUP, GPIO_PIN_7);
    gpio_output_options_set(GPIOB, GPIO_OTYPE_PP, GPIO_OSPEED_25MHZ, GPIO_PIN_7);
    
    /* configure the USART0 TX pin and USART0 RX pin */
    gpio_af_set(GPIOB, GPIO_AF_7, GPIO_PIN_6);
    gpio_af_set(GPIOB, GPIO_AF_7, GPIO_PIN_7);



    /* USART configure */
    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200U);
    usart_parity_config(USART0,USART_PM_NONE);
    usart_word_length_set(USART0,USART_WL_8BIT);
    usart_stop_bit_set(USART0,USART_STB_1BIT);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    
    usart_enable(USART0);


    xTaskCreate((TaskFunction_t )start_task,           
                (const char*    )"start_task",        
                (uint16_t       )START_STK_SIZE,      
                (void*          )NULL,                  
                (UBaseType_t    )START_TASK_PRIO,      
                (TaskHandle_t*  )&StartTask_Handler); 
	
    xTaskCreate((TaskFunction_t )start_task1,           
                (const char*    )"start_task",        
                (uint16_t       )TASK1_STK_SIZE,      
                (void*          )NULL,                  
                (UBaseType_t    )TASK1_TASK_PRIO,      
                (TaskHandle_t*  )&Task1Task_Handler); 

	vTaskStartScheduler(); 
    //(void)littleshell_main_entry(NULL);
}


/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t)ch);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    return ch;
}

uint8_t uartgetchar(uint8_t * pdata)
{
    if(SET == usart_flag_get(USART0, USART_FLAG_RBNE))
    {
        *pdata = usart_data_receive(USART0);
	return 1;
    }
    else
        return 0;
}

unsigned int led(char argc,char ** argv)
{
	if(argc != 2)
		return 0;

	if(strcmp("on",argv[1]) == 0)
	{
        /* turn on LED1 */
        gpio_bit_set(GPIOC, GPIO_PIN_6);
	}
	else if(strcmp("off",argv[1]) == 0)
	{
		gpio_bit_reset(GPIOC, GPIO_PIN_6);
	}
	
	return 1;
}
LTSH_FUNCTION_EXPORT(led,"test led on/off");


