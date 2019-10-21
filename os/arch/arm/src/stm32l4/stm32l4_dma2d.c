/****************************************************************************
 * arch/arm/src/stm32l4/stm32l4_dma2d.c
 *
 *   Copyright (C) 2014-2015, 2018 Marco Krahl. All rights reserved.
 *   Author: Marco Krahl <ocram.lhark@gmail.com>
 *
 * References:
 *   STM32F429 Technical Reference Manual
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <tinyara/config.h>

#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <debug.h>
#include <semaphore.h>

#include <tinyara/irq.h>
#include <tinyara/kmalloc.h>
#include <tinyara/semaphore.h>
#include <tinyara/video/fb.h>

#include <arch/board/board.h>

#include "up_arch.h"
#include "up_internal.h"
#include "stm32l4.h"
#include "stm32l4xx_hal_interface.h"
#include "chip/stm32l4_ltdc.h"
#include "chip/stm32l4_dma2d.h"
#include "stm32l4xx_hal_dma2d.h"
#include "stm32l4_dma2d.h"
#include "stm32l4_ltdc.h"
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* DMA2D supported operation layer (output, foreground, background) */

#define DMA2D_NLAYERS                       3

/* Only 8 bit per pixel overal supported */

#define DMA2D_PF_BYPP(n)                    ((n) / 8)

/* CC clut size */

#define DMA2D_CLUT_SIZE                     STM32_DMA2D_NCLUT - 1

/* Layer argb cmap conversion */

#define DMA2D_CLUT_ALPHA(n)                 ((uint32_t)(n) << 24)
#define DMA2D_CLUT_RED(n)                   ((uint32_t)(n) << 16)
#define DMA2D_CLUT_GREEN(n)                 ((uint32_t)(n) << 8)
#define DMA2D_CLUT_BLUE(n)                  ((uint32_t)(n) << 0)

#define DMA2D_CMAP_ALPHA(n)                 ((uint32_t)(n) >> 24)
#define DMA2D_CMAP_RED(n)                   ((uint32_t)(n) >> 16)
#define DMA2D_CMAP_GREEN(n)                 ((uint32_t)(n) >> 8)
#define DMA2D_CMAP_BLUE(n)                  ((uint32_t)(n) >> 0)

/* Debug option */

#ifdef CONFIG_STM32L4_DMA2D_REGDEBUG
#  define regerr                            gdbg
#  define reginfo                           gdbg
#else
#  define regerr(x...)
#  define reginfo(x...)
#endif

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* DMA2D General layer information */

struct stm32_dma2d_s
{
  struct dma2d_layer_s dma2d;  /* Public dma2d interface */

#ifdef CONFIG_STM32L4_FB_CMAP
  uint32_t *clut;              /* Color lookup table */
#endif

  sem_t   *lock;               /* Ensure mutually exclusive access */
};

/* Interrupt handling */

struct stm32_interrupt_s
{
  int    irq;       /* irq number */
  int  error;       /* Interrupt error */
  sem_t *sem;       /* Semaphore for waiting for irq */
};

/* This enumeration foreground and background layer supported by the dma2d
 * controller
 */

enum stm32_layer_e
{
  DMA2D_LAYER_LFORE = 0,       /* Foreground Layer */
  DMA2D_LAYER_LBACK,           /* Background Layer */
  DMA2D_LAYER_LOUT,            /* Output Layer */
};

/* DMA2D memory address register */

static const uintptr_t stm32_mar_layer_t[DMA2D_NLAYERS] =
{
  STM32_DMA2D_FGMAR,
  STM32_DMA2D_BGMAR,
  STM32_DMA2D_OMAR
};

/* DMA2D offset register */

static const uintptr_t stm32_or_layer_t[DMA2D_NLAYERS] =
{
  STM32_DMA2D_FGOR,
  STM32_DMA2D_BGOR,
  STM32_DMA2D_OOR
};

/* DMA2D pfc control register */

static const uintptr_t stm32_pfccr_layer_t[DMA2D_NLAYERS] =
{
  STM32_DMA2D_FGPFCCR,
  STM32_DMA2D_BGPFCCR,
  STM32_DMA2D_OPFCCR
};

/* DMA2D color register */

static const uintptr_t stm32_color_layer_t[DMA2D_NLAYERS] =
{
  STM32_DMA2D_FGCOLR,
  STM32_DMA2D_BGCOLR,
  STM32_DMA2D_OCOLR
};

