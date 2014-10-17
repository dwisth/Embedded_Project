#include "game_console.h"

// Select a page.
byte select_page(byte page) 
{
	// Make sure page is always within bounds.
	if (page > LCD_MAX_PAGES)
	{
		page = LCD_MAX_PAGES-1;
	}	
	byte page_cmd_address = LCD_CMD_PAGE | page;
	LCD_command_tx(page_cmd_address);
	return TRUE;
}

// Select a column.
byte select_column(byte col) 
{
	byte col_cmd_address_LSB = LCD_CMD_COL_LSB | (col&0x0F);
	byte col_cmd_address_MSB = LCD_CMD_COL_MSB | (col>>4);
	LCD_command_tx(col_cmd_address_LSB);
	LCD_command_tx(col_cmd_address_MSB);
	return TRUE;
}

// Select a row and column to turn on and off a pixel.



// Ideally, the screen automatically refreshes from the frame buffer.



// Send a data byte to the LCD.
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


// Send a data byte to the LCD.
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
