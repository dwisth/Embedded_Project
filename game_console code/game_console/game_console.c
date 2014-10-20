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

// Declare the global frame_buffer and button_state arrays.
volatile byte frame_buffer[LCD_MAX_COLS][LCD_MAX_PAGES];
volatile byte button_state[NUM_BUTTONS] = {0,0,0,0,0,0,0};

/* 
	Interrupt Serivce Routine.
	When there is a change in the interrupt pin state the button values are polled.
*/
ISR(INT1_vect)
{
	// Software debounce the buttons.
	_delay_ms(1);

	button_state[BSTATE_UP] 	= UP_BUTTON;
	button_state[BSTATE_DOWN] 	= DOWN_BUTTON;
	button_state[BSTATE_LEFT] 	= LEFT_BUTTON;
	button_state[BSTATE_RIGHT] 	= RIGHT_BUTTON;
	button_state[BSTATE_A] 		= A_BUTTON;
	button_state[BSTATE_B] 		= B_BUTTON;
	button_state[BSTATE_ACTION] = ACTION_BUTTON;
}

/*
	Main function.
*/
int main(void)
{
	setup();
	clearScreen();
	clearFrameBuffer();

	// Initialise variables.
	byte local_button_state[NUM_BUTTONS];
	byte collision_state = FALSE;
	byte row = INITIAL_CURSOR_ROW, col = INITIAL_CURSOR_COL;
	byte pwm_value = OFF;
	PWM_VALUE(pwm_value);
	byte screen_inverted = FALSE;
	
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
		// INVERT THE LCD DISPLAY.
		if ( local_button_state[BSTATE_B] ) {
			if (screen_inverted) {
				// Uninvert the screen.
				LCD_command_tx(LCD_CMD_NON_INVERTED); // Display inverse off.
				screen_inverted = FALSE;
			} else {
				LCD_command_tx(LCD_CMD_INVERTED); // Display inverse on.
				screen_inverted = TRUE;
			}
			// Delay for 0.5 seconds otherwise it is too quick for the user to select an option.
			int delay; //ms
			for (delay=0; delay<500; delay++) {
				_delay_ms(1);
			}
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
		
		if (collision_state) {
			// If a collision occured draw a pattern over the screen.
			drawScreenPattern();
		} else {
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
		
			// If the cursor has been moved, check for a collision with the previous path.
			if (local_button_state[BSTATE_UP] || local_button_state[BSTATE_DOWN] ||
				local_button_state[BSTATE_LEFT] || local_button_state[BSTATE_RIGHT]) {
				collision_state = checkForCollision(row,col);
			}
			writeToPixel(row, col, ON);
		}
	}
}


/*
	Copies buttons states from global to local array.
	Only accepts arrays of size NUM_BUTTONS.
*/
void copyButtonState(byte src[NUM_BUTTONS], byte dst[NUM_BUTTONS]) 
{
	int i;
	for (i=0; i<NUM_BUTTONS; i++) {
		dst[i] = src[i];
	}
}

/*
	If the battery voltage is less than LOW_BAT_VOLTAGE then turn on the low battery LED.
*/
void checkBatVoltage() 
{

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
