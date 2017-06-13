/**
 * Copyright (c) 2013-2017 Husarion Sp. z o.o.
 * Distributed under the MIT license.
 * For full terms see the file LICENSE.md.
 */

DEFINE_PORT_IRQ(MYGPIO_SENS1_DIGIA_INT_ADC  ,   0, C,  2,  0, EXTI2_IRQn    )
DEFINE_PORT____(MYGPIO_SENS1_DIGIB          ,   1, B, 13, -1)
DEFINE_PORT____(MYGPIO_SENS1_DIGIC          ,   2, A,  8, -1)
DEFINE_PORT____(MYGPIO_SENS1_DIGID          ,   3, C,  9, -1)
DEFINE_PORT_IRQ(MYGPIO_SENS2_DIGIA_INT_ADC  ,   4, C,  4,  1, EXTI4_IRQn    )
DEFINE_PORT____(MYGPIO_SENS2_DIGIB          ,   5, C,  5, -1)
DEFINE_PORT____(MYGPIO_SENS2_DIGIC          ,   6, C,  6, -1)
DEFINE_PORT____(MYGPIO_SENS2_DIGID          ,   7, C,  7, -1)
DEFINE_PORT_IRQ(MYGPIO_SENS3_DIGIA_INT_ADC  ,   8, C,  3,  2, EXTI3_IRQn    )
DEFINE_PORT____(MYGPIO_SENS3_DIGIB          ,   9, B, 14, -1)
DEFINE_PORT____(MYGPIO_SENS3_DIGIC          ,  10, A,  2, -1)
DEFINE_PORT____(MYGPIO_SENS3_DIGID          ,  11, A,  3, -1)
DEFINE_PORT____(MYGPIO_RPI_SERIAL_TX        ,  12, A,  9, -1)
DEFINE_PORT____(MYGPIO_RPI_SERIAL_RX        ,  13, A, 10, -1)
DEFINE_PORT_IRQ(MYGPIO_RPI_BTN              ,  14, C,  8, -1, EXTI9_5_IRQn  )
DEFINE_PORT____(MYGPIO_RPI_CONSOLE          ,  15, B,  4, -1)
DEFINE_PORT____(MYGPIO_RPI_STATUS           ,  16, B,  3, -1)
DEFINE_PORT____(MYGPIO_FT_SERIAL_TX         ,  17, B, 10, -1)
DEFINE_PORT____(MYGPIO_FT_SERIAL_RX         ,  18, B, 11, -1)
DEFINE_PORT____(MYGPIO_SD_MOSI              ,  19, C, 12, -1)
DEFINE_PORT____(MYGPIO_SD_MISO              ,  20, C, 11, -1)
DEFINE_PORT____(MYGPIO_SD_CLK               ,  21, C, 10, -1)
DEFINE_PORT____(MYGPIO_SD_CS                ,  22, D,  2, -1)
DEFINE_PORT____(MYGPIO_SD_IN                ,  23, B,  5, -1)
DEFINE_PORT____(MYGPIO_MOT12_SLEEP          ,  24, C, 13, -1)
DEFINE_PORT____(MYGPIO_MOT1_PWM             ,  25, B,  8, -1)
DEFINE_PORT____(MYGPIO_MOT1A_IN             ,  26, C, 15, -1)
DEFINE_PORT____(MYGPIO_MOT1B_IN             ,  27, C, 14, -1)
DEFINE_PORT____(MYGPIO_MOT1A_ENC            ,  28, A,  0, -1)
DEFINE_PORT____(MYGPIO_MOT1B_ENC            ,  29, A,  1, -1)
DEFINE_PORT____(MYGPIO_MOT2_PWM             ,  30, B,  9, -1)
DEFINE_PORT____(MYGPIO_MOT2A_IN             ,  31, A,  4, -1)
DEFINE_PORT____(MYGPIO_MOT2B_IN             ,  32, A,  5, -1)
DEFINE_PORT____(MYGPIO_MOT2A_ENC            ,  33, A,  6, -1)
DEFINE_PORT____(MYGPIO_MOT2B_ENC            ,  34, A,  7, -1)
DEFINE_PORT____(MYGPIO_SERVO1_PWM           ,  35, B,  0, -1)
DEFINE_PORT____(MYGPIO_SERVO2_PWM           ,  36, B,  1, -1)
DEFINE_PORT____(MYGPIO_SERVO3_PWM           ,  37, B,  6, -1)
DEFINE_PORT____(MYGPIO_SERVO4_PWM           ,  38, B,  7, -1)
DEFINE_PORT____(MYGPIO_SERVO_POWER_ON       ,  39, B, 15, -1)
DEFINE_PORT____(MYGPIO_SERVO_ADC            ,  40, C,  0,  3)
DEFINE_PORT____(MYGPIO_SENS_POWER_ON        ,  41, B,  2, -1)
DEFINE_PORT____(MYGPIO_LED_1                ,  42, A, 15, -1)
DEFINE_PORT____(MYGPIO_LED_2                ,  43, A, 11, -1)
DEFINE_PORT____(MYGPIO_LED_3                ,  44, A, 12, -1)
DEFINE_PORT_IRQ(MYGPIO_BUTTON_1             ,  45, B, 12, -1, EXTI15_10_IRQn)
DEFINE_PORT____(MYGPIO_BAT_MEAS             ,  46, C,  1,  4)

DEFINE_EXTI(-1                        ) // 0
DEFINE_EXTI(-1                        ) // 1
DEFINE_EXTI(MYGPIO_SENS1_DIGIA_INT_ADC) // 2
DEFINE_EXTI(MYGPIO_SENS3_DIGIA_INT_ADC) // 3
DEFINE_EXTI(MYGPIO_SENS2_DIGIA_INT_ADC) // 4
DEFINE_EXTI(-1                        ) // 5
DEFINE_EXTI(-1                        ) // 6
DEFINE_EXTI(-1                        ) // 7
DEFINE_EXTI(MYGPIO_RPI_BTN            ) // 8
DEFINE_EXTI(-1                        ) // 9
DEFINE_EXTI(-1                        ) // 10
DEFINE_EXTI(-1                        ) // 11
DEFINE_EXTI(MYGPIO_BUTTON_1           ) // 12
DEFINE_EXTI(-1                        ) // 13
DEFINE_EXTI(-1                        ) // 14
DEFINE_EXTI(-1                        ) // 15

DEFINE_ADC(2, 12) //  0 ADC123_IN12 MYGPIO_SENS1_DIGIA_INT_ADC
DEFINE_ADC(3, 14) //  2 ADC12_IN14  MYGPIO_SENS3_DIGIA_INT_ADC
DEFINE_ADC(1, 13) //  1 ADC123_IN13 MYGPIO_SENS2_DIGIA_INT_ADC
DEFINE_ADC(1, 10) //  3 ADC123_IN10 MYGPIO_SERVO_ADC          
DEFINE_ADC(2, 11) //  4 ADC123_IN11 MYGPIO_BAT_MEAS           
