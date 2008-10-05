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

#ifndef JPEG_H_
#define JPEG_H_

typedef enum {
	JPEG_FOUR_ZERO_ZERO,
	JPEG_FOUR_TWO_ZERO,
	JPEG_FOUR_TWO_TWO,
	JPEG_FOUR_FOUR_FOUR,
	JPEG_RGB
} JPEGPixelFormat;

unsigned char *jpeg_image_encode(unsigned char *img_data, unsigned char *output_buff, unsigned int quality, JPEGPixelFormat format, unsigned int width, unsigned int height);

#endif /*JPEG_H_*/
