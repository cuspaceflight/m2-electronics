/*
 * M2R ChibiOS Board definition file.
*/

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Board identifier.
 */
#define BOARD_M2R
#define BOARD_NAME                  "M2R"


/*
 * Board oscillators-related settings.
 */
#if !defined(STM32_LSECLK)
#define STM32_LSECLK                0
#endif

#if !defined(STM32_HSECLK)
#define STM32_HSECLK                8000000
#endif

/*
 * Board voltages.
 * Required for performance limits calculation.
 */
#define STM32_VDD                   330

/*
 * MCU type as defined in the ST header.
 */
#define STM32F30X

/*
 * IO pins assignments.
 */
#define GPIOA_PIN0                  0
#define GPIOA_PIN1                  1
#define GPIOA_SER_OUT               2
#define GPIOA_SER_IN                3
#define GPIOA_LED_A                 4
#define GPIOA_LED_C                 5
#define GPIOA_BAT_MON               6
#define GPIOA_PIN7                  7
#define GPIOA_PIN8                  8
#define GPIOA_RB_RX                 9
#define GPIOA_RB_TX                 10
#define GPIOA_PIN11                 11
#define GPIOA_PIN12                 12
#define GPIOA_SWDIO                 13
#define GPIOA_SWCLK                 14
#define GPIOA_PIN15                 15

#define GPIOB_PIN0                  0
#define GPIOB_NTX2B_P0              1
#define GPIOB_NTX2B_P1              2
#define GPIOB_SWO                   3
#define GPIOB_PIN4                  4
#define GPIOB_GPS_RESET             5
#define GPIOB_GPS_SCL               6
#define GPIOB_GPS_SDA               7
#define GPIOB_PIN8                  8
#define GPIOB_PIN9                  9
#define GPIOB_NTX2B_EN              10
#define GPIOB_NTX2B_TXD             11
#define GPIOB_LED_RB                12
#define GPIOB_LED_RADIO             13
#define GPIOB_LED_GPS               14
#define GPIOB_LED_STATUS            15

#define GPIOC_PIN0                  0
#define GPIOC_PIN1                  1
#define GPIOC_PIN2                  2
#define GPIOC_PIN3                  3
#define GPIOC_PIN4                  4
#define GPIOC_PIN5                  5
#define GPIOC_PIN6                  6
#define GPIOC_PIN7                  7
#define GPIOC_PIN8                  8
#define GPIOC_PIN9                  9
#define GPIOC_PIN10                 10
#define GPIOC_PIN11                 11
#define GPIOC_PIN12                 12
#define GPIOC_PIN13                 13
#define GPIOC_PIN14                 14
#define GPIOC_PIN15                 15

#define GPIOD_OSCIN                 0
#define GPIOD_OSCOUT                1
#define GPIOD_PIN2                  2
#define GPIOD_PIN3                  3
#define GPIOD_PIN4                  4
#define GPIOD_PIN5                  5
#define GPIOD_PIN6                  6
#define GPIOD_PIN7                  7
#define GPIOD_PIN8                  8
#define GPIOD_PIN9                  9
#define GPIOD_PIN10                 10
#define GPIOD_PIN11                 11
#define GPIOD_PIN12                 12
#define GPIOD_PIN13                 13
#define GPIOD_PIN14                 14
#define GPIOD_PIN15                 15

#define GPIOE_PIN0                  0
#define GPIOE_PIN1                  1
#define GPIOE_PIN2                  2
#define GPIOE_PIN3                  3
#define GPIOE_PIN4                  4
#define GPIOE_PIN5                  5
#define GPIOE_PIN6                  6
#define GPIOE_PIN7                  7
#define GPIOE_PIN8                  8
#define GPIOE_PIN9                  9
#define GPIOE_PIN10                 10
#define GPIOE_PIN11                 11
#define GPIOE_PIN12                 12
#define GPIOE_PIN13                 13
#define GPIOE_PIN14                 14
#define GPIOE_PIN15                 15

#define GPIOF_PIN0                  0
#define GPIOF_PIN1                  1
#define GPIOF_PIN2                  2
#define GPIOF_PIN3                  3
#define GPIOF_PIN4                  4
#define GPIOF_PIN5                  5
#define GPIOF_PIN6                  6
#define GPIOF_PIN7                  7
#define GPIOF_PIN8                  8
#define GPIOF_PIN9                  9
#define GPIOF_PIN10                 10
#define GPIOF_PIN11                 11
#define GPIOF_PIN12                 12
#define GPIOF_PIN13                 13
#define GPIOF_PIN14                 14
#define GPIOF_PIN15                 15

