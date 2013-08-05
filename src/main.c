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
#include "buttons.h"

#include <stdint.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int main(void)
{
    //set up psx
    psx_setup();

    //enable interrupts
    sei();

    //we will use pd0 as a flash test
    DDRD |= (1 << PD0);
    

    while(1)
    {
        psx_main();
    }

    return 0;
}

void psx_on_att(void)
{
}

void psx_on_recv(uint8_t received)
{
    static uint8_t byteNumber = 0;

    if (received == 0x01)
    {
        byteNumber = 0; //reset back to start again
        psx_ack();
        //the next byte we send is our device type
        psx_send(~PSX_DEVICE_TYPE);
    }

    if (byteNumber == 1)
    {
        if (received == 0x42)
        {
            psx_ack();
            //the next byte we send is our preamble
            psx_send(0xa5);
        }
    }
    else if (byteNumber == 2)
    {
        if (received == 0x00)
        {
            psx_ack();
            //next thing we send is upper button byte
            psx_send(0x00);
        }
    }
    else if (byteNumber == 3)
    {
        if (received == 0x00)
        {
            psx_ack();
            //next thing we send is lower button byte
            psx_send(0x00);
            PORTD |= (1 << PD0);
        }
    }

    byteNumber++;
}
