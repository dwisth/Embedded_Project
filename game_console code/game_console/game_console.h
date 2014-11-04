/*************************************************************************
Title:    Game Console Template Header File
Inital Author:   David Jahshan
Extended by : David Wisth 376673
Software: AVR-GCC 
Hardware: ATMEGA16 @ 8Mhz 

DESCRIPTION:
	Macros for Game Console

*************************************************************************/

#include <avr/io.h> 
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h> // For _delay_ms()f

#define byte unsigned char 

/* ON OFF */
#define ON 0xFF
#define OFF 0x00
#define HIGH 0xFF
#define LOW 0x00
#define IN 0x00
#define OUT 0xFF
#define ALL 0xFF
#define TRUE 1
#define FALSE 0
#define FORWARD 0x00
#define BACK 0xFF

/* SET and GET MACROS */
#define SET(PORT,MASK,VALUE) PORT = ((MASK & VALUE) | (PORT & ~MASK))
#define GET(PORT,MASK) PORT & MASK


/* BUTTONS */

// DIRN macros
#define UP_BUTTON_DIR(DIR) 		SET(DDRA,_BV(PA4),DIR)
#define DOWN_BUTTON_DIR(DIR) 	SET(DDRA,_BV(PA7),DIR)
#define LEFT_BUTTON_DIR(DIR) 	SET(DDRA,_BV(PA5),DIR)
#define RIGHT_BUTTON_DIR(DIR) 	SET(DDRA,_BV(PA6),DIR)
#define A_BUTTON_DIR(DIR) 		SET(DDRC,_BV(PC6),DIR)
#define B_BUTTON_DIR(DIR) 		SET(DDRC,_BV(PC1),DIR)
#define ACTION_BUTTON_DIR(DIR) 	SET(DDRC,_BV(PC0),DIR)

// SET macros
#define UP_BUTTON_PU(STATE) 		SET(PORTA,_BV(PA4),STATE)
#define DOWN_BUTTON_PU(STATE) 		SET(PORTA,_BV(PA7),STATE)
#define LEFT_BUTTON_PU(STATE) 		SET(PORTA,_BV(PA5),STATE)
#define RIGHT_BUTTON_PU(STATE) 		SET(PORTA,_BV(PA6),STATE)
#define A_BUTTON_PU(STATE) 			SET(PORTC,_BV(PC6),STATE)
#define B_BUTTON_PU(STATE) 			SET(PORTC,_BV(PC1),STATE)
#define ACTION_BUTTON_PU(STATE) 	SET(PORTC,_BV(PC0),STATE)

// GET macros (buttons are active low)
#define UP_BUTTON 			~GET(PINA,_BV(PA4))
#define DOWN_BUTTON 		~GET(PINA,_BV(PA7))
#define LEFT_BUTTON 		~GET(PINA,_BV(PA5))
#define RIGHT_BUTTON 		~GET(PINA,_BV(PA6))
#define A_BUTTON 			~GET(PINC,_BV(PC6))
#define B_BUTTON 			~GET(PINC,_BV(PC1))
#define ACTION_BUTTON 		~GET(PINC,_BV(PC0))


/* BATTERY LOW LED */

// LED is active LOW.
#define BAT_LOW_LED_DIR(DIR) 	SET(DDRB,_BV(PB0),DIR)
#define BAT_LOW_LED(STATE) 		SET(PORTB,_BV(PB0),~STATE) 
#define BAT_LOW_LED_STATE 		GET(PINB,_BV(PB0))


/* INTERRUPTS */

// External Interrupts
#define GICR_INT1_ENABLE SET(GICR, _BV(INT1), ON)
//http://www.avr-tutorials.com/interrupts/The-AVR-8-Bits-Microcontrollers-External-Interrupts
#define INT1_RISING_EDGE MCUCR = 1<<ISC11 | 1<<ISC10;	
#define INT1_ANY_LOGIC_CHANGE MCUCR = 0<<ISC11 | 1<<ISC10;

// Timer Interrupts - for screen refreshing

// Disable PWM output, set CLK divider at 256 in normal mode.
#define TIMER_INT_ENABLE SET(TIMSK, _BV(TOIE0), ON)
//#define TIMER_INT_BEGIN SET(TCCR0, _BV(CS02), ON)
// Divider at 1024.
#define TIMER_INT_BEGIN SET(TCCR0, (_BV(CS02)|_BV(CS00)), ON)

/* PWM */

// Make the PWM toggle on compare match.
#define LCD_LED_DIR(DIR) 	SET(DDRD,_BV(PD7),DIR)
#define PWM_SET_UP 			SET(TCCR2,(_BV(WGM21)) | (_BV(WGM20)) | (_BV(COM21)) | (_BV(CS20)), ON)
#define PWM_VALUE(VALUE) 	OCR2 = VALUE
#define PWM_STEP 			16

/* SET UP SPI INTERFACE */

// Pin directions
#define LCD_CS_DIR(DIR) 	SET(DDRB,_BV(PB1), DIR)
#define LCD_CD_DIR(DIR) 	SET(DDRB,_BV(PB4), DIR)
#define LCD_RST_DIR(DIR) 	SET(DDRB,_BV(PB3), DIR)
#define FRAM_WP_DIR(DIR)	SET(DDRD,_BV(PD4), DIR)
#define FRAM_CS_DIR(DIR)	SET(DDRD,_BV(PD5), DIR)
#define FRAM_HOLD_DIR(DIR)	SET(DDRD,_BV(PD6), DIR)
#define SCK_DIR(DIR) 		SET(DDRB,_BV(PB7), DIR)
#define MOSI_DIR(DIR)	 	SET(DDRB,_BV(PB5), DIR)
#define MISO_DIR(DIR)	 	SET(DDRB,_BV(PB6), DIR)