/*
 * I/O ports initial setup, this configuration is established soon after reset
 * in the initialization code.
 * Please refer to the STM32 Reference Manual for details.
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_2M(n)            (0U << ((n) * 2))
#define PIN_OSPEED_25M(n)           (1U << ((n) * 2))
#define PIN_OSPEED_50M(n)           (2U << ((n) * 2))
#define PIN_OSPEED_100M(n)          (3U << ((n) * 2))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2))
#define PIN_AFIO_AF(n, v)           ((v##U) << ((n % 8) * 4))

/*
 * GPIOA setup:
 *
 * PA0  - PIN0                      (input pullup).
 * PA1  - PIN1                      (input pullup).
 * PA2  - SER_OUT                   (alternate 7).
 * PA3  - SER_IN                    (alternate 7).
 * PA4  - LED_A                     (output pushpull).
 * PA5  - LED_C                     (output pushpull).
 * PA6  - BAT_MON                   (analogue input).
 * PA7  - PIN7                      (input pullup).
 * PA8  - PIN8                      (input pullup).
 * PA9  - RB_RX                     (alternate 7).
 * PA10 - RX_TX                     (alternate 7).
 * PA11 - PIN11                     (input pullup).
 * PA12 - PIN12                     (input pullup).
 * PA13 - SWDIO                     (alternate 0).
 * PA14 - SWCLK                     (alternate 0).
 * PA15 - PIN15                     (input pullup).
 */
#define VAL_GPIOA_MODER             (PIN_MODE_INPUT(GPIOA_PIN0)             |\
                                     PIN_MODE_INPUT(GPIOA_PIN1)             |\
                                     PIN_MODE_ALTERNATE(GPIOA_SER_OUT)      |\
                                     PIN_MODE_ALTERNATE(GPIOA_SER_IN)       |\
                                     PIN_MODE_OUTPUT(GPIOA_LED_A)           |\
                                     PIN_MODE_OUTPUT(GPIOA_LED_C)           |\
                                     PIN_MODE_ANALOG(GPIOA_BAT_MON)         |\
                                     PIN_MODE_INPUT(GPIOA_PIN7)             |\
                                     PIN_MODE_INPUT(GPIOA_PIN8)             |\
                                     PIN_MODE_ALTERNATE(GPIOA_RB_TX)        |\
                                     PIN_MODE_ALTERNATE(GPIOA_RB_RX)        |\
                                     PIN_MODE_INPUT(GPIOA_PIN11)            |\
                                     PIN_MODE_INPUT(GPIOA_PIN12)            |\
                                     PIN_MODE_ALTERNATE(GPIOA_SWDIO)        |\
                                     PIN_MODE_ALTERNATE(GPIOA_SWCLK)        |\
                                     PIN_MODE_INPUT(GPIOA_PIN15))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOA_PIN0)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN1)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_SER_OUT)      |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_SER_IN)       |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_LED_A)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_LED_C)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_BAT_MON)      |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN7)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN8)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_RB_TX)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_RB_RX)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN11)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN12)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWDIO)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWCLK)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN15))
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_2M(GPIOA_PIN0)              |\
                                     PIN_OSPEED_2M(GPIOA_PIN1)              |\
                                     PIN_OSPEED_2M(GPIOA_SER_OUT)           |\
                                     PIN_OSPEED_2M(GPIOA_SER_IN)            |\
                                     PIN_OSPEED_2M(GPIOA_LED_A)             |\
                                     PIN_OSPEED_2M(GPIOA_LED_C)             |\
                                     PIN_OSPEED_2M(GPIOA_BAT_MON)           |\
                                     PIN_OSPEED_2M(GPIOA_PIN7)              |\
                                     PIN_OSPEED_2M(GPIOA_PIN8)              |\
                                     PIN_OSPEED_2M(GPIOA_RB_TX)             |\
                                     PIN_OSPEED_2M(GPIOA_RB_RX)             |\
                                     PIN_OSPEED_2M(GPIOA_PIN11)             |\
                                     PIN_OSPEED_2M(GPIOA_PIN12)             |\
                                     PIN_OSPEED_100M(GPIOA_SWDIO)           |\
                                     PIN_OSPEED_100M(GPIOA_SWCLK)           |\
                                     PIN_OSPEED_2M(GPIOA_PIN15))
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_PULLUP(GPIOA_PIN0)           |\
                                     PIN_PUPDR_PULLUP(GPIOA_PIN1)           |\
                                     PIN_PUPDR_FLOATING(GPIOA_SER_OUT)      |\
                                     PIN_PUPDR_FLOATING(GPIOA_SER_IN)       |\
                                     PIN_PUPDR_FLOATING(GPIOA_LED_A)        |\
                                     PIN_PUPDR_FLOATING(GPIOA_LED_C)        |\
                                     PIN_PUPDR_FLOATING(GPIOA_BAT_MON)      |\
                                     PIN_PUPDR_PULLUP(GPIOA_PIN7)           |\
                                     PIN_PUPDR_PULLUP(GPIOA_PIN8)           |\
                                     PIN_PUPDR_FLOATING(GPIOA_RB_TX)        |\
                                     PIN_PUPDR_FLOATING(GPIOA_RB_RX)        |\
                                     PIN_PUPDR_PULLUP(GPIOA_PIN11)          |\
                                     PIN_PUPDR_PULLUP(GPIOA_PIN12)          |\
                                     PIN_PUPDR_FLOATING(GPIOA_SWDIO)        |\
                                     PIN_PUPDR_FLOATING(GPIOA_SWCLK)        |\
                                     PIN_PUPDR_PULLUP(GPIOA_PIN15))
