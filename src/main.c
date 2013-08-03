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

int main(void)
{
    return 0;
}

void psx_on_att(void)
{
    //send our greeting
    psx_send(0xFF); //the impression of being idle
    psx_send(0x41); //identifier as a digital pad
    psx_send(0x5a); //ready to send data

    //send button state
    psx_send(0xff);
    psx_send(0xff);
}
