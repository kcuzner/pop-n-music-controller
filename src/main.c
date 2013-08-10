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

#define PC_INT_MASK2        ((1 << PCINT22) | (1 << PCINT23))
#define SWITCH_MASK_D       ((1 << PD6) | (1 << PD7))

#if (BTN_PD_SELECT | BTN_PD_START) != SWITCH_MASK_D
#error Select and start are not located on PD6 and PD7
#endif

//a 3 second delay at clk/1024 when clk = 8Mhz means that it requires 0x5B8D
//clock cycles to do that. Subtracting that from 0xffff, we get 0xa472 for the
//initial counter value. If the count can complete without being interrupted,
//then the buttons have been pressed for a sufficient amount of time
#define DELAY_3S            0xA472
#define CLK_SOURCE          ((1 << CS12) | (1 << CS10))


#define EMU_FLAG            0x01

uint8_t flags;

void sync_status()
{
    if (flags & EMU_FLAG)
    {
        //we are in emulator mode. PD0 only is lit
        PORTD &= ~(1 << PD1);
        PORTD |= (1 << PD0);
        buttons_reset_lrd_pressed();
    }
    else
    {
        //we are not in emulator mode. PD1 only is lit
        PORTD &= ~(1 << PD0);
        PORTD |= (1 << PD1);
        buttons_set_lrd_pressed();
    }
}

int main(void)
{
    //by default, we are in emulator mode
    flags = EMU_FLAG;

    //set up psx
    psx_setup();

    //set up buttons
    buttons_setup();

    //initially, we are in emulator mode: LRD unpressed
    sync_status();

    //pin change interrupts on PD6 and PD7 (start & select)
    PCMSK2 |= PC_INT_MASK2;
    PCICR |= (1 << PCIE2);

    //we use timer1 for handling the 3 second wait
    TIMSK1 |= (1 << TOIE1);

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

ISR(PCINT2_vect)
{
    if ((~PIND & SWITCH_MASK_D) == SWITCH_MASK_D)
    {
        //the two switches are pressed
        if (!(TCCR1B & (1 << CS12 | 1 << CS11 | 1 << CS10)))
        {
            TCNT1 = DELAY_3S;
            TCCR1B |= CLK_SOURCE;
            //both leds are turned on
            PORTD |= (1 << PD0) | (1 << PD1);
        }
    }
    else
    {
        //we stop the timer...it will star again later
        sync_status(); //reset our status
        TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
    }
}

ISR(TIMER1_OVF_vect)
{
    //if we managed to overflow, we switch modes and switch off the clock
    flags ^= EMU_FLAG;
    sync_status();
    TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
}
