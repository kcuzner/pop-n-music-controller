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

volatile uint8_t flags;
volatile uint8_t received;
volatile uint8_t ackCounter;

void psx_setup(void)
{
    //reset all flags
    flags = 0;

    //set up buffers
    received = 0;
    ackCounter = 0;

    //set up spi for slave mode
    DDRB |= (1 < PB2); //miso is output
    //enable spi interrupt, enable spi, data order reversed, read on rising edge, clock polarity inverted
    SPCR = (1 << SPIE) | (1 << SPE) | (1 << DORD) | (1 << CPOL) | (1 << CPHA);
}

void psx_main(void)
{
    if (flags & PSX_FLAG_RECVD)
    {
        psx_on_recv(received);
    }

    flags = 0;
}

void psx_ack(void)
{
    //we want it to go low for 10uS
    //source of this number:
    //  2uS is clock period for 500Khz and 10uS is clock period for 100Khz
    //  Since 2uS * 8 == 16 > 10, we can use 10uS and avoid bleeding into the
    //  next byte.
}

char psx_send(uint8_t data)
{
    return 1;
}

/**
 * Serial transfer complete on SPI
 */
ISR(SPI_STC_vect)
{
    received = SPDR;
    flags |= PSX_FLAG_RECVD;
}
