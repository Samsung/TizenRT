/****************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
/****************************************************************************
 * drivers/lcd/lcd_frambuffer.c
 *
 *   Copyright (C) 2017=-2018 Gregory Nutt. All rights reserved.
 *   Author: Gregory Nutt <gnutt@nuttx.org>
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
#include <assert.h>
#include <errno.h>
#include <debug.h>

#include <tinyara/board.h>
#include <tinyara/kmalloc.h>
#include <tinyara/lcd/lcd.h>
#include <tinyara/video/fb.h>

#ifdef CONFIG_LCD_FRAMEBUFFER

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/* At present, only video plane 0 is supported */

#define VIDEO_PLANE 0

/****************************************************************************
 * Private Types
 ****************************************************************************/

/* This structure describes the LCD frambuffer */

struct lcdfb_dev_s {
	struct fb_vtable_s vtable;        /* Must be cast compatible with lcdfb_dev_s */
	FAR struct lcdfb_dev_s *flink;    /* Supports a singly linked list */
	FAR struct lcd_dev_s *lcd;        /* Contained LCD device */
	FAR uint8_t *fbmem;               /* Allocated framebuffer */
	FAR struct lcd_planeinfo_s pinfo; /* LCD plane info */
	size_t fblen;                     /* Size of the framebuffer in bytes */
	fb_coord_t xres;                  /* Horizontal resolution in pixel columns */
	fb_coord_t yres;                  /* Vertical resolution in pixel rows */
	fb_coord_t stride;                /* Width of a row in bytes */
	uint8_t display;                  /* Display number */
};

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/* Update the LCD when there is a change to the framebuffer */

static int lcdfb_update(FAR struct lcdfb_dev_s *priv,
			FAR const struct nxgl_rect_s *rect);

/* Get information about the video controller configuration and the
 * configuration of each color plane.
 */

static int lcdfb_getvideoinfo(FAR struct fb_vtable_s *vtable,
			FAR struct fb_videoinfo_s *vinfo);
static int lcdfb_getplaneinfo(FAR struct fb_vtable_s *vtable, int planeno,
			FAR struct fb_planeinfo_s *pinfo);

/* The following is provided only if the video hardware supports RGB color
 * mapping
 */

#ifdef CONFIG_FB_CMAP
static int lcdfb_getcmap(FAR struct fb_vtable_s *vtable,
			FAR struct fb_cmap_s *cmap);
static int lcdfb_putcmap(FAR struct fb_vtable_s *vtable,
			FAR const struct fb_cmap_s *cmap);
#endif

/* The following is provided only if the video hardware supports a hardware
 * cursor
 */

#ifdef CONFIG_FB_HWCURSOR
static int lcdfb_getcursor(FAR struct fb_vtable_s *vtable,
			FAR struct fb_cursorattrib_s *attrib);
static int lcdfb_setcursor(FAR struct fb_vtable_s *vtable,
			FAR struct fb_setcursor_s *settings);
#endif

/****************************************************************************
 * Private Data
 ****************************************************************************/

/* This is a singly linked list that supports look-up of framebuffer state
 * using the display number.
 */

static FAR struct lcdfb_dev_s *g_lcdfb;

/****************************************************************************
 * Private Functions
 ****************************************************************************/

/****************************************************************************
 * Name: lcdfb_find
 *
 * Description:
 *   Find the LCD framebuffer state associated with the display.
 *
 ****************************************************************************/

static FAR struct lcdfb_dev_s *lcdfb_find(int display)
{
	FAR struct lcdfb_dev_s *priv;

	/* Look up the LCD framebuffer state structure for this display.
	*
	* REVISIT:  If many LCD framebuffers are used, then this lookup would be
	* a performance issue.
	* REVISIT: Semaphore protections is needed if there is concurrent access.
	*/

	for (priv = g_lcdfb; priv != NULL; priv = priv->flink) {
		if (priv->display == display) {
			return priv;
		}
	}

	return NULL;
}

/****************************************************************************
 * Name: lcdfb_update
 *
 * Description:
 * Update the LCD when there is a change to the framebuffer.
 *
 ****************************************************************************/

