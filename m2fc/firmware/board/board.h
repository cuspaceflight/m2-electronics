/*
 * M2FC ChibiOS Board definition file.
*/

#ifndef _BOARD_H_
#define _BOARD_H_

/*
 * Board identifier.
 */
#define BOARD_M2FC
#define BOARD_NAME                  "M2FC"


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
#define STM32F40_41xxx

/*
 * IO pins assignments.
 */
#define GPIOA_STRAIN_1              0
#define GPIOA_STRAIN_2              1
#define GPIOA_STRAIN_3              2
#define GPIOA_BAT_MON               3
#define GPIOA_HG_ACCEL_CS           4
#define GPIOA_HG_ACCEL_SCK          5
#define GPIOA_HG_ACCEL_MISO         6
#define GPIOA_HG_ACCEL_MOSI         7
#define GPIOA_PIN8                  8
#define GPIOA_LED_1                 9
#define GPIOA_LED_2                 10
#define GPIOA_LED_3                 11
#define GPIOA_LED_4                 12
#define GPIOA_SWDIO                 13
#define GPIOA_SWCLK                 14
#define GPIOA_PIN15                 15

#define GPIOB_PIN0                  0
#define GPIOB_PIN1                  1
#define GPIOB_PIN2                  2
#define GPIOB_BARO_SCK              3
#define GPIOB_BARO_MISO             4
#define GPIOB_BARO_MOSI             5
#define GPIOB_SER_OUT_1             6
#define GPIOB_SER_IN_1              7
#define GPIOB_MAGNO_SCL             8
#define GPIOB_MAGNO_SDA             9
#define GPIOB_GYRO_SCL              10
#define GPIOB_GYRO_SDA              11
#define GPIOB_LG_ACCEL_CS           12
#define GPIOB_LG_ACCEL_SCK          13
#define GPIOB_LG_ACCEL_MISO         14
#define GPIOB_LG_ACCEL_MOSI         15

#define GPIOC_PIN0                  0
#define GPIOC_THERMO_1              1
#define GPIOC_THERMO_2              2
#define GPIOC_THERMO_3              3
#define GPIOC_HG_ACCEL_INT2         4
#define GPIOC_HG_ACCEL_INT1         5
#define GPIOC_LED_C                 6
#define GPIOC_LED_A                 7
#define GPIOC_SD_DAT0               8
#define GPIOC_SD_DAT1               9
#define GPIOC_SD_DAT2               10
#define GPIOC_SD_DAT3               11
#define GPIOC_SD_CLK                12
#define GPIOC_PIN13                 13
#define GPIOC_PIN14                 14
#define GPIOC_PIN15                 15

#define GPIOD_PIN0                  0
#define GPIOD_SD_CD                 1
#define GPIOD_SD_CMD                2
#define GPIOD_PIN3                  3
#define GPIOD_PIN4                  4
#define GPIOD_SER_OUT_2             5
#define GPIOD_SER_IN_2              6
#define GPIOD_BARO_CS               7
#define GPIOD_LG_ACCEL_INT2         8
#define GPIOD_LG_ACCEL_INT1         9
#define GPIOD_LG_ACCEL_CS           10
#define GPIOD_PIN11                 11
#define GPIOD_PIN12                 12
#define GPIOD_SJ1                   13
#define GPIOD_PIN14                 14
#define GPIOD_PIN15                 15

#define GPIOE_MAGNO_DRDY            0
#define GPIOE_PIN1                  1
#define GPIOE_PIN2                  2
#define GPIOE_PIN3                  3
#define GPIOE_PIN4                  4
#define GPIOE_PIN5                  5
#define GPIOE_PIN6                  6
#define GPIOE_PYRO_1_C              7
#define GPIOE_PYRO_2_C              8
#define GPIOE_PYRO_3_C              9
#define GPIOE_PYRO_1_F              10
#define GPIOE_PYRO_2_F              11
#define GPIOE_PYRO_3_F              12
#define GPIOE_GYRO_INT1             13
#define GPIOE_GYRO_DRDY             14
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

#define GPIOG_PIN0                  0
#define GPIOG_PIN1                  1
#define GPIOG_PIN2                  2
#define GPIOG_PIN3                  3
#define GPIOG_PIN4                  4
#define GPIOG_PIN5                  5
#define GPIOG_PIN6                  6
#define GPIOG_PIN7                  7
#define GPIOG_PIN8                  8
#define GPIOG_PIN9                  9
#define GPIOG_PIN10                 10
#define GPIOG_PIN11                 11
#define GPIOG_PIN12                 12
#define GPIOG_PIN13                 13
#define GPIOG_PIN14                 14
#define GPIOG_PIN15                 15

#define GPIOH_OSC_IN                0
#define GPIOH_OSC_OUT               1
#define GPIOH_PIN2                  2
#define GPIOH_PIN3                  3
#define GPIOH_PIN4                  4
#define GPIOH_PIN5                  5
#define GPIOH_PIN6                  6
#define GPIOH_PIN7                  7
#define GPIOH_PIN8                  8
#define GPIOH_PIN9                  9
#define GPIOH_PIN10                 10
#define GPIOH_PIN11                 11
#define GPIOH_PIN12                 12
#define GPIOH_PIN13                 13
#define GPIOH_PIN14                 14
#define GPIOH_PIN15                 15

#define GPIOI_PIN0                  0
#define GPIOI_PIN1                  1
#define GPIOI_PIN2                  2
#define GPIOI_PIN3                  3
#define GPIOI_PIN4                  4
#define GPIOI_PIN5                  5
#define GPIOI_PIN6                  6
#define GPIOI_PIN7                  7
#define GPIOI_PIN8                  8
#define GPIOI_PIN9                  9
#define GPIOI_PIN10                 10
#define GPIOI_PIN11                 11
#define GPIOI_PIN12                 12
#define GPIOI_PIN13                 13
#define GPIOI_PIN14                 14
#define GPIOI_PIN15                 15

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
 * PA0  - STRAIN_1                  (analogue input).
 * PA1  - STRAIN_2                  (analogue input).
 * PA2  - STRAIN_3                  (analogue input).
 * PA3  - BAT_MON                   (analogue input).
 * PA4  - HG_ACCEL_CS               (output pushpull).
 * PA5  - HG_ACCEL_SCK              (alternate 5).
 * PA6  - HG_ACCEL_MISO             (alternate 5).
 * PA7  - HG_ACCEL_MOSI             (alternate 5).
 * PA8  - PIN8                      (input pullup).
 * PA9  - LED_1                     (output pushpull).
 * PA10 - LED_2                     (output pushpull).
 * PA11 - LED_3                     (output pushpull).
 * PA12 - LED_4                     (output pushpull).
 * PA13 - SWDIO                     (alternate 0).
 * PA14 - SWCLK                     (alternate 0).
 * PA15 - PIN15                     (input pullup).
 */
#define VAL_GPIOA_MODER             (PIN_MODE_ANALOG(GPIOA_STRAIN_1)        |\
                                     PIN_MODE_ANALOG(GPIOA_STRAIN_2)        |\
                                     PIN_MODE_ANALOG(GPIOA_STRAIN_3)        |\
                                     PIN_MODE_ANALOG(GPIOA_BAT_MON)         |\
                                     PIN_MODE_OUTPUT(GPIOA_HG_ACCEL_CS)     |\
                                     PIN_MODE_ALTERNATE(GPIOA_HG_ACCEL_SCK) |\
                                     PIN_MODE_ALTERNATE(GPIOA_HG_ACCEL_MISO)|\
                                     PIN_MODE_ALTERNATE(GPIOA_HG_ACCEL_MOSI)|\
                                     PIN_MODE_INPUT(GPIOA_PIN8)             |\
                                     PIN_MODE_OUTPUT(GPIOA_LED_1)           |\
                                     PIN_MODE_OUTPUT(GPIOA_LED_2)           |\
                                     PIN_MODE_OUTPUT(GPIOA_LED_3)           |\
                                     PIN_MODE_OUTPUT(GPIOA_LED_4)           |\
                                     PIN_MODE_ALTERNATE(GPIOA_SWDIO)        |\
                                     PIN_MODE_ALTERNATE(GPIOA_SWCLK)        |\
                                     PIN_MODE_INPUT(GPIOA_PIN15))
