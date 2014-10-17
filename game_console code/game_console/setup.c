/*************************************************************************
Title:    Game Console Setup Scripts
Inital Author:   David Jahshan
Extended by : David Wisth 376673
Software: AVR-GCC 
Hardware: ATMEGA16 @ 8Mhz 

DESCRIPTION:
	Functions to intialise the game console from power on.

*************************************************************************/

#include "game_console.h"

void setup() {

	// SPI Intialise
	LCD_CHIP_SELECT_DIR(OUT);
	LCD_CD_DIR(OUT);
	SCK_DIR(OUT);
	MOSI_DIR(OUT);

	// LCD SCREEN INITIALISATION
	LCD_initialise();


	// LCD LED PWM SETUP
	

	// LCD LED BACKLIGHT PINS
	LCD_LED(OFF);
	LCD_LED_DIR(OUT);

	// LED OUTPUT PINS
	BAT_LOW_LED(OFF); //Make sure it is off before changing direction
	BAT_LOW_LED_DIR(OUT); //Set BATTERY LED I/Os as outputs.

	/*
	OCR2 = 0x7F;	// Set backlight at 50%.
	// Bit 7, FOC2 = 0 for PWM mode.
	// Bit 3 & 6 controls the PWM Mode.
	// Bit 4 & 5 compare match output.
	TCCR2
	*/

	// BUTTON INPUT PINS
	UP_BUTTON_DIR(IN); //Set UP_BUTTON I/Os as input.
	UP_BUTTON_PU(ON); //Enable the pull up resistor.

	DOWN_BUTTON_DIR(IN); 
	DOWN_BUTTON_PU(ON); 

	LEFT_BUTTON_DIR(IN); 
	LEFT_BUTTON_PU(ON); 

	RIGHT_BUTTON_DIR(IN);
	RIGHT_BUTTON_PU(ON); 

	A_BUTTON_DIR(IN); 
	A_BUTTON_PU(ON);

	B_BUTTON_DIR(IN); 
	B_BUTTON_PU(ON); 

	ACTION_BUTTON_DIR(IN); 
	ACTION_BUTTON_PU(ON); 

	// ENABLE INTERRUPTS

	GICR_INT1_ENABLE(TRUE);
	INT1_LOGIC_CHANGE;
	sei();
}


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
	LCD_CHIP_SELECT(ON);
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