#define VAL_GPIOA_ODR               (PIN_ODR_HIGH(GPIOA_PIN0)               |\
                                     PIN_ODR_HIGH(GPIOA_PIN1)               |\
                                     PIN_ODR_HIGH(GPIOA_SER_OUT)            |\
                                     PIN_ODR_HIGH(GPIOA_SER_IN)             |\
                                     PIN_ODR_LOW(GPIOA_LED_A)               |\
                                     PIN_ODR_LOW(GPIOA_LED_C)               |\
                                     PIN_ODR_HIGH(GPIOA_BAT_MON)            |\
                                     PIN_ODR_HIGH(GPIOA_PIN7)               |\
                                     PIN_ODR_HIGH(GPIOA_PIN8)               |\
                                     PIN_ODR_HIGH(GPIOA_RB_TX)              |\
                                     PIN_ODR_HIGH(GPIOA_RB_RX)              |\
                                     PIN_ODR_HIGH(GPIOA_PIN11)              |\
                                     PIN_ODR_HIGH(GPIOA_PIN12)              |\
                                     PIN_ODR_HIGH(GPIOA_SWDIO)              |\
                                     PIN_ODR_HIGH(GPIOA_SWCLK)              |\
                                     PIN_ODR_HIGH(GPIOA_PIN15))
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIOA_PIN0, 0)             |\
                                     PIN_AFIO_AF(GPIOA_PIN1, 0)             |\
                                     PIN_AFIO_AF(GPIOA_SER_OUT, 7)          |\
                                     PIN_AFIO_AF(GPIOA_SER_IN, 7)           |\
                                     PIN_AFIO_AF(GPIOA_LED_A, 0)            |\
                                     PIN_AFIO_AF(GPIOA_LED_C, 0)            |\
                                     PIN_AFIO_AF(GPIOA_BAT_MON, 0)          |\
                                     PIN_AFIO_AF(GPIOA_PIN7, 0))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_PIN8, 0)             |\
                                     PIN_AFIO_AF(GPIOA_RB_TX, 7)            |\
                                     PIN_AFIO_AF(GPIOA_RB_RX, 7)            |\
                                     PIN_AFIO_AF(GPIOA_PIN11, 0)            |\
                                     PIN_AFIO_AF(GPIOA_PIN12, 0)            |\
                                     PIN_AFIO_AF(GPIOA_SWDIO, 0)            |\
                                     PIN_AFIO_AF(GPIOA_SWCLK, 0)            |\
                                     PIN_AFIO_AF(GPIOA_PIN15, 0))

/*
 * GPIOB setup:
 *
 * PB0  - PIN0                      (input pullup).
 * PB1  - NTX2B_P0                  (output pushpull).
 * PB2  - NTX2B_P1                  (output pushpull).
 * PB3  - SWO                       (alternate 0).
 * PB4  - PIN4                      (input pullup).
 * PB5  - GPS_RESET                 (output pushpull).
 * PB6  - GPS_SCL                   (alternate 4).
 * PB7  - GPS_SDA                   (alternate 4).
 * PB8  - PIN8                      (input pullup).
 * PB9  - PIN9                      (input pullup).
 * PB10 - NTX2B_EN                  (alternate 7).
 * PB11 - NTX2B_TXD                 (output pushpull).
 * PB12 - LED_RB                    (output pushpull).
 * PB13 - LED_RADIO                 (output pushpull).
 * PB14 - LED_GPS                   (output pushpull).
 * PB15 - LED_STATUS                (output pushpull).
 */
