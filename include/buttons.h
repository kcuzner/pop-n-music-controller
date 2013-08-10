/**
 * Gets the button status bytes as required
 */

#ifndef _BUTTONS_H_
#define _BUTTONS_H_

#include <stdint.h>

/**
 * We are a digital controller
 */
#define PSX_DEVICE_TYPE     0x41

/******
 * PORTB buttons
 ******/

#define BTN_PB_TRIANGLE     (1 << PB0)
#define BTN_PB_CIRCLE       (1 << PB1)

#define BTN_B_MASK          BTN_PB_TRIANGLE | BTN_PB_CIRCLE

/******
 * PORTC buttons
 ******/

#define BTN_PC_R1           (1 << PC0)
#define BTN_PC_X            (1 << PC1)
#define BTN_PC_L1           (1 << PC2)
#define BTN_PC_SQUARE       (1 << PC3)
#define BTN_PC_R2           (1 << PC4)
#define BTN_PC_UP           (1 << PC5)

#define BTN_C_MASK          BTN_PC_R1 | BTN_PC_X | BTN_PC_L1 | BTN_PC_SQUARE | BTN_PC_R2 | BTN_PC_UP

/******
 * PORTD buttons
 ******/

#define BTN_PD_L2           (1 << PD5)
#define BTN_PD_START        (1 << PD6)
#define BTN_PD_SELECT       (1 << PD7)

#define BTN_D_MASK          BTN_PD_L2 | BTN_PD_START | BTN_PD_SELECT


extern uint8_t buttonsL;
extern uint8_t buttonsH;

 /**
 * Bit0 Bit1 Bit2 Bit3 Bit4 Bit5 Bit6 Bit7
 * SLCT           STRT UP   RGHT DOWN LEFT      = buttonsL
 * L2   R2    L1  R1   /\   O    X    |_|       = buttonsH
 *
 * All bits are active high since our MISO is inverted
 */

//buttonsL
#define BUTTON_SLCT     0
#define BUTTON_STRT     3
#define BUTTON_UP       4
#define BUTTON_RIGHT    5
#define BUTTON_DOWN     6
#define BUTTON_LEFT     7

//buttonsH
#define BUTTON_L2       0
#define BUTTON_R2       1
#define BUTTON_L1       2
#define BUTTON_R1       3
#define BUTTON_TRIANGLE 4
#define BUTTON_CIRCLE   5
#define BUTTON_X        6
#define BUTTON_SQUARE   7


/**
 * Button setup function
 */
void buttons_setup(void);

/**
 * Sets the left, right, and down buttons pressed
 */
void buttons_set_lrd_pressed(void);

/**
 * Sets the left, right, and down buttons unpressed
 */
void buttons_reset_lrd_pressed(void);

/**
 * Updates the button bytes which are exposed through this header file
 */
void buttons_update(void);

#endif
