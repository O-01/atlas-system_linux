#ifndef _STRACE_H_
#define _STRACE_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/ptrace.h>
/* #include <sys/reg.h> */
#include <sys/user.h>
/* #include <sys/syscall.h> */
#include <sys/wait.h>
#include <unistd.h>
/* #include <asm/unistd_32.h> */
/* #include <asm/unistd_64.h> */

#include "syscalls.h"

#define GETREGS(x) ptrace(PTRACE_GETREGS, pid, NULL, &(x))
#define ALT (!alt || (alt & 1))
#define SYSCALLNAME(x) syscalls_64_g[(x).orig_rax].name
#define SYSCALLNO(x) (x).orig_rax
#define SUPPORTED(x) (SYSCALLNO(x) < 318)
#define PARAM_COUNT(x) syscalls_64_g[(x).orig_rax].nb_params
/* #define HEX_PREFIX(x) (!x ? "" : "0x") */
#define SYSCALLPARAM(x, y) syscalls_64_g[(x).orig_rax].params[y]

typedef struct user_regs_struct user_regs;

#endif
