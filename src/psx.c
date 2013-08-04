/**
 * PSX Bus implementation
 */

#include "psx.h"

#include <avr/io.h>
#include <avr/interrupt.h>

/**
 * The following variables are required by the psx_fast file which contains
 * our ISRs so that they run reasonably
 *
 * As an optimization, they work as follows
 * Send:
 * - A byte is queued to be sent by setting sendByte to the value and sendMask to 0x01
 * - If the sendMask != 0 when queuing occurs, it returns false
 * - If sendMask != 0, the ISR ands the byte with the sendmask, sets data accordingly, and shifts sendMask left, no rotate
 */

volatile uint8_t sendByte, sendMask;
volatile uint8_t recvByte, recvMask;
volatile uint8_t flags;

void psx_setup(void)
{
    unsigned char i;
    //reset all flags
    flags = 0;

    //set up buffers
    sendByte = sendMask = 0;
    recvByte = recvMask = 0;
    flags = 0;

    //set up the direction
    PSX_DDR &= ~(PSX_ATT_MASK | PSX_CLK_MASK);
    PSX_PORT |= PSX_ATT_MASK | PSX_CLK_MASK;

    //set up the interrupts
    //we listen for a high to low level transition on ATT (int1)
    //we listen to both levels (falling, we change; rising, we read) (int0)
    EICRA = (1 << ISC11) | (1 << ISC00);
    EIMSK = (1 << INT0) | (1 << INT1);
}

void psx_ack(void)
{
}

char psx_send(uint8_t data)
{
    if (sendMask != 0x00)
    {
        return 0;
    }

    sendByte = data;
    sendMask = 0x01; //tell it to start
}

/**
 * PSX_ATT falls low
 */
ISR(INT1_vect)
{
    if (recvMask != 0x00) {
       PORTB |= 0x08;
    }

    //start receive
    recvByte = 0;
    recvMask = 0x01;
}
