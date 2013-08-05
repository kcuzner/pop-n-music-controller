/**
 * PSX Bus implementation
 */

#include "psx.h"

#include <avr/io.h>
#include <avr/interrupt.h>

/**
 * Initial value for tmr0 giving us a 10uS delay
 */
const uint8_t tmr0Initial = 255 - (F_CPU / 100000);

static volatile uint8_t flags;
static volatile uint8_t received;

void psx_setup(void)
{
    //reset all flags
    flags = 0;

    //set up buffers
    received = 0;

    //set up ack
    PSX_DDR |= (1 << PSX_ACK);

    //set up spi for slave mode
    DDRB |= (1 << PB4); //miso is output
    //enable spi interrupt, enable spi, data order reversed, read on rising edge, clock polarity inverted
    SPCR = (1 << SPIE) | (1 << SPE) | (1 << DORD) | (1 << CPOL) | (1 << CPHA);

    //enable timer interrupts
    TIMSK0 = (1 << TOIE0);
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
    
    PSX_PORT |= (1 << PSX_ACK); //turn on ack
    TCNT0 = 0;
    TCCR0B = (1 << CS00); //no prescaler
}

void psx_send(uint8_t data)
{
    SPDR = data;
}

/**
 * Serial transfer complete on SPI
 */
ISR(SPI_STC_vect)
{
    received = SPDR;
    flags |= PSX_FLAG_RECVD;
}

ISR(TIMER0_OVF_vect)
{
    //turn off the timer
    TCCR0B = 0;
    PSX_PORT &= ~(1 << PSX_ACK); //turn on ack
}
