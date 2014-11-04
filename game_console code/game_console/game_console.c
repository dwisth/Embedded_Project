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
volatile byte game_state = GAME_STATE_IDLE;

/* 
	External Interrupt method.
	Update the game state when the button state changes.
*/
ISR(INT1_vect)
{
	// Software debounce the buttons.
	_delay_ms(1);

	button_state[BSTATE_UP]		= UP_BUTTON;
	button_state[BSTATE_DOWN] 	= DOWN_BUTTON;
	button_state[BSTATE_LEFT] 	= LEFT_BUTTON;
	button_state[BSTATE_RIGHT] 	= RIGHT_BUTTON;
	button_state[BSTATE_A] 		= A_BUTTON;
	button_state[BSTATE_B] 		= B_BUTTON;
	button_state[BSTATE_ACTION] = ACTION_BUTTON;

	if (button_state[BSTATE_UP]) {
		game_state = GAME_STATE_UP;
	}
	else if (button_state[BSTATE_DOWN]) {
		game_state = GAME_STATE_DOWN;
	}
	else if (button_state[BSTATE_LEFT]) {
		game_state = GAME_STATE_LEFT;
	}
	else if (button_state[BSTATE_RIGHT]) {
		game_state = GAME_STATE_RIGHT;
	}
}

ISR(TIMER0_OVF_vect) {
	//drawFrameBuffer();

	//checkBatVoltage();
}