static int lcdfb_update(FAR struct lcdfb_dev_s *priv,
			FAR const struct nxgl_rect_s *rect)
{
	FAR struct lcd_planeinfo_s *pinfo = &priv->pinfo;
	FAR uint8_t *run;
	fb_coord_t row;
	fb_coord_t startx;
	fb_coord_t endx;
	fb_coord_t width;
	fb_coord_t starty;
	fb_coord_t endy;
	int ret;
#ifdef CONFIG_LCD_DMA_SUPPORT
	int rowbytes;
	int optsize;
	int bytespl;
	int height;
	uint8_t *dmabuff;
	int offset = 0;
#endif

	/* Clip to fit in the framebuffer */

	startx = rect->pt1.x;
	if (startx < 0) {
		startx = 0;
	}

	endx = rect->pt2.x;
	if (endx >= priv->xres) {
		endx = priv->xres-1;
	}

	starty = rect->pt1.y;
	if (starty < 0) {
		starty = 0;
	}

	endy = rect->pt2.y;
	if (endy >= priv->yres) {
		endy = priv->yres-1;
	}

	/* If the display uses a value of BPP < 8, then we may have to extend the
	* rectangle on the left so that it is byte aligned.  Works for BPP={1,2,4}
	*/

	if (pinfo->bpp < 8) {
		unsigned int pixperbyte = 8 / pinfo->bpp;
		startx &= ~(pixperbyte - 1);
	}

	width = endx - startx + 1;

#ifdef CONFIG_LCD_DMA_SUPPORT
	if (pinfo->putdma) {
		if (width == priv->xres) {
			dmabuff = priv->fbmem + starty * priv->stride;
			ret = pinfo->putdma(starty, startx, endy, endx, dmabuff);
		} else {
			/* Send buffer with dma , need to convert buffer
			 * buffer for each line is byte aligned.
			 */
			bytespl = (width * pinfo->bpp + 7) >> 3;
			height = endy - starty + 1;
			
			dmabuff = (uint8_t *)kmm_malloc(height * bytespl);
			if (dmabuff == NULL) {
				gdbg("ERROR: out of memory \n");
				return -ENOMEM;
			}

			optsize = sizeof(uint32_t);
			for (row = starty; row <= endy; row++) {
				/* Get starting position of each row */
				run = priv->fbmem + row * priv->stride;
				run += (startx * pinfo->bpp + 7) >> 3;

				rowbytes = bytespl;
				/* First copy each optsize bytes */
				while (rowbytes >= optsize) {
					*((uint32_t *)(dmabuff + offset)) = *((uint32_t *)run);
					rowbytes -= optsize;
					offset += optsize;
					run += optsize;
				}

				/* Copy left data byte by byte */
				while (rowbytes-- > 0) {
					dmabuff[offset++] = *run++;
				}
			}

			ret = pinfo->putdma(starty, startx, endy, endx, dmabuff);
			kmm_free(dmabuff);
		}

		if (ret < 0) {
			gdbg("ERROR: LCD putdma failed: %d \n", ret);
		}

		return ret;
	}
#endif

	/* Get the starting position in the framebuffer */

	run  = priv->fbmem + starty * priv->stride;
	run += (startx * pinfo->bpp + 7) >> 3;

	for (row = starty; row <= endy; row++) {
		/* REVISIT: Some LCD hardware certain aligment requirements on DMA
		* memory.
		*/

		ret = pinfo->putrun(row, startx, run, width);
		if (ret < 0) {
			return ret;
		}

		run += priv->stride;
	}

	return OK;
}

/****************************************************************************
 * Name: lcdfb_getvideoinfo
 ****************************************************************************/

static int lcdfb_getvideoinfo(FAR struct fb_vtable_s *vtable,
				FAR struct fb_videoinfo_s *vinfo)
{
	FAR struct lcdfb_dev_s *priv;
	FAR struct lcd_dev_s *lcd;
	int ret = -EINVAL;

	gvdbg("vtable=%p vinfo=%p\n", vtable, vinfo);

	DEBUGASSERT(vtable != NULL && vinfo != NULL);
	priv = (FAR struct lcdfb_dev_s *)vtable;

	if (priv != NULL && vinfo != NULL) {
		/* Get the video info from the contained LCD */

		lcd = priv->lcd;
		DEBUGASSERT(lcd->getvideoinfo != NULL);
		ret = lcd->getvideoinfo(lcd, vinfo);
		if (ret < 0) {
			gdbg("ERROR: LCD getvideoinfo() failed: %d\n", ret);
		}
	}

	return ret;
}

/****************************************************************************
 * Name: lcdfb_getplaneinfo
 ****************************************************************************/

