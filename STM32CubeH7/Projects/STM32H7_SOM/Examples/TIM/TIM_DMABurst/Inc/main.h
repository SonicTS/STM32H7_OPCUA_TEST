/**
  ******************************************************************************
  * @file    TIM/TIM_DMABurst/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h743i_eval.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define TIMx                             TIM2

/* Definition for TIMx clock resources */
#define TIMx_CLK_ENABLE                  __HAL_RCC_TIM2_CLK_ENABLE
#define DMAx_CLK_ENABLE                  __HAL_RCC_DMA1_CLK_ENABLE

/* Definition for TIMx Pins */
#define TIMx_CHANNEL1_GPIO_CLK_ENABLE    __HAL_RCC_GPIOA_CLK_ENABLE

#define TIMx_GPIO_CHANNEL1_PORT          GPIOA
#define GPIO_PIN_CHANNEL1                GPIO_PIN_0


#define GPIO_AF_TIMx                     GPIO_AF1_TIM2

/* Definition for TIMx's DMA */
#define TIMx_UPDATE_DMA_INST             DMA1_Stream1

/* Definition for TIMx's NVIC */
#define TIMx_DMA_IRQn                    DMA1_Stream1_IRQn
#define TIMx_DMA_IRQHandler              DMA1_Stream1_IRQHandler

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */


#endif /* __MAIN_H */

