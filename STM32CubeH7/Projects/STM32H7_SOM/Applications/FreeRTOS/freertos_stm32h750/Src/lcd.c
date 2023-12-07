/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"
#include "stm32h7xx_hal_ltdc.h"
#include <string.h>
#include <stdlib.h>
#include "uboot_abi.h"
#include "console.h"
#include "lcd.h"
#include "lcd_test.h"

struct __packed bmp_header_s {
	/* Header */
	char		signature[2];
	uint32_t	file_size;
	uint32_t	reserved;
	uint32_t	data_offset;
        /* InfoHeader */
	uint32_t	size;
	uint32_t	width;
	uint32_t	height;
	uint16_t	planes;
	uint16_t	bit_count;
	uint32_t	compression;
	uint32_t	image_size;
	uint32_t	x_pixels_per_m;
	uint32_t	y_pixels_per_m;
	uint32_t	colors_used;
	uint32_t	colors_important;
        /* ColorTable */
} bmp_header_t;

static uint8_t bmp_img[480 * 272 * 3 + ((sizeof(bmp_header_t) + 4) & ~3)] = {0};

/** @defgroup STM32H743I_EVAL_LCD_Private_Variables LCD Private Variables
  * @{
  */
DMA2D_HandleTypeDef	hdma2d_eval = {0};
LTDC_HandleTypeDef	hltdc_eval;
static uint32_t		PCLK_profile = LCD_MAX_PCLK;

/* Timer handler declaration */
static TIM_HandleTypeDef LCD_TimHandle;

/* Default LCD configuration with LCD Layer 1 */
static uint32_t		ActiveLayer = 0;

extern void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);
static void LL_FillBuffer(uint32_t LayerIndex, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex);
static void LL_ConvertLineToARGB8888(void * pSrc, void *pDst, uint32_t xSize, uint32_t ColorMode);
static void TIMx_PWM_DeInit(TIM_HandleTypeDef *htim);
static void TIMx_PWM_Init(TIM_HandleTypeDef *htim);

/**
  * @brief  AMPIRE480272 Size
  */
#define  LCD480272_WIDTH    ((uint16_t)480)	/* LCD PIXEL WIDTH            */
#define  LCD480272_HEIGHT   ((uint16_t)272)	/* LCD PIXEL HEIGHT           */

/**
  * @brief  LCD480272 Timing
  */
#define  LCD480272_HSYNC	((uint16_t)41)	/* Horizontal synchronization */
#define  LCD480272_HBP		((uint16_t)2)	/* Horizontal back porch      */
#define  LCD480272_HFP		((uint16_t)2)	/* Horizontal front porch     */
#define  LCD480272_VSYNC	((uint16_t)10)	/* Vertical synchronization   */
#define  LCD480272_VBP		((uint16_t)2)	/* Vertical back porch        */
#define  LCD480272_VFP		((uint16_t)2)	/* Vertical front porch       */

void BSP_LCD_Driver_Init(uint32_t fb_addr)
{
	/* Initialize the LCD pixel width and pixel height */
	hltdc_eval.LayerCfg->ImageWidth  = LCD480272_WIDTH;
	hltdc_eval.LayerCfg->ImageHeight = LCD480272_HEIGHT;
	hltdc_eval.LayerCfg[ActiveLayer].FBStartAdress = fb_addr;
}

/**
  * @brief  Initializes the LCD.
  * @retval LCD state
  */
uint8_t BSP_LCD_Init(void)
{
  return (BSP_LCD_InitEx(LCD_MAX_PCLK));
}

/**
  * @brief  Initializes the LCD.
  * @param  PclkConfig : pixel clock profile
  * @retval LCD state
  */
