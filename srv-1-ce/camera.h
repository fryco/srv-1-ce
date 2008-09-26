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

#define CAMERA_I2C_ADDRESS		0x30
#define NUMBER_OF_IMAGE_BUFFERS	4

#define QQVGA_WIDTH				160
#define	QQVGA_HEIGHT			120
#define QVGA_WIDTH				320
#define	QVGA_HEIGHT				240
#define VGA_WIDTH				640
#define	VGA_HEIGHT				480
#define SXGA_WIDTH				1280
#define	SXGA_HEIGHT				1024

#define YUYV_BYTES_PER_PIXEL	2
#define RGB565_BYTES_PER_PIXEL	2

#define WAITING_BUFF_SIZE		(NUMBER_OF_IMAGE_BUFFERS + 1)
#define FILLED_BUFF_SIZE		(NUMBER_OF_IMAGE_BUFFERS + 1)

typedef enum {
	QQVGA,
	QVGA,
	VGA,
	SXGA
} Resolution;

typedef enum {
	YUYV,
	RGB565
} PixelFormat;

typedef struct {
	unsigned int	width;
	unsigned int	height;
	PixelFormat		format;
	ubyte			bytes_per_pixel;
	unsigned char	*data;
} Image;

typedef struct {
	byte			id;
	unsigned int	number;
	Image			image;
} Frame;

typedef enum {
	OV9655 = 0x9657
} CameraProductID;

ubyte			*img_buffer[NUMBER_OF_IMAGE_BUFFERS];
ubyte			waiting_buff[WAITING_BUFF_SIZE];
ubyte			filled_buff[FILLED_BUFF_SIZE];

ubyte			*waiting_tail;
ubyte			*waiting_ptr;
ubyte			*filled_tail;
ubyte			*filled_ptr;

CameraProductID	product_id;
unsigned int	curr_width, curr_height;
unsigned int	curr_frame_number;
ubyte			curr_bytes_per_pixel;
ubyte			curr_pixel_format;
byte			curr_buffer;
bool			camera_initialised;
bool			camera_running;

void	camera_init();
int		camera_set_attributes(Resolution res, PixelFormat pxlfmt);
int		camera_start();
int		camera_stop();
bool	camera_connected();
void	camera_ISR()	__attribute__((interrupt_handler));
int		camera_get_new_frame(Frame *frame);
void	camera_return_frame(Frame *frame);

byte	buff_waiting_push(byte *data);
byte	buff_waiting_pull(byte *data);
byte	buff_filled_push(byte *data);
byte	buff_filled_pull(byte *data);
#endif /*CAMERA_H_*/
