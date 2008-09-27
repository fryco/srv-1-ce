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
			//printf("Frame %d\xd\xa",myframe.number);
			camera_return_frame(&myframe);
		}

		if(uart_getchar(&data))
		{
			uart_putchar(&data);
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
				
				for(j = 0; j < 640 * 480 * 2; j++)
					uart_putchar(&(myframe.image.data[j]));
				
				camera_return_frame(&myframe);
				break;
			case '4':
				camera_get_new_frame(&myframe);
				break;
			case '5':
				camera_return_frame(&myframe);
				break;
			case '6':
				if(camera_get_new_frame(&myframe) == 1)
				{
					for(j = 0; j < 32; j++)
						printf("%d\xd\xa",myframe.image.data[i]);
					
					camera_return_frame(&myframe);
				}
				break;
			case '7':
				printf("PPI_STATUS: %X\xd\xa",*pPPI_STATUS >> 8);
				break;
			case '8':
				j = 0;
				while(1)
					printf("%d\xd\xa",j++);
				break;
			}
		}
	}

	return(0);
}

