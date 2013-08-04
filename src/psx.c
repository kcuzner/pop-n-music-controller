/**
 * PSX Bus implementation
 */

#include "psx.h"

#include <avr/io.h>
#include <avr/interrupt.h>

static const uint8_t ATT_SIGNALED = 1 << 1;
static const uint8_t RECEIVED_READY = 1 << 2;

static volatile uint8_t sendSize, sendIndex, sendMask;
static volatile uint8_t sendBuffer[5];
static volatile uint8_t recvMask, recvBuffer;
static volatile uint8_t received;

static volatile uint8_t flags;

void psx_setup(void)
{
    //reset all flags
    flags = 0;

    //set up buffers
    sendSize = sendIndex = 0;
    sendMask = 0x01;
    recvBuffer = received = 0;
    recvMask = 0x01;

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
    if (sendSize == 5) {
        return 0;
    }

    sendBuffer[sendSize] = data;

    sendSize++;

    return sendSize;
}

/**
 * PSX_ATT falls low
 */
ISR(INT1_vect)
{
    //reset our transmission state
    sendSize = 0; //we set this first so thing is transmitted
    sendIndex = 0;
    sendMask = 0x01;

    if (recvMask == 0x20) {
        PORTB = 0x08;
    }

    //reset our receive state
    recvBuffer = 0;
    recvMask = 0x01;

    //notify that we have had our attention called
    //psx_on_att();
}

/**
 * PSX_CLK rises high or falls low
 */
ISR(INT0_vect)
{
    if (!(PSX_PIN & PSX_ATT_MASK))
    {
        //this isn't meant for us
        return;
    }

    if (PSX_PIN & PSX_CLK_MASK)
    {
        //rising edge: we read cmd
        if (PSX_PIN & PSX_CMD_MASK)
        {
            //recvBuffer |= recvMask;
        }

        if (recvMask == 0x80) {
            
        }

        if (recvMask == 0x80) //we finished
        {
            received = recvBuffer; //copy what we got
            recvBuffer = 0; //reset the buffer
            recvMask = 0x01; //reset the mask
            //flags |= RECEIVED_READY;
        }
        else
        {
            recvMask = recvMask << 1; //shift over for next bit
        }
    }
    else
    {
        //falling edge: we write data
        if (sendIndex >= sendSize)
        {
            //nothing to send
            return;
        }

        if (sendBuffer[sendIndex] & sendMask)
        {
            //data becomes input with pull up
            PSX_DDR &= ~PSX_DATA_MASK;
            PSX_PORT |= PSX_DATA_MASK;
        }
        else
        {
            //data pulled low
            PSX_DDR |= PSX_DATA_MASK;
            PSX_PORT &= ~PSX_DATA_MASK;
        }

        if (sendMask == 0x80)
        {
            //next byte
            sendMask = 0x01;
            sendIndex++;
        }
        else
        {
            //increment send mask
            sendMask <<= 1;
        }
    }
}
