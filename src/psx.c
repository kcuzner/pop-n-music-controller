/**
 * PSX Bus implementation
 */

#include "psx.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#define PSX_DDR_IN_MASK     ~(PSX_ATT_MASK | PSX_CLK_MASK | PSX_CMD_MASK)
#define PSX_DDR_OUT_MASK    PSX_DATA_MASK | PSX_ACK_MASK

unsigned char buffer[5];
/**
 * Holds the send state for the psx bus
 */
struct
{
    /**
     * Length of the buffer
     */
    unsigned size:3;
    /**
     * Current byte to send
     */
    unsigned index:3;
    /**
     * Mask for the current bit to send
     */
    unsigned char sendMask;
} sendState;

void psx_setup(void)
{
    sendState.size = sendState.index = 0;
    sendState.sendMask = 0x01; //we send lsb first, shifting left

    //set up the direction
    PSX_DDR &= PSX_DDR_IN_MASK;
    PSX_DDR |= PSX_DDR_OUT_MASK;

    //set up the interrupts
    //we listen for a high to low level transition on ATT (int1)
    //we listen to both levels (falling, we change; rising, we read) (int0)
    EICRA = ISC11 | ISC00;
    EIMSK = INT1 | INT0;

}

/**
 * Sets the data line (1)
 */
static void set_data(void)
{
    //we set the data to be an input
    PSX_DDR &= ~PSX_DATA_MASK;
    PSX_PORT |= PSX_DATA_MASK; //pull-up enabled
}

/**
 * Resets the data line (0)
 */
static void reset_data(void)
{
    //we set the data to be an output
    PSX_DDR |= PSX_DATA_MASK;
    PSX_PORT &= ~PSX_DATA_MASK; //pull it low
}

char psx_send(unsigned char data)
{
    if (sendState.size == 5) {
        return 0;
    }

    buffer[sendState.size] = data;

    sendState.size++;

    return sendState.size;
}

ISR(INT1_vect)
{
    //reset our transmission state
    sendState.size = 0; //we set this first so thing is transmitted
    sendState.index = 0;
    sendState.sendMask = 0x01;

    //notify that we have had our attention called
    psx_on_att();
}

ISR(INT0_vect)
{

    if (PSX_PORT & PSX_CLK_MASK)
    {
        //rising edge: we can receive
    }
    else
    {
        if (sendState.index >= sendState.size)
        {
            //we have nothing to send
            return;
        }

        //falling edge: we send
        if (buffer[sendState.index] & sendState.sendMask)
        {
            //it is high
            set_data();
        }
        else
        {
            //it is low
            reset_data();
        }

        //we now increment our things
        if (sendState.sendMask == 0x80)
        {
            //we have completed a byte and need to reset and increment
            sendState.sendMask = 0x01;
            sendState.index++;
        }
        else
        {
            sendState.sendMask = sendState.sendMask << 1;
        }
    }
}