#define VAL_GPIOB_MODER             (PIN_MODE_INPUT(GPIOB_PIN0)              |\
                                     PIN_MODE_OUTPUT(GPIOB_NTX2B_P0)         |\
                                     PIN_MODE_OUTPUT(GPIOB_NTX2B_P1)         |\
                                     PIN_MODE_ALTERNATE(GPIOB_SWO)           |\
                                     PIN_MODE_INPUT(GPIOB_PIN4)              |\
                                     PIN_MODE_OUTPUT(GPIOB_GPS_RESET)        |\
                                     PIN_MODE_ALTERNATE(GPIOB_GPS_SCL)       |\
                                     PIN_MODE_ALTERNATE(GPIOB_GPS_SDA)       |\
                                     PIN_MODE_INPUT(GPIOB_PIN8)              |\
                                     PIN_MODE_INPUT(GPIOB_PIN9)              |\
                                     PIN_MODE_OUTPUT(GPIOB_NTX2B_EN)         |\
                                     PIN_MODE_OUTPUT(GPIOB_NTX2B_TXD)        |\
                                     PIN_MODE_OUTPUT(GPIOB_LED_RB)           |\
                                     PIN_MODE_OUTPUT(GPIOB_LED_RADIO)        |\
                                     PIN_MODE_OUTPUT(GPIOB_LED_GPS)          |\
                                     PIN_MODE_OUTPUT(GPIOB_LED_STATUS))
#define VAL_GPIOB_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOB_PIN0)          |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_NTX2B_P0)      |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_NTX2B_P1)      |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_SWO)           |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN4)          |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_GPS_RESET)     |\
                                     PIN_OTYPE_OPENDRAIN(GPIOB_GPS_SCL)      |\
                                     PIN_OTYPE_OPENDRAIN(GPIOB_GPS_SDA)      |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN8)          |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN9)          |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_NTX2B_EN)       |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_NTX2B_TXD)     |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_LED_RB)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_LED_RADIO)     |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_LED_GPS)       |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_LED_STATUS))
#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_2M(GPIOB_PIN0)               |\
                                     PIN_OSPEED_2M(GPIOB_NTX2B_P0)           |\
                                     PIN_OSPEED_2M(GPIOB_NTX2B_P1)           |\
                                     PIN_OSPEED_100M(GPIOB_SWO)               |\
                                     PIN_OSPEED_25M(GPIOB_PIN4)              |\
                                     PIN_OSPEED_25M(GPIOB_GPS_RESET)         |\
                                     PIN_OSPEED_25M(GPIOB_GPS_SCL)           |\
                                     PIN_OSPEED_25M(GPIOB_GPS_SDA)           |\
                                     PIN_OSPEED_2M(GPIOB_PIN8)               |\
                                     PIN_OSPEED_2M(GPIOB_PIN9)               |\
                                     PIN_OSPEED_2M(GPIOB_NTX2B_EN)           |\
                                     PIN_OSPEED_2M(GPIOB_NTX2B_TXD)          |\
                                     PIN_OSPEED_2M(GPIOB_LED_RB)             |\
                                     PIN_OSPEED_2M(GPIOB_LED_RADIO)          |\
                                     PIN_OSPEED_2M(GPIOB_LED_GPS)            |\
                                     PIN_OSPEED_2M(GPIOB_LED_STATUS))
#define VAL_GPIOB_PUPDR             (PIN_PUPDR_PULLUP(GPIOB_PIN0)            |\
                                     PIN_PUPDR_FLOATING(GPIOB_NTX2B_P0)      |\
                                     PIN_PUPDR_FLOATING(GPIOB_NTX2B_P1)      |\
                                     PIN_PUPDR_FLOATING(GPIOB_SWO)           |\
                                     PIN_PUPDR_PULLUP(GPIOB_PIN4)            |\
                                     PIN_PUPDR_FLOATING(GPIOB_GPS_RESET)     |\
                                     PIN_PUPDR_FLOATING(GPIOB_GPS_SCL)       |\
                                     PIN_PUPDR_FLOATING(GPIOB_GPS_SDA)       |\
                                     PIN_PUPDR_PULLUP(GPIOB_PIN8)            |\
                                     PIN_PUPDR_PULLUP(GPIOB_PIN9)            |\
                                     PIN_PUPDR_FLOATING(GPIOB_NTX2B_EN)      |\
                                     PIN_PUPDR_FLOATING(GPIOB_NTX2B_TXD)     |\
                                     PIN_PUPDR_FLOATING(GPIOB_LED_RB)        |\
                                     PIN_PUPDR_FLOATING(GPIOB_LED_RADIO)     |\
                                     PIN_PUPDR_FLOATING(GPIOB_LED_GPS)       |\
                                     PIN_PUPDR_FLOATING(GPIOB_LED_STATUS))