uint8_t BSP_LCD_InitEx(uint32_t PclkConfig)
{
  PCLK_profile = PclkConfig;

  /* Timing Configuration */
  hltdc_eval.Init.HorizontalSync = (LCD480272_HSYNC - 1);
  hltdc_eval.Init.VerticalSync = (LCD480272_VSYNC - 1);
  hltdc_eval.Init.AccumulatedHBP = (LCD480272_HSYNC + LCD480272_HBP - 1);
  hltdc_eval.Init.AccumulatedVBP = (LCD480272_VSYNC + LCD480272_VBP - 1);
  hltdc_eval.Init.AccumulatedActiveH = (LCD480272_HEIGHT + LCD480272_VSYNC + LCD480272_VBP - 1);
  hltdc_eval.Init.AccumulatedActiveW = (LCD480272_WIDTH + LCD480272_HSYNC + LCD480272_HBP - 1);
  hltdc_eval.Init.TotalHeigh = (LCD480272_HEIGHT + LCD480272_VSYNC + LCD480272_VBP + LCD480272_VFP - 1);
  hltdc_eval.Init.TotalWidth = (LCD480272_WIDTH + LCD480272_HSYNC + LCD480272_HBP + LCD480272_HFP - 1);

  /* Initialize the LCD pixel width and pixel height */
  hltdc_eval.LayerCfg->ImageWidth  = LCD480272_WIDTH;
  hltdc_eval.LayerCfg->ImageHeight = LCD480272_HEIGHT;

  /* Background value */
  hltdc_eval.Init.Backcolor.Blue = 0;
  hltdc_eval.Init.Backcolor.Green = 0;
  hltdc_eval.Init.Backcolor.Red = 0;

  /* Polarity */
  hltdc_eval.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc_eval.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc_eval.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc_eval.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc_eval.Instance = LTDC;

  if(HAL_LTDC_GetState(&hltdc_eval) == HAL_LTDC_STATE_RESET)
  {
    /* Initialize the LCD Msp: this __weak function can be rewritten by the application */
    HAL_LTDC_MspInit(&hltdc_eval);
  }
  HAL_LTDC_Init(&hltdc_eval);

  /* Initialize TIM in PWM mode to control brightness */
  TIMx_PWM_Init(&LCD_TimHandle);

  return LCD_OK;
}

/**
  * @brief  DeInitializes the LCD.
  * @retval LCD state
  */
uint8_t BSP_LCD_DeInit(void)
{
  /* Initialize the hltdc_eval Instance parameter */
  hltdc_eval.Instance = LTDC;

 /* Disable LTDC block */
  __HAL_LTDC_DISABLE(&hltdc_eval);

  /* DeInit the LTDC */
  HAL_LTDC_DeInit(&hltdc_eval);

  /* DeInit the LTDC MSP : this __weak function can be rewritten by the application */
  HAL_LTDC_MspDeInit(&hltdc_eval);

  /* DeInit TIM PWM */
  TIMx_PWM_DeInit(&LCD_TimHandle);

  return LCD_OK;
}

/**
  * @brief  Gets the LCD X size.
  * @retval Used LCD X size
  */
uint32_t BSP_LCD_GetXSize(void)
{
  return hltdc_eval.LayerCfg[ActiveLayer].ImageWidth;
}

/**
  * @brief  Gets the LCD Y size.
  * @retval Used LCD Y size
  */
uint32_t BSP_LCD_GetYSize(void)
{
  return hltdc_eval.LayerCfg[ActiveLayer].ImageHeight;
}

/**
  * @brief  Set the LCD X size.
  * @param  imageWidthPixels : image width in pixels unit
  * @retval None
  */
void BSP_LCD_SetXSize(uint32_t imageWidthPixels)
{
  hltdc_eval.LayerCfg[ActiveLayer].ImageWidth = imageWidthPixels;
}

/**
  * @brief  Set the LCD Y size.
  * @param  imageHeightPixels : image height in lines unit
  * @retval None
  */
void BSP_LCD_SetYSize(uint32_t imageHeightPixels)
{
  hltdc_eval.LayerCfg[ActiveLayer].ImageHeight = imageHeightPixels;
}

/**
  * @brief  Initializes the LCD layers.
  * @param  LayerIndex: Layer foreground or background
  * @param  FB_Address: Layer frame buffer
  * @retval None
  */
void BSP_LCD_LayerDefaultInit(uint16_t LayerIndex, uint32_t FB_Address)
{
  LCD_LayerCfgTypeDef  layer_cfg;

  /* Layer Init */
  layer_cfg.WindowX0 = 0;
  layer_cfg.WindowX1 = BSP_LCD_GetXSize();
  layer_cfg.WindowY0 = 0;
  layer_cfg.WindowY1 = BSP_LCD_GetYSize();
  layer_cfg.PixelFormat = LTDC_PIXEL_FORMAT_ARGB8888;
  layer_cfg.FBStartAdress = FB_Address;
  layer_cfg.Alpha = 255;
  layer_cfg.Alpha0 = 0;
  layer_cfg.Backcolor.Blue = 0;
  layer_cfg.Backcolor.Green = 0;
  layer_cfg.Backcolor.Red = 0;
  layer_cfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_PAxCA;
  layer_cfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_PAxCA;
  layer_cfg.ImageWidth = BSP_LCD_GetXSize();
  layer_cfg.ImageHeight = BSP_LCD_GetYSize();

  HAL_LTDC_ConfigLayer(&hltdc_eval, &layer_cfg, LayerIndex);
}

