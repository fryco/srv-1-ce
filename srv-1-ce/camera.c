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

int temp;

byte buff_waiting_push(byte *data)
{
	disable_interrupts(temp);
	
	// Check for full buffer
	if (waiting_tail >= waiting_ptr)
	{
		if ((waiting_tail - waiting_ptr) >= WAITING_BUFF_SIZE - 1)
		{
			enable_interrupts(temp);
			return(0);
		}
	}
	else
	{
		if ((waiting_tail + WAITING_BUFF_SIZE - waiting_ptr) >= WAITING_BUFF_SIZE - 1)
		{
			enable_interrupts(temp);
			return(0);
		}
	}

	*waiting_tail = *data; /* Store Data */
	waiting_tail++;

	/* Connect Ringbuffer */
	if (waiting_tail >= waiting_buff + WAITING_BUFF_SIZE)
		waiting_tail = waiting_buff;

	enable_interrupts(temp);
	return(1);
}

byte buff_waiting_pull(byte *data)
{
	disable_interrupts(temp);
	
	// Check if there's any data in the buffer
	if (waiting_ptr == waiting_tail)
	{
		enable_interrupts(temp);
		return(0);
	}

	*data = *waiting_ptr;
	waiting_ptr++;

	/* Connect ringbuffer */
	if (waiting_ptr >= waiting_buff + WAITING_BUFF_SIZE)
		waiting_ptr = waiting_buff;

	enable_interrupts(temp);
	return(1);
}

byte buff_filled_push(byte *data)
{
	disable_interrupts(temp);
	
	// Check for full buffer
	if (filled_tail >= filled_ptr)
	{
		if ((filled_tail - filled_ptr) >= FILLED_BUFF_SIZE - 1)
		{
			enable_interrupts(temp);
			return(0);
		}
	}
	else
	{
		if ((filled_tail + FILLED_BUFF_SIZE - filled_ptr) >= FILLED_BUFF_SIZE - 1)
		{
			enable_interrupts(temp);
			return(0);
		}
	}

	*filled_tail = *data; /* Store Data */
	filled_tail++;

	/* Connect Ringbuffer */
	if (filled_tail >= filled_buff + FILLED_BUFF_SIZE)
		filled_tail = filled_buff;
	
	enable_interrupts(temp);
	return(1);
}

byte buff_filled_pull(byte *data)
{
	disable_interrupts(temp);
	
	// Check if there's any data in the buffer
	if (filled_ptr == filled_tail)
	{
		enable_interrupts(temp);
		return(0);
	}

	*data = *filled_ptr;
	filled_ptr++;

	/* Connect ringbuffer */
	if (filled_ptr >= filled_buff + FILLED_BUFF_SIZE)
		filled_ptr = filled_buff;
	
	enable_interrupts(temp);
	return(1);
}

