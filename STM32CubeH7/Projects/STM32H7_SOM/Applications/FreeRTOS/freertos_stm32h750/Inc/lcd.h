/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_H
#define __LCD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_ltdc.h"
#include "stm32h7xx_hal_dma2d.h"

/** @defgroup STM32H743I_EVAL_LCD_Exported_Constants LCD Exported Constants
 * @{
 */
#define MAX_LAYER_NUMBER       ((uint32_t)2)
#define LCD_MAX_WIDTH          (640)
#define LCD_MAX_HIGHT          (480)

#define LCD_LayerCfgTypeDef    LTDC_LayerCfgTypeDef

#define LTDC_ACTIVE_LAYER      ((uint32_t)1) /* Layer 1 */
/**
 * @brief  LCD status structure definition
 */
#define LCD_OK                 ((uint8_t)0x00)
#define LCD_ERROR              ((uint8_t)0x01)
#define LCD_TIMEOUT            ((uint8_t)0x02)

/**
 * @brief  LCD Layer_Number
 */

/* The programmed LTDC pixel clock depends on the vertical refresh rate of the panel 60Hz => 25.16MHz and
  the LCD/SDRAM bandwidth affected by the several access on the bus and the number of used layers.
  when only one layer is enabled "LCD_MAX_PCLK" can be used and when two layers are enabled simultaneously
  or/and there is several access on the bus "LCD_MIN_PCLK" parameter is recommended */
#define LCD_MAX_PCLK       ((uint8_t)0x00)
#define LCD_MIN_PCLK       ((uint8_t)0x01)

/**
 * @brief  LCD Reload Types
 */
#define LCD_RELOAD_IMMEDIATE               ((uint32_t)LTDC_SRCR_IMR)
#define LCD_RELOAD_VERTICAL_BLANKING       ((uint32_t)LTDC_SRCR_VBR)

/**
 * @brief Definition for LCD Timer used to control the Brightnes
 */
#define LCD_TIMx                           TIM1
#define LCD_TIMx_CLK_ENABLE()              __HAL_RCC_TIM1_CLK_ENABLE()
#define LCD_TIMx_CLK_DISABLE()             __HAL_RCC_TIM1_CLK_DISABLE()
#define LCD_TIMx_CHANNEL                   TIM_CHANNEL_1
#define LCD_TIMx_CHANNEL_AF                GPIO_AF1_TIM1
#define LCD_TIMX_PERIOD_VALUE              ((uint32_t)50000) /* Period Value    */
#define LCD_TIMX_PRESCALER_VALUE           ((uint32_t)4)     /* Prescaler Value */

/**
 * @}
 */

/** @defgroup STM32H743I_EVAL_LCD_Exported_Functions LCD Exported Functions
 * @{
 */
void     BSP_LCD_Driver_Init(uint32_t fb_addr);
uint8_t  BSP_LCD_Init(void);
uint8_t  BSP_LCD_InitEx(uint32_t PclkConfig);

uint8_t  BSP_LCD_DeInit(void);
uint32_t BSP_LCD_GetXSize(void);
uint32_t BSP_LCD_GetYSize(void);
void     BSP_LCD_SetXSize(uint32_t imageWidthPixels);
void     BSP_LCD_SetYSize(uint32_t imageHeightPixels);

/* Functions using the LTDC controller */
void     BSP_LCD_LayerDefaultInit(uint16_t LayerIndex, uint32_t FrameBuffer);
void     BSP_LCD_SetTransparency(uint32_t LayerIndex, uint8_t Transparency);
void     BSP_LCD_SetTransparency_NoReload(uint32_t LayerIndex, uint8_t Transparency);
void     BSP_LCD_SetLayerAddress(uint32_t LayerIndex, uint32_t Address);
void     BSP_LCD_SetLayerAddress_NoReload(uint32_t LayerIndex, uint32_t Address);
void     BSP_LCD_SetColorKeying(uint32_t LayerIndex, uint32_t RGBValue);
void     BSP_LCD_SetColorKeying_NoReload(uint32_t LayerIndex, uint32_t RGBValue);
void     BSP_LCD_ResetColorKeying(uint32_t LayerIndex);
void     BSP_LCD_ResetColorKeying_NoReload(uint32_t LayerIndex);
void     BSP_LCD_SetLayerWindow(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void     BSP_LCD_SetLayerWindow_NoReload(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);
void     BSP_LCD_SelectLayer(uint32_t LayerIndex);
void     BSP_LCD_SetLayerVisible(uint32_t LayerIndex, FunctionalState State);
void     BSP_LCD_SetLayerVisible_NoReload(uint32_t LayerIndex, FunctionalState State);
void     BSP_LCD_Relaod(uint32_t ReloadType);

void     BSP_LCD_Clear(uint32_t Color);

void     BSP_LCD_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height);

void     BSP_LCD_DisplayOff(void);
void     BSP_LCD_DisplayOn(void);

void BSP_LCD_SetBrightness(uint8_t BrightnessValue);

/* These functions can be modified in case the current settings
  need to be changed for specific application needs */
void     BSP_LCD_ClockConfig(LTDC_HandleTypeDef *hltdc, void *Params);

void test_picture(void);
void test_picture2(void);

#ifdef __cplusplus
}
#endif

#endif /* __LCD_H */
