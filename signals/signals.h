#ifndef _SIGNALS_H_
#define _SIGNALS_H_

#define _XOPEN_SOURCE 700

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct sigaction sigact_t;

int handle_signal(void);
void (*current_handler_signal(void))(int);
int handle_sigaction(void);
void (*current_handler_sigaction(void))(int);

#endif