#define VAL_GPIOA_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOA_STRAIN_1)     |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_STRAIN_2)     |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_STRAIN_3)     |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_BAT_MON)      |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_HG_ACCEL_CS)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_HG_ACCEL_SCK) |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_HG_ACCEL_MISO)|\
                                     PIN_OTYPE_PUSHPULL(GPIOA_HG_ACCEL_MOSI)|\
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN8)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_LED_1)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_LED_2)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_LED_3)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_LED_4)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWDIO)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWCLK)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN15))
#define VAL_GPIOA_OSPEEDR           (PIN_OSPEED_2M(GPIOA_STRAIN_1)          |\
                                     PIN_OSPEED_2M(GPIOA_STRAIN_2)          |\
                                     PIN_OSPEED_2M(GPIOA_STRAIN_3)          |\
                                     PIN_OSPEED_2M(GPIOA_BAT_MON)           |\
                                     PIN_OSPEED_25M(GPIOA_HG_ACCEL_CS)      |\
                                     PIN_OSPEED_25M(GPIOA_HG_ACCEL_SCK)     |\
                                     PIN_OSPEED_25M(GPIOA_HG_ACCEL_MISO)    |\
                                     PIN_OSPEED_25M(GPIOA_HG_ACCEL_MOSI)    |\
                                     PIN_OSPEED_2M(GPIOA_PIN8)              |\
                                     PIN_OSPEED_2M(GPIOA_LED_1)             |\
                                     PIN_OSPEED_2M(GPIOA_LED_2)             |\
                                     PIN_OSPEED_2M(GPIOA_LED_3)             |\
                                     PIN_OSPEED_2M(GPIOA_LED_4)             |\
                                     PIN_OSPEED_100M(GPIOA_SWDIO)           |\
                                     PIN_OSPEED_100M(GPIOA_SWCLK)           |\
                                     PIN_OSPEED_2M(GPIOA_PIN15))
#define VAL_GPIOA_PUPDR             (PIN_PUPDR_FLOATING(GPIOA_STRAIN_1)     |\
                                     PIN_PUPDR_FLOATING(GPIOA_STRAIN_2)     |\
                                     PIN_PUPDR_FLOATING(GPIOA_STRAIN_3)     |\
                                     PIN_PUPDR_FLOATING(GPIOA_BAT_MON)      |\
                                     PIN_PUPDR_FLOATING(GPIOA_HG_ACCEL_CS)  |\
                                     PIN_PUPDR_FLOATING(GPIOA_HG_ACCEL_SCK) |\
                                     PIN_PUPDR_FLOATING(GPIOA_HG_ACCEL_MISO)|\
                                     PIN_PUPDR_FLOATING(GPIOA_HG_ACCEL_MOSI)|\
                                     PIN_PUPDR_PULLUP(GPIOA_PIN8)           |\
                                     PIN_PUPDR_FLOATING(GPIOA_LED_1)        |\
                                     PIN_PUPDR_FLOATING(GPIOA_LED_2)        |\
                                     PIN_PUPDR_FLOATING(GPIOA_LED_3)        |\
                                     PIN_PUPDR_FLOATING(GPIOA_LED_4)        |\
                                     PIN_PUPDR_FLOATING(GPIOA_SWDIO)        |\
                                     PIN_PUPDR_FLOATING(GPIOA_SWCLK)        |\
                                     PIN_PUPDR_PULLUP(GPIOA_PIN15))
#define VAL_GPIOA_ODR               (PIN_ODR_HIGH(GPIOA_STRAIN_1)           |\
                                     PIN_ODR_HIGH(GPIOA_STRAIN_2)           |\
                                     PIN_ODR_HIGH(GPIOA_STRAIN_3)           |\
                                     PIN_ODR_HIGH(GPIOA_BAT_MON)            |\
                                     PIN_ODR_HIGH(GPIOA_HG_ACCEL_CS)        |\
                                     PIN_ODR_HIGH(GPIOA_HG_ACCEL_SCK)       |\
                                     PIN_ODR_HIGH(GPIOA_HG_ACCEL_MISO)      |\
                                     PIN_ODR_HIGH(GPIOA_HG_ACCEL_MOSI)      |\
                                     PIN_ODR_HIGH(GPIOA_PIN8)               |\
                                     PIN_ODR_LOW(GPIOA_LED_1)               |\
                                     PIN_ODR_LOW(GPIOA_LED_2)               |\
                                     PIN_ODR_LOW(GPIOA_LED_3)               |\
                                     PIN_ODR_LOW(GPIOA_LED_4)               |\
                                     PIN_ODR_HIGH(GPIOA_SWDIO)              |\
                                     PIN_ODR_HIGH(GPIOA_SWCLK)              |\
                                     PIN_ODR_HIGH(GPIOA_PIN15))
#define VAL_GPIOA_AFRL              (PIN_AFIO_AF(GPIOA_STRAIN_1, 0)         |\
                                     PIN_AFIO_AF(GPIOA_STRAIN_2, 0)         |\
                                     PIN_AFIO_AF(GPIOA_STRAIN_3, 0)         |\
                                     PIN_AFIO_AF(GPIOA_BAT_MON, 0)          |\
                                     PIN_AFIO_AF(GPIOA_HG_ACCEL_CS, 0)      |\
                                     PIN_AFIO_AF(GPIOA_HG_ACCEL_SCK, 5)     |\
                                     PIN_AFIO_AF(GPIOA_HG_ACCEL_MISO, 5)    |\
                                     PIN_AFIO_AF(GPIOA_HG_ACCEL_MOSI, 5))
#define VAL_GPIOA_AFRH              (PIN_AFIO_AF(GPIOA_PIN8, 0)             |\
                                     PIN_AFIO_AF(GPIOA_LED_1, 0)            |\
                                     PIN_AFIO_AF(GPIOA_LED_2, 0)            |\
                                     PIN_AFIO_AF(GPIOA_LED_3, 0)            |\
                                     PIN_AFIO_AF(GPIOA_LED_4, 0)            |\
                                     PIN_AFIO_AF(GPIOA_SWDIO, 0)            |\
                                     PIN_AFIO_AF(GPIOA_SWCLK, 0)            |\
                                     PIN_AFIO_AF(GPIOA_PIN15, 0))

/*
 * GPIOB setup:
 *
 * PB0  - PIN0                      (input pullup).
 * PB1  - PIN1                      (input pullup).
 * PB2  - PIN2                      (input pullup).
 * PB3  - BARO_SCK                  (alternate 6).
 * PB4  - BARO_MISO                 (alternate 6).
 * PB5  - BARO_MOSI                 (alternate 6).
 * PB6  - SER_OUT_1                 (alternate 7).
 * PB7  - SER_IN_1                  (alternate 7).
 * PB8  - MAGNO_SCL                 (alternate 4).
 * PB9  - MAGNO_SDA                 (alternate 4).
 * PB10 - GYRO_SCL                  (alternate 4).
 * PB11 - GYRO_SDA                  (alternate 4).
 * PB12 - LG_ACCEL_CS               (output pushpull).
 * PB13 - LG_ACCEL_SCK              (alternate 5).
 * PB14 - LG_ACCEL_MISO             (alternate 5).
 * PB15 - LG_ACCEL_MOSI             (alternate 5).
 */
