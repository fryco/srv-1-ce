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

#include "syscalls.h"

void _exit(int n)
{
	while(1)
		n = n;
}

int _read(int file, char *ptr, int len)
{
	errno = ENOTSUP;
	return -1;
}

int _lseek(int file, int ptr, int dir)
{
  errno = ENOTSUP;
  return -1;
}

int _write (int file, char *ptr, int len)
{
	int i;
	char *loc = ptr;

	if(file == FILENO_STDOUT)
	{
		for(i = 0; i < len; i++)
		{
			uart_putchar(loc);
			loc++;
		}

		return 0;
	}
	
	errno = ENOTSUP;
	return -1;
}

int _open (const char *path, int flags)
{
	errno = ENOTSUP;
	return -1;
}

int _close(int file)
{
  errno = ENOTSUP;
  return -1;
}

int _fstat(int file, struct stat * st)
{
	  errno = ENOTSUP;
	  return -1;
}

int isatty(int fd)
{
  return 1;
}