static int lcdfb_getplaneinfo(FAR struct fb_vtable_s *vtable, int planeno,
				FAR struct fb_planeinfo_s *pinfo)
{
	FAR struct lcdfb_dev_s *priv;
	int ret = -EINVAL;

	gvdbg("vtable=%p planeno=%d pinfo=%p\n", vtable, planeno, pinfo);

	DEBUGASSERT(vtable != NULL && planeno == VIDEO_PLANE && pinfo != NULL);
	priv = (FAR struct lcdfb_dev_s *)vtable;

	if (priv != NULL && planeno == VIDEO_PLANE && pinfo != NULL) {
		/* Return the plane info */

		pinfo->fbmem   = priv->fbmem;
		pinfo->fblen   = priv->fblen;
		pinfo->stride  = priv->stride;
		pinfo->display = priv->display;
		pinfo->bpp     = priv->pinfo.bpp;

		ret = OK;
	}

	return ret;
}

/****************************************************************************
 * Name: lcdfb_getcmap
 ****************************************************************************/

#ifdef CONFIG_FB_CMAP
static int lcdfb_getcmap(FAR struct fb_vtable_s *vtable,
			FAR struct fb_cmap_s *cmap)
{
	FAR struct lcdfb_dev_s *priv;
	FAR struct lcd_dev_s *lcd;
	int ret = -EINVAL;

	gvdbg("vtable=%p cmap=%p\n", vtable, cmap);

	DEBUGASSERT(vtable != NULL && cmap != NULL);
	priv = (FAR struct lcdfb_dev_s *)vtable;

	if (priv != NULL && cmap != NULL) {
		/* Get the video info from the contained LCD */

		lcd = priv->lcd
		DEBUGASSERT(lcd->getcmap != NULL);
		ret = lcd->getcmap(lcd, cmap);
		if (ret < 0) {
			gdbg("ERROR: LCD getcmap() failed: %d\n", ret);
		}
	}

	return ret;
}
#endif

/****************************************************************************
 * Name: lcdfb_putcmap
 ****************************************************************************/

#ifdef CONFIG_FB_CMAP
static int lcdfb_putcmap(FAR struct fb_vtable_s *vtable,
			FAR const struct fb_cmap_s *cmap)
{
	FAR struct lcdfb_dev_s *priv;
	FAR struct lcd_dev_s *lcd;
	int ret = -EINVAL;

	gvdbg("vtable=%p cmap=%p\n", vtable, cmap);

	DEBUGASSERT(vtable != NULL && cmap != NULL);
	priv = (FAR struct lcdfb_dev_s *)vtable;

	if (priv != NULL && cmap != NULL) {
		/* Get the video info from the contained LCD */

		lcd = priv->lcd
		DEBUGASSERT(lcd->putcmap != NULL);
		ret = lcd->putcmap(lcd, cmap);
		if (ret < 0) {
			gdbg("ERROR: LCD putcmap() failed: %d\n", ret);
		}
	}

	return ret;
}
#endif

/****************************************************************************
 * Name: lcdfb_getcursor
 ****************************************************************************/

#ifdef CONFIG_FB_HWCURSOR
static int lcdfb_getcursor(FAR struct fb_vtable_s *vtable,
			FAR struct fb_cursorattrib_s *attrib)
{
	gvdbg("vtable=%p attrib=%p\n", vtable, attrib);
	FAR struct lcdfb_dev_s *priv;
	FAR struct lcd_dev_s *lcd;
	int ret = -EINVAL;

	gvdbg("vtable=%p attrib=%p\n", vtable, attrib);

	DEBUGASSERT(vtable != NULL && attrib != NULL);
	priv = (FAR struct lcdfb_dev_s *)vtable;

	if (priv != NULL && attrib != NULL) {
		/* Get the video info from the contained LCD */

		lcd = priv->lcd
		DEBUGASSERT(lcd->getcursor != NULL);
		ret = lcd->getcursor(lcd, attrib);
		if (ret < 0) {
			gdbg("ERROR: LCD getcursor() failed: %d\n", ret);
		}
	}

	return ret;
}
#endif

/****************************************************************************
 * Name: lcdfb_setcursor
 ****************************************************************************/

#ifdef CONFIG_FB_HWCURSOR
static int lcdfb_setcursor(FAR struct fb_vtable_s *vtable,
			FAR struct fb_setcursor_s *settings)
{
	FAR struct lcdfb_dev_s *priv;
	FAR struct lcd_dev_s *lcd;
	int ret = -EINVAL;

	gvdbg("vtable=%p settings=%p\n", vtable, settings);

	DEBUGASSERT(vtable != NULL && settings != NULL);
	priv = (FAR struct lcdfb_dev_s *)vtable;

	if (priv != NULL && settings != NULL) {
		/* Get the video info from the contained LCD */

		lcd = priv->lcd
		DEBUGASSERT(lcd->setcursor != NULL);
		ret = lcd->setcursor(lcd, settings);
		if (ret < 0) {
			gdbg("ERROR: LCD setcursor() failed: %d\n", ret);
		}
	}

	return ret;
}
#endif

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * Name: up_fbinitialize
 *
 * Description:
 *   Initialize the framebuffer video hardware associated with the display.
 *
 * Input Parameters:
 *   display - In the case of hardware with multiple displays, this
 *     specifies the display.  Normally this is zero.
 *
 * Returned Value:
 *   Zero is returned on success; a negated errno value is returned on any
 *   failure.
 *
 ****************************************************************************/