/**
  * @brief  Selects the LCD Layer.
  * @param  LayerIndex: Layer foreground or background
  * @retval None
  */
void BSP_LCD_SelectLayer(uint32_t LayerIndex)
{
  ActiveLayer = LayerIndex;
}

/**
  * @brief  Sets an LCD Layer visible
  * @param  LayerIndex: Visible Layer
  * @param  State: New state of the specified layer
  *          This parameter can be one of the following values:
  *            @arg  ENABLE
  *            @arg  DISABLE
  * @retval None
  */
void BSP_LCD_SetLayerVisible(uint32_t LayerIndex, FunctionalState State)
{
  if(State == ENABLE)
  {
    __HAL_LTDC_LAYER_ENABLE(&hltdc_eval, LayerIndex);
  }
  else
  {
    __HAL_LTDC_LAYER_DISABLE(&hltdc_eval, LayerIndex);
  }
  __HAL_LTDC_RELOAD_IMMEDIATE_CONFIG(&(hltdc_eval));
}

/**
  * @brief  Sets an LCD Layer visible without reloading.
  * @param  LayerIndex: Visible Layer
  * @param  State: New state of the specified layer
  *          This parameter can be one of the following values:
  *            @arg  ENABLE
  *            @arg  DISABLE
  * @retval None
  */
void BSP_LCD_SetLayerVisible_NoReload(uint32_t LayerIndex, FunctionalState State)
{
  if(State == ENABLE)
  {
    __HAL_LTDC_LAYER_ENABLE(&hltdc_eval, LayerIndex);
  }
  else
  {
    __HAL_LTDC_LAYER_DISABLE(&hltdc_eval, LayerIndex);
  }
  /* Do not Sets the Reload  */
}

/**
  * @brief  Configures the transparency.
  * @param  LayerIndex: Layer foreground or background.
  * @param  Transparency: Transparency
  *           This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF
  * @retval None
  */
void BSP_LCD_SetTransparency(uint32_t LayerIndex, uint8_t Transparency)
{
  HAL_LTDC_SetAlpha(&hltdc_eval, Transparency, LayerIndex);
}

/**
  * @brief  Configures the transparency without reloading.
  * @param  LayerIndex: Layer foreground or background.
  * @param  Transparency: Transparency
  *           This parameter must be a number between Min_Data = 0x00 and Max_Data = 0xFF
  * @retval None
  */
void BSP_LCD_SetTransparency_NoReload(uint32_t LayerIndex, uint8_t Transparency)
{
  HAL_LTDC_SetAlpha_NoReload(&hltdc_eval, Transparency, LayerIndex);
}

/**
  * @brief  Sets an LCD layer frame buffer address.
  * @param  LayerIndex: Layer foreground or background
  * @param  Address: New LCD frame buffer value
  * @retval None
  */
void BSP_LCD_SetLayerAddress(uint32_t LayerIndex, uint32_t Address)
{
  HAL_LTDC_SetAddress(&hltdc_eval, Address, LayerIndex);
}

/**
  * @brief  Sets an LCD layer frame buffer address without reloading.
  * @param  LayerIndex: Layer foreground or background
  * @param  Address: New LCD frame buffer value
  * @retval None
  */
void BSP_LCD_SetLayerAddress_NoReload(uint32_t LayerIndex, uint32_t Address)
{
  HAL_LTDC_SetAddress_NoReload(&hltdc_eval, Address, LayerIndex);
}

/**
  * @brief  Sets display window.
  * @param  LayerIndex: Layer index
  * @param  Xpos: LCD X position
  * @param  Ypos: LCD Y position
  * @param  Width: LCD window width
  * @param  Height: LCD window height
  * @retval None
  */
