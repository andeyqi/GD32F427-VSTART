#include "drv_gpio.h"

static const struct pin_index pins[] =
{
#ifdef GPIOA
    GD32_PIN(0,  A, 0),
    GD32_PIN(1,  A, 1),
    GD32_PIN(2,  A, 2),
    GD32_PIN(3,  A, 3),
    GD32_PIN(4,  A, 4),
    GD32_PIN(5,  A, 5),
    GD32_PIN(6,  A, 6),
    GD32_PIN(7,  A, 7),
    GD32_PIN(8,  A, 8),
    GD32_PIN(9,  A, 9),
    GD32_PIN(10, A, 10),
    GD32_PIN(11, A, 11),
    GD32_PIN(12, A, 12),
    GD32_PIN(13, A, 13),
    GD32_PIN(14, A, 14),
    GD32_PIN(15, A, 15),
#endif
#ifdef GPIOB
    GD32_PIN(16, B, 0),
    GD32_PIN(17, B, 1),
    GD32_PIN(18, B, 2),
    GD32_PIN(19, B, 3),
    GD32_PIN(20, B, 4),
    GD32_PIN(21, B, 5),
    GD32_PIN(22, B, 6),
    GD32_PIN(23, B, 7),
    GD32_PIN(24, B, 8),
    GD32_PIN(25, B, 9),
    GD32_PIN(26, B, 10),
    GD32_PIN(27, B, 11),
    GD32_PIN(28, B, 12),
    GD32_PIN(29, B, 13),
    GD32_PIN(30, B, 14),
    GD32_PIN(31, B, 15),
#endif
#ifdef GPIOC
    GD32_PIN(32, C, 0),
    GD32_PIN(33, C, 1),
    GD32_PIN(34, C, 2),
    GD32_PIN(35, C, 3),
    GD32_PIN(36, C, 4),
    GD32_PIN(37, C, 5),
    GD32_PIN(38, C, 6),
    GD32_PIN(39, C, 7),
    GD32_PIN(40, C, 8),
    GD32_PIN(41, C, 9),
    GD32_PIN(42, C, 10),
    GD32_PIN(43, C, 11),
    GD32_PIN(44, C, 12),
    GD32_PIN(45, C, 13),
    GD32_PIN(46, C, 14),
    GD32_PIN(47, C, 15),
#endif
#ifdef GPIOD
    GD32_PIN(48, D, 0),
    GD32_PIN(49, D, 1),
    GD32_PIN(50, D, 2),
    GD32_PIN(51, D, 3),
    GD32_PIN(52, D, 4),
    GD32_PIN(53, D, 5),
    GD32_PIN(54, D, 6),
    GD32_PIN(55, D, 7),
    GD32_PIN(56, D, 8),
    GD32_PIN(57, D, 9),
    GD32_PIN(58, D, 10),
    GD32_PIN(59, D, 11),
    GD32_PIN(60, D, 12),
    GD32_PIN(61, D, 13),
    GD32_PIN(62, D, 14),
    GD32_PIN(63, D, 15),
#endif
#ifdef GPIOE
    GD32_PIN(64, E, 0),
    GD32_PIN(65, E, 1),
    GD32_PIN(66, E, 2),
    GD32_PIN(67, E, 3),
    GD32_PIN(68, E, 4),
    GD32_PIN(69, E, 5),
    GD32_PIN(70, E, 6),
    GD32_PIN(71, E, 7),
    GD32_PIN(72, E, 8),
    GD32_PIN(73, E, 9),
    GD32_PIN(74, E, 10),
    GD32_PIN(75, E, 11),
    GD32_PIN(76, E, 12),
    GD32_PIN(77, E, 13),
    GD32_PIN(78, E, 14),
    GD32_PIN(79, E, 15),
#endif
#ifdef GPIOF
    GD32_PIN(80, F, 0),
    GD32_PIN(81, F, 1),
    GD32_PIN(82, F, 2),
    GD32_PIN(83, F, 3),
    GD32_PIN(84, F, 4),
    GD32_PIN(85, F, 5),
    GD32_PIN(86, F, 6),
    GD32_PIN(87, F, 7),
    GD32_PIN(88, F, 8),
    GD32_PIN(89, F, 9),
    GD32_PIN(90, F, 10),
    GD32_PIN(91, F, 11),
    GD32_PIN(92, F, 12),
    GD32_PIN(93, F, 13),
    GD32_PIN(94, F, 14),
    GD32_PIN(95, F, 15),
#endif
#ifdef GPIOG
    GD32_PIN(96, G, 0),
    GD32_PIN(97, G, 1),
    GD32_PIN(98, G, 2),
    GD32_PIN(99, G, 3),
    GD32_PIN(100, G, 4),
    GD32_PIN(101, G, 5),
    GD32_PIN(102, G, 6),
    GD32_PIN(103, G, 7),
    GD32_PIN(104, G, 8),
    GD32_PIN(105, G, 9),
    GD32_PIN(106, G, 10),
    GD32_PIN(107, G, 11),
    GD32_PIN(108, G, 12),
    GD32_PIN(109, G, 13),
    GD32_PIN(110, G, 14),
    GD32_PIN(111, G, 15),
#endif
#ifdef GPIOH
    GD32_PIN(112, H, 0),
    GD32_PIN(113, H, 1),
    GD32_PIN(114, H, 2),
    GD32_PIN(115, H, 3),
    GD32_PIN(116, H, 4),
    GD32_PIN(117, H, 5),
    GD32_PIN(118, H, 6),
    GD32_PIN(119, H, 7),
    GD32_PIN(120, H, 8),
    GD32_PIN(121, H, 9),
    GD32_PIN(122, H, 10),
    GD32_PIN(123, H, 11),
    GD32_PIN(124, H, 12),
    GD32_PIN(125, H, 13),
    GD32_PIN(126, H, 14),
    GD32_PIN(127, H, 15),
#endif
#ifdef GPIOI
    GD32_PIN(128, I, 0),
    GD32_PIN(129, I, 1),
    GD32_PIN(130, I, 2),
    GD32_PIN(131, I, 3),
    GD32_PIN(132, I, 4),
    GD32_PIN(133, I, 5),
    GD32_PIN(134, I, 6),
    GD32_PIN(135, I, 7),
    GD32_PIN(136, I, 8),
    GD32_PIN(137, I, 9),
    GD32_PIN(138, I, 10),
    GD32_PIN(139, I, 11),
    GD32_PIN(140, I, 12),
    GD32_PIN(141, I, 13),
    GD32_PIN(142, I, 14),
    GD32_PIN(143, I, 15),
#endif
};

