/**
 * Gets the button status bytes as required
 */

#ifndef _BUTTONS_H_
#define _BUTTONS_H_

#include <stdint.h>

/**
 * We are a digital controller
 */
#define PSX_DEVICE_TYPE     0x42
/**
 * We have 3 bytes of response, including the 0x5A introduction byte
 */
#define DATA_LENGTH         3

/******
 * PORTB buttons
 ******/

#define BTN_PB_TRIANGLE     0x01
#define BTN_PB_CIRCLE       0x02

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
 * Returns the button data
 * @return      Pointer to an array of DATA_LENGTH length
 */
uint8_t* buttons_get_data(void);

#endif