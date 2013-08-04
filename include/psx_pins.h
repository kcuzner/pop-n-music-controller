/**
 * Pins file for psx bus
 */

#ifndef _PSX_PINS_H_
#define _PSX_PINS_H_

#define PSX_PORT        PORTD
#define PSX_PIN         PIND
#define PSX_DDR         DDRD

#define PSX_ACK         PD4

#define PSX_ACK_MASK    1 << PD4

#define PSX_FLAG_RECVD  0x01
#define PSX_FLAG_SENT   0x02

#endif