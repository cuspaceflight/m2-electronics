/*
 * M2Serial
 * Send and receive packets (and shells) over the serial port.
 * 2015 Adam Greig, Cambridge University Spaceflight
 */

#ifndef M2SERIAL_H
#define M2SERIAL_H

#include "ch.h"
#include "m2telem.h"
#include <stdint.h>

/* Point to a shell running command */
extern void(*m2serial_shell)(BaseSequentialStream*);

/* Process incoming serial data. */
msg_t m2serial_thread(void* arg);

/* Blocking transmit a packet over the serial port. */
void m2serial_send_packet(TelemPacket* packet);

#endif /* M2SERIAL_H */
