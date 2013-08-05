/**
 * PSX Bus Include file
 *
 * The PSX Bus requires 5 pins:
 *  - att: Will be held low by the PSX during transaction
 *  - clk: Will be clocked by the PSX during transaction
 *  - data: controller->psx data. Latched by PSX on rising clk edge. Changed on
 *          falling clk edge
 *  - cmd: psx->controller data. Latched by controller on rising clk edge.
 *  - ack: Pulled low after controller receives PSX command for at least one
 *         clock cycle
 */

#ifndef _PSX_H_
#define _PSX_H_

#include "psx_pins.h"
#include <stdint.h>
/**
 * PSX bus setup function
 */
void psx_setup(void);

/**
 * PSX Main loop function. Should be called as often as possible so bytes aren't missed
 */
void psx_main(void);

/**
 * Asks the bus to hold ack low until the next clock cycle
 */
void psx_ack(void);

/**
 * Sends data via the PSX bus
 * @param data Data character to send
 */
void psx_send(uint8_t data);

/**
 * Called when something has been received over the psx bus
 * @param received Byte that was received
 */
void psx_on_recv(uint8_t received);

#endif //_PSX_H_