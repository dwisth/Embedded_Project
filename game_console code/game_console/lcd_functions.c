#include "game_console.h"

// Declare the global variable initialised in the game_console.c file.
extern byte frame_buffer[LCD_MAX_COLS][LCD_MAX_PAGES];

/*
	Set all values in the frame buffer array to OFF.
*/
byte clearFrameBuffer()
{
	// Initialise the frame_buffer to all OFF.
	byte row, col;
	for (col=0; col<LCD_MAX_COLS; col++) {
		for (row=0; row<LCD_MAX_ROWS; row++) {
			frame_buffer[col][row] = 0;
		}
	}
	return(TRUE);
}

/*
	Turn off all pixels in the screen.
*/
void clearScreen() 
{
	byte page, col;
	for (page=0; page<LCD_MAX_PAGES; page++) {
		select_page(page);
		for (col=0; col<LCD_MAX_COLS; col++) {
			select_column(col);
			LCD_data_tx(0x00);
		}
	}
}

/*
	Draw a ON-OFF pattern over the screen.
*/
void drawScreenPattern() {
	byte page, col;
	for (page=0; page<LCD_MAX_PAGES; page+=1) {
		select_page(page);
		for (col=0; col<LCD_MAX_COLS; col+=2) {
			select_column(col);
			LCD_data_tx(0xAA|frame_buffer[col][page]);
		}
	}
}

/*
	Select a page on the LCD.
*/
byte select_page(byte page) 
{
	// Make sure page is always within bounds.
	if (page > LCD_MAX_PAGES) {
		page = LCD_MAX_PAGES-1;
	}	
	byte page_cmd_address = LCD_CMD_PAGE | page;
	LCD_command_tx(page_cmd_address);
	return(TRUE);
}

/*
	Select a column of the LCD.
*/
byte select_column(byte col) 
{
	byte col_cmd_address_LSB = LCD_CMD_COL_LSB | (col&0x0F);
	byte col_cmd_address_MSB = LCD_CMD_COL_MSB | (col>>4);
	LCD_command_tx(col_cmd_address_LSB);
	LCD_command_tx(col_cmd_address_MSB);
	return TRUE;
}

/*
	Turn ON or OFF a pixel at a certain row and column of the LCD display.
	Define the origin at the top left corner of the LCD screen. Down is rows, across is columns.
*/
byte writeToPixel(byte row, byte col, byte value) 
{
	// Find the pixel location in terms of page and column number.
	if (col>LCD_MAX_COLS) {
		col = LCD_MAX_COLS - 1;
	}
	if (row>LCD_MAX_ROWS) {
		row = LCD_MAX_ROWS - 1;
	}

	byte page = row/8;
	byte pixel = _BV(row%8);

	// Check the current screen status in frame buffer and add to it.
	SET(frame_buffer[col][page], pixel, value);
	
	// Write the new pixel to the screen.
	select_page(page);
	select_column(col);
	LCD_data_tx(frame_buffer[col][page]);

	return(TRUE);
}

/*
	Check if the pixel at row,col has already been written to.
*/
byte checkForCollision(byte row, byte col) {
	
	// Check for valid inputs.
	if (col<0 || col>LCD_MAX_COLS || row<0 || row>LCD_MAX_ROWS) {
		return (FALSE);
	}
	
	// Check if that pixel has already been written to, according to the frame buffer.
	byte page = row/8;
	byte pixel = _BV(row%8);

	if ( frame_buffer[col][page] & pixel ) {
		return(TRUE);
	} else {
		return (FALSE);
	}

}


/*
	Send a data byte to the LCD.
*/
byte LCD_data_tx(byte tx_byte)
{
	byte tx_processed;
	byte tx_mask = 0x80;
	LCD_CHIP_SELECT(HIGH);
	LCD_DATA;

	// Manually set the SPI clock low and high for each bit.
	while (tx_mask != 0x00) 
	{
		tx_processed = tx_byte & tx_mask;
		SCK_SET(HIGH);

		// MOSI - send one bit of data out.
		if (tx_processed) {
			MOSI_SET(HIGH);
		} else {
			MOSI_SET(LOW);
		}
		SCK_SET(LOW);
		tx_mask>>=1;
	}
	SCK_SET(HIGH);
	LCD_CHIP_SELECT(LOW);
	return(TRUE);

}


/*
	Send a command byte to the LCD.
*/
byte LCD_command_tx(byte tx_byte)
{
	byte tx_processed;
	byte tx_mask = 0x80;
	LCD_CHIP_SELECT(HIGH);
	LCD_COMMAND;

	// Manually set the SPI clock low and high for each bit.
	while (tx_mask != 0x00) 
	{
		tx_processed = tx_byte & tx_mask;
		SCK_SET(HIGH);

		// MOSI - send one bit of data out.
		if (tx_processed) {
			MOSI_SET(HIGH);
		} else {
			MOSI_SET(LOW);
		}
		SCK_SET(LOW);
		tx_mask>>=1;
	}
	SCK_SET(HIGH);
	LCD_CHIP_SELECT(LOW);
	return(TRUE);

}

/*
	Initialise the LCD display using a sequence defined in the LCD-DOGS 102-6E datasheet.
*/
byte LCD_initialise()
{
	LCD_command_tx(0x40); // Display start line 0.
	LCD_command_tx(0xA1); // SEG reverse
	LCD_command_tx(0xC0); // Normal COM0~COM69
	LCD_command_tx(0xA4); // Disable -> Set all pixels to ON.
	LCD_command_tx(0xA6); // Display inverse off.	
	_delay_ms(120);
	LCD_command_tx(0xA2); // LCD bias ratio A2/A3
	LCD_command_tx(0x2F); // Set power control 28..2F
	LCD_command_tx(0x27); // Set VLCD resistor ratio 20..27
	LCD_command_tx(0x81); // Set electronic volume
	LCD_command_tx(0x10); // Set electronic volume 00..3F
	LCD_command_tx(0xFA); // Set advanced program control.
	LCD_command_tx(0x90); // Set advanced program control x00100yz yz column wrap X temp comp.
	LCD_command_tx(0xAF); // Display ON.
	return(TRUE);
}