/* DMA2D clut memory address register */

#ifdef CONFIG_STM32L4_FB_CMAP
static const uintptr_t stm32_cmar_layer_t[DMA2D_NLAYERS - 1] =
{
  STM32_DMA2D_FGCMAR,
  STM32_DMA2D_BGCMAR
};
#endif

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* Private functions */

static void stm32_dma2d_control(uint32_t setbits, uint32_t clrbits);
static int stm32_dma2dirq(int irq, void *context, FAR void *arg);
static int stm32_dma2d_waitforirq(void);
static int stm32_dma2d_start(void);
#ifdef CONFIG_STM32L4_FB_CMAP
static int stm32_dma2d_loadclut(uintptr_t reg);
#endif
static uint32_t stm32_dma2d_memaddress(FAR struct stm32_dma2d_overlay_s *oinfo,
                                       uint32_t xpos, uint32_t ypos);
static uint32_t stm32_dma2d_lineoffset(FAR struct stm32_dma2d_overlay_s *oinfo,
                                       FAR const struct fb_area_s *area);
static void stm32_dma2d_lfifo(FAR struct stm32_dma2d_overlay_s *oinfo, int lid,
                              uint32_t xpos, uint32_t ypos,
                              FAR const struct fb_area_s *area);
static void stm32_dma2d_lcolor(int lid, uint32_t argb);
static void stm32_dma2d_llnr(FAR const struct fb_area_s *area);
static int stm32_dma2d_loutpfc(uint8_t fmt);
static void stm32_dma2d_lpfc(int lid, uint32_t blendmode, uint8_t alpha,
                             uint8_t fmt);

/* Public functions */

#ifdef CONFIG_STM32L4_FB_CMAP
static int stm32_dma2d_setclut(FAR const struct fb_cmap_s *cmap);
#endif
static int stm32_dma2d_fillcolor(FAR struct stm32_dma2d_overlay_s *oinfo,
                                 FAR const struct fb_area_s *area,
                                 uint32_t argb);
static int stm32_dma2d_blit(FAR struct stm32_dma2d_overlay_s *doverlay,
                            uint32_t destxpos, uint32_t destypos,
                            FAR struct stm32_dma2d_overlay_s *soverlay,
                            FAR const struct fb_area_s *sarea);
static int stm32_dma2d_blend(FAR struct stm32_dma2d_overlay_s *doverlay,
                             uint32_t destxpos, uint32_t destypos,
                             FAR struct stm32_dma2d_overlay_s *foverlay,
                             uint32_t forexpos, uint32_t foreypos,
                             FAR struct stm32_dma2d_overlay_s *boverlay,
                             FAR const struct fb_area_s *barea);
#ifdef USE_HAL_DRIVER
static void STM32_DMA2D_CopyBuffer(uint32_t *psrc, uint32_t *pdst, uint16_t x, uint16_t y,
                            uint16_t xsize, uint16_t ysize);
static void STM32_DMA2D_SetLayer_InputColor(uint32_t pf);
static HAL_StatusTypeDef HAL_DMA2D_InitConfig(DMA2D_HandleTypeDef *hdma2d);
#endif
/****************************************************************************
 * Private Data
 ****************************************************************************/

/* The initialized state of the driver */

static bool g_initialized;

/* Allocate clut */

#ifdef CONFIG_STM32L4_FB_CMAP
static uint32_t g_clut[STM32_DMA2D_NCLUT *
#  ifdef CONFIG_STM32L4_FB_TRANSPARENCY
                      4
#  else
                      3
#  endif
                      / 4 ];
#endif /* CONFIG_STM32L4_FB_CMAP */

/* The DMA2D semaphore that enforces mutually exclusive access */

static sem_t g_lock;

/* Semaphore for interrupt handling */

static sem_t g_semirq;

/* This structure provides irq handling */

static struct stm32_interrupt_s g_interrupt =
{
  .irq     = STM32L4_IRQ_DMA2D,
  .error   = OK,
  .sem     = &g_semirq
};


