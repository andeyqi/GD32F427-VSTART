#ifndef __DRV_GPIO_H__
#define __DRV_GPIO_H__

#ifdef __cplusplus
extern "C" {
#endif

#if defined SOC_SERIES_GD32F10x
#include "gd32f10x_gpio.h"
#elif defined SOC_SERIES_GD32F20x
#include "gd32f20x_gpio.h"
#elif defined SOC_SERIES_GD32F30x
#include "gd32f30x_gpio.h"
#elif defined SOC_SERIES_GD32F4xx
#include "gd32f4xx_gpio.h"
#endif

#define __GD32_PORT(port)  GPIO##port

#if defined SOC_SERIES_GD32F4xx
#define GD32_PIN(index, port, pin) {index, RCU_GPIO##port,      \
                                    GPIO##port, GPIO_PIN_##pin, \
                                    EXTI_SOURCE_GPIO##port,     \
                                    EXTI_SOURCE_PIN##pin}
#else
#define GD32_PIN(index, port, pin) {index, RCU_GPIO##port,        \
                                    GPIO##port, GPIO_PIN_##pin,   \
                                    GPIO_PORT_SOURCE_GPIO##port,  \
                                    GPIO_PIN_SOURCE_##pin}

#endif

#define GD32_PIN_DEFAULT            {-1, (rcu_periph_enum)0, 0, 0, 0, 0}

#define PIN_NONE                (-1)

#define PIN_LOW                 0x00
#define PIN_HIGH                0x01

#define PIN_MODE_OUTPUT         0x00
#define PIN_MODE_INPUT          0x01
#define PIN_MODE_INPUT_PULLUP   0x02
#define PIN_MODE_INPUT_PULLDOWN 0x03
#define PIN_MODE_OUTPUT_OD      0x04

struct pin_index
{
    int16_t index;
    rcu_periph_enum clk;
    uint32_t gpio_periph;
    uint32_t pin;
    uint8_t port_src;
    uint8_t pin_src;
};

void gd32_pin_mode(int32_t pin, int32_t mode);
void gd32_pin_write(int32_t pin, int32_t value);
int gd32_pin_read(int32_t pin);


#ifdef __cplusplus
}
#endif

#endif /* __DRV_GPIO_H__ */