#define VAL_GPIOB_MODER             (PIN_MODE_INPUT(GPIOB_PIN0)             |\
                                     PIN_MODE_INPUT(GPIOB_PIN1)             |\
                                     PIN_MODE_INPUT(GPIOB_PIN2)             |\
                                     PIN_MODE_ALTERNATE(GPIOB_BARO_SCK)     |\
                                     PIN_MODE_ALTERNATE(GPIOB_BARO_MISO)    |\
                                     PIN_MODE_ALTERNATE(GPIOB_BARO_MOSI)    |\
                                     PIN_MODE_ALTERNATE(GPIOB_SER_OUT_1)    |\
                                     PIN_MODE_ALTERNATE(GPIOB_SER_IN_1)     |\
                                     PIN_MODE_ALTERNATE(GPIOB_MAGNO_SCL)    |\
                                     PIN_MODE_ALTERNATE(GPIOB_MAGNO_SDA)    |\
                                     PIN_MODE_ALTERNATE(GPIOB_GYRO_SCL)     |\
                                     PIN_MODE_ALTERNATE(GPIOB_GYRO_SDA)     |\
                                     PIN_MODE_OUTPUT(GPIOB_LG_ACCEL_CS)     |\
                                     PIN_MODE_ALTERNATE(GPIOB_LG_ACCEL_SCK) |\
                                     PIN_MODE_ALTERNATE(GPIOB_LG_ACCEL_MISO)|\
                                     PIN_MODE_ALTERNATE(GPIOB_LG_ACCEL_MOSI))
#define VAL_GPIOB_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOB_PIN0)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN1)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN2)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_BARO_SCK)     |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_BARO_MISO)    |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_BARO_MOSI)    |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_SER_OUT_1)    |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_SER_IN_1)     |\
                                     PIN_OTYPE_OPENDRAIN(GPIOB_MAGNO_SCL)   |\
                                     PIN_OTYPE_OPENDRAIN(GPIOB_MAGNO_SDA)   |\
                                     PIN_OTYPE_OPENDRAIN(GPIOB_GYRO_SCL)    |\
                                     PIN_OTYPE_OPENDRAIN(GPIOB_GYRO_SDA)    |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_LG_ACCEL_CS)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_LG_ACCEL_SCK) |\
                                     PIN_OTYPE_PUSHPULL(GPIOB_LG_ACCEL_MISO)|\
                                     PIN_OTYPE_PUSHPULL(GPIOB_LG_ACCEL_MOSI))
#define VAL_GPIOB_OSPEEDR           (PIN_OSPEED_2M(GPIOB_PIN0)              |\
                                     PIN_OSPEED_2M(GPIOB_PIN1)              |\
                                     PIN_OSPEED_2M(GPIOB_PIN2)              |\
                                     PIN_OSPEED_25M(GPIOB_BARO_SCK)         |\
                                     PIN_OSPEED_25M(GPIOB_BARO_MISO)        |\
                                     PIN_OSPEED_25M(GPIOB_BARO_MOSI)        |\
                                     PIN_OSPEED_2M(GPIOB_SER_OUT_1)         |\
                                     PIN_OSPEED_2M(GPIOB_SER_IN_1)          |\
                                     PIN_OSPEED_2M(GPIOB_MAGNO_SCL)         |\
                                     PIN_OSPEED_2M(GPIOB_MAGNO_SDA)         |\
                                     PIN_OSPEED_2M(GPIOB_GYRO_SCL)          |\
                                     PIN_OSPEED_2M(GPIOB_GYRO_SDA)          |\
                                     PIN_OSPEED_25M(GPIOB_LG_ACCEL_CS)      |\
                                     PIN_OSPEED_25M(GPIOB_LG_ACCEL_SCK)     |\
                                     PIN_OSPEED_25M(GPIOB_LG_ACCEL_MISO)    |\
                                     PIN_OSPEED_25M(GPIOB_LG_ACCEL_MOSI))
#define VAL_GPIOB_PUPDR             (PIN_PUPDR_PULLUP(GPIOB_PIN0)           |\
                                     PIN_PUPDR_PULLUP(GPIOB_PIN1)           |\
                                     PIN_PUPDR_PULLUP(GPIOB_PIN2)           |\
                                     PIN_PUPDR_FLOATING(GPIOB_BARO_SCK)     |\
                                     PIN_PUPDR_FLOATING(GPIOB_BARO_MISO)    |\
                                     PIN_PUPDR_FLOATING(GPIOB_BARO_MOSI)    |\
                                     PIN_PUPDR_FLOATING(GPIOB_SER_OUT_1)    |\
                                     PIN_PUPDR_FLOATING(GPIOB_SER_IN_1)     |\
                                     PIN_PUPDR_FLOATING(GPIOB_MAGNO_SCL)    |\
                                     PIN_PUPDR_FLOATING(GPIOB_MAGNO_SDA)    |\
                                     PIN_PUPDR_FLOATING(GPIOB_GYRO_SCL)     |\
                                     PIN_PUPDR_FLOATING(GPIOB_GYRO_SDA)     |\
                                     PIN_PUPDR_FLOATING(GPIOB_LG_ACCEL_CS)  |\
                                     PIN_PUPDR_FLOATING(GPIOB_LG_ACCEL_SCK) |\
                                     PIN_PUPDR_FLOATING(GPIOB_LG_ACCEL_MISO)|\
                                     PIN_PUPDR_FLOATING(GPIOB_LG_ACCEL_MOSI))
#define VAL_GPIOB_ODR               (PIN_ODR_HIGH(GPIOB_PIN0)               |\
                                     PIN_ODR_HIGH(GPIOB_PIN1)               |\
                                     PIN_ODR_HIGH(GPIOB_PIN2)               |\
                                     PIN_ODR_HIGH(GPIOB_BARO_SCK)           |\
                                     PIN_ODR_HIGH(GPIOB_BARO_MISO)          |\
                                     PIN_ODR_HIGH(GPIOB_BARO_MOSI)          |\
                                     PIN_ODR_HIGH(GPIOB_SER_OUT_1)          |\
                                     PIN_ODR_HIGH(GPIOB_SER_IN_1)           |\
                                     PIN_ODR_HIGH(GPIOB_MAGNO_SCL)          |\
                                     PIN_ODR_HIGH(GPIOB_MAGNO_SDA)          |\
                                     PIN_ODR_HIGH(GPIOB_GYRO_SCL)           |\
                                     PIN_ODR_HIGH(GPIOB_GYRO_SDA)           |\
                                     PIN_ODR_HIGH(GPIOB_LG_ACCEL_CS)        |\
                                     PIN_ODR_HIGH(GPIOB_LG_ACCEL_SCK)       |\
                                     PIN_ODR_HIGH(GPIOB_LG_ACCEL_MISO)      |\
                                     PIN_ODR_HIGH(GPIOB_LG_ACCEL_MOSI))
#define VAL_GPIOB_AFRL              (PIN_AFIO_AF(GPIOB_PIN0, 0)             |\
                                     PIN_AFIO_AF(GPIOB_PIN1, 0)             |\
                                     PIN_AFIO_AF(GPIOB_PIN2, 0)             |\
                                     PIN_AFIO_AF(GPIOB_BARO_SCK, 6)         |\
                                     PIN_AFIO_AF(GPIOB_BARO_MISO, 6)        |\
                                     PIN_AFIO_AF(GPIOB_BARO_MOSI, 6)        |\
                                     PIN_AFIO_AF(GPIOB_SER_OUT_1, 7)        |\
                                     PIN_AFIO_AF(GPIOB_SER_IN_1, 7))