static struct stm32_dma2d_s g_dma2ddev =
{
  .dma2d =
  {
#ifdef CONFIG_STM32L4_FB_CMAP
    .setclut     = stm32_dma2d_setclut,
#endif
    .fillcolor   = stm32_dma2d_fillcolor,
    .blit        = stm32_dma2d_blit,
    .blend       = stm32_dma2d_blend,
#ifdef USE_HAL_DRIVER
    .copybuffer  = STM32_DMA2D_CopyBuffer,
    .inputcolor  = STM32_DMA2D_SetLayer_InputColor
#endif
  },
#ifdef CONFIG_STM32L4_FB_CMAP
  .clut = g_clut,
#endif
  .lock = &g_lock
};

/****************************************************************************
 * Public Data
 ****************************************************************************/
#ifdef USE_HAL_DRIVER
DMA2D_HandleTypeDef   hdma2d;
#endif
/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_dma2d_control
 *
 * Description:
 *   Change the DMA2D control register
 *
 * Input Parameters:
 *   setbits - The bits to set
 *   clrbits - The bits to clear
 *
 ****************************************************************************/

static void stm32_dma2d_control(uint32_t setbits, uint32_t clrbits)
{
    return 0;
}

/****************************************************************************
 * Name: stm32_dma2dirq
 *
 * Description:
 *   DMA2D interrupt handler
 *
 ****************************************************************************/

static int stm32_dma2dirq(int irq, void *context, FAR void *arg)
{
    return 0;
}

/****************************************************************************
 * Name: stm32_dma2d_waitforirq
 *
 * Description:
 *   Helper waits until the dma2d irq occurs. That means that an ongoing clut
 *   loading or dma transfer was completed.
 *   Note! The caller must use this function within a critical section.
 *
 * Returned Value:
 *   On success OK otherwise ERROR
 *
 ****************************************************************************/

static int stm32_dma2d_waitforirq(void)
{
    return 0;
}

/****************************************************************************
 * Name: stm32_dma2d_loadclut
 *
 * Description:
 *   Starts clut loading but doesn't wait until loading is complete!
 *
 * Input Parameters:
 *   pfcreg - PFC control Register
 *
 * Returned Value:
 *   On success - OK
 *   On error - -EINVAL
 *
 ****************************************************************************/

#ifdef CONFIG_STM32L4_DMA2D_L8
static int stm32_dma2d_loadclut(uintptr_t pfcreg)
{
    return 0;
}
#endif

/****************************************************************************
 * Name: stm32_dma2d_start
 *
 * Description:
 *   Starts the dma transfer and waits until completed.
 *
 * Input Parameters:
 *   reg       - Register to set the start
 *   startflag - The related flag to start the dma transfer
 *   irqflag   - The interrupt enable flag in the DMA2D_CR register
 *
 ****************************************************************************/

static int stm32_dma2d_start(void)
{
    return 0;
}

/****************************************************************************
 * Name: stm32_dma2d_memaddress
 *
 * Description:
 *   Helper to calculate the layer memory address
 *
 * Input Parameters:
 *   oinfo - Reference to overlay information
 *   xpos  - x-Offset
 *   ypos  - y-Offset
 *
 * Returned Value:
 *   memory address
 *
 ****************************************************************************/

static uint32_t stm32_dma2d_memaddress(FAR struct stm32_dma2d_overlay_s *oinfo,
                                       uint32_t xpos, uint32_t ypos)
{
    return 0;
}

/****************************************************************************
 * Name: stm32_dma2d_lineoffset
 *
 * Description:
 *   Helper to calculate the layer line offset
 *
 * Input Parameters:
 *   oinfo - Reference to overlay information
 *
 * Returned Value:
 *   line offset
 *
 ****************************************************************************/

static uint32_t stm32_dma2d_lineoffset(FAR struct stm32_dma2d_overlay_s *oinfo,
                                       FAR const struct fb_area_s *area)
{
    return 0;
}

/****************************************************************************
 * Name: stm32_dma2d_lfifo
 *
 * Description:
 *   Set the fifo for the foreground, background and output layer
 *   Configures the memory address register
 *   Configures the line offset register
 *
 * Input Parameters:
 *   layer - Reference to the common layer state structure
 *
 ****************************************************************************/

static void stm32_dma2d_lfifo(FAR struct stm32_dma2d_overlay_s *oinfo,
                              int lid, uint32_t xpos, uint32_t ypos,
                              FAR const struct fb_area_s *area)
{

}

