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

#include "camera.h"
#include "led.h"

void camera_init()
{
	ubyte send;
	ubyte ret;
	unsigned int i;
	
	camera_running = 0;

	// Check whether the camera is attached
	send = 0xa;
	i2c_write(CAMERA_I2C_ADDRESS, &send, 1, 1);
	i2c_read(CAMERA_I2C_ADDRESS, &ret, 1, 1);
	product_id = (ret << 8);
	send = 0xb;
	i2c_write(CAMERA_I2C_ADDRESS, &send, 1, 1);
	i2c_read(CAMERA_I2C_ADDRESS, &ret, 1, 1);
	product_id += ret;

	switch(product_id)
	{
	case OV9655:
		break;
	default:
		camera_initialised = 0;
		return;
	}

//	// Setup camera registers
//	for(i = 0; i < (sizeof(ov9655_setup) / 2); i++)
//		i2c_write(CAMERA_I2C_ADDRESS, &(ov9655_setup[i * 2]), 2, 1);
//
//#ifdef OMNIVISION_CAMERA_RESET_WORKAROUND
//	for(i = 0; i < (sizeof(ov9655_setup) / 2); i++)
//		i2c_write(CAMERA_I2C_ADDRESS, &(ov9655_setup[i * 2]), 2, 1);
//#endif

	camera_initialised = 1;
	
	// Enable PPI pins
	*pPORTG_FER |= (PG0 | PG1 | PG2 | PG3 | PG4 | PG5 | PG6 | PG7);
	*pPORTF_FER |= (PF8 | PF9 | PF15);

	// Setup the PPI registers
	*pPPI_CONTROL = PACK_EN | (2 << 4) | XFR_TYPE;

	// Configure the DMA descriptors (with interrupts)
	for(i = 0; i < NUMBER_OF_IMAGE_BUFFERS; i++)
	{
		image_buffer_descriptor[i].config = (FLOW_LARGE | NDSIZE_9 | DI_EN | DMA2D | WDSIZE_16 | WNR | DMAEN);
		image_buffer_descriptor[i].next = &(image_buffer_descriptor[i + 1]);
	}

	// Make the descriptor loop
	image_buffer_descriptor[NUMBER_OF_IMAGE_BUFFERS - 1].next = &(image_buffer_descriptor[0]);

	// Configure the intial CONFIG register, without setting the start bit
	*pDMA0_CONFIG = (FLOW_LARGE | NDSIZE_9);
	
	// Enable DMA0 interrupts
	*pEVT8 = camera_ISR;
	*pIMASK |= EVT_IVG8;
	
	// Unmask DMA0 interrupt
	*pSIC_IMASK |= IRQ_DMA0;

	// Put the camera in an initial state
	camera_set_attributes(VGA, YUYV);
}

int camera_set_attributes(Resolution res, PixelFormat pxlfmt)
{
	ubyte bytes_per_pixel;
	unsigned int i, width, height;

	if(!camera_initialised)
		return -1;

	if(camera_running)
		camera_stop();

	switch(res)
	{
	case QQVGA:
		for(i = 0; i < (sizeof(ov9655_qqvga) / 2); i++)
			i2c_write(CAMERA_I2C_ADDRESS, &(ov9655_qqvga[i * 2]), 2, 1);

		width = QQVGA_WIDTH;
		height = QQVGA_HEIGHT;
		break;
	case QVGA:
		for(i = 0; i < (sizeof(ov9655_qvga) / 2); i++)
			i2c_write(CAMERA_I2C_ADDRESS, &(ov9655_qvga[i * 2]), 2, 1);

		width = QVGA_WIDTH;
		height = QVGA_HEIGHT;
		break;
	case VGA:
		for(i = 0; i < (sizeof(ov9655_vga) / 2); i++)
			i2c_write(CAMERA_I2C_ADDRESS, &(ov9655_vga[i * 2]), 2, 1);

		width = VGA_WIDTH;
		height = VGA_HEIGHT;
		break;
	case SXGA:
		for(i = 0; i < (sizeof(ov9655_sxga) / 2); i++)
			i2c_write(CAMERA_I2C_ADDRESS, &(ov9655_sxga[i * 2]), 2, 1);

		width = SXGA_WIDTH;
		height = SXGA_HEIGHT;
		break;
	default:
		return -1;
	}

	switch(pxlfmt)
	{
	case YUYV:
		bytes_per_pixel = 2;
		break;
	default:
		return -1;
	}

	// Program the PPI
	*pPPI_COUNT = (width * bytes_per_pixel) - 1;
	*pPPI_FRAME = height;

	// Configure the DMA descriptors
	for(i = 0; i < NUMBER_OF_IMAGE_BUFFERS; i++)
	{
		image_buffer_descriptor[i].xcount = width;
		image_buffer_descriptor[i].ycount = height;
		image_buffer_descriptor[i].xmodify = bytes_per_pixel;
		image_buffer_descriptor[i].ymodify = bytes_per_pixel;
		
		// Reallocate memory for new image size
		free(image_buffer_descriptor[i].buff);
		image_buffer_descriptor[i].buff = memalign(16, width * height * bytes_per_pixel);
		
		if(!image_buffer_descriptor[i].buff)
			return -1;
	}

	// Reset the DMA starting descriptor
	*pDMA0_NEXT_DESC_PTR = &(image_buffer_descriptor[0]);
	
	if(camera_running)
	{
		if(camera_start() < 0)
			return -1;
	}

	return 0;
}

int camera_start()
{
	if(!camera_initialised)
		return -1;
	
	if(camera_running)
		return 0;
	
	*pDMA0_CONFIG |= DMAEN;
	*pPPI_CONTROL |= PORT_EN;
	
	camera_running = 1;

	return 0;
}

int camera_stop()
{
	if(!camera_initialised)
		return -1;
	
	if(!camera_running)
		return 0;

	*pPPI_CONTROL &= ~PORT_EN;
	*pDMA0_CONFIG &= ~DMAEN;
	
	camera_running = 0;

	return 0;
}

bool camera_connected()
{
	if(camera_initialised)
		return 1;

	return 0;
}

void camera_ISR()
{
	*pDMA0_IRQ_STATUS |= DMA_DONE;
	LED_1_TOGGLE;
}