/*
	Main function.
*/
int main(void)
{
	SET(DDRD, _BV(PD7), ON);
	LCD_CD_DIR(OUT);
	SCK_DIR(OUT);
	MOSI_DIR(OUT);
	MISO_DIR(IN);

	LCD_CS_SET(OFF);
	FRAM_HOLD_SET(OFF);
	FRAM_CS_SET(OFF);
	FRAM_WP_SET(OFF);
	// SS pin!
	BAT_LOW_LED_DIR(OUT);

	SPI_ENABLE;
	
	byte rx_data;

	rx_data = FRAM_read_sr();
	FRAM_write_enable();
	rx_data = FRAM_read_sr();

	while (TRUE) {};
/*
	FRAM_write_enable();

	// ~~~~~~~~~~~~~~~~ BEGIN TEST CODE  ~~~~~~~~~~~~~~~~~~~~~~

	byte tx_byte, rx_byte = OFF;;
	while (TRUE) {
		tx_byte = 0x03;

		rx_byte = FRAM_read_sr();

		//writeNumToScreen(rx_byte);

		FRAM_write_enable();
		FRAM_write_byte(FRAM_FRAME_BUFFER_START_MSB, FRAM_FRAME_BUFFER_START_LSB, tx_byte);
		rx_byte = FRAM_read_byte(FRAM_FRAME_BUFFER_START_MSB, FRAM_FRAME_BUFFER_START_LSB);

		BAT_LOW_LED(rx_byte);

		//writeNumToScreen(rx_byte);
		
		
		//_delay_ms(GAME_LOOP_DELAY);
		//_delay_ms(GAME_LOOP_DELAY);
		//_delay_ms(GAME_LOOP_DELAY);
		//_delay_ms(GAME_LOOP_DELAY);
		//_delay_ms(GAME_LOOP_DELAY);
		

		tx_byte = 0x04;
		FRAM_write_enable();
		FRAM_write_byte(FRAM_FRAME_BUFFER_START_MSB, FRAM_FRAME_BUFFER_START_LSB, tx_byte);
		rx_byte = FRAM_read_byte(FRAM_FRAME_BUFFER_START_MSB, FRAM_FRAME_BUFFER_START_LSB);

		BAT_LOW_LED(rx_byte);

		//writeNumToScreen(rx_byte);

		
	}
	
	// ~~~~~~~~~~~~~~~~ END TEST CODE ~~~~~~~~~~~~~~~~~~~~~~ */






/*
	setup();
	clearScreen();
	clearFrameBuffer();
	

	// Initialise variables.
	byte local_button_state[NUM_BUTTONS];
	byte collision_state = FALSE;
	byte row = INITIAL_CURSOR_ROW, col = INITIAL_CURSOR_COL;
	byte pwm_value = OFF;
	byte screen_inverted = FALSE;
	
	PWM_VALUE(pwm_value);



	// Create a state machine based on the button values.
	while (TRUE) {
		
		// Slow down the game for the user.
		_delay_ms(GAME_LOOP_DELAY);
		// Create a local button state variable to avoid bstate changing during one cycle.
		copyButtonState(button_state, local_button_state);

		// ADMINISTRATIVE BUTTONS //

		// RESET THE GAME
		if ( local_button_state[BSTATE_A] ) {
			clearFrameBuffer();
			clearScreen();
			collision_state = FALSE;
			row = INITIAL_CURSOR_ROW;
			col = INITIAL_CURSOR_COL;
			game_state = GAME_STATE_IDLE;
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
			_delay_ms(GAME_LOOP_DELAY);
			_delay_ms(GAME_LOOP_DELAY);
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

		// GAME PLAY BUTTONS //

		// IF COLLISION, GAME HAS BEEN LOST. WAIT UNTIL USER RESET.
		
		if (collision_state) {
			// If a collision occured draw a pattern over the screen.
			drawScreenPattern();
		} else {
			// MOVE CURSOR UP.
			if ( game_state == GAME_STATE_UP ) {
				if (row==0) {
					row = LCD_MAX_ROWS;
				} else {
					row--;
				}
			} 
			// MOVE CURSOR DOWN
			if ( game_state == GAME_STATE_DOWN ) {
				if (row == LCD_MAX_ROWS) {
					row = 0;
				} else {
				row++;
				}
			} 
			// MOVE CURSOR LEFT
			if ( game_state == GAME_STATE_LEFT ) {
				if (col==0) {
					col = LCD_MAX_COLS;
				} else {
					col--;
				}
			} 
			// MOVE CURSOR RIGHT
			if ( game_state == GAME_STATE_RIGHT ) {
				if (col == LCD_MAX_COLS) {
					col = 0;
				} else {
					col++;
				}
			} 
		
			// If the cursor has been moved, check for a collision with the previous path.
			if ( game_state != GAME_STATE_IDLE ) {
				collision_state = checkForCollision(row,col);
			}
			addPixelToFrameBuffer(row, col, ON);
			drawFrameBuffer();
		}
	}
	*/
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



/*
	Send a data byte to the FRAM and receive a byte at the same time.
*/
void FRAM_write_byte(byte address_high, byte address_low, byte tx_byte) {
	
	FRAM_CS_SET(HIGH);

	SPI_SEND_DATA(FRAM_WRITE_MEMORY_DATA);
	while (!SPI_SEND_DATA_COMPLETE){};

	SPI_SEND_DATA(address_high);
	while (!SPI_SEND_DATA_COMPLETE){};

	SPI_SEND_DATA(address_low);
	while (!SPI_SEND_DATA_COMPLETE){};

	SPI_SEND_DATA(tx_byte);
	while (!SPI_SEND_DATA_COMPLETE){};

	FRAM_CS_SET(LOW);
}

byte FRAM_read_byte(byte address_high, byte address_low) {
		
	FRAM_CS_SET(HIGH);

	SPI_SEND_DATA(FRAM_READ_MEMORY_DATA);
	while (!SPI_SEND_DATA_COMPLETE){};

	SPI_SEND_DATA(address_high);
	while (!SPI_SEND_DATA_COMPLETE){};

	SPI_SEND_DATA(address_low);
	while (!SPI_SEND_DATA_COMPLETE){};

	// Send a dummy byte to start the SPI bus.
	SPI_SEND_DATA(0xBB);
	while (!SPI_SEND_DATA_COMPLETE){};

	byte rx_data = SPI_RECEIVE_DATA;

	FRAM_CS_SET(LOW);

	return rx_data;
}

void FRAM_write_enable() {
	FRAM_CS_SET(HIGH);
	SPI_SEND_DATA(FRAM_WRITE_ENABLE);
	while (!SPI_SEND_DATA_COMPLETE){};
	FRAM_CS_SET(LOW);
}

void FRAM_write_sr(byte cmd) {
	FRAM_CS_SET(HIGH);
	SPI_SEND_DATA(FRAM_WRITE_STATUS_REG);
	while (!SPI_SEND_DATA_COMPLETE){};
	SPI_SEND_DATA(cmd);
	while (!SPI_SEND_DATA_COMPLETE){};
	FRAM_CS_SET(LOW);
}

/*
	Note: Reading the SPIF (SPI Interrupt Flag register) clears the flag.
*/
byte FRAM_read_sr() {

	FRAM_CS_SET(HIGH);

	SPI_SEND_DATA(FRAM_READ_STATUS_REG);
	while (!SPI_SEND_DATA_COMPLETE){};

	// Send dummy byte to enable SPI.
	SPI_SEND_DATA(0xAA);
	while (!SPI_SEND_DATA_COMPLETE){};

	byte rx_data = SPI_RECEIVE_DATA;
	FRAM_CS_SET(LOW);

	return rx_data;
}