/****************************************************************************
 * Name: stm32_dma2d_lcolor
 *
 * Description:
 *   Set the color for the layer
 *
 * Input Parameters:
 *   lid  - Layer type (output, foreground, background)
 *   argb - argb8888 color
 *
 ****************************************************************************/

static void stm32_dma2d_lcolor(int lid, uint32_t argb)
{
  gdbg("lid=%d, argb=%08x\n", lid, argb);
  putreg32(argb, stm32_color_layer_t[lid]);
}

/****************************************************************************
 * Name: stm32_dma2d_llnr
 *
 * Description:
 *   Set the number of line register
 *
 * Input Parameters:
 *   area - Reference to area information
 *
 ****************************************************************************/

static void stm32_dma2d_llnr(FAR const struct fb_area_s *area)
{

}

/****************************************************************************
 * Name: stm32_dma2d_loutpfc
 *
 * Description:
 *   Set the output PFC control register
 *
 * Input Parameters:
 *   fmt - DMA2D pixel format
 *
 ****************************************************************************/

static int stm32_dma2d_loutpfc(uint8_t fmt)
{
  return 0;
}

/****************************************************************************
 * Name: stm32_dma2d_lpfc
 *
 * Description:
 *   Configure foreground and background layer PFC control register
 *
 * Input Parameters:
 *   lid       - Layer id (output, foreground, background)
 *   blendmode - Layer blendmode (dma2d register values)
 *   alpha     - Transparency
 *
 ****************************************************************************/

static void stm32_dma2d_lpfc(int lid, uint32_t blendmode, uint8_t alpha,
                             uint8_t fmt)
{

}

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: stm32_dma2d_setclut
 *
 * Description:
 *   Configure layer clut (color lookup table).
 *
 * Input Parameters:
 *   cmap   - Color lookup table with up the 256 entries
 *
 * Returned Value:
 *   On success - OK
 *   On error   - -EINVAL
 *
 ****************************************************************************/

#ifdef CONFIG_STM32L4_FB_CMAP
static int stm32_dma2d_setclut(FAR const struct fb_cmap_s *cmap)
{
    return 0;
}
#endif /* CONFIG_STM32L4_FB_CMAP */

/****************************************************************************
 * Name: stm32_dma2d_fillcolor
 *
 * Description:
 *   Fill the selected area of the whole overlay with a specific color.
 *   The caller must ensure that the area is within the entire overlay.
 *
 * Input Parameters:
 *   oinfo - Overlay to fill
 *   area  - Reference to the valid area structure select the area
 *   argb  - Color to fill the selected area. Color must be argb8888 formated.
 *
 * Returned Value:
 *    OK        - On success
 *   -EINVAL    - If one of the parameter invalid or if the size of the selected
 *                area outside the visible area of the layer.
 *   -ECANCELED - Operation cancelled, something goes wrong.
 *
 ****************************************************************************/

static int stm32_dma2d_fillcolor(FAR struct stm32_dma2d_overlay_s *oinfo,
                                 FAR const struct fb_area_s *area,
                                 uint32_t argb)
{
    return 0;
}

/****************************************************************************
 * Name: stm32_dma2d_blit
 *
 * Description:
 *   Copy memory from a source overlay (defined by sarea) to destination
 *   overlay position (defined by destxpos and destypos).
 *
 * Input Parameters:
 *   doverlay - Valid reference to the destination overlay
 *   destxpos - Valid selected x position of the destination overlay
 *   destypos - Valid selected y position of the destination overlay
 *   soverlay - Valid reference to the source overlay
 *   sarea    - Valid reference to the selected area of the source overlay
 *
 * Returned Value:
 *    OK        - On success
 *   -EINVAL    - If one of the parameter invalid or if the size of the selected
 *                source area outside the visible area of the destination layer.
 *                (The visible area usually represents the display size)
 *   -ECANCELED - Operation cancelled, something goes wrong.
 *
 ****************************************************************************/

static int stm32_dma2d_blit(FAR struct stm32_dma2d_overlay_s *doverlay,
                            uint32_t destxpos, uint32_t destypos,
                            FAR struct stm32_dma2d_overlay_s *soverlay,
                            FAR const struct fb_area_s *sarea)
{

  return 0;
}

