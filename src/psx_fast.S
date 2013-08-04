/**
 * Fast interrupts for psx bus since C can't seem to get it right
 *
 * Assembly is the very definition of speghetti code...hopefully it can be
 * avoided
 */

#include <avr/io.h>
#include "psx_pins.h"

#define sreg_save   r2

;; Byte to send (psx.c)
.extern sendByte
;; Mask for current bit sending (psx.c)
.extern sendMask
;; Byte received (psx.c)
.extern recvByte
;; Mask for current bit receiving (psx.c)
.extern recvMask


/**
 * INT0 vector
 *
 * This happens on any change, this is for reading cmd or writing data
 */
.global INT0_vect
INT0_vect:
    in      sreg_save, _SFR_IO_ADDR(SREG)

    ;; Save working registers
    push    r24
    push    r25

    ;;if ATT is set, we end since its not talking to us
    sbis    _SFR_IO_ADDR(PSX_PIN), PSX_ATT
    rjmp    INT0_vect_done

    ;;if CLK is high, we are reading CMD. Otherwise, we are writing DATA
    sbic    _SFR_IO_ADDR(PSX_PIN), PSX_CLK
    rjmp    INT0_vect_read

    INT0_vect_write:
        ;;load sendByte and AND it with sendMask. If sendMask == 0, we are done
        lds     r25, sendMask
        andi    r25, 0xFF
        breq    INT0_vect_done
        lds     r24, sendByte
        and     r24, r25
        ;;if result of AND is 0, pull data low
        breq    INT0_vect_data_low

        INT0_vect_write_high:
            ;;we allow data to float high by clearing the ddr
            cbi     _SFR_IO_ADDR(PSX_DDR), PSX_DATA
            
            rjmp    INT0_vect_write_shift
        INT0_vect_write_low:
            ;;we bring data low by making it an output and setting it low
            sbi     _SFR_IO_ADDR(PSX_DDR), PSX_DATA
            cbi     _SFR_IO_ADDR(PSX_PORT), PSX_DATA

        INT0_vect_write_shift:
            ;;as a final step to all writes, we shift the mask over (still in r25)
            lsl     r25
            sts     sendMask, r25

            rjmp    INT0_vect_done

    INT0_vect_read:
        ;;load recvMask. If == 0, we are done
        lds     r25, recvMask
        andi    r25, 0xFF
        breq    INT0_vect_done
        ;;we load the send byte
        lds     r24, recvByte
        ;;if cmd is set, we or the mask with the byte and either way, store the result
        sbic    _SFR_IO_ADDR(PSX_PIN), PSX_CMD
        or      r24, r25
        sts     recvByte, r24
        ;;we shift the mask (r25) and store it
        lsl     r25
        ;brne    INT0_vect_read_store
        
        INT0_vect_read_store:
            sts     recvMask, r25

        ;;finishing int0
    INT0_vect_done:
        pop     r25
        pop     r24
        out     _SFR_IO_ADDR(SREG), sreg_save
        reti

