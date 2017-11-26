/*
 *  Dell AIO Serial Backlight Driver
 *
 *  Copyright (C) 2017 AceLan Kao <acelan.kao@canonical.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 */

#ifndef _DELL_UART_BACKLIGHT_H_
#define _DELL_UART_BACKLIGHT_H_

enum {
	DELL_UART_GET_FIRMWARE_VER,
	DELL_UART_GET_BRIGHTNESS,
	DELL_UART_SET_BRIGHTNESS,
	DELL_UART_SET_BACKLIGHT_POWER,
};

struct dell_uart_bl_cmd {
	unsigned char	cmd[10];
	unsigned char	ret[80];
	unsigned short	tx_len;
	unsigned short	rx_len;
};

static struct dell_uart_bl_cmd uart_cmd[] = {
	/*
	 * Get Firmware Version: Tool uses this command to get firmware version.
	 * Command: 0x6A 0x06 0x8F (Length:3 Type: 0x0A, Cmd:6 Checksum:0x8F)
	 * Return data: 0x0D 0x06 Data checksum (Length:13,Cmd:0x06,
	 * 	        Data :F/W version(APRILIA=APR27-VXXX,PHINE=PHI23-VXXX),
	 * 	        checksum:SUM(Length and Cmd and Data)xor 0xFF .
	 */
	[DELL_UART_GET_FIRMWARE_VER] = {
		.cmd	= {0x6A,0x06,0x8F},
		.tx_len	= 3,
	},
	/*
	 * Get Brightness level: Application uses this command for scaler to get brightness.
	 * Command: 0x6A 0x0C 0x89 (Length:3 Type: 0x0A, Cmd:0x0C, Checksum:0x89)
	 * Return data: 0x04 0x0C Data checksum
	 * (Length:4 Cmd: 0x0C Data: brightness level checksum: SUM(Length and Cmd and Data)xor 0xFF)
	 *           brightness level which ranges from 0~100.
	 */
	[DELL_UART_GET_BRIGHTNESS] = {
		.cmd	= {0x6A,0x0C,0x89},
		.ret	= {0x04,0x0C,0x00,0x00},
		.tx_len	= 3,
		.rx_len	= 4,
	},
	/* Set Brightness level: Application uses this command for scaler to set brightness.
	 * Command: 0x8A 0x0B Byte2 Checksum (Length:4 Type: 0x0A, Cmd:0x0B)
	 * 	    where Byte2 is the brightness level which ranges from 0~100.
	 * Return data: 0x03 0x0B 0xF1(Length:3,Cmd:B,checksum:0xF1)
	 * Scaler must send the 3bytes ack within 1 second when success, other value if error
	 */
	[DELL_UART_SET_BRIGHTNESS] = {
		.cmd	= {0x8A,0x0B,0x0,0x0},
		.ret	= {0x03,0x0B,0xF1},
		.tx_len	= 4,
		.rx_len	= 3,
	},
	/*
	 * Screen ON/OFF Control: Application uses this command to control screen ON or OFF.
	 * Command: 0x8A 0x0E Byte2 Checksum (Length:4 Type: 0x0A, Cmd:0x0E) where
	 * 	    Byte2=0 to turn OFF the screen.
	 * 	    Byte2=1 to turn ON the screen
	 * 	    Other value of Byte2 is reserved and invalid.
	 * Return data: 0x03 0x0E 0xEE(Length:3,Cmd:E,checksum:0xEE)
	 */
	[DELL_UART_SET_BACKLIGHT_POWER] = {
		.cmd	= {0x8A,0x0E,0x00,0x0},
		.ret	= {0x03,0x0E,0xEE},
		.tx_len	= 4,
		.rx_len	= 3,
	},
};

#endif /* _DELL_UART_BACKLIGHT_H_ */