/****************************************************************************
 * Name: stm32_dma2d_blend
 *
 * Description:
 *   Blends the selected area from a background layer with selected position
 *   of the foreground layer. Copies the result to the selected position of
 *   the destination layer. Note! The content of the foreground and background
 *   layer keeps unchanged as long destination layer is unequal to the
 *   foreground and background layer.
 *
 * Input Parameters:
 *   doverlay - Destination overlay
 *   destxpos - x-Offset destination overlay
 *   destypos - y-Offset destination overlay
 *   foverlay - Foreground overlay
 *   forexpos - x-Offset foreground overlay
 *   foreypos - y-Offset foreground overlay
 *   boverlay - Background overlay
 *   barea    - x-Offset, y-Offset, x-resolution and y-resolution of background
 *              overlay
 *
 * Returned Value:
 *    OK        - On success
 *   -EINVAL    - If one of the parameter invalid or if the size of the selected
 *                source area outside the visible area of the destination layer.
 *                (The visible area usually represents the display size)
 *   -ECANCELED - Operation cancelled, something goes wrong.
 *
 ****************************************************************************/

static int stm32_dma2d_blend(FAR struct stm32_dma2d_overlay_s *doverlay,
                             uint32_t destxpos, uint32_t destypos,
                             FAR struct stm32_dma2d_overlay_s *foverlay,
                             uint32_t forexpos, uint32_t foreypos,
                             FAR struct stm32_dma2d_overlay_s *boverlay,
                             FAR const struct fb_area_s *barea)
{
    return 0;
}

#ifdef USE_HAL_DRIVER
extern uint32_t              PhysFrameBuffer[]; 
static void STM32_DMA2D_CopyBuffer(uint32_t *psrc, uint32_t *pdst, uint16_t x, uint16_t y,
                                   uint16_t xsize, uint16_t ysize)
{
    pdst = (uint32_t *)(0x60000000);//PhysFrameBuffer;

    uint32_t destination = (uint32_t)pdst + (y * 390 + x) * 4;
    uint32_t source      = (uint32_t)psrc;    
    printf("destination : 0x%X, source : 0x%X, xsize %d ysize %d\n", destination, source, xsize, ysize);

    hdma2d.Init.OutputOffset = 390 - xsize;

    if(hdma2d.LayerCfg[1].InputColorMode == DMA2D_INPUT_ARGB8888)
    {
      hdma2d.Init.Mode = DMA2D_M2M_BLEND;

      hdma2d.LayerCfg[0].AlphaMode   = DMA2D_NO_MODIF_ALPHA;
      hdma2d.LayerCfg[0].InputColorMode = DMA2D_INPUT_ARGB8888;
      hdma2d.LayerCfg[0].InputOffset = 390 - xsize;

      if(HAL_DMA2D_InitConfig(&hdma2d) == HAL_OK){
        if(HAL_DMA2D_ConfigLayer(&hdma2d, 1) == HAL_OK){
          if(HAL_DMA2D_ConfigLayer(&hdma2d, 0) == HAL_OK){
              if(HAL_DMA2D_BlendingStart(&hdma2d, source, destination, destination, xsize, ysize) == HAL_OK){
                  if(HAL_DMA2D_PollForTransfer(&hdma2d, 100) != HAL_OK){
                      printf("buffer copy error\n");
                  }else{
                      printf("buffer copy done\n");
                  }
              }
          }
        }
      }
    }
    else
    {
      hdma2d.Init.Mode = DMA2D_M2M_PFC;

      hdma2d.LayerCfg[1].InputAlpha  = 0xFF;

      if(HAL_DMA2D_InitConfig(&hdma2d) == HAL_OK){
          if(HAL_DMA2D_ConfigLayer(&hdma2d, 1) == HAL_OK){
              if(HAL_DMA2D_Start(&hdma2d, source, destination, xsize, ysize) == HAL_OK){
                  if(HAL_DMA2D_PollForTransfer(&hdma2d, 100) != HAL_OK){
                      printf("buffer copy error\n");
                  }else{
                      printf("buffer copy done\n");
                  }
              }
          }
      }
    }
}

static void STM32_DMA2D_SetLayer_InputColor(uint32_t pf)
{
  if(pf == DMA2D_PF_ARGB8888)
  {
    hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  }
  else
  {
    hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB888;
  }
}
#endif
/****************************************************************************
 * Name: stm32_dma2dinitialize
 *
 * Description:
 *   Initialize the dma2d controller
 *
 * Returned Value:
 *   OK - On success
 *   An error if initializing failed.
 *
 ****************************************************************************/
