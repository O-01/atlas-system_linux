#ifndef _SIGNALS_H_
#define _SIGNALS_H_

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int handle_signal(void);
void (*current_handler_signal(void))(int);

#endif
