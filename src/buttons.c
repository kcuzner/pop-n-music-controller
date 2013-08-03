#include "buttons.h"

#include <avr/io.h>

struct
{
    unsigned lrdPressed:1;
} status;

static uint8_t state[DATA_LENGTH];

void buttons_setup(void)
{
    status.lrdPressed = 0;

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
    status.lrdPressed = 1;
}

void buttons_reset_lrd_pressed(void)
{
    status.lrdPressed = 0;
}

uint8_t* buttons_get_data(void)
{
    unsigned char i;
    //we get to refresh the state array

    state[0] = 0x5A; //we are about the send data

    /**
     * Bit0 Bit1 Bit2 Bit3 Bit4 Bit5 Bit6 Bit7
     * SLCT           STRT UP   RGHT DOWN LEFT      = state[1]
     * L2   R2    L1  R1   /\   O    X    |_|       = state[2]
     *
     * All bits are active low
     */
    
    state[1] = status.lrdPressed ? 0x1f : 0xff; //initially, just the rdl flag
    state[1] &= PORTC & BTN_PC_UP ? 0xef : 0xff; //read up
    state[1] &= PORTD & BTN_PD_START ? 0xf7 : 0xff; //read start
    state[1] &= PORTD & BTN_PD_SELECT ? 0xfe : 0xff; //read select

    state[2] = 0xff; //default is all that all are unpressed
    state[2] &= PORTC & BTN_PC_SQUARE ? 0x7f : 0xff; //read square
    state[2] &= PORTC & BTN_PC_X ? 0xbf : 0xff; //read x
    state[2] &= PORTB & BTN_PB_CIRCLE ? 0xdf : 0xff; //read circle
    state[2] &= PORTB & BTN_PB_TRIANGLE ? 0xef : 0xff; //read triangle
    state[2] &= PORTB & BTN_PB_R1 ? 0xf7 : 0xff; //read r1
    state[2] &= PORTC & BTN_PC_L1 ? 0xfb : 0xff; //read l1
    state[2] &= PORTC & BTN_PC_R2 ? 0xfd : 0xff; //read r2
    state[2] &= PORTC & BTN_PC_L2 ? 0xfe : 0xff; //read l2

    for(i = 3; i < DATA_LENGTH; i++)
    {
        //we have no further data
        state[i] = 0xff; //idle byte
    }

    return state;
}