#define VAL_GPIOB_AFRH              (PIN_AFIO_AF(GPIOB_MAGNO_SCL, 4)        |\
                                     PIN_AFIO_AF(GPIOB_MAGNO_SDA, 4)        |\
                                     PIN_AFIO_AF(GPIOB_GYRO_SCL, 4)         |\
                                     PIN_AFIO_AF(GPIOB_GYRO_SDA, 4)         |\
                                     PIN_AFIO_AF(GPIOB_LG_ACCEL_CS, 5)      |\
                                     PIN_AFIO_AF(GPIOB_LG_ACCEL_SCK, 5)     |\
                                     PIN_AFIO_AF(GPIOB_LG_ACCEL_MISO, 5)    |\
                                     PIN_AFIO_AF(GPIOB_LG_ACCEL_MOSI, 5))

/*
 * GPIOC setup:
 *
 * PC0  - PIN0                      (input pullup).
 * PC1  - THERMO_1                  (analogue input).
 * PC2  - THERMO_2                  (analogue input).
 * PC3  - THERMO_3                  (analogue input).
 * PC4  - HG_ACCEL_INT2             (input pullup).
 * PC5  - HG_ACCEL_INT1             (input pullup).
 * PC6  - LED_C                     (output pushpull).
 * PC7  - LED_A                     (output pushpull).
 * PC8  - SD_DAT0                   (alternate 12).
 * PC9  - SD_DAT1                   (alternate 12).
 * PC10 - SD_DAT2                   (alternate 12).
 * PC11 - SD_DAT3                   (alternate 12).
 * PC12 - SD_CLK                    (alternate 12).
 * PC13 - PIN13                     (input pullup).
 * PC14 - PIN14                     (input pullup).
 * PC15 - PIN15                     (input pullup).
 */
#define VAL_GPIOC_MODER             (PIN_MODE_INPUT(GPIOC_PIN0)             |\
                                     PIN_MODE_ANALOG(GPIOC_THERMO_1)        |\
                                     PIN_MODE_ANALOG(GPIOC_THERMO_2)        |\
                                     PIN_MODE_ANALOG(GPIOC_THERMO_3)        |\
                                     PIN_MODE_INPUT(GPIOC_HG_ACCEL_INT2)    |\
                                     PIN_MODE_INPUT(GPIOC_HG_ACCEL_INT1)    |\
                                     PIN_MODE_OUTPUT(GPIOC_LED_C)           |\
                                     PIN_MODE_OUTPUT(GPIOC_LED_A)           |\
                                     PIN_MODE_ALTERNATE(GPIOC_SD_DAT0)      |\
                                     PIN_MODE_ALTERNATE(GPIOC_SD_DAT1)      |\
                                     PIN_MODE_ALTERNATE(GPIOC_SD_DAT2)      |\
                                     PIN_MODE_ALTERNATE(GPIOC_SD_DAT3)      |\
                                     PIN_MODE_ALTERNATE(GPIOC_SD_CLK)       |\
                                     PIN_MODE_INPUT(GPIOC_PIN13)            |\
                                     PIN_MODE_INPUT(GPIOC_PIN14)            |\
                                     PIN_MODE_INPUT(GPIOC_PIN15))
#define VAL_GPIOC_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOC_PIN0)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_THERMO_1)     |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_THERMO_2)     |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_THERMO_3)     |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_HG_ACCEL_INT2)|\
                                     PIN_OTYPE_PUSHPULL(GPIOC_HG_ACCEL_INT1)|\
                                     PIN_OTYPE_PUSHPULL(GPIOC_LED_C)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_LED_A)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_DAT0)      |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_DAT1)      |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_DAT2)      |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_DAT3)      |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_SD_CLK)       |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN13)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN14)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN15))
#define VAL_GPIOC_OSPEEDR           (PIN_OSPEED_2M(GPIOC_PIN0)              |\
                                     PIN_OSPEED_2M(GPIOC_THERMO_1)          |\
                                     PIN_OSPEED_2M(GPIOC_THERMO_2)          |\
                                     PIN_OSPEED_2M(GPIOC_THERMO_3)          |\
                                     PIN_OSPEED_2M(GPIOC_HG_ACCEL_INT2)     |\
                                     PIN_OSPEED_2M(GPIOC_HG_ACCEL_INT1)     |\
                                     PIN_OSPEED_2M(GPIOC_LED_C)             |\
                                     PIN_OSPEED_2M(GPIOC_LED_A)             |\
                                     PIN_OSPEED_100M(GPIOC_SD_DAT0)         |\
                                     PIN_OSPEED_100M(GPIOC_SD_DAT1)         |\
                                     PIN_OSPEED_100M(GPIOC_SD_DAT2)         |\
                                     PIN_OSPEED_100M(GPIOC_SD_DAT3)         |\
                                     PIN_OSPEED_100M(GPIOC_SD_CLK)          |\
                                     PIN_OSPEED_2M(GPIOC_PIN13)             |\
                                     PIN_OSPEED_2M(GPIOC_PIN14)             |\
                                     PIN_OSPEED_2M(GPIOC_PIN15))
#define VAL_GPIOC_PUPDR             (PIN_PUPDR_PULLUP(GPIOC_PIN0)           |\
                                     PIN_PUPDR_FLOATING(GPIOC_THERMO_1)     |\
                                     PIN_PUPDR_FLOATING(GPIOC_THERMO_2)     |\
                                     PIN_PUPDR_FLOATING(GPIOC_THERMO_3)     |\
                                     PIN_PUPDR_FLOATING(GPIOC_HG_ACCEL_INT2)|\
                                     PIN_PUPDR_FLOATING(GPIOC_HG_ACCEL_INT1)|\
                                     PIN_PUPDR_FLOATING(GPIOC_LED_C)        |\
                                     PIN_PUPDR_FLOATING(GPIOC_LED_A)        |\
                                     PIN_PUPDR_FLOATING(GPIOC_SD_DAT0)      |\
                                     PIN_PUPDR_FLOATING(GPIOC_SD_DAT1)      |\
                                     PIN_PUPDR_FLOATING(GPIOC_SD_DAT2)      |\
                                     PIN_PUPDR_FLOATING(GPIOC_SD_DAT3)      |\
                                     PIN_PUPDR_FLOATING(GPIOC_SD_CLK)       |\
                                     PIN_PUPDR_PULLUP(GPIOC_PIN13)          |\
                                     PIN_PUPDR_PULLUP(GPIOC_PIN14)          |\
                                     PIN_PUPDR_PULLUP(GPIOC_PIN15))
#define VAL_GPIOC_ODR               (PIN_ODR_HIGH(GPIOC_PIN0)               |\
                                     PIN_ODR_HIGH(GPIOC_THERMO_1)           |\
                                     PIN_ODR_HIGH(GPIOC_THERMO_2)           |\
                                     PIN_ODR_HIGH(GPIOC_THERMO_3)           |\
                                     PIN_ODR_HIGH(GPIOC_HG_ACCEL_INT2)      |\
                                     PIN_ODR_HIGH(GPIOC_HG_ACCEL_INT1)      |\
                                     PIN_ODR_LOW(GPIOC_LED_C)               |\
                                     PIN_ODR_LOW(GPIOC_LED_A)               |\
                                     PIN_ODR_HIGH(GPIOC_SD_DAT0)            |\
                                     PIN_ODR_HIGH(GPIOC_SD_DAT1)            |\
                                     PIN_ODR_HIGH(GPIOC_SD_DAT2)            |\
                                     PIN_ODR_HIGH(GPIOC_SD_DAT3)            |\
                                     PIN_ODR_HIGH(GPIOC_SD_CLK)             |\
                                     PIN_ODR_HIGH(GPIOC_PIN13)              |\
                                     PIN_ODR_HIGH(GPIOC_PIN14)              |\
                                     PIN_ODR_HIGH(GPIOC_PIN15))