int up_fbinitialize(int display)
{
	FAR struct lcdfb_dev_s *priv;
	FAR struct lcd_dev_s *lcd;
	struct fb_videoinfo_s vinfo;
	struct nxgl_rect_s rect;
	int ret;

	gvdbg("display=%d\n", display);
	DEBUGASSERT((unsigned)display < UINT8_MAX);

	/* Allocate the framebuffer state structure */

	priv = (FAR struct lcdfb_dev_s *)kmm_zalloc(sizeof(struct lcdfb_dev_s));
	if (priv == NULL) {
		gdbg("ERROR: Failed to allocate state structure\n");
		return -ENOMEM;
	}

	/* Initialize the LCD-independent fields of the state structure */

	priv->display             = display;

	priv->vtable.getvideoinfo = lcdfb_getvideoinfo,
	priv->vtable.getplaneinfo = lcdfb_getplaneinfo,
#ifdef CONFIG_FB_CMAP
	priv->vtable.getcmap      = lcdfb_getcmap,
	priv->vtable.putcmap      = lcdfb_putcmap,
#endif
#ifdef CONFIG_FB_HWCURSOR
	priv->vtable.getcursor    = lcdfb_getcursor,
	priv->vtable.setcursor    = lcdfb_setcursor,
#endif

#ifdef  CONFIG_LCD_EXTERNINIT
	/* Use external graphics driver initialization */

	lcd = board_graphics_setup(display);
	if (lcd == NULL) {
		gdbg("ERROR: board_graphics_setup failed, devno=%d\n", display);
		return EXIT_FAILURE;
	}
#else
	/* Initialize the LCD device */

	ret = board_lcd_initialize();
	if (ret < 0) {
		gdbg("ERROR: board_lcd_initialize() failed: %d\n", ret);
		goto errout_with_state;
	}

	/* Get the device instance */

	lcd = board_lcd_getdev(display);
	if (lcd == NULL) {
		gdbg("ERROR: board_lcd_getdev failed, devno=%d\n", display);
		ret = -ENODEV;
		goto errout_with_lcd;
	}
#endif

	priv->lcd = lcd;

	/* Initialize the LCD-dependent fields of the state structure */

	DEBUGASSERT(lcd->getvideoinfo != NULL);
	ret = lcd->getvideoinfo(lcd, &vinfo);
	if (ret < 0) {
		gdbg("ERROR:  LCD getvideoinfo() failed: %d\n", ret);
		goto errout_with_lcd;
	}

	priv->xres = vinfo.xres;
	priv->yres = vinfo.yres;

	DEBUGASSERT(lcd->getplaneinfo != NULL);
	ret = lcd->getplaneinfo(lcd, VIDEO_PLANE, &priv->pinfo);
	if (ret < 0) {
		gdbg("ERROR: LCD getplaneinfo() failed: %d\n", ret);
		goto errout_with_lcd;
	}

	/* Allocate (and clear) the framebuffer */

	priv->stride = ((size_t)priv->xres * priv->pinfo.bpp + 7) >> 3;
	priv->fblen  = priv->stride * priv->yres;

	priv->fbmem  = (FAR uint8_t *)kmm_zalloc(priv->fblen);
	if (priv->fbmem == NULL) {
		gdbg("ERROR: Failed to allocate frame buffer memory\n");
		ret = -ENOMEM;
		goto errout_with_lcd;
	}

	/* Add the state structure to the list of framebuffer interfaces */

	priv->flink = g_lcdfb;
	g_lcdfb     = priv;

	/* Write the entire framebuffer to the LCD */

	rect.pt1.x = 0;
	rect.pt1.y = 0;
	rect.pt2.x = priv->xres - 1;
	rect.pt2.y = priv->yres - 1;

	ret = lcdfb_update(priv, &rect);
	if (ret < 0) {
		gdbg("FB update failed: %d\n", ret);
	}

	/* Turn the LCD on at 75% power */

	(void)priv->lcd->setpower(priv->lcd, ((3*CONFIG_LCD_MAXPOWER + 3)/4));
	return OK;

errout_with_lcd:
#ifndef CONFIG_LCD_EXTERNINIT
	board_lcd_uninitialize();

errout_with_state:
#endif
	kmm_free(priv);
	return ret;
}

