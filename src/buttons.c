#include "buttons.h"

#include <avr/io.h>

#define BUTTONS_STATUS_LRD      0x01

/**
 * Bit0 Bit1 Bit2 Bit3 Bit4 Bit5 Bit6 Bit7
 * SLCT           STRT UP   RGHT DOWN LEFT      = buttonsL
 * L2   R2    L1  R1   /\   O    X    |_|       = buttonsH
 *
 * All bits are active high since our MISO is inverted
 */

//buttonsL
#define BUTTON_SLCT     0
#define BUTTON_STRT     3
#define BUTTON_UP       4
#define BUTTON_RIGHT    5
#define BUTTON_DOWN     6
#define BUTTON_LEFT     7

//buttonsH
#define BUTTON_L2       0
#define BUTTON_R2       1
#define BUTTON_L1       2
#define BUTTON_R1       3
#define BUTTON_TRIANGLE 4
#define BUTTON_CIRCLE   5
#define BUTTON_X        6
#define BUTTON_SQUARE   7

uint8_t buttonsL;
uint8_t buttonsH;

static uint8_t flags;

void buttons_setup(void)
{
    flags = 0;

    buttonsL = 0;
    buttonsH = 0;

    //set everything inputs
    DDRB &= ~BTN_B_MASK;
    DDRC &= ~BTN_C_MASK;
    DDRD &= ~BTN_D_MASK;

    //enable the pullup resistors
    PORTB |= BTN_B_MASK;
    PORTC |= BTN_C_MASK;
    PORTD |= BTN_D_MASK;
}

void buttons_set_lrd_pressed(void)
{
    flags |= BUTTONS_STATUS_LRD;
}

void buttons_reset_lrd_pressed(void)
{
    flags &= ~BUTTONS_STATUS_LRD;
}

void buttons_update(void)
{
    uint8_t tempL;
    uint8_t tempH;

    //we store these here so there aren't volatile...its like a snapshot
    //This allows the compiler to use registers for doing the math here rather
    //than needing to read the port for each question
    uint8_t tPinB = PINB;
    uint8_t tPinC = PINC;
    uint8_t tPinD = PIND;

    //since we have pullups turned on, each of these are pressed when the
    //corrisponding pin is low. Thus, if the AND passes, the button is not
    //pressed
    
    tempL = flags & BUTTONS_STATUS_LRD ? (1 << BUTTON_RIGHT) | (1 << BUTTON_DOWN) | (1 << BUTTON_LEFT) : 0x00; //initially, just the rdl flag
    tempH = 0x00; //default is all that all are unpressed

    //portb first
    tempH |= tPinB & BTN_PB_CIRCLE ? 0x00 : (1 << BUTTON_CIRCLE); //read circle
    tempH |= tPinB & BTN_PB_TRIANGLE ? 0x00 : (1 << BUTTON_TRIANGLE); //read triangle

    //portc
    tempL |= tPinC & BTN_PC_UP ? 0x00 : (1 << BUTTON_UP); //read up
    tempH |= tPinC & BTN_PC_SQUARE ? 0x00 : (1 << BUTTON_SQUARE); //read square
    tempH |= tPinC & BTN_PC_X ? 0x00 : (1 << BUTTON_X); //read x
    tempH |= tPinC & BTN_PC_R1 ? 0x00 : (1 << BUTTON_R1); //read r1
    tempH |= tPinC & BTN_PC_L1 ? 0x00 : (1 << BUTTON_L1); //read l1
    tempH |= tPinC & BTN_PC_R2 ? 0x00 : (1 << BUTTON_R2); //read r2

    //last portd
    tempL |= tPinD & BTN_PD_START ? 0x00 : (1 << BUTTON_STRT); //read start
    tempL |= tPinD & BTN_PD_SELECT ? 0x00 : (1 << BUTTON_SLCT); //read select
    tempH |= tPinD & BTN_PD_L2 ? 0x00 : (1 << BUTTON_L2); //read l2

    //we update here to do it all at once
    buttonsL = tempL;
    buttonsH = tempH;
}