void camera_init()
{
	ubyte send;
	ubyte ret;
	
	curr_frame_number = 0;
	camera_running = 0;
	camera_initialised = 0;
	
	// Initialise ringbuffer
	waiting_tail = waiting_buff;
	waiting_ptr = waiting_buff;
	filled_tail = filled_buff;
	filled_ptr = filled_buff;

	// Check whether the camera is attached
	send = 0xa;
	i2c_write(CAMERA_I2C_ADDRESS, (byte*)&send, 1, 1);
	i2c_read(CAMERA_I2C_ADDRESS, (byte*)&ret, 1, 1);
	product_id = (ret << 8);
	send = 0xb;
	i2c_write(CAMERA_I2C_ADDRESS, (byte*)&send, 1, 1);
	i2c_read(CAMERA_I2C_ADDRESS, (byte*)&ret, 1, 1);
	product_id += ret;

	switch(product_id)
	{
	case OV9655:
		break;
	default:
		return;
	}
	
	// Enable PPI pins
	*pPORTG_FER |= (PG0 | PG1 | PG2 | PG3 | PG4 | PG5 | PG6 | PG7);
	*pPORTF_FER |= (PF8 | PF9 | PF15);

	// Setup the PPI registers
	*pPPI_CONTROL = PACK_EN | (2 << 4) | XFR_TYPE;
	
	camera_initialised = 1;
	
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
	unsigned int i;
	byte j;
	bool was_running;

	if(!camera_initialised)
		return -1;

	was_running = 0;
	
	if(camera_running)
	{
		was_running = 1;
		camera_stop();
	}

	switch(res)
	{
	case QQVGA:
		for(i = 0; i < (sizeof(ov9655_qqvga) / 2); i++)
			i2c_write(CAMERA_I2C_ADDRESS, (byte*)&(ov9655_qqvga[i * 2]), 2, 1);

		curr_width = QQVGA_WIDTH;
		curr_height = QQVGA_HEIGHT;
		break;
	case QVGA:
		for(i = 0; i < (sizeof(ov9655_qvga) / 2); i++)
			i2c_write(CAMERA_I2C_ADDRESS, (byte*)&(ov9655_qvga[i * 2]), 2, 1);

		curr_width = QVGA_WIDTH;
		curr_height = QVGA_HEIGHT;
		break;
	case VGA:
		for(i = 0; i < (sizeof(ov9655_vga) / 2); i++)
			i2c_write(CAMERA_I2C_ADDRESS, (byte*)&(ov9655_vga[i * 2]), 2, 1);

		curr_width = VGA_WIDTH;
		curr_height = VGA_HEIGHT;
		break;
	case SXGA:
		for(i = 0; i < (sizeof(ov9655_sxga) / 2); i++)
			i2c_write(CAMERA_I2C_ADDRESS, (byte*)&(ov9655_sxga[i * 2]), 2, 1);

		curr_width = SXGA_WIDTH;
		curr_height = SXGA_HEIGHT;
		break;
	default:
		return -1;
	}

	switch(pxlfmt)
	{
	case YUYV:
		curr_pixel_format = YUYV;
		curr_bytes_per_pixel = YUYV_BYTES_PER_PIXEL;
		break;
	case RGB565:
		curr_pixel_format = RGB565;
		curr_bytes_per_pixel = RGB565_BYTES_PER_PIXEL;
	default:
		return -1;
	}
	
	// Reallocate the image buffers
	for(i = 0; i < NUMBER_OF_IMAGE_BUFFERS; i++)
	{
		free(img_buffer[i]);
		img_buffer[i] = memalign(16, curr_width * curr_height * curr_bytes_per_pixel);
	}

	// Program the PPI
	*pPPI_COUNT = (curr_width * curr_bytes_per_pixel) - 1;
	*pPPI_FRAME = curr_height;

	// Configure the DMA registers
	*pDMA0_X_COUNT = (curr_width * curr_bytes_per_pixel) / 2; // 8-bit data is packed into 16, so divide by 2
	*pDMA0_Y_COUNT = curr_height;
	*pDMA0_X_MODIFY = curr_bytes_per_pixel;
	*pDMA0_Y_MODIFY = curr_bytes_per_pixel;
	*pDMA0_CONFIG = ( DI_EN | DMA2D | WDSIZE_16 | WNR );
	
	// Add all the buffers to the queue
	for(j = 0; j < NUMBER_OF_IMAGE_BUFFERS; j++)
	{
		buff_waiting_push(&j);
	}
	
	// Add the first buffer addres into the dma
	if(!buff_waiting_pull(&j))
		return -1;
	
	curr_buffer = j;
	*pDMA0_START_ADDR = img_buffer[(ubyte)j];
	
	if(was_running)
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
	byte i;
	
	*pDMA0_IRQ_STATUS |= DMA_DONE;
	//LED_1_TOGGLE;
	
	// Move to the new buffer
	if(buff_waiting_pull(&i))
	{
		*pDMA0_START_ADDR = img_buffer[(ubyte)i];
		buff_filled_push(&curr_buffer);
		curr_buffer = i;
	}
	
	curr_frame_number++;
	
	// Start another DMA transfer
	*pDMA0_CONFIG |= DMAEN;
}

int camera_get_new_frame(Frame *frame)
{
	byte i;
	
	if(!buff_filled_pull(&i))
		return -1;
	
	frame->id = i;
	frame->number = curr_frame_number;
	frame->image.width = curr_width;
	frame->image.height = curr_height;
	frame->image.format = curr_pixel_format;
	frame->image.bytes_per_pixel = curr_bytes_per_pixel;
	frame->image.data = img_buffer[(ubyte)i];
	
	return 1;
}

void camera_return_frame(Frame *frame)
{
	while(!buff_waiting_push(&(frame->id)));
}
