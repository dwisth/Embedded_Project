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
volatile byte button_state[7] = {0,0,0,0,0,0,0};

// Interrupt Serivce Routine.

ISR(INT1_vect)
{

	// Software debounce the buttons.
	_delay_ms(1);

	button_state[BSTATE_UP] = UP_BUTTON;
	button_state[BSTATE_DOWN] = DOWN_BUTTON;
	button_state[BSTATE_LEFT] = LEFT_BUTTON;
	button_state[BSTATE_RIGHT] = RIGHT_BUTTON;
	button_state[BSTATE_A] = A_BUTTON;
	button_state[BSTATE_B] = B_BUTTON;
	button_state[BSTATE_ACTION] = ACTION_BUTTON;

/*
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
*/
}


int main(void)
{
	
	setup();
	clearScreen();
	clearFrameBuffer();

	byte local_button_state[7];
	byte collision_state = FALSE;
	byte row = INITIAL_CURSOR_ROW, col = INITIAL_CURSOR_COL;
	byte pwm_value = 0x00;
	
	// Create a state machine based on the button values.
	while (TRUE) {
		checkBatVoltage();
		
		// Slow down the game for the user.
		_delay_ms(200);
		// Create a local button state variable to avoid bstate changing during one cycle.
		copyButtonState(button_state, local_button_state);

		/* ADMINISTRATIVE BUTTONS */

		// RESET THE GAME
		if ( local_button_state[BSTATE_A] ) {
			clearFrameBuffer();
			clearScreen();
			collision_state = FALSE;
			row = INITIAL_CURSOR_ROW;
			col = INITIAL_CURSOR_COL;
		} 
		// BUTTON FUNCTION TO BE ADDED.
		if ( local_button_state[BSTATE_B] ) {
			// Do nothing for now.
		} 
		// CHANGE THE BRIGHTNESS OF THE SCREEN
		if ( local_button_state[BSTATE_ACTION] ) {
			if (pwm_value == OFF) {
				pwm_value = ON;
			} else if (pwm_value < PWM_STEP) {
				pwm_value = OFF;
			} else {
				pwm_value -= PWM_STEP;
			}
		
			PWM_VALUE(pwm_value);
		}

		/* GAME PLAY BUTTONS */

		// IF COLLISION, GAME HAS BEEN LOST. WAIT UNTIL USER RESET.
		
		if (!collision_state) {

			// MOVE CURSOR UP.
			if ( local_button_state[BSTATE_UP] ) {
				if (row==0) {
					row = LCD_MAX_ROWS;
				} else {
					row--;
				}
			} 
			// MOVE CURSOR DOWN
			if ( local_button_state[BSTATE_DOWN] ) {
				if (row == LCD_MAX_ROWS) {
					row = 0;
				} else {
				row++;
				}
			} 
			// MOVE CURSOR LEFT
			if ( local_button_state[BSTATE_LEFT] ) {
				if (col==0) {
					col = LCD_MAX_COLS;
				} else {
					col--;
				}
			} 
			// MOVE CURSOR RIGHT
			if ( local_button_state[BSTATE_RIGHT] ) {
				if (col == LCD_MAX_COLS) {
					col = 0;
				} else {
					col++;
				}
			} 
		
			// If the cursor has been moved then check for a collision with the previous path.
			if (local_button_state[BSTATE_UP] || local_button_state[BSTATE_DOWN] ||
				local_button_state[BSTATE_LEFT] || local_button_state[BSTATE_RIGHT]) {
				collision_state = checkForCollision(row,col);
			}
			writeToPixel(row, col, ON);
		}
	}
}


// Functions copies one button state array to another.
void copyButtonState(byte src[7], byte dst[7]) {
	int i;
	for (i=0; i<7; i++) {
		dst[i] = src[i];
	}
}


void checkBatVoltage() {

	double adc_value;
	ADC_START_CONVERSION;
	while (~ADC_CONVERSION_FINISHED){}
	adc_value = ADC_GET_VALUE;

	// Turn ON the battery low LED if the battery voltage is less than 1.1V.
	if (adc_value < LOW_BAT_VOLTAGE) {
		BAT_LOW_LED(ON);
	} else {
		BAT_LOW_LED(OFF);
	}
}