#define VAL_GPIOB_ODR               (PIN_ODR_HIGH(GPIOB_PIN0)                |\
                                     PIN_ODR_HIGH(GPIOB_NTX2B_P0)            |\
                                     PIN_ODR_HIGH(GPIOB_NTX2B_P1)            |\
                                     PIN_ODR_HIGH(GPIOB_SWO)                 |\
                                     PIN_ODR_HIGH(GPIOB_PIN4)                |\
                                     PIN_ODR_HIGH(GPIOB_GPS_RESET)           |\
                                     PIN_ODR_HIGH(GPIOB_GPS_SCL)             |\
                                     PIN_ODR_HIGH(GPIOB_GPS_SDA)             |\
                                     PIN_ODR_HIGH(GPIOB_PIN8)                |\
                                     PIN_ODR_HIGH(GPIOB_PIN9)                |\
                                     PIN_ODR_HIGH(GPIOB_NTX2B_EN)            |\
                                     PIN_ODR_HIGH(GPIOB_NTX2B_TXD)           |\
                                     PIN_ODR_HIGH(GPIOB_LED_RB)              |\
                                     PIN_ODR_HIGH(GPIOB_LED_RADIO)           |\
                                     PIN_ODR_HIGH(GPIOB_LED_GPS)             |\
                                     PIN_ODR_HIGH(GPIOB_LED_STATUS))
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOB_PIN0, 0)              |\
                                     PIN_AFIO_AF(GPIOB_NTX2B_P0, 0)          |\
                                     PIN_AFIO_AF(GPIOB_NTX2B_P1, 0)          |\
                                     PIN_AFIO_AF(GPIOB_SWO, 0)               |\
                                     PIN_AFIO_AF(GPIOB_PIN4, 0)              |\
                                     PIN_AFIO_AF(GPIOB_GPS_RESET, 0)         |\
                                     PIN_AFIO_AF(GPIOB_GPS_SCL, 4)           |\
                                     PIN_AFIO_AF(GPIOB_GPS_SDA, 4))
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(GPIOB_PIN8, 0)              |\
                                     PIN_AFIO_AF(GPIOB_PIN9, 0)              |\
                                     PIN_AFIO_AF(GPIOB_NTX2B_EN, 7)          |\
                                     PIN_AFIO_AF(GPIOB_NTX2B_TXD, 0)         |\
                                     PIN_AFIO_AF(GPIOB_LED_RB, 0)            |\
                                     PIN_AFIO_AF(GPIOB_LED_RADIO, 0)         |\
                                     PIN_AFIO_AF(GPIOB_LED_GPS, 0)           |\
                                     PIN_AFIO_AF(GPIOB_LED_STATUS, 0))

/*
 * GPIOC setup:
 *
 * PD0  - PIN0                      (input pullup).
 * PD1  - PIN1                      (input pullup).
 * PD2  - PIN2                      (input pullup).
 * PD3  - PIN3                      (input pullup).
 * PD4  - PIN4                      (input pullup).
 * PD5  - PIN5                      (input pullup).
 * PD6  - PIN6                      (input pullup).
 * PD7  - PIN7                      (input pullup).
 * PD8  - PIN8                      (input pullup).
 * PD9  - PIN9                      (input pullup).
 * PD10 - PIN10                     (input pullup).
 * PD11 - PIN11                     (input pullup).
 * PD12 - PIN12                     (input pullup).
 * PD13 - PIN13                     (input pullup).
 * PD14 - PIN14                     (input pullup).
 * PD15 - PIN15                     (input pullup).
 */
#define VAL_GPIOC_MODER             (PIN_MODE_INPUT(GPIOC_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOC_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOC_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOC_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOC_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOC_PIN15))
#define VAL_GPIOC_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOC_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN15))
#define VAL_GPIOC_OSPEEDR           (PIN_OSPEED_2M(GPIOC_PIN0) |            \
                                     PIN_OSPEED_2M(GPIOC_PIN1) |            \
                                     PIN_OSPEED_2M(GPIOC_PIN2) |            \
                                     PIN_OSPEED_2M(GPIOC_PIN3) |            \
                                     PIN_OSPEED_2M(GPIOC_PIN4) |            \
                                     PIN_OSPEED_2M(GPIOC_PIN5) |            \
                                     PIN_OSPEED_2M(GPIOC_PIN6) |            \
                                     PIN_OSPEED_2M(GPIOC_PIN7) |            \
                                     PIN_OSPEED_2M(GPIOC_PIN8) |            \
                                     PIN_OSPEED_2M(GPIOC_PIN9) |            \
                                     PIN_OSPEED_2M(GPIOC_PIN10) |           \
                                     PIN_OSPEED_2M(GPIOC_PIN11) |           \
                                     PIN_OSPEED_2M(GPIOC_PIN12) |           \
                                     PIN_OSPEED_2M(GPIOC_PIN13) |           \
                                     PIN_OSPEED_2M(GPIOC_PIN14) |           \
                                     PIN_OSPEED_2M(GPIOC_PIN15))
