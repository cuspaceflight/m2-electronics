/*
 * M2Serial
 * Send and receive packets (and shells) over the serial port.
 * 2015 Adam Greig, Cambridge University Spaceflight
 */

#ifndef M2SERIAL_H
#define M2SERIAL_H

#include "ch.h"
#include "hal.h"
#include "m2telem.h"
#include <stdint.h>

/* Point to a shell running command */
extern void(*m2serial_shell)(BaseSequentialStream*);

/* Point to the serial driver to use. */
extern SerialDriver *M2SerialSD;

/* Process incoming serial data. */
msg_t m2serial_thread(void* arg);

/* Blocking transmit an arbitrary buffer over the serial port.
 * Performs relevant flagging, escaping and checksumming.
 */
void m2serial_send_buffer(void* buf, size_t n);

#endif /* M2SERIAL_H */