void BSP_LCD_SetLayerWindow(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  /* Reconfigure the layer size */
  HAL_LTDC_SetWindowSize(&hltdc_eval, Width, Height, LayerIndex);

  /* Reconfigure the layer position */
  HAL_LTDC_SetWindowPosition(&hltdc_eval, Xpos, Ypos, LayerIndex);
}

/**
  * @brief  Sets display window without reloading.
  * @param  LayerIndex: Layer index
  * @param  Xpos: LCD X position
  * @param  Ypos: LCD Y position
  * @param  Width: LCD window width
  * @param  Height: LCD window height
  * @retval None
  */
void BSP_LCD_SetLayerWindow_NoReload(uint16_t LayerIndex, uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  /* Reconfigure the layer size */
  HAL_LTDC_SetWindowSize_NoReload(&hltdc_eval, Width, Height, LayerIndex);

  /* Reconfigure the layer position */
  HAL_LTDC_SetWindowPosition_NoReload(&hltdc_eval, Xpos, Ypos, LayerIndex);
}

/**
  * @brief  Configures and sets the color keying.
  * @param  LayerIndex: Layer foreground or background
  * @param  RGBValue: Color reference
  * @retval None
  */
void BSP_LCD_SetColorKeying(uint32_t LayerIndex, uint32_t RGBValue)
{
  /* Configure and Enable the color Keying for LCD Layer */
  HAL_LTDC_ConfigColorKeying(&hltdc_eval, RGBValue, LayerIndex);
  HAL_LTDC_EnableColorKeying(&hltdc_eval, LayerIndex);
}

/**
  * @brief  Configures and sets the color keying without reloading.
  * @param  LayerIndex: Layer foreground or background
  * @param  RGBValue: Color reference
  * @retval None
  */
void BSP_LCD_SetColorKeying_NoReload(uint32_t LayerIndex, uint32_t RGBValue)
{
  /* Configure and Enable the color Keying for LCD Layer */
  HAL_LTDC_ConfigColorKeying_NoReload(&hltdc_eval, RGBValue, LayerIndex);
  HAL_LTDC_EnableColorKeying_NoReload(&hltdc_eval, LayerIndex);
}

/**
  * @brief  Disables the color keying.
  * @param  LayerIndex: Layer foreground or background
  * @retval None
  */
void BSP_LCD_ResetColorKeying(uint32_t LayerIndex)
{
  /* Disable the color Keying for LCD Layer */
  HAL_LTDC_DisableColorKeying(&hltdc_eval, LayerIndex);
}

/**
  * @brief  Disables the color keying without reloading.
  * @param  LayerIndex: Layer foreground or background
  * @retval None
  */
void BSP_LCD_ResetColorKeying_NoReload(uint32_t LayerIndex)
{
  /* Disable the color Keying for LCD Layer */
  HAL_LTDC_DisableColorKeying_NoReload(&hltdc_eval, LayerIndex);
}

/**
  * @brief  Disables the color keying without reloading.
  * @param  ReloadType: can be one of the following values
  *         - LCD_RELOAD_IMMEDIATE
  *         - LCD_RELOAD_VERTICAL_BLANKING
  * @retval None
  */
void BSP_LCD_Relaod(uint32_t ReloadType)
{
  HAL_LTDC_Reload (&hltdc_eval, ReloadType);
}

/**
  * @brief  Clears the hole LCD.
  * @param  Color: Color of the background
  * @retval None
  */
void BSP_LCD_Clear(uint32_t Color)
{
  /* Clear the LCD */
  LL_FillBuffer(ActiveLayer, (uint32_t *)(hltdc_eval.LayerCfg[ActiveLayer].FBStartAdress), BSP_LCD_GetXSize(), BSP_LCD_GetYSize(), 0, Color);
}

/**
  * @brief  Draws a bitmap picture loaded in the internal Flash (32 bpp).
  * @param  Xpos: Bmp X position in the LCD
  * @param  Ypos: Bmp Y position in the LCD
  * @param  pbmp: Pointer to Bmp picture address in the internal Flash
  * @retval None
  */
