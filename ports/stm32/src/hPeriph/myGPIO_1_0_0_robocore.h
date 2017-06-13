/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

DEFINE_PORT____(MYGPIO_SENS1_DIGIA          ,   0, F,  1, -1)
DEFINE_PORT____(MYGPIO_SENS1_DIGIB          ,   1, F,  0, -1)
DEFINE_PORT_IRQ(MYGPIO_SENS1_INT_ADC        ,   2, F,  4,  0, EXTI4_IRQn    )
DEFINE_PORT____(MYGPIO_SENS1_ON             ,   3, E,  3, -1)
DEFINE_PORT____(MYGPIO_SENS2_DIGIA          ,   4, A,  8, -1)
DEFINE_PORT____(MYGPIO_SENS2_DIGIB          ,   5, C,  9, -1)
DEFINE_PORT_IRQ(MYGPIO_SENS2_INT_ADC        ,   6, F, 10,  1, EXTI15_10_IRQn)
DEFINE_PORT____(MYGPIO_SENS2_ON             ,   7, F,  2, -1)
DEFINE_PORT____(MYGPIO_SENS3_DIGIA          ,   8, C, 12, -1)
DEFINE_PORT____(MYGPIO_SENS3_DIGIB          ,   9, D,  2, -1)
DEFINE_PORT_IRQ(MYGPIO_SENS3_INT_ADC        ,  10, C,  5,  2, EXTI9_5_IRQn  )
DEFINE_PORT____(MYGPIO_SENS3_ON             ,  11, C, 15, -1)
DEFINE_PORT____(MYGPIO_SENS4_DIGIA          ,  12, C,  1, -1)
DEFINE_PORT____(MYGPIO_SENS4_DIGIB          ,  13, C,  3,  3)
DEFINE_PORT_IRQ(MYGPIO_SENS4_INT_ADC        ,  14, F,  3,  4, EXTI3_IRQn    )
DEFINE_PORT____(MYGPIO_SENS4_ON             ,  15, C,  0, -1)
DEFINE_PORT____(MYGPIO_SENS5_DIGIA          ,  16, G,  0, -1)
DEFINE_PORT____(MYGPIO_SENS5_DIGIB          ,  17, G,  1, -1)
DEFINE_PORT_IRQ(MYGPIO_SENS5_INT_ADC        ,  18, B,  0,  5, EXTI0_IRQn    )
DEFINE_PORT____(MYGPIO_SENS5_ON             ,  19, E,  7, -1)
DEFINE_PORT_IRQ(MYGPIO_EXT1_PIN1            ,  20, E, 12, -1, EXTI15_10_IRQn)
DEFINE_PORT_IRQ(MYGPIO_EXT1_PIN2            ,  21, E, 13, -1, EXTI15_10_IRQn)
DEFINE_PORT____(MYGPIO_EXT1_PIN3            ,  22, E, 14, -1)
DEFINE_PORT____(MYGPIO_EXT1_PIN4            ,  23, E, 15, -1)
DEFINE_PORT____(MYGPIO_EXT1_PIN5            ,  24, F, 14, -1)
DEFINE_PORT____(MYGPIO_EXT1_PIN6            ,  25, F, 15, -1)
DEFINE_PORT_IRQ(MYGPIO_EXT1_PIN7            ,  26, B,  1,  6, EXTI1_IRQn    )
DEFINE_PORT____(MYGPIO_EXT1_PIN8_URX        ,  27, G,  9, -1)
DEFINE_PORT____(MYGPIO_EXT1_PIN9_UTX        ,  28, G, 14, -1)
DEFINE_PORT_IRQ(MYGPIO_EXT2_PIN1            ,  29, G,  2, -1, EXTI2_IRQn    )
DEFINE_PORT_IRQ(MYGPIO_EXT2_PIN2            ,  30, G,  7, -1, EXTI9_5_IRQn  )
DEFINE_PORT____(MYGPIO_EXT2_PIN3            ,  31, G,  3, -1)
DEFINE_PORT____(MYGPIO_EXT2_PIN4            ,  32, G,  4, -1)
DEFINE_PORT____(MYGPIO_EXT2_PIN5            ,  33, G,  5, -1)
DEFINE_PORT____(MYGPIO_EXT2_PIN6            ,  34, G,  6, -1)
DEFINE_PORT_IRQ(MYGPIO_EXT2_PIN7            ,  35, A,  6,  7, EXTI9_5_IRQn  )
DEFINE_PORT____(MYGPIO_EXT2_PIN8_URX        ,  36, D,  6, -1)
DEFINE_PORT____(MYGPIO_EXT2_PIN9_UTX        ,  37, D,  5, -1)
DEFINE_PORT____(MYGPIO_EXT_SPI_MOSI         ,  38, B, 15, -1)
DEFINE_PORT____(MYGPIO_EXT_SPI_MISO         ,  39, C,  2, -1)
DEFINE_PORT____(MYGPIO_EXT_SPI_SCK          ,  40, B, 13, -1)
DEFINE_PORT____(MYGPIO_EXT_I2C_SDA          ,  41, B,  9, -1)
DEFINE_PORT____(MYGPIO_EXT_I2C_SCL          ,  42, B,  8, -1)
DEFINE_PORT____(MYGPIO_EDI_SERIAL_TX        ,  43, B, 10, -1)
DEFINE_PORT____(MYGPIO_EDI_SERIAL_RX        ,  44, B, 11, -1)
DEFINE_PORT____(MYGPIO_EDI_SERIAL_RTS       ,  45, B, 14, -1)
DEFINE_PORT____(MYGPIO_EDI_SERIAL_CTS       ,  46, D, 11, -1)
DEFINE_PORT_IRQ(MYGPIO_EDI_IRQ_A            ,  47, G, 15, -1, EXTI15_10_IRQn)
DEFINE_PORT____(MYGPIO_EDI_PIN1             ,  48, E,  1, -1)
DEFINE_PORT____(MYGPIO_EDI_PIN2             ,  49, B,  7, -1)
DEFINE_PORT____(MYGPIO_EDI_ADC              ,  50, F,  5, -1)
DEFINE_PORT____(MYGPIO_FT_SERIAL_TX         ,  51, A,  9, -1)
DEFINE_PORT____(MYGPIO_FT_SERIAL_RX         ,  52, A, 10, -1)
DEFINE_PORT_IRQ(MYGPIO_FT_V_USB             ,  53, C, 14, -1, EXTI15_10_IRQn)
DEFINE_PORT____(MYGPIO_CAN_TX               ,  54, D,  1, -1)
DEFINE_PORT____(MYGPIO_CAN_RX               ,  55, D,  0, -1)
DEFINE_PORT____(MYGPIO_CAN_EN               ,  56, E,  2, -1)
DEFINE_PORT____(MYGPIO_SD_MOSI              ,  57, B,  5, -1)
DEFINE_PORT____(MYGPIO_SD_MISO              ,  58, C, 11, -1)
DEFINE_PORT____(MYGPIO_SD_CLK               ,  59, C, 10, -1)
DEFINE_PORT____(MYGPIO_SD_CS                ,  60, B,  6, -1)
DEFINE_PORT____(MYGPIO_SD_IN                ,  61, E,  0, -1)
DEFINE_PORT____(MYGPIO_SD_POWER_ON          ,  62, B,  2, -1)
DEFINE_PORT____(MYGPIO_USB_DM               ,  63, A, 11, -1)
DEFINE_PORT____(MYGPIO_USB_DP               ,  64, A, 12, -1)
DEFINE_PORT____(MYGPIO_USB_CHARGE           ,  65, G,  8, -1)
DEFINE_PORT____(MYGPIO_MOT12_SLEEP          ,  66, E, 10, -1)
DEFINE_PORT____(MYGPIO_MOT1_PWM             ,  67, A,  3, -1)
DEFINE_PORT____(MYGPIO_MOT1A_IN             ,  68, D,  8, -1)
DEFINE_PORT____(MYGPIO_MOT1B_IN             ,  69, D,  9, -1)
DEFINE_PORT____(MYGPIO_MOT1A_ENC            ,  70, E,  9, -1)
DEFINE_PORT____(MYGPIO_MOT1B_ENC            ,  71, E, 11, -1)
DEFINE_PORT____(MYGPIO_MOT2_PWM             ,  72, A,  2, -1)
DEFINE_PORT____(MYGPIO_MOT2A_IN             ,  73, D, 10, -1)
DEFINE_PORT____(MYGPIO_MOT2B_IN             ,  74, D, 14, -1)
DEFINE_PORT____(MYGPIO_MOT2A_ENC            ,  75, A, 15, -1)
DEFINE_PORT____(MYGPIO_MOT2B_ENC            ,  76, B,  3, -1)
DEFINE_PORT____(MYGPIO_MOT34_SLEEP          ,  77, B, 12, -1)
DEFINE_PORT____(MYGPIO_MOT3_PWM             ,  78, F,  9, -1)
DEFINE_PORT____(MYGPIO_MOT3A_IN             ,  79, D,  7, -1)
DEFINE_PORT____(MYGPIO_MOT3B_IN             ,  80, G, 10, -1)
DEFINE_PORT____(MYGPIO_MOT3A_ENC            ,  81, A,  0, -1)
DEFINE_PORT____(MYGPIO_MOT3B_ENC            ,  82, A,  1, -1)
DEFINE_PORT____(MYGPIO_MOT4_PWM             ,  83, F,  8, -1)
DEFINE_PORT____(MYGPIO_MOT4A_IN             ,  84, D,  3, -1)
DEFINE_PORT____(MYGPIO_MOT4B_IN             ,  85, D,  4, -1)
DEFINE_PORT____(MYGPIO_MOT4A_ENC            ,  86, C,  6, -1)
DEFINE_PORT____(MYGPIO_MOT4B_ENC            ,  87, C,  7, -1)
DEFINE_PORT____(MYGPIO_MOT56_SLEEP          ,  88, A,  5, -1)
DEFINE_PORT____(MYGPIO_MOT5_PWM             ,  89, F,  7, -1)
DEFINE_PORT____(MYGPIO_MOT5A_IN             ,  90, G, 12, -1)
DEFINE_PORT____(MYGPIO_MOT5B_IN             ,  91, G, 13, -1)
DEFINE_PORT____(MYGPIO_MOT5A_ENC            ,  92, B,  4, -1)
DEFINE_PORT____(MYGPIO_MOT5B_ENC            ,  93, A,  7, -1)
DEFINE_PORT____(MYGPIO_MOT6_PWM             ,  94, F,  6, -1)
DEFINE_PORT____(MYGPIO_MOT6A_IN             ,  95, D, 15, -1)
DEFINE_PORT____(MYGPIO_MOT6B_IN             ,  96, C,  8, -1)
DEFINE_PORT____(MYGPIO_MOT6A_ENC            ,  97, D, 12, -1)
DEFINE_PORT____(MYGPIO_MOT6B_ENC            ,  98, D, 13, -1)
DEFINE_PORT____(MYGPIO_LED_1                ,  99, F, 11, -1)
DEFINE_PORT____(MYGPIO_LED_2                , 100, F, 12, -1)
DEFINE_PORT____(MYGPIO_LED_3                , 101, F, 13, -1)
DEFINE_PORT_IRQ(MYGPIO_BUTTON_1             , 102, G, 11, -1, EXTI15_10_IRQn)
DEFINE_PORT_IRQ(MYGPIO_BUTTON_2             , 103, E,  8, -1, EXTI9_5_IRQn  )
DEFINE_PORT____(MYGPIO_BAT_MEAS             , 104, C,  4,  8)
DEFINE_PORT____(MYGPIO_TEST_POINT_1         , 105, A,  4, -1)
DEFINE_PORT____(MYGPIO_TEST_POINT_4         , 106, E,  4, -1)
DEFINE_PORT____(MYGPIO_TEST_POINT_3         , 107, C, 13, -1)
DEFINE_PORT____(MYGPIO_TEST_POINT_5         , 108, E,  5, -1)
DEFINE_PORT____(MYGPIO_TEST_POINT_6         , 109, E,  6, -1)