#define VAL_GPIOC_PUPDR             (PIN_PUPDR_PULLUP(GPIOC_PIN0) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN1) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN2) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN3) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN4) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN5) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN6) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN7) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN8) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN9) |         \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN10) |        \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN11) |        \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN12) |        \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN13) |        \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN14) |        \
                                     PIN_PUPDR_PULLUP(GPIOC_PIN15))
#define VAL_GPIOC_ODR               (PIN_ODR_HIGH(GPIOC_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOC_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOC_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOC_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOC_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOC_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOC_PIN15))
#define VAL_GPIOC_AFRL              (PIN_AFIO_AF(GPIOC_PIN0, 0) |           \
                                     PIN_AFIO_AF(GPIOC_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOC_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOC_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOC_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOC_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOC_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOC_PIN7, 0))
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(GPIOC_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOC_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOC_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOC_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOC_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOC_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOC_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOC_PIN15, 0))
/*
 * GPIOD setup:
 *
 * PD0  - OSCIN                     (input floating).
 * PD1  - OSCOUT                    (input floating).
 * PD2  - PIN2                      (input pullup).
 * PD3  - PIN3                      (input pullup).
 * PD4  - PIN4                      (input pullup).
 * PD5  - PIN5                      (input pullup).
 * PD6  - PIN6                      (input pullup).
 * PD7  - PIN7                      (input pullup).
 * PD8  - PIN8                      (input pullup).
 * PD9  - PIN9                      (input pullup).
 * PD10 - PIN10                     (input pullup).
 * PD11 - PIN11                     (input pullup).
 * PD12 - PIN12                     (input pullup).
 * PD13 - PIN13                     (input pullup).
 * PD14 - PIN14                     (input pullup).
 * PD15 - PIN15                     (input pullup).
 */
#define VAL_GPIOD_MODER             (PIN_MODE_INPUT(GPIOD_OSCIN) |            \
                                     PIN_MODE_INPUT(GPIOD_OSCOUT) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN2) |             \
                                     PIN_MODE_INPUT(GPIOD_PIN3) |             \
                                     PIN_MODE_INPUT(GPIOD_PIN4) |             \
                                     PIN_MODE_INPUT(GPIOD_PIN5) |             \
                                     PIN_MODE_INPUT(GPIOD_PIN6) |             \
                                     PIN_MODE_INPUT(GPIOD_PIN7) |             \
                                     PIN_MODE_INPUT(GPIOD_PIN8) |             \
                                     PIN_MODE_INPUT(GPIOD_PIN9) |             \
                                     PIN_MODE_INPUT(GPIOD_PIN10) |            \
                                     PIN_MODE_INPUT(GPIOD_PIN11) |            \
                                     PIN_MODE_INPUT(GPIOD_PIN12) |            \
                                     PIN_MODE_INPUT(GPIOD_PIN13) |            \
                                     PIN_MODE_INPUT(GPIOD_PIN14) |            \
                                     PIN_MODE_INPUT(GPIOD_PIN15))
#define VAL_GPIOD_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOD_OSCIN) |        \
                                     PIN_OTYPE_PUSHPULL(GPIOD_OSCOUT) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN2) |         \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN3) |         \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN4) |         \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN5) |         \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN6) |         \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN7) |         \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN8) |         \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN9) |         \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN10) |        \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN11) |        \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN12) |        \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN13) |        \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN14) |        \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN15))
#define VAL_GPIOD_OSPEEDR           (PIN_OSPEED_100M(GPIOD_OSCIN) |           \
                                     PIN_OSPEED_100M(GPIOD_OSCOUT) |          \
                                     PIN_OSPEED_2M(GPIOD_PIN2) |              \
                                     PIN_OSPEED_2M(GPIOD_PIN3) |              \
                                     PIN_OSPEED_2M(GPIOD_PIN4) |              \
                                     PIN_OSPEED_2M(GPIOD_PIN5) |              \
                                     PIN_OSPEED_2M(GPIOD_PIN6) |              \
                                     PIN_OSPEED_2M(GPIOD_PIN7) |              \
                                     PIN_OSPEED_2M(GPIOD_PIN8) |              \
                                     PIN_OSPEED_2M(GPIOD_PIN9) |              \
                                     PIN_OSPEED_2M(GPIOD_PIN10) |             \
                                     PIN_OSPEED_2M(GPIOD_PIN11) |             \
                                     PIN_OSPEED_2M(GPIOD_PIN12) |             \
                                     PIN_OSPEED_2M(GPIOD_PIN13) |             \
                                     PIN_OSPEED_2M(GPIOD_PIN14) |             \
                                     PIN_OSPEED_2M(GPIOD_PIN15))