void BSP_LCD_DrawBitmap(uint32_t Xpos, uint32_t Ypos, uint8_t *pbmp)
{
  uint32_t index = 0, width = 0, height = 0, bit_pixel = 0;
  uint32_t address;
  uint32_t input_color_mode = 0;

  /* Get bitmap data address offset */
  index = pbmp[10] + (pbmp[11] << 8) + (pbmp[12] << 16)  + (pbmp[13] << 24);

  /* Read bitmap width */
  width = pbmp[18] + (pbmp[19] << 8) + (pbmp[20] << 16)  + (pbmp[21] << 24);

  /* Read bitmap height */
  height = pbmp[22] + (pbmp[23] << 8) + (pbmp[24] << 16)  + (pbmp[25] << 24);

  /* Read bit/pixel */
  bit_pixel = pbmp[28] + (pbmp[29] << 8);

  /* Set the address */
  address = hltdc_eval.LayerCfg[ActiveLayer].FBStartAdress + (((BSP_LCD_GetXSize()*Ypos) + Xpos)*(4));

  /* Get the layer pixel format */
  if ((bit_pixel/8) == 4)
  {
    input_color_mode = DMA2D_INPUT_ARGB8888;
  }
  else if ((bit_pixel/8) == 2)
  {
    input_color_mode = DMA2D_INPUT_RGB565;
  }
  else
  {
    input_color_mode = DMA2D_INPUT_RGB888;
  }

  /* Bypass the bitmap header */
  pbmp += (index + (width * (height - 1) * (bit_pixel/8)));

  /* Convert picture to ARGB8888 pixel format */
  for(index=0; index < height; index++)
  {
    /* Pixel format conversion */
    LL_ConvertLineToARGB8888((uint32_t *)pbmp, (uint32_t *)address, width, input_color_mode);

    /* Increment the source and destination buffers */
    address += (BSP_LCD_GetXSize()*4);
    pbmp -= width*(bit_pixel/8);
  }
}

/**
  * @brief  Enables the display.
  * @retval None
  */
void BSP_LCD_DisplayOn(void)
{
  /* Display On */
  __HAL_LTDC_ENABLE(&hltdc_eval);
}

/**
  * @brief  Disables the display.
  * @retval None
  */
void BSP_LCD_DisplayOff(void)
{
  /* Display Off */
  __HAL_LTDC_DISABLE(&hltdc_eval);
}

/**
  * @brief  DeInitializes BSP_LCD MSP.
  * @param  hltdc: LTDC handle
  * @param  Params: Pointer to void
  * @retval None
  */
__weak void BSP_LCD_MspDeInit(LTDC_HandleTypeDef *hltdc, void *Params)
{
  GPIO_InitTypeDef  gpio_init_structure;

  /* Disable LTDC block */
  __HAL_LTDC_DISABLE(hltdc);

  /* LTDC Pins deactivation */
  /* GPIOI deactivation */
  gpio_init_structure.Pin       = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_DeInit(GPIOI, gpio_init_structure.Pin);
  /* GPIOJ deactivation */
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                                  GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | \
                                  GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | \
                                  GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_DeInit(GPIOJ, gpio_init_structure.Pin);
  /* GPIOK deactivation */
  gpio_init_structure.Pin       = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
                                  GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7;
  HAL_GPIO_DeInit(GPIOK, gpio_init_structure.Pin);

  /* Disable LTDC clock */
  __HAL_RCC_LTDC_CLK_DISABLE();

  /* GPIO pins clock can be shut down in the application
     by surcharging this __weak function */
}

/**
  * @brief  Set the brightness value
  * @param  BrightnessValue: [00: Min (black), 100 Max]
  */
void BSP_LCD_SetBrightness(uint8_t BrightnessValue)
{
  /* Timer Configuration */
  TIM_OC_InitTypeDef LCD_TIM_Config;

  /* Stop PWM Timer channel */
  HAL_TIM_PWM_Stop(&LCD_TimHandle, LCD_TIMx_CHANNEL);

  /* Common configuration for all channels */
  LCD_TIM_Config.OCMode       = TIM_OCMODE_PWM1;
  LCD_TIM_Config.OCPolarity   = TIM_OCPOLARITY_HIGH;
  LCD_TIM_Config.OCFastMode   = TIM_OCFAST_DISABLE;
  LCD_TIM_Config.OCNPolarity  = TIM_OCNPOLARITY_HIGH;
  LCD_TIM_Config.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  LCD_TIM_Config.OCIdleState  = TIM_OCIDLESTATE_RESET;

  /* Set the pulse value for channel */
  LCD_TIM_Config.Pulse =  (uint32_t)((LCD_TIMX_PERIOD_VALUE * BrightnessValue) / 100);

  HAL_TIM_PWM_ConfigChannel(&LCD_TimHandle, &LCD_TIM_Config, LCD_TIMx_CHANNEL);

  /* Start PWM Timer channel */
  HAL_TIM_PWM_Start(&LCD_TimHandle, LCD_TIMx_CHANNEL);
}

