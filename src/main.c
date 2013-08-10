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
 *
 * To switch the mode of the controller, start and select should both be
 * held down for 3 seconds. PD0 will be on for emulator mode, PD1 will be
 * on for PSX mode.
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

    //set up buttons
    buttons_setup();

    //enable interrupts
    sei();

    //PD0 and PD1 are the LEDs
    DDRD |= (1 << PD0) | (1 << PD1);
    

    while(1)
    {
        buttons_update();
        psx_main();
    }

    return 0;
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
            //next thing we send is lower button byte
            psx_send(buttonsL);
        }
    }
    else if (byteNumber == 3)
    {
        if (received == 0x00)
        {
            psx_ack();
            //next thing we send is upper button byte
            psx_send(buttonsH);
        }
    }

    byteNumber++;
}
