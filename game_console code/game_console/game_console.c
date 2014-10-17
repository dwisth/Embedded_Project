/*************************************************************************
Title:    game_console Template
Initial Author:   David Jahshan
Extended by : DAVID WISTH 376673
Software: AVR-GCC 
Hardware: ATMEGA16 @ 8Mhz 

DESCRIPTION:
	Main and functions for Game Console basic sample code

*************************************************************************/


#include "game_console.h" 

// Interrupt Serivce Routine.

ISR(INT1_vect)
{

	// Software debounce the buttons.
	_delay_ms(1);

	if (UP_BUTTON)
	{
		LCD_LED(ON);	
	} 	
	else
	{
		LCD_LED(OFF);
	}

	if (DOWN_BUTTON)
	{
		byte page, col;
		for (page=0; page<LCD_MAX_PAGES; page++) {
			select_page(page);
			for (col=0; col<LCD_MAX_COLS; col++) {
				select_column(col);
				LCD_data_tx(0x00);
			}
		}
	} else {
		byte page, col;
		for (page=0; page<LCD_MAX_PAGES; page+=1) {
			select_page(page);
			for (col=0; col<LCD_MAX_COLS; col+=2) {
				select_column(col);
				LCD_data_tx(0xAA);
			}
		}


	}
}




int main(void)
{
	
	setup();
/*
	select_page(3);
	select_column(50);
	LCD_data_tx(0xFF);

	select_column(51);
	LCD_data_tx(0xFF);

	select_column(52);
	LCD_data_tx(0xFF);

	select_column(53);
	LCD_data_tx(0xFF);

	select_column(54);
	LCD_data_tx(0xFF);

	select_column(55);
	LCD_data_tx(0xFF);
*/
	

/*
	while (TRUE)//Master loop always true so always loop
	{
		//GICR_INT1_ENABLE(TRUE);
		BAT_LOW_LED(ON);
		_delay_ms(500);	
		BAT_LOW_LED(OFF);
		_delay_ms(500);
		//Turn on the LED if UP_BUTTON is pressed
		

	}
*/
	
}

// Program structure with interrrupts.
/*
#define BSTATE_UP 0
#define BSTATE_DOWN 1
#define BSTATE_LEFT 2
#define BSTATE_RIGHT 3
#define BSTATE_A 4
#define BSTATE_B 5
#define BSTATE_ACTION 6
#define BSTATE_LEFT 7
#define BSTATE_IDLE 8

static volatile button_state = 0;

For the etch-a-sketch game we need: UDLR, clear and backlight buttons.

static volatile button_state[7];
ISR(...) {
	button_state[0] = UP_BUTTON;
	button_state[1] = DOWN_BUTTON;
	button_state[2] = LEFT_BUTTON;
	button_state[3] = RIGHT_BUTTON;
	button_state[4] = A_BUTTON;
	button_state[5] = B_BUTTON;
	button_state[6] = ACTION_BUTTON;
}

*/