/**
  * @brief  Fills a buffer.
  * @param  LayerIndex: Layer index
  * @param  pDst: Pointer to destination buffer
  * @param  xSize: Buffer width
  * @param  ySize: Buffer height
  * @param  OffLine: Offset
  * @param  ColorIndex: Color index
  * @retval None
  */
static void LL_FillBuffer(uint32_t LayerIndex, void *pDst, uint32_t xSize, uint32_t ySize, uint32_t OffLine, uint32_t ColorIndex)
{
  /* Register to memory mode with ARGB8888 as color Mode */
  hdma2d_eval.Init.Mode         = DMA2D_R2M;
  hdma2d_eval.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
  hdma2d_eval.Init.OutputOffset = OffLine;

  hdma2d_eval.Instance = DMA2D;

  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&hdma2d_eval) == HAL_OK)
  {
    if(HAL_DMA2D_ConfigLayer(&hdma2d_eval, 1) == HAL_OK)
    {
      if (HAL_DMA2D_Start(&hdma2d_eval, ColorIndex, (uint32_t)pDst, xSize, ySize) == HAL_OK)
      {
        /* Polling For DMA transfer */
        HAL_DMA2D_PollForTransfer(&hdma2d_eval, 25);
      }
    }
  }
}

/**
  * @brief  Converts a line to an ARGB8888 pixel format.
  * @param  pSrc: Pointer to source buffer
  * @param  pDst: Output color
  * @param  xSize: Buffer width
  * @param  ColorMode: Input color mode
  * @retval None
  */
static void LL_ConvertLineToARGB8888(void *pSrc, void *pDst, uint32_t xSize, uint32_t ColorMode)
{
  /* Configure the DMA2D Mode, Color Mode and output offset */
  hdma2d_eval.Init.Mode         = DMA2D_M2M_PFC;
  hdma2d_eval.Init.ColorMode    = DMA2D_OUTPUT_ARGB8888;
  hdma2d_eval.Init.OutputOffset = 0;

  /* Foreground Configuration */
  hdma2d_eval.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d_eval.LayerCfg[1].InputAlpha = 0xFF;
  hdma2d_eval.LayerCfg[1].InputColorMode = ColorMode;
  hdma2d_eval.LayerCfg[1].InputOffset = 0;

  hdma2d_eval.Instance = DMA2D;

  /* DMA2D Initialization */
  if(HAL_DMA2D_Init(&hdma2d_eval) == HAL_OK)
  {
    if(HAL_DMA2D_ConfigLayer(&hdma2d_eval, 1) == HAL_OK)
    {
      if (HAL_DMA2D_Start(&hdma2d_eval, (uint32_t)pSrc, (uint32_t)pDst, xSize, 1) == HAL_OK)
      {
        /* Polling For DMA transfer */
        HAL_DMA2D_PollForTransfer(&hdma2d_eval, 25);
      }
    }
  }
}

/**
  * @brief  Initializes TIM in PWM mode
  * @param  htim: TIM handle
  * @retval None
  */
static void TIMx_PWM_Init(TIM_HandleTypeDef *htim)
{
  /* Timer_Clock = 2 x  APB2_clock = 200 MHz */
  /* PWM_freq = Timer_Clock /(Period x (Prescaler + 1))*/
  /* PWM_freq = 200 MHz /(50000 x 5) = 800 Hz*/

  htim->Instance = LCD_TIMx;
  HAL_TIM_PWM_DeInit(htim);

  htim->Init.Prescaler         = LCD_TIMX_PRESCALER_VALUE;
  htim->Init.Period            = LCD_TIMX_PERIOD_VALUE;
  htim->Init.ClockDivision     = 0;
  htim->Init.CounterMode       = TIM_COUNTERMODE_UP;
  htim->Init.RepetitionCounter = 0;

  HAL_TIM_PWM_Init(htim);
  HAL_TIM_MspPostInit(htim);
}

