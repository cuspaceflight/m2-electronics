#ifndef PYRO_H
#define PYRO_H

#include "ch.h"
msg_t pyro_thread(void* arg);



extern bool pyro_cont;
extern bool pyro_fired;

void pyro_fire(void);

#endif