#define ITEM_NUM(items) sizeof(items) / sizeof(items[0])

/**
  * @brief  get pin
  * @param  pin
  * @retval None
  */
const struct pin_index *get_pin(uint8_t pin)
{
    const struct pin_index *index;

    if (pin < ITEM_NUM(pins))
    {
        index = &pins[pin];
        if (index->index == -1)
        index = NULL;
    }
    else
    {
        index = NULL;
    }

    return index;
}

/**
  * @brief  set pin mode
  * @param  pin, mode
  * @retval None
  */
void gd32_pin_mode(int32_t pin, int32_t mode)
{
    const struct pin_index *index = NULL;
    uint32_t pin_mode = 0;

#if defined SOC_SERIES_GD32F4xx
    uint32_t pin_pupd = 0, pin_odpp = 0;
#endif

    index = get_pin(pin);
    if (index == NULL)
    {
        return;
    }

    /* GPIO Periph clock enable */
    rcu_periph_clock_enable(index->clk);
#if defined SOC_SERIES_GD32F4xx
    pin_mode = GPIO_MODE_OUTPUT;
#else
    pin_mode = GPIO_MODE_OUT_PP;
#endif

    switch(mode)
    {
    case PIN_MODE_OUTPUT:
        /* output setting */
#if defined SOC_SERIES_GD32F4xx
        pin_mode = GPIO_MODE_OUTPUT;
        pin_pupd = GPIO_PUPD_NONE;
        pin_odpp = GPIO_OTYPE_PP;
#else
        pin_mode = GPIO_MODE_OUT_PP;
#endif
        break;
    case PIN_MODE_OUTPUT_OD:
        /* output setting: od. */
#if defined SOC_SERIES_GD32F4xx
        pin_mode = GPIO_MODE_OUTPUT;
        pin_pupd = GPIO_PUPD_NONE;
        pin_odpp = GPIO_OTYPE_OD;
#else
        pin_mode = GPIO_MODE_OUT_OD;
#endif
        break;
    case PIN_MODE_INPUT:
        /* input setting: not pull. */
#if defined SOC_SERIES_GD32F4xx
        pin_mode = GPIO_MODE_INPUT;
        pin_pupd = GPIO_PUPD_PULLUP | GPIO_PUPD_PULLDOWN;
#else
        pin_mode = GPIO_MODE_IN_FLOATING;
#endif
        break;
    case PIN_MODE_INPUT_PULLUP:
        /* input setting: pull up. */
#if defined SOC_SERIES_GD32F4xx
        pin_mode = GPIO_MODE_INPUT;
        pin_pupd = GPIO_PUPD_PULLUP;
#else
        pin_mode = GPIO_MODE_IPU;
#endif
        break;
    case PIN_MODE_INPUT_PULLDOWN:
        /* input setting: pull down. */
#if defined SOC_SERIES_GD32F4xx
        pin_mode = GPIO_MODE_INPUT;
        pin_pupd = GPIO_PUPD_PULLDOWN;
#else
        pin_mode = GPIO_MODE_IPD;
#endif
        break;
    default:
            break;
    }

#if defined SOC_SERIES_GD32F4xx
    gpio_mode_set(index->gpio_periph, pin_mode, pin_pupd, index->pin);
    if(pin_mode == GPIO_MODE_OUTPUT)
    {
        gpio_output_options_set(index->gpio_periph, pin_odpp, GPIO_OSPEED_50MHZ, index->pin);
    }
#else
        gpio_init(index->gpio_periph, pin_mode, GPIO_OSPEED_50MHZ, index->pin);
#endif
}


/**
  * @brief  pin write
  * @param   pin, valuie
  * @retval None
  */
void gd32_pin_write(int32_t pin, int32_t value)
{
    const struct pin_index *index = NULL;

    index = get_pin(pin);
    if (index == NULL)
    {
        return;
    }

    gpio_bit_write(index->gpio_periph, index->pin, (bit_status)value);
}

/**
  * @brief  pin read
  * @param  dev, pin
  * @retval None
  */
int gd32_pin_read(int32_t pin)
{
    int value = PIN_LOW;
    const struct pin_index *index = NULL;

    index = get_pin(pin);
    if (index == NULL)
    {
        return value;
    }

    value = gpio_input_bit_get(index->gpio_periph, index->pin);
    return value;
}