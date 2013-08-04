/**
 * Pop 'n Music Controller
 *
 * This functions as a dual-mode PS1 controller for the game Pop 'n Music. The
 * buttons are mapped as follows:
 * - Emulator mode:
 *   - 9 Buttons mapped in a way compatible with the emulators
 *   - left, down, and right buttons not pressed
 * - Playstation mode:
 *   - 9 Buttons mapped the way that they are supposed to be
 *   - left, down, and right buttons are pressed   
 */

#include "psx.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
    //set up psx
    psx_setup();

    //enable interrupts
    sei();

    //we will use pb3 as a flash test
    DDRB |= 0x08; //pb3 output

    while(1);

    return 0;
}

void psx_on_att(void)
{
}

void psx_on_recv(uint8_t received)
{
    if (received == 0x01)
    {
    }
}
