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

/*ON OFF*/
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

/*SET and GET MACRO*/
#define SET(PORT,MASK,VALUE) PORT = ((MASK & VALUE) | (PORT & ~MASK))
#define GET(PORT,MASK) PORT & MASK

//Devices Direction MACROs
//#define MULTI_LINES_DIR(DIR) SET(DDR?,(_BV(P?#)|_BV(P?#)|_BV(P?#)),DIR)
#define BAT_LOW_LED_DIR(DIR) SET(DDRB,_BV(PB0),DIR)
#define LCD_LED_DIR(DIR) SET(DDRD,_BV(PD7),DIR)

// BUTTON MACROS
#define UP_BUTTON_DIR(DIR) SET(DDRA,_BV(PA4),DIR)
#define UP_BUTTON_PU(STATE) SET(PORTA,_BV(PA4),STATE)

#define DOWN_BUTTON_DIR(DIR) SET(DDRA,_BV(PA7),DIR)
#define DOWN_BUTTON_PU(STATE) SET(PORTA,_BV(PA7),STATE)

#define LEFT_BUTTON_DIR(DIR) SET(DDRA,_BV(PA5),DIR)
#define LEFT_BUTTON_PU(STATE) SET(PORTA,_BV(PA5),STATE)

#define RIGHT_BUTTON_DIR(DIR) SET(DDRA,_BV(PA6),DIR)
#define RIGHT_BUTTON_PU(STATE) SET(PORTA,_BV(PA6),STATE)

#define A_BUTTON_DIR(DIR) SET(DDRC,_BV(PC6),DIR)
#define A_BUTTON_PU(STATE) SET(PORTC,_BV(PC6),STATE)

#define B_BUTTON_DIR(DIR) SET(DDRC,_BV(PC1),DIR)
#define B_BUTTON_PU(STATE) SET(PORTC,_BV(PC1),STATE)

#define ACTION_BUTTON_DIR(DIR) SET(DDRC,_BV(PC0),DIR)
#define ACTION_BUTTON_PU(STATE) SET(PORTC,_BV(PC0),STATE)

//Devices Outputs
//#define MULTI_LINES(STATE) SET(DDR?,(_BV(P?#)|_BV(P?#)|_BV(P?#)),DIR)

// LED is active LOW.
#define BAT_LOW_LED(STATE) SET(PORTB,_BV(PB0),~STATE)

// LCD is active HIGH.
#define LCD_LED(STATE) SET(PORTD,_BV(PD7),STATE)

//Devices Inputs
#define UP_BUTTON ~GET(PINA,_BV(PA4))
#define DOWN_BUTTON ~GET(PINA,_BV(PA7))
#define LEFT_BUTTON ~GET(PINA,_BV(PA5))
#define RIGHT_BUTTON ~GET(PINA,_BV(PA6))
#define A_BUTTON ~GET(PINC,_BV(PC6))
#define B_BUTTON ~GET(PINC,_BV(PC1))
#define ACTION_BUTTON ~GET(PINC,_BV(PC0))

// Interrupts
#define GICR_INT1_ENABLE(INPUT) GICR |= INPUT<<INT1
#define GICR_INT2_ENABLE(INPUT) GICR |= INPUT<<INT2
#define GIFR_INT1_ENABLE(INPUT) GIFR |= INPUT<<INTF1
#define GIFR_INT2_ENABLE(INPUT) GIFR |= INPUT<<INTF2
// Trigger INT1 on any logic change.
//http://www.avr-tutorials.com/interrupts/The-AVR-8-Bits-Microcontrollers-External-Interrupts
//#define INT0_RISING_EDGE MCUCR = 1<<ISC11 | 1<<ISC10;
#define INT1_RISING_EDGE MCUCR = 1<<ISC11 | 1<<ISC10;	
#define INT1_LOGIC_CHANGE MCUCR = 0<<ISC11 | 1<<ISC10;

// SPI Interface.
#define CS_LCD PB1
#define CD_LCD PB4
#define MOSI PB5
#define MISO PB6
#define SCK PB7
#define RST_LCD PB3
#define WP_FRAM PD4
#define CS_FRAM PD5
#define HOLD_FRAM PD6

/* LCD MACROS */
#define LCD_CHIP_SELECT
#define LCD_CHIP_DESELECT
#define LCD_COMMAND
#define LCD_DATA
#define SCK_SET_HIGH
#define SCK_SET_LOW
#define MOSI_SET_HIGH
#define MOSI_SET_LOW


/* MISC MACROS */


// FUNCTION DEFINITIONS

void setup();
byte LCD_data_tx(byte tx_byte);
byte LCD_initialise(void);