/**
  * @brief  De-Initializes TIM in PWM mode
  * @param  htim: TIM handle
  * @retval None
  */
static void TIMx_PWM_DeInit(TIM_HandleTypeDef *htim)
{
  htim->Instance = LCD_TIMx;
  /* Timer de-intialization */
  HAL_TIM_PWM_DeInit(htim);
}

static void fb_putpixel(void *buf, int x, int y, uint32_t color)
{
	if (x < 0 || y < 0 || x >= BSP_LCD_GetXSize() || y >= BSP_LCD_GetYSize())
		return;

	/*
	 * We set the bits 24..31 to 0xFF which represents opacity in
	 * ARGB8888 to make the image 100% opaque. This does not break
	 * compatibility with platforms that do not support alpha
	 * component (e.g. Kinetis K61/K70) because their LCD
	 * controllers do not look in this fourth byte.
	 */
	((uint32_t *)buf)[y * BSP_LCD_GetXSize() + x] = 0xFF000000 | color;
}

#define min(x, y) ((x) > (y) ? (y) : (x))
#define max(x, y) ((x) < (y) ? (y) : (x))

/*
 * Draw color stripes, solid and dashed lines and a grayscale
 */
void test_picture(void)
{
	void *buf;
	int x, y;
	int scale;
	/* Brightness */
	uint8_t br;

	/*
	 * Colors of vertical stripes
	 */
	const uint32_t colors[] = {
		0xFF0000,
		0xFF8000,
		0xFFFF00,
		0x0000FF,
		0x008000,
		0x800080,
		0xFFFFFF,
		0x000000};
	const int num_colors = sizeof(colors) / sizeof(colors[0]);

	/*
	 * Allocate a pixmap buffer to speed up drawing
	 */
	buf = (void *)hltdc_eval.LayerCfg[ActiveLayer].FBStartAdress;

	/*
	 * Draw color stripes
	 */
	for (y = 0; y < BSP_LCD_GetYSize(); y++)
		for (x = 0; x < BSP_LCD_GetXSize(); x++)
			fb_putpixel(buf, x, y,
				colors[(x * num_colors) / BSP_LCD_GetXSize()]);

	/*
	 * Diagonal line
	 */
	for (x = 0; x < min(BSP_LCD_GetXSize(), BSP_LCD_GetYSize()); x++)
		fb_putpixel(buf, x, x, 0xFFFFFF);	/* White */

	/*
	 * Left and right borders
	 */
	for (y = 0; y < BSP_LCD_GetYSize(); y++) {
		fb_putpixel(buf, 0, y, 0xFFFFFF);
		fb_putpixel(buf, BSP_LCD_GetXSize() - 1, y, 0xFFFFFF);
	}

	/*
	 * Horizontal lines
	 */
	for (x = 0; x < BSP_LCD_GetXSize(); x++) {
		/* Top border (white with green dots) */
		fb_putpixel(buf, x, 0, (x & 3) ? 0xFFFFFF : 0x00B000);
		/* Horizontal line in the middle (white with green dots) */
		fb_putpixel(buf, x, BSP_LCD_GetYSize() / 2,
			(x & 7) ? 0xFFFFFF : 0x00B000);
		/* Bottom border */
		fb_putpixel(buf, x, BSP_LCD_GetYSize() - 1, 0xFFFFFF);
	}

	/*
	 * Grayscale
	 */
	scale = 256 / max(BSP_LCD_GetXSize() - 50, 0) + 1;
	br = 0;
	for (x = BSP_LCD_GetXSize() - 5 - 256 / scale; x < BSP_LCD_GetXSize() - 5; x++, br++)
		for (y = BSP_LCD_GetYSize() / 6; y < BSP_LCD_GetYSize() / 3; y++)
			fb_putpixel(buf, x, y, br | (br << 8) | (br << 16));
}

void test_picture2(void)
{
	if (*bmp_img == 0) {
		ubi_volume_read("splash", bmp_img, sizeof(bmp_img));
	}
	BSP_LCD_DrawBitmap(0, 0, bmp_img);
}

BaseType_t cmd_lcdtest(char *wBuf, size_t wBufferLen, const char *pcCommandString)
{
	test_picture();

	sprintf(wBuf, "%s", "Check picture on LCD");
	return pdFALSE;
}