// Pin states
#define LCD_CS_SET(STATE) 		SET(PORTB,_BV(PB1),~STATE)
#define LCD_COMMAND 			SET(PORTB,_BV(PB4),OFF)
#define LCD_DATA				SET(PORTB,_BV(PB4),ON)
#define FRAM_WP_SET(STATE) 		SET(PORTD,_BV(PD4),~STATE)
#define FRAM_CS_SET(STATE) 		SET(PORTD,_BV(PD5),~STATE)
#define FRAM_HOLD_SET(STATE) 	SET(PORTD,_BV(PD6),~STATE)
#define SCK_SET(STATE) 			SET(PORTB,_BV(PB7),STATE)
#define MOSI_SET(STATE) 		SET(PORTB,_BV(PB5),STATE)
#define LCD_RST_SET(STATE) 		SET(PORTB,_BV(PB3),~STATE)

// Enable the SPI bus as master with fastest clock rate.
#define SPI_ENABLE  			SET(SPCR,( _BV(SPE)|_BV(MSTR) ),ON)
#define SPI_SEND_DATA(DATA)		SPDR = DATA
#define SPI_SEND_DATA_COMPLETE 	(SPSR & _BV(SPIF))
#define SPI_RECEIVE_DATA		SPDR

/* FRAM COMMANDS */

// Note: There is 9 bit FRAM addresses!
#define FRAM_WRITE_ENABLE 				0x06
#define FRAM_WRITE_DISABLE				0x04
#define FRAM_READ_STATUS_REG			0x05
#define FRAM_WRITE_STATUS_REG			0x01
#define FRAM_READ_MEMORY_DATA(ADDRESS_MSB) 		(0x03 | (ADDRESS_MSB*0x08) )
#define FRAM_WRITE_MEMORY_DATA(ADDRESS_MSB) 	(0x02 | (ADDRESS_MSB*0x08) )

#define FRAM_FRAME_BUFFER_START_MSB 	0
#define FRAM_FRAME_BUFFER_START_LSB 	0x00

#define FRAM_WRITE 0
#define FRAM_READ  1


/* LCD MACROS */

// LCD Screen constants.
#define LCD_MAX_PAGES 8
#define LCD_MAX_COLS 102
#define LCD_MAX_ROWS 64
#define LCD_CMD_PAGE 0xB0
#define LCD_CMD_COL_LSB 0x00
#define LCD_CMD_COL_MSB 0x10
#define INITIAL_CURSOR_ROW 32
#define INITIAL_CURSOR_COL 51
#define LCD_CMD_NON_INVERTED 0xA6
#define LCD_CMD_INVERTED 0xA7


/* GAME PLAY VARIABLES */
#define NUM_BUTTONS 7
#define BSTATE_UP 0
#define BSTATE_DOWN 1
#define BSTATE_LEFT 2
#define BSTATE_RIGHT 3
#define BSTATE_A 4
#define BSTATE_B 5
#define BSTATE_ACTION 6


#define GAME_STATE_IDLE 0
#define GAME_STATE_UP 1
#define GAME_STATE_DOWN 2
#define GAME_STATE_LEFT 3
#define GAME_STATE_RIGHT 4

#define GAME_LOOP_DELAY 200

/* ADC */
// Set up pin.
#define BAT_VOLTAGE_PIN(DIR) SET(DDRA,_BV(PA3),DIR)

// Internal reference, values Left adjusted, only care about AD3.
#define ADC_MUX_SETUP ADMUX = _BV(REFS0) | _BV(ADLAR) | _BV(MUX1) | _BV(MUX0)
// Enable ADC, 
#define ADC_ADCSRA_SETUP ADCSRA = _BV(ADEN)
#define ADC_START_CONVERSION ADCSRA |= _BV(ADSC)
#define ADC_CONVERSION_FINISHED ~(ADCSRA & _BV(ADSC))
// Only read the MSB, don't worry about last 2 bits.
// Conversion factor on page 216 of datasheet.
#define ADC_GET_VALUE ((double)(ADCH<<2))*3.3/1024
// Low battery voltage
#define LOW_BAT_VOLTAGE 1.1




/* FUNCTION DEFINITIONS */

void setup();
byte LCD_data_tx(byte tx_byte);
//byte LCD_data_tx_bit_bash(byte tx_byte);
byte LCD_command_tx(byte tx_byte);
//byte LCD_command_tx_bit_bash(byte tx_byte);
byte LCD_initialise();
byte select_page(byte page);
byte select_column(byte col);
byte addPixelToFrameBuffer(byte row, byte col, byte value) ;
void clearScreen();
void drawScreenPattern();
void drawFrameBuffer();
byte clearFrameBuffer();
void copyButtonState(byte src[NUM_BUTTONS], byte dst[NUM_BUTTONS]);
void checkBatVoltage();
byte checkForCollision(byte row, byte col);
byte FRAM_read_byte (byte address_msb, byte address_byte);
void FRAM_write_byte(byte address_msb, byte address_byte, byte tx_byte);
byte FRAM_read_sr();
void FRAM_write_sr(byte cmd);
void FRAM_write_enable();


void writeNumToScreen(byte num);
