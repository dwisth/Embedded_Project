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

volatile byte frame_buffer[LCD_MAX_COLS][LCD_MAX_PAGES];
volatile byte temp_row = 30;
volatile byte temp_col = 50;
volatile byte pwm_value = 0;
volatile 

// Interrupt Serivce Routine.

ISR(INT1_vect)
{

	// Software debounce the buttons.
	_delay_ms(1);

	if (ACTION_BUTTON)
	{
		if (pwm_value == 0x00) {
			pwm_value = ON;
		} else if (pwm_value < PWM_STEP) {
			pwm_value = OFF;
		} else {
			pwm_value -= PWM_STEP;
		}
		
		PWM_VALUE(pwm_value);
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
		writeToPixel(temp_row, temp_col, ON);
	}
	
	if (DOWN_BUTTON)
	{
		temp_row++;
		writeToPixel(temp_row, temp_col, ON);
	}

	if (LEFT_BUTTON) 
	{
		temp_col--;
		writeToPixel(temp_row, temp_col, ON);
	}

	if (RIGHT_BUTTON) 
	{
		temp_col++;
		writeToPixel(temp_row, temp_col, ON);
	}

}


int main(void)
{
	setup();
	clearScreen();
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