#define VAL_GPIOC_AFRL              (PIN_AFIO_AF(GPIOC_PIN0, 0)             |\
                                     PIN_AFIO_AF(GPIOC_THERMO_1, 0)         |\
                                     PIN_AFIO_AF(GPIOC_THERMO_2, 0)         |\
                                     PIN_AFIO_AF(GPIOC_THERMO_3, 0)         |\
                                     PIN_AFIO_AF(GPIOC_HG_ACCEL_INT2, 0)    |\
                                     PIN_AFIO_AF(GPIOC_HG_ACCEL_INT1, 0)    |\
                                     PIN_AFIO_AF(GPIOC_LED_C, 0)            |\
                                     PIN_AFIO_AF(GPIOC_LED_A, 0))
#define VAL_GPIOC_AFRH              (PIN_AFIO_AF(GPIOC_SD_DAT0, 12)         |\
                                     PIN_AFIO_AF(GPIOC_SD_DAT1, 12)         |\
                                     PIN_AFIO_AF(GPIOC_SD_DAT2, 12)         |\
                                     PIN_AFIO_AF(GPIOC_SD_DAT3, 12)         |\
                                     PIN_AFIO_AF(GPIOC_SD_CLK, 12)          |\
                                     PIN_AFIO_AF(GPIOC_PIN13, 0)            |\
                                     PIN_AFIO_AF(GPIOC_PIN14, 0)            |\
                                     PIN_AFIO_AF(GPIOC_PIN15, 0))


/*
 * GPIOD setup:
 *
 * PD0  - PIN0                      (input pullup).
 * PD1  - SD_CD                     (input pullup).
 * PD2  - SD_CMD                    (alternate 12).
 * PD3  - PIN3                      (input pullup).
 * PD4  - PIN4                      (input pullup).
 * PD5  - SER_OUT_2                 (alternate 7).
 * PD6  - SER_IN_2                  (alternate 7).
 * PD7  - BARO_CS                   (output pushpull).
 * PD8  - LG_ACCEL_INT2             (input pullup).
 * PD9  - LG_ACCEL_INT1             (input pullup).
 * PD10 - LG_ACCEL_CS               (input pullup, pin driven by PB12).
 * PD11 - PIN11                     (input pullup).
 * PD12 - PIN12                     (input pullup).
 * PD13 - SJ1                       (input pullup).
 * PD14 - PIN14                     (input pullup).
 * PD15 - PIN15                     (input pullup).
 */
#define VAL_GPIOD_MODER             (PIN_MODE_INPUT(GPIOD_PIN0)             |\
                                     PIN_MODE_INPUT(GPIOD_SD_CD)            |\
                                     PIN_MODE_ALTERNATE(GPIOD_SD_CMD)       |\
                                     PIN_MODE_INPUT(GPIOD_PIN3)             |\
                                     PIN_MODE_INPUT(GPIOD_PIN4)             |\
                                     PIN_MODE_ALTERNATE(GPIOD_SER_OUT_2)    |\
                                     PIN_MODE_ALTERNATE(GPIOD_SER_IN_2)     |\
                                     PIN_MODE_OUTPUT(GPIOD_BARO_CS)         |\
                                     PIN_MODE_INPUT(GPIOD_LG_ACCEL_INT2)    |\
                                     PIN_MODE_INPUT(GPIOD_LG_ACCEL_INT1)    |\
                                     PIN_MODE_INPUT(GPIOD_LG_ACCEL_CS)      |\
                                     PIN_MODE_INPUT(GPIOD_PIN11)            |\
                                     PIN_MODE_INPUT(GPIOD_PIN12)            |\
                                     PIN_MODE_INPUT(GPIOD_SJ1)              |\
                                     PIN_MODE_INPUT(GPIOD_PIN14)            |\
                                     PIN_MODE_INPUT(GPIOD_PIN15))
#define VAL_GPIOD_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOD_PIN0)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_SD_CD)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_SD_CMD)       |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN3)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN4)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_SER_OUT_2)    |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_SER_IN_2)     |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_BARO_CS)      |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_LG_ACCEL_INT2)|\
                                     PIN_OTYPE_PUSHPULL(GPIOD_LG_ACCEL_INT1)|\
                                     PIN_OTYPE_PUSHPULL(GPIOD_LG_ACCEL_CS)  |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN11)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN12)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_SJ1)          |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN14)        |\
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN15))
#define VAL_GPIOD_OSPEEDR           (PIN_OSPEED_2M(GPIOD_PIN0)              |\
                                     PIN_OSPEED_2M(GPIOD_SD_CD)             |\
                                     PIN_OSPEED_100M(GPIOD_SD_CMD)          |\
                                     PIN_OSPEED_2M(GPIOD_PIN3)              |\
                                     PIN_OSPEED_2M(GPIOD_PIN4)              |\
                                     PIN_OSPEED_2M(GPIOD_SER_OUT_2)         |\
                                     PIN_OSPEED_2M(GPIOD_SER_IN_2)          |\
                                     PIN_OSPEED_25M(GPIOD_BARO_CS)          |\
                                     PIN_OSPEED_2M(GPIOD_LG_ACCEL_INT2)     |\
                                     PIN_OSPEED_2M(GPIOD_LG_ACCEL_INT1)     |\
                                     PIN_OSPEED_25M(GPIOD_LG_ACCEL_CS)      |\
                                     PIN_OSPEED_2M(GPIOD_PIN11)             |\
                                     PIN_OSPEED_2M(GPIOD_PIN12)             |\
                                     PIN_OSPEED_2M(GPIOD_SJ1)               |\
                                     PIN_OSPEED_2M(GPIOD_PIN14)             |\
                                     PIN_OSPEED_2M(GPIOD_PIN15))
#define VAL_GPIOD_PUPDR             (PIN_PUPDR_PULLUP(GPIOD_PIN0)           |\
                                     PIN_PUPDR_FLOATING(GPIOD_SD_CD)        |\
                                     PIN_PUPDR_FLOATING(GPIOD_SD_CMD)       |\
                                     PIN_PUPDR_PULLUP(GPIOD_PIN3)           |\
                                     PIN_PUPDR_PULLUP(GPIOD_PIN4)           |\
                                     PIN_PUPDR_FLOATING(GPIOD_SER_OUT_2)    |\
                                     PIN_PUPDR_FLOATING(GPIOD_SER_IN_2)     |\
                                     PIN_PUPDR_FLOATING(GPIOD_BARO_CS)      |\
                                     PIN_PUPDR_FLOATING(GPIOD_LG_ACCEL_INT2)|\
                                     PIN_PUPDR_FLOATING(GPIOD_LG_ACCEL_INT1)|\
                                     PIN_PUPDR_FLOATING(GPIOD_LG_ACCEL_CS)  |\
                                     PIN_PUPDR_PULLUP(GPIOD_PIN11)          |\
                                     PIN_PUPDR_PULLUP(GPIOD_PIN12)          |\
                                     PIN_PUPDR_PULLUP(GPIOD_SJ1)            |\
                                     PIN_PUPDR_PULLUP(GPIOD_PIN14)          |\
                                     PIN_PUPDR_PULLUP(GPIOD_PIN15))
#define VAL_GPIOD_ODR               (PIN_ODR_HIGH(GPIOD_PIN0)               |\
                                     PIN_ODR_HIGH(GPIOD_SD_CD)              |\
                                     PIN_ODR_HIGH(GPIOD_SD_CMD)             |\
                                     PIN_ODR_HIGH(GPIOD_PIN3)               |\
                                     PIN_ODR_HIGH(GPIOD_PIN4)               |\
                                     PIN_ODR_HIGH(GPIOD_SER_OUT_2)          |\
                                     PIN_ODR_HIGH(GPIOD_SER_IN_2)           |\
                                     PIN_ODR_HIGH(GPIOD_BARO_CS)            |\
                                     PIN_ODR_HIGH(GPIOD_LG_ACCEL_INT2)      |\
                                     PIN_ODR_HIGH(GPIOD_LG_ACCEL_INT1)      |\
                                     PIN_ODR_HIGH(GPIOD_LG_ACCEL_CS)        |\
                                     PIN_ODR_HIGH(GPIOD_PIN11)              |\
                                     PIN_ODR_HIGH(GPIOD_PIN12)              |\
                                     PIN_ODR_HIGH(GPIOD_SJ1)                |\
                                     PIN_ODR_HIGH(GPIOD_PIN14)              |\
                                     PIN_ODR_HIGH(GPIOD_PIN15))