#define VAL_GPIOD_PUPDR             (PIN_PUPDR_FLOATING(GPIOD_OSCIN) |        \
                                     PIN_PUPDR_FLOATING(GPIOD_OSCOUT) |       \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN2) |           \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN3) |           \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN4) |           \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN5) |           \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN6) |           \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN7) |           \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN8) |           \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN9) |           \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN10) |          \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN11) |          \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN12) |          \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN13) |          \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN14) |          \
                                     PIN_PUPDR_PULLUP(GPIOD_PIN15))
#define VAL_GPIOD_ODR               (PIN_ODR_HIGH(GPIOD_OSCIN) |              \
                                     PIN_ODR_HIGH(GPIOD_OSCOUT) |             \
                                     PIN_ODR_HIGH(GPIOD_PIN2) |               \
                                     PIN_ODR_HIGH(GPIOD_PIN3) |               \
                                     PIN_ODR_HIGH(GPIOD_PIN4) |               \
                                     PIN_ODR_HIGH(GPIOD_PIN5) |               \
                                     PIN_ODR_HIGH(GPIOD_PIN6) |               \
                                     PIN_ODR_HIGH(GPIOD_PIN7) |               \
                                     PIN_ODR_HIGH(GPIOD_PIN8) |               \
                                     PIN_ODR_HIGH(GPIOD_PIN9) |               \
                                     PIN_ODR_HIGH(GPIOD_PIN10) |              \
                                     PIN_ODR_HIGH(GPIOD_PIN11) |              \
                                     PIN_ODR_HIGH(GPIOD_PIN12) |              \
                                     PIN_ODR_HIGH(GPIOD_PIN13) |              \
                                     PIN_ODR_HIGH(GPIOD_PIN14) |              \
                                     PIN_ODR_HIGH(GPIOD_PIN15))
#define VAL_GPIOD_AFRL              (PIN_AFIO_AF(GPIOD_OSCIN, 0) |            \
                                     PIN_AFIO_AF(GPIOD_OSCOUT, 0) |           \
                                     PIN_AFIO_AF(GPIOD_PIN2, 0) |             \
                                     PIN_AFIO_AF(GPIOD_PIN3, 0) |             \
                                     PIN_AFIO_AF(GPIOD_PIN4, 0) |             \
                                     PIN_AFIO_AF(GPIOD_PIN5, 0) |             \
                                     PIN_AFIO_AF(GPIOD_PIN6, 0) |             \
                                     PIN_AFIO_AF(GPIOD_PIN7, 0))
#define VAL_GPIOD_AFRH              (PIN_AFIO_AF(GPIOD_PIN8, 0) |             \
                                     PIN_AFIO_AF(GPIOD_PIN9, 0) |             \
                                     PIN_AFIO_AF(GPIOD_PIN10, 0) |            \
                                     PIN_AFIO_AF(GPIOD_PIN11, 0) |            \
                                     PIN_AFIO_AF(GPIOD_PIN12, 0) |            \
                                     PIN_AFIO_AF(GPIOD_PIN13, 0) |            \
                                     PIN_AFIO_AF(GPIOD_PIN14, 0) |            \
                                     PIN_AFIO_AF(GPIOD_PIN15, 0))

/*
 * GPIOE setup:
 *
 * PD0  - PIN0                      (input pullup).
 * PD1  - PIN1                      (input pullup).
 * PD2  - PIN2                      (input pullup).
 * PD3  - PIN3                      (input pullup).
 * PD4  - PIN4                      (input pullup).
 * PD5  - PIN5                      (input pullup).
 * PD6  - PIN6                      (input pullup).
 * PD7  - PIN7                      (input pullup).
 * PD8  - PIN8                      (input pullup).
 * PD9  - PIN9                      (input pullup).
 * PD10 - PIN10                     (input pullup).
 * PD11 - PIN11                     (input pullup).
 * PD12 - PIN12                     (input pullup).
 * PD13 - PIN13                     (input pullup).
 * PD14 - PIN14                     (input pullup).
 * PD15 - PIN15                     (input pullup).
 */
#define VAL_GPIOE_MODER             (PIN_MODE_INPUT(GPIOE_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN15))
#define VAL_GPIOE_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOE_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN15))
#define VAL_GPIOE_OSPEEDR           (PIN_OSPEED_2M(GPIOE_PIN0) |            \
                                     PIN_OSPEED_2M(GPIOE_PIN1) |            \
                                     PIN_OSPEED_2M(GPIOE_PIN2) |            \
                                     PIN_OSPEED_2M(GPIOE_PIN3) |            \
                                     PIN_OSPEED_2M(GPIOE_PIN4) |            \
                                     PIN_OSPEED_2M(GPIOE_PIN5) |            \
                                     PIN_OSPEED_2M(GPIOE_PIN6) |            \
                                     PIN_OSPEED_2M(GPIOE_PIN7) |            \
                                     PIN_OSPEED_2M(GPIOE_PIN8) |            \
                                     PIN_OSPEED_2M(GPIOE_PIN9) |            \
                                     PIN_OSPEED_2M(GPIOE_PIN10) |           \
                                     PIN_OSPEED_2M(GPIOE_PIN11) |           \
                                     PIN_OSPEED_2M(GPIOE_PIN12) |           \
                                     PIN_OSPEED_2M(GPIOE_PIN13) |           \
                                     PIN_OSPEED_2M(GPIOE_PIN14) |           \
                                     PIN_OSPEED_2M(GPIOE_PIN15))
