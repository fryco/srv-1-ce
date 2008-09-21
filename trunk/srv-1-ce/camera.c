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

void camera_init()
{
	ubyte send;
	ubyte ret;
	unsigned short product_id;
	unsigned int i;
	
	// Check whether the camera is attached
	send = 0x0a;
	i2c_write(CAMERA_I2C_ADDRESS, &send, 1, 1);
	i2c_read(CAMERA_I2C_ADDRESS, &ret, 1, 1);
	product_id = (ret << 8);
	send = 0x0b;
	i2c_write(CAMERA_I2C_ADDRESS, &send, 1, 1);
	i2c_read(CAMERA_I2C_ADDRESS, &ret, 1, 1);
	product_id += ret;
	
	if(product_id != CAMERA_PRODUCT_ID)
		printf("Camera product id mismatch (0x%04X)\xd\xa",product_id);
	
	// Setup camera registers
	for(i = 0; i < (sizeof(ov9655_setup) / 2); i++)
		i2c_write(CAMERA_I2C_ADDRESS, &(ov9655_setup[i * 2]), 2, 1);
	
#ifdef OMNIVISION_CAMERA_RESET_WORKAROUND
	for(i = 0; i < (sizeof(ov9655_setup) / 2); i++)
		i2c_write(CAMERA_I2C_ADDRESS, &(ov9655_setup[i * 2]), 2, 1);
#endif
	
	// Enable PPI pins
	*pPORTG_FER |= (PG0 | PG1 | PG2 | PG3 | PG4 | PG5 | PG6 | PG7);
	*pPORTF_FER |= (PF8 | PF9 | PF15);
	
	// Setup the PPI registers
	*pPPI_CONTROL = PACK_EN | (2 << 4) | XFR_TYPE;
	
	// Configure the DMA descriptors
	ping.config = (FLOW_LARGE | NDSIZE_5 | DMA2D | WDSIZE_16 | WNR | DMAEN);
	pong.config = ping.config;

	// Allocate memory for the images
	ping.buff = memalign(16, (SXGA_WIDTH * SXGA_HEIGHT * RGB));
	pong.buff = memalign(16, (SXGA_WIDTH * SXGA_HEIGHT * RGB));
	
	// Make the descriptor loop
	ping.next = &pong;
	pong.next = &ping;
	
	// Program the descriptor addresses
	*pDMA0_CURR_DESC_PTR = &ping;
	*pDMA0_NEXT_DESC_PTR = &pong;
	
	// Put the camera in an initial state
	camera_set_attributes(QVGA);
	
	// Configure the intial CONFIG register, without setting the start bit
	*pDMA0_CONFIG = (FLOW_LARGE | NDSIZE_5 | DMA2D | WDSIZE_16 | WNR);
}

void camera_set_attributes(Resolution res)
{
	unsigned int i;
	
	*pDMA0_X_MODIFY = 2;
	*pDMA0_Y_MODIFY = *pDMA0_X_MODIFY;
	
	switch(res)
	{
	case QQVGA:
		for(i = 0; i < (sizeof(ov9655_qqvga) / 2); i++)
			i2c_write(CAMERA_I2C_ADDRESS, &(ov9655_qqvga[i * 2]), 2, 1);
		
		*pPPI_COUNT = (QQVGA_WIDTH * 2) - 1;
		*pPPI_FRAME = QQVGA_HEIGHT;
		*pDMA0_X_COUNT = QQVGA_WIDTH;
		*pDMA0_Y_COUNT = QQVGA_HEIGHT;
		break;
	case QVGA:
		for(i = 0; i < (sizeof(ov9655_qvga) / 2); i++)
			i2c_write(CAMERA_I2C_ADDRESS, &(ov9655_qvga[i * 2]), 2, 1);
		
		*pPPI_COUNT = (QVGA_WIDTH * 2) - 1;
		*pPPI_FRAME = QVGA_HEIGHT;
		*pDMA0_X_COUNT = QVGA_WIDTH;
		*pDMA0_Y_COUNT = QVGA_HEIGHT;
		break;
	case VGA:
		for(i = 0; i < (sizeof(ov9655_vga) / 2); i++)
			i2c_write(CAMERA_I2C_ADDRESS, &(ov9655_vga[i * 2]), 2, 1);
		
		*pPPI_COUNT = (VGA_WIDTH * 2) - 1;
		*pPPI_FRAME = VGA_HEIGHT;
		*pDMA0_X_COUNT = VGA_WIDTH;
		*pDMA0_Y_COUNT = VGA_HEIGHT;
		break;
	case SXGA:
		for(i = 0; i < (sizeof(ov9655_sxga) / 2); i++)
			i2c_write(CAMERA_I2C_ADDRESS, &(ov9655_sxga[i * 2]), 2, 1);
		
		*pPPI_COUNT = (SXGA_WIDTH * 2) - 1;
		*pPPI_FRAME = SXGA_HEIGHT;
		*pDMA0_X_COUNT = SXGA_WIDTH;
		*pDMA0_Y_COUNT = SXGA_HEIGHT;
		break;
	}
}

void camera_start()
{
	*pDMA0_CONFIG |= DMAEN;
	*pPPI_CONTROL |= PORT_EN;
}

void camera_stop()
{
	*pPPI_CONTROL &= ~PORT_EN;
	*pDMA0_CONFIG &= ~DMAEN;
}