#define VAL_GPIOD_AFRL              (PIN_AFIO_AF(GPIOD_PIN0, 0)             |\
                                     PIN_AFIO_AF(GPIOD_SD_CD, 0)            |\
                                     PIN_AFIO_AF(GPIOD_SD_CMD, 12)          |\
                                     PIN_AFIO_AF(GPIOD_PIN3, 0)             |\
                                     PIN_AFIO_AF(GPIOD_PIN4, 0)             |\
                                     PIN_AFIO_AF(GPIOD_SER_OUT_2, 7)        |\
                                     PIN_AFIO_AF(GPIOD_SER_IN_2, 7)         |\
                                     PIN_AFIO_AF(GPIOD_BARO_CS, 0))
#define VAL_GPIOD_AFRH              (PIN_AFIO_AF(GPIOD_LG_ACCEL_INT2, 0)    |\
                                     PIN_AFIO_AF(GPIOD_LG_ACCEL_INT1, 0)    |\
                                     PIN_AFIO_AF(GPIOD_LG_ACCEL_CS, 0)      |\
                                     PIN_AFIO_AF(GPIOD_PIN11, 0)            |\
                                     PIN_AFIO_AF(GPIOD_PIN12, 0)            |\
                                     PIN_AFIO_AF(GPIOD_SJ1, 0)              |\
                                     PIN_AFIO_AF(GPIOD_PIN14, 0)            |\
                                     PIN_AFIO_AF(GPIOD_PIN15, 0))

/*
 * GPIOE setup:
 *
 * PE0  - MAGNO_DRDY                (input pullup).
 * PE1  - PIN1                      (input pullup).
 * PE2  - PIN2                      (input pullup).
 * PE3  - PIN3                      (input pullup).
 * PE4  - PIN4                      (input pullup).
 * PE5  - PIN5                      (input pullup).
 * PE6  - PIN6                      (input pullup).
 * PE7  - PYRO_1_C                  (input pullup).
 * PE8  - PYRO_2_C                  (input pullup).
 * PE9  - PYRO_3_C                  (input pullup).
 * PE10 - PYRO_1_F                  (output pushpull).
 * PE11 - PYRO_2_F                  (output pushpull).
 * PE12 - PYRO_3_F                  (output pushpull).
 * PE13 - GYRO_INT1                 (input pullup).
 * PE14 - GYRO_DRDY                 (input pullup).
 * PE15 - PIN15                     (input pullup).
 */
#define VAL_GPIOE_MODER             (PIN_MODE_INPUT(GPIOE_MAGNO_DRDY)       |\
                                     PIN_MODE_INPUT(GPIOE_PIN1)             |\
                                     PIN_MODE_INPUT(GPIOE_PIN2)             |\
                                     PIN_MODE_INPUT(GPIOE_PIN3)             |\
                                     PIN_MODE_INPUT(GPIOE_PIN4)             |\
                                     PIN_MODE_INPUT(GPIOE_PIN5)             |\
                                     PIN_MODE_INPUT(GPIOE_PIN6)             |\
                                     PIN_MODE_INPUT(GPIOE_PYRO_1_C)         |\
                                     PIN_MODE_INPUT(GPIOE_PYRO_2_C)         |\
                                     PIN_MODE_INPUT(GPIOE_PYRO_3_C)         |\
                                     PIN_MODE_OUTPUT(GPIOE_PYRO_1_F)        |\
                                     PIN_MODE_OUTPUT(GPIOE_PYRO_2_F)        |\
                                     PIN_MODE_OUTPUT(GPIOE_PYRO_3_F)        |\
                                     PIN_MODE_INPUT(GPIOE_GYRO_INT1)        |\
                                     PIN_MODE_INPUT(GPIOE_GYRO_DRDY)        |\
                                     PIN_MODE_INPUT(GPIOE_PIN15))
#define VAL_GPIOE_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOE_MAGNO_DRDY)   |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN1)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN2)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN3)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN4)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN5)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN6)         |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PYRO_1_C)     |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PYRO_2_C)     |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PYRO_3_C)     |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PYRO_1_F)     |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PYRO_2_F)     |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PYRO_3_F)     |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_GYRO_INT1)    |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_GYRO_DRDY)    |\
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN15))
#define VAL_GPIOE_OSPEEDR           (PIN_OSPEED_2M(GPIOE_MAGNO_DRDY)        |\
                                     PIN_OSPEED_2M(GPIOE_PIN1)              |\
                                     PIN_OSPEED_2M(GPIOE_PIN2)              |\
                                     PIN_OSPEED_2M(GPIOE_PIN3)              |\
                                     PIN_OSPEED_2M(GPIOE_PIN4)              |\
                                     PIN_OSPEED_2M(GPIOE_PIN5)              |\
                                     PIN_OSPEED_2M(GPIOE_PIN6)              |\
                                     PIN_OSPEED_2M(GPIOE_PYRO_1_C)          |\
                                     PIN_OSPEED_2M(GPIOE_PYRO_2_C)          |\
                                     PIN_OSPEED_2M(GPIOE_PYRO_3_C)          |\
                                     PIN_OSPEED_2M(GPIOE_PYRO_1_F)          |\
                                     PIN_OSPEED_2M(GPIOE_PYRO_2_F)          |\
                                     PIN_OSPEED_2M(GPIOE_PYRO_3_F)          |\
                                     PIN_OSPEED_2M(GPIOE_GYRO_INT1)         |\
                                     PIN_OSPEED_2M(GPIOE_GYRO_DRDY)         |\
                                     PIN_OSPEED_2M(GPIOE_PIN15))
#define VAL_GPIOE_PUPDR             (PIN_PUPDR_PULLUP(GPIOE_MAGNO_DRDY)     |\
                                     PIN_PUPDR_PULLUP(GPIOE_PIN1)           |\
                                     PIN_PUPDR_PULLUP(GPIOE_PIN2)           |\
                                     PIN_PUPDR_PULLUP(GPIOE_PIN3)           |\
                                     PIN_PUPDR_PULLUP(GPIOE_PIN4)           |\
                                     PIN_PUPDR_PULLUP(GPIOE_PIN5)           |\
                                     PIN_PUPDR_PULLUP(GPIOE_PIN6)           |\
                                     PIN_PUPDR_PULLUP(GPIOE_PYRO_1_C)       |\
                                     PIN_PUPDR_PULLUP(GPIOE_PYRO_2_C)       |\
                                     PIN_PUPDR_PULLUP(GPIOE_PYRO_3_C)       |\
                                     PIN_PUPDR_PULLDOWN(GPIOE_PYRO_1_F)     |\
                                     PIN_PUPDR_PULLDOWN(GPIOE_PYRO_2_F)     |\
                                     PIN_PUPDR_PULLDOWN(GPIOE_PYRO_3_F)     |\
                                     PIN_PUPDR_PULLUP(GPIOE_GYRO_INT1)      |\
                                     PIN_PUPDR_PULLUP(GPIOE_GYRO_DRDY)      |\
                                     PIN_PUPDR_PULLUP(GPIOE_PIN15))
