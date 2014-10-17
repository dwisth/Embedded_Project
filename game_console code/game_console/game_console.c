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

//volatile byte frame_buffer[LCD_MAX_COLS][LCD_MAX_ROWS];
volatile byte temp_row = 30;
volatile byte temp_col = 50;

// Interrupt Serivce Routine.

ISR(INT1_vect)
{

	// Software debounce the buttons.
	_delay_ms(1);

	if (ACTION_BUTTON)
	{
		LCD_LED(ON);	
	} 	
	else
	{
		LCD_LED(OFF);
	}

	if (A_BUTTON)
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
	
	if (B_BUTTON) {
		byte page, col;
		for (page=0; page<LCD_MAX_PAGES; page+=1) {
			select_page(page);
			for (col=0; col<LCD_MAX_COLS; col+=2) {
				select_column(col);
				LCD_data_tx(0xAA);
			}
		}
	}

	if (UP_BUTTON)
	{
		temp_row--;
		writeToPixel(temp_row, temp_col, 1);
	}
	
	if (DOWN_BUTTON)
	{
		temp_row++;
		writeToPixel(temp_row, temp_col, 1);
	}

	if (LEFT_BUTTON) 
	{
		temp_col--;
		writeToPixel(temp_row, temp_col, 1);
	}

	if (RIGHT_BUTTON) 
	{
		temp_col++;
		writeToPixel(temp_row, temp_col, 1);
	}

}


int main(void)
{
	//temp_row = 30;
	//temp_col = 50;

	setup();

/*
	byte frame_buffer[LCD_MAX_COLS][LCD_MAX_ROWS];
	// Initialise the frame_buffer to all OFF.
	byte row, col;
	for (col=0; col<LCD_MAX_COLS; col++) {
		for (row=0; row<LCD_MAX_ROWS; row++) {
			frame_buffer[col][row] = 0;
		}
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