#ifdef USE_HAL_DRIVER

int stm32l4_dma2dinitialize(void)
{
  hdma2d.Instance          = DMA2D;

  /*##-1- Configure the DMA2D Mode, Color Mode and output offset #############*/
  hdma2d.Init.Mode           = DMA2D_M2M_BLEND;
  hdma2d.Init.ColorMode      = DMA2D_OUTPUT_ARGB8888;
  hdma2d.Init.OutputOffset   = 0;//1024 - 390;
  hdma2d.Init.AlphaInverted  = DMA2D_REGULAR_ALPHA;  /* No Output Alpha Inversion */
  hdma2d.Init.RedBlueSwap    = DMA2D_RB_REGULAR;     /* No Output Red & Blue swap */
  hdma2d.Init.BytesSwap      = DMA2D_BYTES_REGULAR;  /* Regular output byte order */
  hdma2d.Init.LineOffsetMode = DMA2D_LOM_PIXELS;     /* Pixel mode                */

  /*##-2- Foreground Configuration ###########################################*/
  hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_RGB888;
  hdma2d.LayerCfg[1].InputOffset    = 0;
  hdma2d.LayerCfg[1].AlphaMode      = DMA2D_NO_MODIF_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha     = 0xFF;                /* Not used */
  hdma2d.LayerCfg[1].RedBlueSwap    = DMA2D_RB_SWAP; //DMA22D_RB_REGULAR;    /* No ForeGround Red/Blue swap */
  hdma2d.LayerCfg[1].AlphaInverted  = DMA2D_REGULAR_ALPHA; /* No ForeGround Alpha inversion */

  /* DMA2D Initialization */
  HAL_DMA2D_Init(&hdma2d);
  HAL_DMA2D_ConfigLayer(&hdma2d, 1);
}