#define VAL_GPIOE_ODR               (PIN_ODR_HIGH(GPIOE_MAGNO_DRDY)         |\
                                     PIN_ODR_HIGH(GPIOE_PIN1)               |\
                                     PIN_ODR_HIGH(GPIOE_PIN2)               |\
                                     PIN_ODR_HIGH(GPIOE_PIN3)               |\
                                     PIN_ODR_HIGH(GPIOE_PIN4)               |\
                                     PIN_ODR_HIGH(GPIOE_PIN5)               |\
                                     PIN_ODR_HIGH(GPIOE_PIN6)               |\
                                     PIN_ODR_HIGH(GPIOE_PYRO_1_C)           |\
                                     PIN_ODR_HIGH(GPIOE_PYRO_2_C)           |\
                                     PIN_ODR_HIGH(GPIOE_PYRO_3_C)           |\
                                     PIN_ODR_LOW(GPIOE_PYRO_1_F)            |\
                                     PIN_ODR_LOW(GPIOE_PYRO_2_F)            |\
                                     PIN_ODR_LOW(GPIOE_PYRO_3_F)            |\
                                     PIN_ODR_HIGH(GPIOE_GYRO_INT1)          |\
                                     PIN_ODR_HIGH(GPIOE_GYRO_DRDY)          |\
                                     PIN_ODR_HIGH(GPIOE_PIN15))
#define VAL_GPIOE_AFRL              (PIN_AFIO_AF(GPIOE_MAGNO_DRDY, 0)       |\
                                     PIN_AFIO_AF(GPIOE_PIN1, 0)             |\
                                     PIN_AFIO_AF(GPIOE_PIN2, 0)             |\
                                     PIN_AFIO_AF(GPIOE_PIN3, 0)             |\
                                     PIN_AFIO_AF(GPIOE_PIN4, 0)             |\
                                     PIN_AFIO_AF(GPIOE_PIN5, 0)             |\
                                     PIN_AFIO_AF(GPIOE_PIN6, 0)             |\
                                     PIN_AFIO_AF(GPIOE_PYRO_1_C, 0))
#define VAL_GPIOE_AFRH              (PIN_AFIO_AF(GPIOE_PYRO_2_C, 0)         |\
                                     PIN_AFIO_AF(GPIOE_PYRO_3_C, 0)         |\
                                     PIN_AFIO_AF(GPIOE_PYRO_1_F, 0)         |\
                                     PIN_AFIO_AF(GPIOE_PYRO_2_F, 0)         |\
                                     PIN_AFIO_AF(GPIOE_PYRO_3_F, 0)         |\
                                     PIN_AFIO_AF(GPIOE_GYRO_INT1, 0)        |\
                                     PIN_AFIO_AF(GPIOE_GYRO_DRDY, 0)        |\
                                     PIN_AFIO_AF(GPIOE_PIN15, 0))

/*
 * GPIOF setup:
 *
 * PF0  - PIN0                      (input floating).
 * PF1  - PIN1                      (input floating).
 * PF2  - PIN2                      (input floating).
 * PF3  - PIN3                      (input floating).
 * PF4  - PIN4                      (input floating).
 * PF5  - PIN5                      (input floating).
 * PF6  - PIN6                      (input floating).
 * PF7  - PIN7                      (input floating).
 * PF8  - PIN8                      (input floating).
 * PF9  - PIN9                      (input floating).
 * PF10 - PIN10                     (input floating).
 * PF11 - PIN11                     (input floating).
 * PF12 - PIN12                     (input floating).
 * PF13 - PIN13                     (input floating).
 * PF14 - PIN14                     (input floating).
 * PF15 - PIN15                     (input floating).
 */
#define VAL_GPIOF_MODER             (PIN_MODE_INPUT(GPIOF_PIN0)             |\
                                     PIN_MODE_INPUT(GPIOF_PIN1)             |\
                                     PIN_MODE_INPUT(GPIOF_PIN2)             |\
                                     PIN_MODE_INPUT(GPIOF_PIN3)             |\
                                     PIN_MODE_INPUT(GPIOF_PIN4)             |\
                                     PIN_MODE_INPUT(GPIOF_PIN5)             |\
                                     PIN_MODE_INPUT(GPIOF_PIN6)             |\
                                     PIN_MODE_INPUT(GPIOF_PIN7)             |\
                                     PIN_MODE_INPUT(GPIOF_PIN8)             |\
                                     PIN_MODE_INPUT(GPIOF_PIN9)             |\
                                     PIN_MODE_INPUT(GPIOF_PIN10)            |\
                                     PIN_MODE_INPUT(GPIOF_PIN11)            |\
                                     PIN_MODE_INPUT(GPIOF_PIN12)            |\
                                     PIN_MODE_INPUT(GPIOF_PIN13)            |\
                                     PIN_MODE_INPUT(GPIOF_PIN14)            |\
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
#define VAL_GPIOF_OSPEEDR           (PIN_OSPEED_2M(GPIOF_PIN0) |          \
                                     PIN_OSPEED_2M(GPIOF_PIN1) |          \
                                     PIN_OSPEED_2M(GPIOF_PIN2) |          \
                                     PIN_OSPEED_2M(GPIOF_PIN3) |          \
                                     PIN_OSPEED_2M(GPIOF_PIN4) |          \
                                     PIN_OSPEED_2M(GPIOF_PIN5) |          \
                                     PIN_OSPEED_2M(GPIOF_PIN6) |          \
                                     PIN_OSPEED_2M(GPIOF_PIN7) |          \
                                     PIN_OSPEED_2M(GPIOF_PIN8) |          \
                                     PIN_OSPEED_2M(GPIOF_PIN9) |          \
                                     PIN_OSPEED_2M(GPIOF_PIN10) |         \
                                     PIN_OSPEED_2M(GPIOF_PIN11) |         \
                                     PIN_OSPEED_2M(GPIOF_PIN12) |         \
                                     PIN_OSPEED_2M(GPIOF_PIN13) |         \
                                     PIN_OSPEED_2M(GPIOF_PIN14) |         \
                                     PIN_OSPEED_2M(GPIOF_PIN15))
#define VAL_GPIOF_PUPDR             (PIN_PUPDR_FLOATING(GPIOF_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN15))
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

/*
 * GPIOG setup:
 *
 * PG0  - PIN0                      (input floating).
 * PG1  - PIN1                      (input floating).
 * PG2  - PIN2                      (input floating).
 * PG3  - PIN3                      (input floating).
 * PG4  - PIN4                      (input floating).
 * PG5  - PIN5                      (input floating).
 * PG6  - PIN6                      (input floating).
 * PG7  - PIN7                      (input floating).
 * PG8  - PIN8                      (input floating).
 * PG9  - PIN9                      (input floating).
 * PG10 - PIN10                     (input floating).
 * PG11 - PIN11                     (input floating).
 * PG12 - PIN12                     (input floating).
 * PG13 - PIN13                     (input floating).
 * PG14 - PIN14                     (input floating).
 * PG15 - PIN15                     (input floating).
 */
#define VAL_GPIOG_MODER             (PIN_MODE_INPUT(GPIOG_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN15))
#define VAL_GPIOG_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOG_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN15))
#define VAL_GPIOG_OSPEEDR           (PIN_OSPEED_2M(GPIOG_PIN0) |          \
                                     PIN_OSPEED_2M(GPIOG_PIN1) |          \
                                     PIN_OSPEED_2M(GPIOG_PIN2) |          \
                                     PIN_OSPEED_2M(GPIOG_PIN3) |          \
                                     PIN_OSPEED_2M(GPIOG_PIN4) |          \
                                     PIN_OSPEED_2M(GPIOG_PIN5) |          \
                                     PIN_OSPEED_2M(GPIOG_PIN6) |          \
                                     PIN_OSPEED_2M(GPIOG_PIN7) |          \
                                     PIN_OSPEED_2M(GPIOG_PIN8) |          \
                                     PIN_OSPEED_2M(GPIOG_PIN9) |          \
                                     PIN_OSPEED_2M(GPIOG_PIN10) |         \
                                     PIN_OSPEED_2M(GPIOG_PIN11) |         \
                                     PIN_OSPEED_2M(GPIOG_PIN12) |         \
                                     PIN_OSPEED_2M(GPIOG_PIN13) |         \
                                     PIN_OSPEED_2M(GPIOG_PIN14) |         \
                                     PIN_OSPEED_2M(GPIOG_PIN15))