DEFINE_EXTI(MYGPIO_SENS5_INT_ADC      ) // 0
DEFINE_EXTI(MYGPIO_EXT1_PIN7          ) // 1
DEFINE_EXTI(MYGPIO_EXT2_PIN1          ) // 2
DEFINE_EXTI(MYGPIO_SENS4_INT_ADC      ) // 3
DEFINE_EXTI(MYGPIO_SENS1_INT_ADC      ) // 4
DEFINE_EXTI(MYGPIO_SENS3_INT_ADC      ) // 5
DEFINE_EXTI(MYGPIO_EXT2_PIN7          ) // 6
DEFINE_EXTI(MYGPIO_EXT2_PIN2          ) // 7
DEFINE_EXTI(MYGPIO_BUTTON_2           ) // 8
DEFINE_EXTI(-1                        ) // 9
DEFINE_EXTI(MYGPIO_SENS2_INT_ADC      ) // 10
DEFINE_EXTI(MYGPIO_BUTTON_1           ) // 11
DEFINE_EXTI(MYGPIO_EXT1_PIN1          ) // 12
DEFINE_EXTI(MYGPIO_EXT1_PIN2          ) // 13
DEFINE_EXTI(MYGPIO_FT_V_USB           ) // 14
DEFINE_EXTI(MYGPIO_EDI_IRQ_A          ) // 15

DEFINE_ADC(3, 14) //  0 ADC3_IN14   MYGPIO_SENS1_INT_ADC      
DEFINE_ADC(3,  8) //  1 ADC3_IN8    MYGPIO_SENS2_INT_ADC      
DEFINE_ADC(1, 15) //  2 ADC12_IN15  MYGPIO_SENS3_INT_ADC      
DEFINE_ADC(2, 13) //  3 ADC123_IN13 MYGPIO_SENS4_DIGIB        
DEFINE_ADC(3,  9) //  4 ADC3_IN9    MYGPIO_SENS4_INT_ADC      
DEFINE_ADC(2,  8) //  5 ADC12_IN8   MYGPIO_SENS5_INT_ADC      
DEFINE_ADC(1,  9) //  6 ADC12_IN9   MYGPIO_EXT1_PIN7          
DEFINE_ADC(2,  6) //  7 ADC12_IN6   MYGPIO_EXT2_PIN7          
DEFINE_ADC(1, 14) //  8 ADC12_IN14  MYGPIO_BAT_MEAS           
