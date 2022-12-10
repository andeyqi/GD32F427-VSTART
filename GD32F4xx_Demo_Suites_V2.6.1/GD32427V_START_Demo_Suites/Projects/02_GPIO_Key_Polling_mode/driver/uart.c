#include "gd32f4xx.h"
#include <stdio.h>
#include <string.h>
#include "utilities.h"
#include "ringbuffer.h"

static  RingBuffer uart_rx;

void uart_init(void)
{
    static uint8_t uart_rx_buff[128] = {0};
    RingBuffer_Init(&uart_rx,uart_rx_buff,128);

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

    /* enable USART0 receive interrupt */
    usart_interrupt_enable(USART0, USART_INT_RBNE);

    /* USART interrupt configuration */
    nvic_irq_enable(USART0_IRQn, 0, 0);

    usart_enable(USART0);
}


/*!
    \brief      this function handles USART0 exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void USART0_IRQHandler(void)
{
    uint8_t data = 0;
    if(RESET != usart_interrupt_flag_get(USART0, USART_INT_FLAG_RBNE)){
        /* read one byte from the receive data register */
        data = (uint8_t)usart_data_receive(USART0);
        RingBuffer_Write(&uart_rx,&data,1);
    }
}

uint8_t uartgetchar(uint8_t * pdata)
{
    return RingBuffer_Read(&uart_rx,pdata,1);
}

/* retarget the C library printf function to the USART */
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t)ch);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    return ch;
}