/****************************************************************************
 * Name: up_fbgetvplane
 *
 * Description:
 *   Return a a reference to the framebuffer object for the specified video
 *   plane of the specified plane.  Many OSDs support multiple planes of video.
 *
 * Input Parameters:
 *   display - In the case of hardware with multiple displays, this
 *     specifies the display.  Normally this is zero.
 *   vplane - Identifies the plane being queried.
 *
 * Returned Value:
 *   A non-NULL pointer to the frame buffer access structure is returned on
 *   success; NULL is returned on any failure.
 *
 ****************************************************************************/

FAR struct fb_vtable_s *up_fbgetvplane(int display, int vplane)
{
	FAR struct lcdfb_dev_s *priv;

	gvdbg("display=%d vplane=%d\n", display, vplane);
	DEBUGASSERT(vplane == VIDEO_PLANE);

	/* Look up the LCD framebuffer state structure for this display. */

	priv = lcdfb_find(display);
	if (priv == NULL) {
		gdbg("ERROR: lcd_find(%d) failed\n", display);
		return NULL;
	}

	return &priv->vtable;
}

/****************************************************************************
 * Name: up_fbuninitialize
 *
 * Description:
 *   Uninitialize the framebuffer support for the specified display.
 *
 * Input Parameters:
 *   display - In the case of hardware with multiple displays, this
 *     specifies the display.  Normally this is zero.
 *
 * Returned Value:
 *   None
 *
 ****************************************************************************/

void up_fbuninitialize(int display)
{
	FAR struct lcdfb_dev_s *priv;
	FAR struct lcdfb_dev_s *prev;

	/* Find the LCD framebuffer state associated with this display.
	* REVISIT: Semaphore protections is needed if there is concurrent access.
	*/

	for (prev = NULL, priv = g_lcdfb;
		priv != NULL;
		prev = priv, priv = priv->flink) {
		if (priv->display == display) {
			/* Remove the state structure from the list */

			if (prev != NULL) {
				prev->flink = priv->flink;
			} else {
				g_lcdfb = priv->flink;
			}

#ifndef  CONFIG_LCD_EXTERNINIT
			/* Uninitialize the LCD */

			board_lcd_uninitialize();
#endif

			/* Free the frame buffer allocation */

			kmm_free(priv->fbmem);

			/* Free the state structure allocation */

			kmm_free(priv);
			break;
		}
	}
}

/****************************************************************************
 * Name: nx_notify_rectangle
 *
 * Description:
 *   When CONFIG_LCD_UPDATE=y, then the graphics system will callout to
 *   inform some external module that the display has been updated.  This
 *   would be useful in a couple for cases.
 *
 *   - When a serial LCD is used, but a framebuffer is used to access the
 *     LCD.  In this case, the update callout can be used to refresh the
 *     affected region of the display.
 *
 *   - When VNC is enabled.  This is case, this callout is necessary to
 *     update the remote frame buffer to match the local framebuffer.
 *
 *   When this feature is enabled, some external logic must provide this
 *   interface.  This is the function that will handle the notification.  It
 *   receives the rectangular region that was updated on the provided plane.
 *
 *   NOTE: This function is also required for use with the LCD framebuffer
 *   driver front end when CONFIG_LCD_UPDATE=y, although that use does not
 *   depend on CONFIG_NX (and this function seems misnamed in that case).
 *
 ****************************************************************************/

#if defined(CONFIG_LCD_UPDATE)
void nx_notify_rectangle(FAR struct fb_planeinfo_s *pinfo,
				FAR const struct nxgl_rect_s *rect)
{
	FAR struct fb_planeinfo_s *fpinfo = (FAR struct fb_planeinfo_s *)pinfo;
	FAR struct lcdfb_dev_s *priv;
	int ret;

	DEBUGASSERT(fpinfo != NULL && rect != NULL);

	/* Look up the LCD framebuffer state structure for this display.
	*
	* REVISIT:  If many LCD framebuffers are used, then this lookup would be
	* a performance issue.
	*/

	priv = lcdfb_find(fpinfo->display);
	if (priv != NULL) {
		ret = lcdfb_update(priv, rect);
		if (ret < 0) {
			gdbg("FB update failed: %d\n", ret);
		}
	}
}
#endif

#endif /* CONFIG_LCD_FRAMEBUFFER */