static HAL_StatusTypeDef HAL_DMA2D_InitConfig(DMA2D_HandleTypeDef *hdma2d)
{
  /* Check the DMA2D peripheral state */
  if(hdma2d == NULL)
  {
     return HAL_ERROR;
  }

  /* Check the parameters */
  assert_param(IS_DMA2D_ALL_INSTANCE(hdma2d->Instance));
  assert_param(IS_DMA2D_MODE(hdma2d->Init.Mode));
  assert_param(IS_DMA2D_CMODE(hdma2d->Init.ColorMode));
  assert_param(IS_DMA2D_OFFSET(hdma2d->Init.OutputOffset));
  assert_param(IS_DMA2D_ALPHA_INVERTED(hdma2d->Init.AlphaInverted));
  assert_param(IS_DMA2D_RB_SWAP(hdma2d->Init.RedBlueSwap));
#if defined(DMA2D_LINE_OFFSET_MODE_SUPPORT)
  assert_param(IS_DMA2D_LOM_MODE(hdma2d->Init.LineOffsetMode));
#endif /* DMA2D_LINE_OFFSET_MODE_SUPPORT */
#if defined(DMA2D_OUTPUT_TWO_BY_TWO_SWAP_SUPPORT)
  assert_param(IS_DMA2D_BYTES_SWAP(hdma2d->Init.BytesSwap));
#endif /* DMA2D_OUTPUT_TWO_BY_TWO_SWAP_SUPPORT */

  /* Change DMA2D peripheral state */
  hdma2d->State = HAL_DMA2D_STATE_BUSY;

  /* DMA2D CR register configuration -------------------------------------------*/
#if defined(DMA2D_LINE_OFFSET_MODE_SUPPORT)
  MODIFY_REG(hdma2d->Instance->CR, DMA2D_CR_MODE | DMA2D_CR_LOM, hdma2d->Init.Mode | hdma2d->Init.LineOffsetMode);
#else
  MODIFY_REG(hdma2d->Instance->CR, DMA2D_CR_MODE, hdma2d->Init.Mode);
#endif /* DMA2D_LINE_OFFSET_MODE_SUPPORT */

  /* DMA2D OPFCCR register configuration ---------------------------------------*/
#if defined(DMA2D_OUTPUT_TWO_BY_TWO_SWAP_SUPPORT)
  MODIFY_REG(hdma2d->Instance->OPFCCR, DMA2D_OPFCCR_CM | DMA2D_OPFCCR_SB, hdma2d->Init.ColorMode | hdma2d->Init.BytesSwap);
#else
  MODIFY_REG(hdma2d->Instance->OPFCCR, DMA2D_OPFCCR_CM, hdma2d->Init.ColorMode);
#endif /* DMA2D_OUTPUT_TWO_BY_TWO_SWAP_SUPPORT */

  /* DMA2D OOR register configuration ------------------------------------------*/
  MODIFY_REG(hdma2d->Instance->OOR, DMA2D_OOR_LO, hdma2d->Init.OutputOffset);
  /* DMA2D OPFCCR AI and RBS fields setting (Output Alpha Inversion)*/
  MODIFY_REG(hdma2d->Instance->OPFCCR,(DMA2D_OPFCCR_AI|DMA2D_OPFCCR_RBS), ((hdma2d->Init.AlphaInverted << DMA2D_OPFCCR_AI_Pos) | (hdma2d->Init.RedBlueSwap << DMA2D_OPFCCR_RBS_Pos)));


  /* Update error code */
  hdma2d->ErrorCode = HAL_DMA2D_ERROR_NONE;

  /* Initialize the DMA2D state*/
  hdma2d->State  = HAL_DMA2D_STATE_READY;

  return HAL_OK;
}
#else
int stm32l4_dma2dinitialize(void)
{
  gdbg("Initialize DMA2D driver\n");

  if (g_initialized == false)
    {
      /* Abort current dma2d data transfer */

      stm32l4_dma2duninitialize();

      /* Enable dma2d is done in rcc_enableahb1, see
       * arch/arm/src/stm32/stm32f40xxx_rcc.c
       */

      /* Initialize the DMA2D semaphore that enforces mutually exclusive access
       * to the driver
       */

      sem_init(&g_lock, 0, 1);

      /* Initialize the semaphore for interrupt handling.  This waitsem
       * semaphore is used for signaling and, hence, should not have
       * priority inheritance enabled.
       */

      sem_init(g_interrupt.sem, 0, 0);
      sem_setprotocol(g_interrupt.sem, SEM_PRIO_NONE);

#ifdef CONFIG_STM32L4_FB_CMAP
      /* Enable dma2d transfer and clut loading interrupts only */

      stm32_dma2d_control(DMA2D_CR_TCIE | DMA2D_CR_CTCIE, DMA2D_CR_TEIE |
                          DMA2D_CR_TWIE | DMA2D_CR_CAEIE | DMA2D_CR_CEIE);
#else
      /* Enable dma transfer interrupt only */

      stm32_dma2d_control(DMA2D_CR_TCIE, DMA2D_CR_TEIE | DMA2D_CR_TWIE |
                          DMA2D_CR_CAEIE | DMA2D_CR_CTCIE | DMA2D_CR_CEIE);
#endif

      stm32_dma2d_control(DMA2D_CR_TCIE | DMA2D_CR_CTCIE | DMA2D_CR_TEIE |
                          DMA2D_CR_CAEIE | DMA2D_CR_CTCIE | DMA2D_CR_CEIE, 0);

      /* Attach DMA2D interrupt vector */

      (void)irq_attach(g_interrupt.irq, stm32_dma2dirq, NULL);

      /* Enable the IRQ at the NVIC */

      up_enable_irq(g_interrupt.irq);

      g_initialized = true;
    }

  return OK;
}

/****************************************************************************
 * Name: stm32_dma2duninitialize
 *
 * Description:
 *   Uninitialize the dma2d controller
 *
 ****************************************************************************/

void stm32l4_dma2duninitialize(void)
{

  /* Disable DMA2D interrupts */

  up_disable_irq(g_interrupt.irq);
  irq_detach(g_interrupt.irq);

  /* Abort current dma2d transfer */

  stm32_dma2d_control(DMA2D_CR_ABORT, 0);

  /* Set initialized state */

  g_initialized = false;

}
#endif
/****************************************************************************
 * Name: stm32_dma2ddev
 *
 * Description:
 *   Get a reference to the dma2d controller.
 *
 * Returned Value:
 *   On success - A valid dma2d layer reference
 *   On error   - NULL
 *
 ****************************************************************************/

FAR struct dma2d_layer_s *stm32l4_dma2ddev(void)
{
    return &g_dma2ddev.dma2d;
}



