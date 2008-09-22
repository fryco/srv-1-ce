/*
 * (C) Copyright 2008 James Buckley
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <stdio.h>
#include <malloc.h>

#include "blackfin.h"
#include "i2c.h"
#include "camera_ov9655.h"

#define OMNIVISION_CAMERA_RESET_WORKAROUND

#define CAMERA_I2C_ADDRESS		0x30
#define NUMBER_OF_IMAGE_BUFFERS	4

#define QQVGA_WIDTH		160
#define	QQVGA_HEIGHT	128
#define QVGA_WIDTH		320
#define	QVGA_HEIGHT		256
#define VGA_WIDTH		640
#define	VGA_HEIGHT		512
#define SXGA_WIDTH		1280
#define	SXGA_HEIGHT		1024

typedef enum {
	QQVGA,
	QVGA,
	VGA,
	SXGA
} Resolution;

typedef enum {
	YUYV
} PixelFormat;

typedef enum {
	OV9655 = 0x9657
} CameraProductID;

dma_desc_list image_buffer_descriptor[NUMBER_OF_IMAGE_BUFFERS];
bool camera_initialised;
bool camera_running;
CameraProductID product_id;

void camera_init();
int camera_set_attributes(Resolution res, PixelFormat pxlfmt);
int camera_start();
int camera_stop();
bool camera_connected();
void camera_ISR()	__attribute__((interrupt_handler));

#endif /*CAMERA_H_*/