#define VAL_GPIOG_PUPDR             (PIN_PUPDR_FLOATING(GPIOG_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN15))
#define VAL_GPIOG_ODR               (PIN_ODR_HIGH(GPIOG_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOG_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOG_PIN15))
#define VAL_GPIOG_AFRL              (PIN_AFIO_AF(GPIOG_PIN0, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN7, 0))
#define VAL_GPIOG_AFRH              (PIN_AFIO_AF(GPIOG_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOG_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOG_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOG_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOG_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOG_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOG_PIN15, 0))

/*
 * GPIOH setup:
 *
 * PH0  - OSC_IN                    (input floating).
 * PH1  - OSC_OUT                   (input floating).
 * PH2  - PIN2                      (input floating).
 * PH3  - PIN3                      (input floating).
 * PH4  - PIN4                      (input floating).
 * PH5  - PIN5                      (input floating).
 * PH6  - PIN6                      (input floating).
 * PH7  - PIN7                      (input floating).
 * PH8  - PIN8                      (input floating).
 * PH9  - PIN9                      (input floating).
 * PH10 - PIN10                     (input floating).
 * PH11 - PIN11                     (input floating).
 * PH12 - PIN12                     (input floating).
 * PH13 - PIN13                     (input floating).
 * PH14 - PIN14                     (input floating).
 * PH15 - PIN15                     (input floating).
 */
#define VAL_GPIOH_MODER             (PIN_MODE_INPUT(GPIOH_OSC_IN) |         \
                                     PIN_MODE_INPUT(GPIOH_OSC_OUT) |        \
                                     PIN_MODE_INPUT(GPIOH_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN15))
#define VAL_GPIOH_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOH_OSC_IN) |     \
                                     PIN_OTYPE_PUSHPULL(GPIOH_OSC_OUT) |    \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN15))
#define VAL_GPIOH_OSPEEDR           (PIN_OSPEED_100M(GPIOH_OSC_IN) |        \
                                     PIN_OSPEED_100M(GPIOH_OSC_OUT) |       \
                                     PIN_OSPEED_2M(GPIOH_PIN2) |          \
                                     PIN_OSPEED_2M(GPIOH_PIN3) |          \
                                     PIN_OSPEED_2M(GPIOH_PIN4) |          \
                                     PIN_OSPEED_2M(GPIOH_PIN5) |          \
                                     PIN_OSPEED_2M(GPIOH_PIN6) |          \
                                     PIN_OSPEED_2M(GPIOH_PIN7) |          \
                                     PIN_OSPEED_2M(GPIOH_PIN8) |          \
                                     PIN_OSPEED_2M(GPIOH_PIN9) |          \
                                     PIN_OSPEED_2M(GPIOH_PIN10) |         \
                                     PIN_OSPEED_2M(GPIOH_PIN11) |         \
                                     PIN_OSPEED_2M(GPIOH_PIN12) |         \
                                     PIN_OSPEED_2M(GPIOH_PIN13) |         \
                                     PIN_OSPEED_2M(GPIOH_PIN14) |         \
                                     PIN_OSPEED_2M(GPIOH_PIN15))
#define VAL_GPIOH_PUPDR             (PIN_PUPDR_FLOATING(GPIOH_OSC_IN) |     \
                                     PIN_PUPDR_FLOATING(GPIOH_OSC_OUT) |    \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN15))
#define VAL_GPIOH_ODR               (PIN_ODR_HIGH(GPIOH_OSC_IN) |           \
                                     PIN_ODR_HIGH(GPIOH_OSC_OUT) |          \
                                     PIN_ODR_HIGH(GPIOH_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOH_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOH_PIN15))
#define VAL_GPIOH_AFRL              (PIN_AFIO_AF(GPIOH_OSC_IN, 0) |         \
                                     PIN_AFIO_AF(GPIOH_OSC_OUT, 0) |        \
                                     PIN_AFIO_AF(GPIOH_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN7, 0))
#define VAL_GPIOH_AFRH              (PIN_AFIO_AF(GPIOH_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOH_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOH_PIN15, 0))

/*
 * GPIOI setup:
 *
 * PI0  - PIN0                      (input floating).
 * PI1  - PIN1                      (input floating).
 * PI2  - PIN2                      (input floating).
 * PI3  - PIN3                      (input floating).
 * PI4  - PIN4                      (input floating).
 * PI5  - PIN5                      (input floating).
 * PI6  - PIN6                      (input floating).
 * PI7  - PIN7                      (input floating).
 * PI8  - PIN8                      (input floating).
 * PI9  - PIN9                      (input floating).
 * PI10 - PIN10                     (input floating).
 * PI11 - PIN11                     (input floating).
 * PI12 - PIN12                     (input floating).
 * PI13 - PIN13                     (input floating).
 * PI14 - PIN14                     (input floating).
 * PI15 - PIN15                     (input floating).
 */
#define VAL_GPIOI_MODER             (PIN_MODE_INPUT(GPIOI_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOI_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOI_PIN15))
#define VAL_GPIOI_OTYPER            (PIN_OTYPE_PUSHPULL(GPIOI_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOI_PIN15))
#define VAL_GPIOI_OSPEEDR           (PIN_OSPEED_2M(GPIOI_PIN0) |          \
                                     PIN_OSPEED_2M(GPIOI_PIN1) |          \
                                     PIN_OSPEED_2M(GPIOI_PIN2) |          \
                                     PIN_OSPEED_2M(GPIOI_PIN3) |          \
                                     PIN_OSPEED_2M(GPIOI_PIN4) |          \
                                     PIN_OSPEED_2M(GPIOI_PIN5) |          \
                                     PIN_OSPEED_2M(GPIOI_PIN6) |          \
                                     PIN_OSPEED_2M(GPIOI_PIN7) |          \
                                     PIN_OSPEED_2M(GPIOI_PIN8) |          \
                                     PIN_OSPEED_2M(GPIOI_PIN9) |          \
                                     PIN_OSPEED_2M(GPIOI_PIN10) |         \
                                     PIN_OSPEED_2M(GPIOI_PIN11) |         \
                                     PIN_OSPEED_2M(GPIOI_PIN12) |         \
                                     PIN_OSPEED_2M(GPIOI_PIN13) |         \
                                     PIN_OSPEED_2M(GPIOI_PIN14) |         \
                                     PIN_OSPEED_2M(GPIOI_PIN15))
#define VAL_GPIOI_PUPDR             (PIN_PUPDR_FLOATING(GPIOI_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOI_PIN15))
#define VAL_GPIOI_ODR               (PIN_ODR_HIGH(GPIOI_PIN0) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN1) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN2) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN3) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN4) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN5) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN6) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN7) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN8) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN9) |             \
                                     PIN_ODR_HIGH(GPIOI_PIN10) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN11) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN12) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN13) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN14) |            \
                                     PIN_ODR_HIGH(GPIOI_PIN15))
#define VAL_GPIOI_AFRL              (PIN_AFIO_AF(GPIOI_PIN0, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN1, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN2, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN3, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN4, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN5, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN6, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN7, 0))
#define VAL_GPIOI_AFRH              (PIN_AFIO_AF(GPIOI_PIN8, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN9, 0) |           \
                                     PIN_AFIO_AF(GPIOI_PIN10, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN11, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN12, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN13, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN14, 0) |          \
                                     PIN_AFIO_AF(GPIOI_PIN15, 0))


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
