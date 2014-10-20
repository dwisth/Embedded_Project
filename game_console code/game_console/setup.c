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
extern byte frame_buffer[LCD_MAX_COLS][LCD_MAX_PAGES];

/*
Function to set up the pins of the ATMEGA and the LCD screen.
*/
void setup() {

	// SPI Intialise
	LCD_RST_DIR(OUT);
	LCD_CHIP_SELECT_DIR(OUT);
	LCD_CD_DIR(OUT);
	SCK_DIR(OUT);
	MOSI_DIR(OUT);

	// LCD SCREEN INITIALISATION
	LCD_RST_DISABLE;
	LCD_initialise();

	// LCD LED PWM Initialise
	PWM_SET_UP;
	LCD_LED_DIR(OUT);

	// BAT LOW LED OUTPUT PINS
	BAT_LOW_LED(OFF); //Make sure it is off before changing direction
	BAT_LOW_LED_DIR(OUT); //Set BATTERY LED I/Os as outputs.

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

	// SET UP THE ADC.
	BAT_VOLTAGE_PIN(IN);
	ADC_MUX_SETUP;
	ADC_ADCSRA_SETUP;

}