#define VAL_GPIOE_PUPDR             (PIN_PUPDR_PULLUP(GPIOE_PIN0) |         \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN1) |         \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN2) |         \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN3) |         \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN4) |         \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN5) |         \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN6) |         \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN7) |         \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN8) |         \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN9) |         \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN10) |        \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN11) |        \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN12) |        \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN13) |        \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN14) |        \
                                     PIN_PUPDR_PULLUP(GPIOE_PIN15))
#define VAL_GPIOE_ODR               (PIN_ODR_HIGH(GPIOE_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOE_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOE_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOE_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOE_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOE_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOE_PIN15))
#define VAL_GPIOE_AFRL              (PIN_AFIO_AF(GPIOE_PIN0, 0) |           \
                                     PIN_AFIO_AF(GPIOE_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOE_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOE_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOE_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOE_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOE_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOE_PIN7, 0))
#define VAL_GPIOE_AFRH              (PIN_AFIO_AF(GPIOE_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOE_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOE_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOE_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOE_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOE_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOE_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOE_PIN15, 0))

/*
 * GPIOF setup:
 *
 * PD0  - PIN0                      (input pullup).
 * PD1  - PIN1                      (input pullup).
 * PD2  - PIN2                      (input pullup).
 * PD3  - PIN3                      (input pullup).
 * PD4  - PIN4                      (input pullup).
 * PD5  - PIN5                      (input pullup).
 * PD6  - PIN6                      (input pullup).
 * PD7  - PIN7                      (input pullup).
 * PD8  - PIN8                      (input pullup).
 * PD9  - PIN9                      (input pullup).
 * PD10 - PIN10                     (input pullup).
 * PD11 - PIN11                     (input pullup).
 * PD12 - PIN12                     (input pullup).
 * PD13 - PIN13                     (input pullup).
 * PD14 - PIN14                     (input pullup).
 * PD15 - PIN15                     (input pullup).
 */
#define VAL_GPIOF_MODER             (PIN_MODE_INPUT(GPIOF_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN15))
#define VAL_GPIOF_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOF_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN15))
#define VAL_GPIOF_OSPEEDR           (PIN_OSPEED_2M(GPIOF_PIN0) |            \
                                     PIN_OSPEED_2M(GPIOF_PIN1) |            \
                                     PIN_OSPEED_2M(GPIOF_PIN2) |            \
                                     PIN_OSPEED_2M(GPIOF_PIN3) |            \
                                     PIN_OSPEED_2M(GPIOF_PIN4) |            \
                                     PIN_OSPEED_2M(GPIOF_PIN5) |            \
                                     PIN_OSPEED_2M(GPIOF_PIN6) |            \
                                     PIN_OSPEED_2M(GPIOF_PIN7) |            \
                                     PIN_OSPEED_2M(GPIOF_PIN8) |            \
                                     PIN_OSPEED_2M(GPIOF_PIN9) |            \
                                     PIN_OSPEED_2M(GPIOF_PIN10) |           \
                                     PIN_OSPEED_2M(GPIOF_PIN11) |           \
                                     PIN_OSPEED_2M(GPIOF_PIN12) |           \
                                     PIN_OSPEED_2M(GPIOF_PIN13) |           \
                                     PIN_OSPEED_2M(GPIOF_PIN14) |           \
                                     PIN_OSPEED_2M(GPIOF_PIN15))
#define VAL_GPIOF_PUPDR             (PIN_PUPDR_PULLUP(GPIOF_PIN0) |         \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN1) |         \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN2) |         \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN3) |         \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN4) |         \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN5) |         \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN6) |         \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN7) |         \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN8) |         \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN9) |         \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN10) |        \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN11) |        \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN12) |        \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN13) |        \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN14) |        \
                                     PIN_PUPDR_PULLUP(GPIOF_PIN15))
#define VAL_GPIOF_ODR               (PIN_ODR_HIGH(GPIOF_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOF_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOF_PIN15))
#define VAL_GPIOF_AFRL              (PIN_AFIO_AF(GPIOF_PIN0, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN7, 0))
#define VAL_GPIOF_AFRH              (PIN_AFIO_AF(GPIOF_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOF_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOF_PIN15, 0))

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* _BOARD_H_ */
