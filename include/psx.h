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

#include <stdint.h>

#define PSX_PORT        PORTD
#define PSX_PIN         PIND
#define PSX_DDR         DDRD
#define PSX_ATT_MASK    1 << PD3
#define PSX_CLK_MASK    1 << PD2
#define PSX_DATA_MASK   1 << PD1
#define PSX_CMD_MASK    1 << PD0
#define PSX_ACK_MASK    1 << PD4

/**
 * PSX bus setup function
 */
void psx_setup(void);

/**
 * Asks the bus to hold ack low until the next clock cycle
 */
void psx_ack(void);

/**
 * Sends data via the PSX bus
 * @param data Data character to send
 */
char psx_send(uint8_t data);

/**
 * Called when something has been received over the psx bus
 * @param received Byte that was received
 */
void psx_on_recv(uint8_t received);

/**
 * Called when the att pin is signaled low. This should be implemented wherever
 * the PSX bus is going to be used
 */
void psx_on_att(void);

#endif //_PSX_H_