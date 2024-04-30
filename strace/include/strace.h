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

#define SYSCALL_NAME(x) syscalls_64_g[(x).orig_rax].name

typedef struct user_regs_struct user_regs;

#endif
