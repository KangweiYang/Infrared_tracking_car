# Infrared_tracking_car
my infrared tracking car, maincontroler is STM32F401CCU6, has matrix keyboard and 2 motors with brush. IDE is Keil
`dirctory tree:
└── Infrared_tracking_car
    ├── EventRecorderStub.scvd
    ├── Hardware
    │   ├── DCmotorDir.c
    │   ├── DCmotorDir.h
    │   ├── DCmotorPWM.c
    │   ├── DCmotorPWM.h
    │   ├── KeyDriver.c
    │   ├── KeyDriver.h
    │   ├── LEDDriver.c
    │   ├── LEDDriver.h
    │   ├── MatrixKeyDriver.c
    │   ├── MatrixKeyDriver.h
    │   ├── MotorCount.c
    │   ├── MotorCount.h
    │   ├── OLED.c
    │   ├── OLED_Font.h
    │   ├── OLED.h
    │   ├── OLED_Wave
    │   │   ├── codetab.h
    │   │   ├── OLED_I2C.c
    │   │   └── OLED_I2C.h
    │   ├── PDSearch.c
    │   └── PDSearch.h
    ├── keilkilll.bat
    ├── Libraries
    │   ├── misc.c
    │   ├── misc.h
    │   ├── stm32f4xx_adc.c
    │   ├── stm32f4xx_adc.h
    │   ├── stm32f4xx_can.c
    │   ├── stm32f4xx_can.h
    │   ├── stm32f4xx_cec.c
    │   ├── stm32f4xx_cec.h
    │   ├── stm32f4xx_crc.c
    │   ├── stm32f4xx_crc.h
    │   ├── stm32f4xx_cryp_aes.c
    │   ├── stm32f4xx_cryp.c
    │   ├── stm32f4xx_cryp_des.c
    │   ├── stm32f4xx_cryp.h
    │   ├── stm32f4xx_cryp_tdes.c
    │   ├── stm32f4xx_dac.c
    │   ├── stm32f4xx_dac.h
    │   ├── stm32f4xx_dbgmcu.c
    │   ├── stm32f4xx_dbgmcu.h
    │   ├── stm32f4xx_dcmi.c
    │   ├── stm32f4xx_dcmi.h
    │   ├── stm32f4xx_dfsdm.c
    │   ├── stm32f4xx_dfsdm.h
    │   ├── stm32f4xx_dma2d.c
    │   ├── stm32f4xx_dma2d.h
    │   ├── stm32f4xx_dma.c
    │   ├── stm32f4xx_dma.h
    │   ├── stm32f4xx_dsi.c
    │   ├── stm32f4xx_dsi.h
    │   ├── stm32f4xx_exti.c
    │   ├── stm32f4xx_exti.h
    │   ├── stm32f4xx_flash.c
    │   ├── stm32f4xx_flash.h
    │   ├── stm32f4xx_flash_ramfunc.c
    │   ├── stm32f4xx_flash_ramfunc.h
    │   ├── stm32f4xx_fmc.c
    │   ├── stm32f4xx_fmc.h
    │   ├── stm32f4xx_fmpi2c.c
    │   ├── stm32f4xx_fmpi2c.h
    │   ├── stm32f4xx_fsmc.c
    │   ├── stm32f4xx_fsmc.h
    │   ├── stm32f4xx_gpio.c
    │   ├── stm32f4xx_gpio.h
    │   ├── stm32f4xx_hash.c
    │   ├── stm32f4xx_hash.h
    │   ├── stm32f4xx_hash_md5.c
    │   ├── stm32f4xx_hash_sha1.c
    │   ├── stm32f4xx_i2c.c
    │   ├── stm32f4xx_i2c.h
    │   ├── stm32f4xx_iwdg.c
    │   ├── stm32f4xx_iwdg.h
    │   ├── stm32f4xx_lptim.c
    │   ├── stm32f4xx_lptim.h
    │   ├── stm32f4xx_ltdc.c
    │   ├── stm32f4xx_ltdc.h
    │   ├── stm32f4xx_pwr.c
    │   ├── stm32f4xx_pwr.h
    │   ├── stm32f4xx_qspi.c
    │   ├── stm32f4xx_qspi.h
    │   ├── stm32f4xx_rcc.c
    │   ├── stm32f4xx_rcc.h
    │   ├── stm32f4xx_rng.c
    │   ├── stm32f4xx_rng.h
    │   ├── stm32f4xx_rtc.c
    │   ├── stm32f4xx_rtc.h
    │   ├── stm32f4xx_sai.c
    │   ├── stm32f4xx_sai.h
    │   ├── stm32f4xx_sdio.c
    │   ├── stm32f4xx_sdio.h
    │   ├── stm32f4xx_spdifrx.c
    │   ├── stm32f4xx_spdifrx.h
    │   ├── stm32f4xx_spi.c
    │   ├── stm32f4xx_spi.h
    │   ├── stm32f4xx_syscfg.c
    │   ├── stm32f4xx_syscfg.h
    │   ├── stm32f4xx_tim.c
    │   ├── stm32f4xx_tim.h
    │   ├── stm32f4xx_usart.c
    │   ├── stm32f4xx_usart.h
    │   ├── stm32f4xx_wwdg.c
    │   └── stm32f4xx_wwdg.h
    ├── project.uvguix.17200
    ├── project.uvguix.杨康韦
    ├── project.uvoptx
    ├── project.uvprojx
    ├── Start
    │   ├── core_cm4.h
    │   ├── core_cmFunc.h
    │   ├── core_cmInstr.h
    │   ├── core_cmSimd.h
    │   ├── startup_stm32f401xx.s
    │   ├── startup_stm32f40_41xxx.s
    │   ├── startup_stm32f40xx.s
    │   ├── stm32f4xx.h
    │   ├── system_stm32f4xx.c
    │   └── system_stm32f4xx.h
    ├── System
    │   ├── Delay.c
    │   ├── Delay.h
    │   ├── GPTimer.c
    │   ├── GPTimer.h
    │   ├── main.c
    │   ├── main.h
    │   ├── PIcontroler.c
    │   └── PIcontroler.h
    └── User
        ├── main.c
        ├── stm32f4xx_conf.h
        ├── stm32f4xx_it.c
        └── stm32f4xx_it.h

7 directories, 129 files`
