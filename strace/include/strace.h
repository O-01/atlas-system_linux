#ifndef _STRACE_H_
#define _STRACE_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

#include "syscalls.h"

#define ALT (!alt || (alt & 1))
#define SYSCALLNAME(x) syscalls_64_g[(x).orig_rax].name
#define SYSCALLNO(x) (x).orig_rax
#define SUPPORTED(x) (SYSCALLNO(x) < 318)

typedef struct user_regs_struct user_regs;

#endif
