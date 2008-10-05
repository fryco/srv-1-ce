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

#include "srv-1-ce.h"
#include "jpeg.h"



void init(void)
{
	pll_init();
	sdram_init();
	led_init();
	timer_init();
	uart_init();
	i2c_init();
	camera_init();
	motor_init();
	laser_init();
}

int main(void)
{
	Frame myframe;
	timer *mytimer;
	byte data;
	ubyte *data2;
	int i = 0;
	unsigned int j;

	init();

	mytimer = timer_get_new();
	
	camera_start();
	
	while(1)
	{
		ubyte *jpeg_output_buffer_start;
		ubyte *jpeg_output_buffer_end;
		
		/* 
		 * Life LED
		 * If this stops flashing, somethings probably gone wrong
		 */
		if(!timer_get(mytimer))
		{
			timer_set(mytimer, 500);
			LED_0_TOGGLE;
		}
		
		if(camera_get_new_frame(&myframe) == 1)
		{
			// Do the blob detection and extraction
			
			camera_return_frame(&myframe);
		}

		if(uart_getchar(&data))
		{
			//uart_putchar(&data);
			switch(data)
			{
			case 'q':
				motor_left_level_set_percentage(0);
				motor_right_level_set_percentage(0);
				break;
			case 'w':
				motor_left_level_set_percentage(100);
				motor_right_level_set_percentage(100);
				break;
			case 's':
				motor_left_level_set_percentage(-100);
				motor_right_level_set_percentage(-100);
				break;
			case 'a':
				motor_left_level_set_percentage(-100);
				motor_right_level_set_percentage(100);
				break;
			case 'd':
				motor_left_level_set_percentage(100);
				motor_right_level_set_percentage(-100);
				break;
			case 'z':
				LASER_LEFT_TOGGLE;
				LASER_RIGHT_TOGGLE;
				break;
			case '1':
				camera_start();
				printf("Camera started\xd\xa");
				break;
			case '2':
				camera_stop();
				printf("Camera stopped\xd\xa");
				break;
			case '3':
				while(camera_get_new_frame(&myframe) != 1);
				
				jpeg_output_buffer_start = (ubyte*)memalign(16, 640*480*2);
				
				if(jpeg_output_buffer_start != NULL)
				{	
					// Do the JPEG Compress

					jpeg_output_buffer_end = jpeg_image_encode(myframe.image.data, jpeg_output_buffer_start, 2, JPEG_FOUR_TWO_TWO, myframe.image.width, myframe.image.height);

					ubyte mytmp;
					
					mytmp = (jpeg_output_buffer_end - jpeg_output_buffer_start) & 0xFF;
					uart_putchar((byte*)&mytmp);
					mytmp = ((jpeg_output_buffer_end - jpeg_output_buffer_start) & 0xFF00) >> 8;
					uart_putchar((byte*)&mytmp);
					mytmp = ((jpeg_output_buffer_end - jpeg_output_buffer_start) & 0xFF0000) >> 16;
					uart_putchar((byte*)&mytmp);
					
					for(i = 0; i < (int)(jpeg_output_buffer_end - jpeg_output_buffer_start); i++)
						uart_putchar((byte*)&(jpeg_output_buffer_start[i]));
			
					free(jpeg_output_buffer_start);
				}
				
				camera_return_frame(&myframe);
				break;
			case '4':
				camera_init();
				break;
			}
		}
	}

	return(0);
}

