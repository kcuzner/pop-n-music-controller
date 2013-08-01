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

#define PSX_PORT        PORTD
#define PSX_ATT_MASK    0x08
#define PSX_CLK_MASK    0x04
#define PSX_DATA_MASK   0x02
#define PSX_CMD_MASK    0x01
#define PSX_ACK_MASK    0x10

/**
 * PSX bus setup function
 */
void psx_setup(void);

/**
 * Sends data via the PSX bus
 * @param data Data character to send
 */
void psx_send(unsigned char data);

/**
 * Called when the att pin is signaled low. This should be implemented wherever
 * the PSX bus is going to be used
 */
void psx_on_att(void);

#endif //_PSX_H_