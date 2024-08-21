# C - Signals
## Resources
#### Read or watch:
- [Signals](https://en.wikipedia.org/wiki/Signal_%28IPC%29)
- [Asynchrony](https://en.wikipedia.org/wiki/Asynchrony_%28computer_programming%29)
- [Signal sets](https://www.gnu.org/software/libc/manual/html_node/Signal-Sets.html)
#### man or help:
- `signal (7)`
- `signal (2)`
- `sigaction (2)`
- `kill (2)`
## Learning Objectives
At the end of this project, you are expected to be able to [explain to anyone](https://fs.blog/feynman-learning-technique/), **without the help of Google**:
### General
- What is a signal
- Why do they exist
- When are they delivered and by whom
- What are the default actions of signals
- How to set up a handler for a signal
- How to send signals
- What signals can’t be caught
## Requirements
### General
- Allowed editors: `vi`, `vim`, `emacs`
- All your files will be compiled on Ubuntu 14.04 LTS
- Your C programs and functions will be compiled with `gcc 4.8.4` using the flags `-Wall` `-Werror` `-Wextra` and `-pedantic`
- All your files should end with a new line
- A `README.md` file, at the root of the folder of the project is mandatory
- Your code should use the `Betty` style. It will be checked using [betty-style.pl](https://github.com/hs-hq/Betty/blob/main/betty-style.pl) and [betty-doc.pl](https://github.com/hs-hq/Betty/blob/main/betty-doc.pl)
- You are allowed to have more than 5 functions per file
- The prototypes of all your functions should be included in your header file called `signals.h`
- Don’t forget to push your header file
- All your header files should be include guarded
### Allowed functions and system calls
- Unless specified otherwise in a task, you are allowed to use the C standard library
#
## Tasks
#### 0. Handle signal
Write a function that set a handler for the signal `SIGINT`
- Prototype: `int handle_signal(void);`
- Your function must return `0` on success, or `-1` on error
- The program should print `Gotcha! [<signum>]` followed by a new line, every time `Control-C` is pressed (See example below)
    - where `<signum>` must be replaced with the signal number that was caught
- `sigaction`(2) is not allowed
```
alex@~/signals$ cat 0-main.c
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "signals.h"

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    int i;

    if (handle_signal() == -1)
    {
        printf("Failure\n");
        return (EXIT_FAILURE);
    }
    for (i = 0; ; i++)
    {
        printf("[%d] Wait for it ...\n", i);
        sleep(1);
    }
    return (EXIT_SUCCESS);
}
alex@~/signals$ gcc -Wall -Wextra -Werror -pedantic 0-main.c 0-handle_signal.c -o 0-handle_signal
alex@~/signals$ ./0-handle_signal 
[0] Wait for it ...
[1] Wait for it ...
[2] Wait for it ...
^CGotcha! [2]
[3] Wait for it ...
^CGotcha! [2]
[4] Wait for it ...
[5] Wait for it ...
^CGotcha! [2]
[6] Wait for it ...
[7] Wait for it ...
^CGotcha! [2]
[8] Wait for it ...
[9] Wait for it ...
^\Quit (core dumped)
alex@~/signals$
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `signals`
- File: `0-handle_signal.c`
#
#### 1. Current handler - signal
Write a function that retrieves the current handler of the signal SIGINT
- Prototype: `void (*current_handler_signal(void))(int);`
- Your function returns a pointer to the current handler of SIGINT, or `NULL` on failure
- You are not allowed to use `sigaction`(2)
- The handler must be unchanged after calling your function
```
alex@~/signals$ cat 1-main.c 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "signals.h"

/* Our functions */
void print_hello(int);
void set_print_hello(void);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    void (*handler)(int);
    int i;

    handler = current_handler_signal();
    printf("Address of the current handler: %#lx\n", (unsigned long int)handler);

    /* Set 'print_hello()` as the handler for SIGINT */
    set_print_hello();

    handler = current_handler_signal();
    printf("Address of the 'print_hello' function: %#lx\n", (unsigned long int)&print_hello);
    printf("Address of the current handler: %#lx\n", (unsigned long int)handler);

    for (i = 0; ; i++)
    {
        printf("[%d] Wait for it ...\n", i);
        sleep(1);
    }
    return (EXIT_SUCCESS);
}
alex@~/signals$ gcc -Wall -Wextra -Werror -pedantic 1-main.c 1-set_print_hello.c 1-current_handler_signal.c -o 1-current_handler_signal
alex@~/signals$ ./1-current_handler_signal
Address of the current handler: 0
Address of the 'print_hello' function: 0x4006da
Address of the current handler: 0x4006da
[0] Wait for it ...
[1] Wait for it ...
^CHello :)
[2] Wait for it ...
^CHello :)
[3] Wait for it ...
[4] Wait for it ...
^CHello :)
[5] Wait for it ...
^\Quit (core dumped)
alex@~/signals$
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `signals`
- File: `1-current_handler_signal.c`
#
#### 2. Gotta catch them all
Write a function that set a handler for the signal `SIGINT`
- Prototype: `int handle_sigaction(void);`
- Your function must return `0` on success, or `-1` on error
- The program should print `Gotcha! [<signum>]` followed by a new line, every time `Control-C` is pressed (See example below)
    - where `<signum>` must be replaced with the signal number that was caught
- `signal`(2) is not allowed
```
alex@~/signals$ cat 2-main.c 
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "signals.h"

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    int i;

    if (handle_sigaction() == -1)
    {
        printf("Failure\n");
        return (EXIT_FAILURE);
    }
    for (i = 0; ; i++)
    {
        printf("[%d] Wait for it ...\n", i);
        sleep(1);
    }
    return (EXIT_SUCCESS);
}
alex@~/signals$ gcc -Wall -Wextra -Werror -pedantic 2-main.c 2-handle_sigaction.c -o 2-handle_sigaction
alex@~/signals$ ./2-handle_sigaction 
[0] Wait for it ...
[1] Wait for it ...
^CGotcha! [2]
[2] Wait for it ...
[3] Wait for it ...
^CGotcha! [2]
[4] Wait for it ...
^CGotcha! [2]
[5] Wait for it ...
^CGotcha! [2]
[6] Wait for it ...
[7] Wait for it ...
^CGotcha! [2]
[8] Wait for it ...
^\Quit (core dumped)
alex@~/signals$
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `signals`
- File: `2-handle_sigaction.c`
#
#### 3. Current handler - sigaction
Write a function that retrieves the current handler of the signal SIGINT
- Prototype: `void (*current_handler_sigaction(void))(int);`
- Your function returns a pointer to the current handler of SIGINT, or `NULL` on failure
- You have to use the function `sigaction` (`signal` is not allowed)
- The handler must be unchanged after calling your function
```
alex@~/signals$ cat 3-main.c 
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "signals.h"

/* Our functions */
void print_hello(int);
void set_print_hello(void);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    void (*handler)(int);
    int i;

    handler = current_handler_sigaction();
    printf("Address of the current handler: %#lx\n", (unsigned long int)handler);

    /* Set 'print_hello()` as the handler for SIGINT */
    set_print_hello();

    handler = current_handler_sigaction();
    printf("Address of the 'print_hello' function: %#lx\n", (unsigned long int)&print_hello);
    printf("Address of the current handler: %#lx\n", (unsigned long int)handler);

    for (i = 0; ; i++)
    {
        printf("[%d] Wait for it ...\n", i);
        sleep(1);
    }
    return (EXIT_SUCCESS);
}
alex@~/signals$ gcc -Wall -Wextra -Werror -pedantic 3-main.c 3-set_print_hello.c 3-current_handler_sigaction.c -o 3-current_handler_sigaction
alex@~/signals$ ./3-current_handler_sigaction 
Address of the current handler: 0
Address of the 'print_hello' function: 0x4006ea
Address of the current handler: 0x4006ea
[0] Wait for it ...
[1] Wait for it ...
^CHello :)
[2] Wait for it ...
^CHello :)
[3] Wait for it ...
[4] Wait for it ...
^CHello :)
[5] Wait for it ...
^CHello :)
[6] Wait for it ...
^\Quit (core dumped)
alex@~/signals$
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `signals`
- File: `3-current_handler_sigaction.c`
#
#### 4. Who said that?!
Write a function that defines a handler for the signal `SIGQUIT` (`Control-\` in a shell)
- Prototype: `int trace_signal_sender(void);`
- The handler must print `SIGQUIT sent by <pid>` each time a `SIGQUIT` (and only a `SIGQUIT`) is caught
    - Where `<pid>` must be replaced by the PID of the process that sent the signal
- Your function must return `0` on success, or `-1` on error
```
alex@~/signals$ cat 4-main.c
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "signals.h"

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    int i;

    if (trace_signal_sender() == -1)
    {
        printf("Failure\n");
        return (EXIT_FAILURE);
    }
    for (i = 0; ; i++)
    {
        printf("[%d] Wait for it ...\n", i);
        sleep(1);
    }
    return (EXIT_SUCCESS);
}
alex@~/signals$ gcc -Wall -Wextra -Werror -pedantic 4-main.c 4-trace_signal_sender.c -o 4-trace_signal_sender
alex@~/signals$ ./4-trace_signal_sender
[0] Wait for it ...
[1] Wait for it ...
^\SIGQUIT sent by 0
[2] Wait for it ...
^\SIGQUIT sent by 0
[3] Wait for it ...
^\SIGQUIT sent by 0
[4] Wait for it ...
[5] Wait for it ...
^\SIGQUIT sent by 0
[6] Wait for it ...
[7] Wait for it ...
[8] Wait for it ...
SIGQUIT sent by 95337
[9] Wait for it ...
^C
alex@~/signals$
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `signals`
- File: `4-trace_signal_sender.c`
#
#### 5. Description
Write a program that prints a description of a given signal
- Usage: `./describe <signum>`
    - Where `<signum>` is the signal number to be described
    - If the number of arguments is not correct, your program must print `Usage: %s <signum>` (where `%s` is argv[0]), followed by a new line, and exit with `EXIT_FAILURE`
- You’re not allowed to have more than `1` function in your file
- You’re not allowed to have more than `12` lines in your function
- You can assume that if a parameter is given, it will be a number
```
alex@~/signals$ gcc -Wall -Wextra -Werror -pedantic 5-signal_describe.c -o 5-signal_describe
alex@~/signals$ ./5-signal_describe
Usage: ./5-signal_describe <signum>
alex@~/signals$ ./5-signal_describe 1
1: Hangup
alex@~/signals$ ./5-signal_describe 9
9: Killed
alex@~/signals$ ./5-signal_describe 3
3: Quit
alex@~/signals$ ./5-signal_describe 2
2: Interrupt
alex@~/signals$ ./5-signal_describe 0
0: Unknown signal 0
alex@~/signals$ ./5-signal_describe 100
100: Unknown signal 100
alex@~/signals$ ./5-signal_describe 20
20: Stopped
alex@~/signals$
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `signals`
- File: `5-signal_describe.c`
#
#### 6. Catch a single time
Write a program that sets a handler for the signal `SIGINT`, and exits right after the signal is received and handled
- Your program does not take any argument
- Your program should suspend indefinitely until a signal is received
- When a `SIGINT` is received, you must print `Caught %d` (where `%d` must be replaced by the signal number), followed by a new line
- After the first `SIGINT` is received, your program must:
    - Print `Signal received`, followed by a new line
    - Exit with `EXIT_SUCCESS`
- You are not allowed to use the functions `exit`, `sleep` or `_exit`
- You are not allowed to use any kind of loop (`while`, `for`, `do/while`)
```
alex@~/signals$ gcc -Wall -Wextra -Werror -pedantic 6-suspend.c -o 6-suspend
alex@~/signals$ ./6-suspend 
^CCaught 2
Signal received
alex@~/signals$ echo $?
0
alex@~/signals$
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `signals`
- File: `6-suspend.c`
#
#### 7. Sending a signal
Write a program that sends the signal `SIGINT` to a process, given its PID
- Usage: `signal_send <pid>`
    - Where `<pid>` is the PID of the process to send a signal to
    - If the number of arguments is not correct, your program must print `Usage: %s <pid>` (where `%s` is argv[0]), followed by a new line, and exit with `EXIT_FAILURE`
    - `<pid>` won’t be `0`
- Your program must return `EXIT_SUCCESS` on success, or `EXIT_FAILURE`
- You’re not allowed to have more than `1` function in your file
```
alex@~/signals$ cat 7-main.c
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

/**
 * main - Simple program, printing its PID and running infinitely
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    pid_t pid;

    pid = getpid();
    printf("PID: %d\n", (int)pid);
    while (98)
    {
        printf("Waiting ...\n");
        sleep(1);
    }
    return (EXIT_SUCCESS);
}
alex@~/signals$ gcc -Wall -Wextra -Werror -pedantic 7-main.c -o 7-wait_for_it
alex@~/signals$ ./7-wait_for_it 
PID: 98631
Waiting ...
Waiting ...
Waiting ...
Waiting ...
Waiting ...
Waiting ...
Waiting ...
Waiting ...
Waiting ...
Waiting ...
Waiting ...
Waiting ...

alex@~/signals$
```
While the above example is in its `Waiting` state, execute the following, and it should terminate the process above.
```
alex@~/signals$ gcc -Wall -Wextra -Werror -pedantic 7-signal_send.c -o 7-signal_send
alex@~/signals$ ./7-signal_send
Usage: ./7-signal_send <pid>
alex@~/signals$ ./7-signal_send 98631
alex@~/signals$
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `signals`
- File: `7-signal_send.c`
#
#### 8. Sending a signal in a shell
Write a `sh` script that sends the signal `SIGQUIT` to a process, given its PID
- Usage: `signal_send.sh <pid>`
    - Where `<pid>` is the PID of the process to send a signal to
    - If the number of arguments is not correct, your program must print `Usage: %s <pid>` (where `%s` is argv[0]), followed by a new line, and exit with `1`
```
alex@~/signals$ cat 8-main.c
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

/**
 * main - Simple program, printing its PID and running infinitely
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    pid_t pid;

    pid = getpid();
    printf("PID: %d\n", (int)pid);
    while (98)
    {
        printf("Waiting ...\n");
        sleep(1);
    }
    return (EXIT_SUCCESS);
}
alex@~/signals$ gcc -Wall -Wextra -Werror -pedantic 8-main.c -o 8-wait_for_it
alex@~/signals$ ./8-wait_for_it 
PID: 98988
Waiting ...
Waiting ...
Waiting ...
Waiting ...
Waiting ...
Waiting ...
Waiting ...
Waiting ...
Waiting ...

alex@~/signals$
```
While the above example is in its `Waiting` state, execute the following, and it should terminate the process above.
```
alex@~/signals$ sh ./8-signal_send.sh
Usage: ./8-signal_send <pid>
alex@~/signals$ sh ./8-signal_send.sh 98988
alex@~/signals$
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `signals`
- File: `8-signal_send.sh`
#
#### 9. Catch with sh
Write a `sh` script that set a handler for the signals `SIGABRT`, `SIGIO` and `SIGTERM`
- The script must print `Nope` followed by a new line, each time a `SIGABRT`, `SIGIO` or `SIGTERM` is caught
- Your script must contain a shebang
- You’re not allowed to have more than 2 lines in your script
- You’re not allowed to have more than 1 instruction per line
```
alex@~/signals$ cat 9-main.sh
#!/bin/sh
. ./9-handle_signal.sh

echo "PID: $$"

while :
do
    echo "Waiting ..."
    sleep 2
done
alex@~/signals$ sh 9-main.sh
PID: 99440
Waiting ...
Waiting ...
Nope
Waiting ...
Nope
Waiting ...
Nope
Waiting ...
Nope
Waiting ...
Waiting ...
Nope
Waiting ...
Nope
Waiting ...
Waiting ...
Waiting ...
Nope
Waiting ...
Nope
Waiting ...
Waiting ...
Killed
alex@~/signals$
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `signals`
- File: `9-handle_signal.sh`
#
#### 10. Does it exist?
Write a function that tests if a process exists, given its PID
- Prototype: `int pid_exist(pid_t pid);`
- Your function must return `1` if the process with the PID `pid` exists, or `0` otherwise
- You’re not allowed to have more than `1` function in your file
- You’re not allowed to have more than `1` line in your function
- You’re not allowed to include more than `2` headers in your file
- You’re not allowed to include your header file `signals.h`
- You’re not allowed to use the function `getpgid`
```
alex@~/signals$ cat 10-main.c 
#include <stdlib.h>
#include <stdio.h>

#include "signals.h"

/**
 * main - Entry point
 * @argc: Arguments counter
 * @argv: Arguments vector
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, const char *argv[])
{
    pid_t pid;

    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <pid>\n", argv[0]);
        return (EXIT_FAILURE);
    }
    pid = atoi(argv[1]);

    if (pid_exist(pid))
    {
        printf("PID %d exists\n", pid);
    }
    else
    {
        printf("PID %d does not exist\n", pid);
    }
    return (EXIT_SUCCESS);
}
alex@~/signals$ gcc -Wall -Wextra -Werror -pedantic 10-main.c 10-pid_exist.c -o 10-pid_exist
alex@~/signals$ ./10-pid_exist 1
PID 1 exists
alex@~/signals$ ./10-pid_exist 2
PID 2 exists
alex@~/signals$ ./10-pid_exist 1234
PID 1234 does not exist
alex@~/signals$ ./10-pid_exist 98
PID 98 does not exist
alex@~/signals$ ./10-pid_exist 890
PID 890 exists
alex@~/signals$
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `signals`
- File: `10-pid_exist.c`
#
#### 11. Blog post
Write a blog post about signals

Your blog post should cover the following:
- What is a signal
- Why do they exist
- When are they delivered and by whom
- What are the default actions of signals
- What happens to a process when it receives a signal without handling it
- What happens to a process when it receives a signal and handles it

Your posts should have examples and at least one picture, at the top. Publish your blog post on `Medium` or `LinkedIn`, and share it at least on `LinkedIn`.

When done, please add all urls below (blog post, tweet, etc.)

Please, remember that these blogs must be written in English to further your technical ability in a variety of settings.
#
#### Repo:
- GitHub repository: `atlas-system_linux`
#
## ADVANCED TASKS
#### 12. I'm saying it's unkillable.
Write a function that sets up a single handler for all the signals
- Prototype: `void all_in_one(void);`
- You’re not allowed to have more than 2 functions in your file
- You’re not allowed to have more than 10 lines in your functions
- You’re not allowed to use `signal`(2)
- In your handler, you must use the function `psiginfo` to print information about the signal received (see example below)
```
alex@~/signals$ cat 100-main.c
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "signals.h"

/**
 * main - Simple program, printing its PID and running infinitely
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    pid_t pid;

    all_in_one();

    pid = getpid();
    printf("PID: %d\n", (int)pid);
    while (98)
    {
        sleep(2);
    }
    return (EXIT_SUCCESS);
}
alex@~/signals$ gcc -Wall -Wextra -Werror -pedantic 100-main.c 100-all_in_one.c -o 100-all_in_one
alex@~/signals$ ./100-all_in_one
PID: 101656
^CCaught: Interrupt (Signal sent by the kernel 0 0)
^\Caught: Quit (Signal sent by the kernel 0 0)
^ZCaught: Stopped (Signal sent by the kernel 0 0)
Caught: Hangup (Signal sent by kill() 98081 1000)
Caught: Interrupt (Signal sent by kill() 98081 1000)
Caught: Quit (Signal sent by kill() 98081 1000)
Caught: Illegal instruction (Signal sent by kill() [0x3e800017f21])
Caught: Trace/breakpoint trap (Signal sent by kill() 98081 1000)
Caught: Aborted (Signal sent by kill() 98081 1000)
Caught: Bus error (Signal sent by kill() [0x3e800017f21])
Caught: Floating point exception (Signal sent by kill() [0x3e800017f21])
Caught: User defined signal 1 (Signal sent by kill() 98081 1000)
Caught: Segmentation fault (Signal sent by kill() [0x3e800017f21])
Caught: User defined signal 2 (Signal sent by kill() 98081 1000)
Caught: Broken pipe (Signal sent by kill() 98081 1000)
Caught: Alarm clock (Signal sent by kill() 98081 1000)
Caught: Terminated (Signal sent by kill() 98081 1000)
Caught: Stack fault (Signal sent by kill() 98081 1000)
Caught: Child exited (Signal sent by kill() 98081 -1693514734 1000)
Caught: Continued (Signal sent by kill() 98081 1000)
Caught: Stopped (Signal sent by kill() 98081 1000)
Caught: Stopped (tty input) (Signal sent by kill() 98081 1000)
Caught: Stopped (tty output) (Signal sent by kill() 98081 1000)
Caught: Urgent I/O condition (Signal sent by kill() 98081 1000)
Caught: CPU time limit exceeded (Signal sent by kill() 98081 1000)
Caught: File size limit exceeded (Signal sent by kill() 98081 1000)
Caught: Virtual timer expired (Signal sent by kill() 98081 1000)
Caught: Profiling timer expired (Signal sent by kill() 98081 1000)
Caught: Window changed (Signal sent by kill() 98081 1000)
Caught: I/O possible (Signal sent by kill() 4294967394081)
Caught: Power failure (Signal sent by kill() 98081 1000)
Caught: Bad system call (Signal sent by kill() 98081 1000)
Killed
alex@~/signals$
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `signals`
- File: `100-all_in_one.c`
#
#### 13. Sigset
Write a function that initializes a sigset
- Prototype: `int sigset_init(sigset_t *set, int *signals);`
- Where `set` is a pointer to the signal set to initialize
- And `signals` is a `0-terminated` array of int, each one being a signal number
- Your function must return `0` on success, or `-1` on error
```
alex@~/signals$ cat 101-main.c
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>

#include "signals.h"

/**
 * sigset_print - Prints a signal set
 * @set: Set to be printed
 */
void sigset_print(const sigset_t *set)
{
    int sig;
    int cnt;

    cnt = 0;
    for (sig = 1; sig < NSIG; sig++)
    {
        if (sigismember(set, sig))
        {
            cnt++;
            printf("%d (%s)\n", sig, strsignal(sig));
        }
    }
    if (cnt == 0)
        printf("Empty signal set\n");
}

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    sigset_t set;
    int signals[] = {
        SIGINT,
        SIGQUIT,
        SIGSEGV,
        SIGTRAP,
        0
    };

    if (sigset_init(&set, signals) == -1)
    {
        fprintf(stderr, "Failed to init sigset\n");
        return (EXIT_FAILURE);
    }
    sigset_print(&set);
    return (EXIT_SUCCESS);
}
alex@~/signals$ gcc -Wall -Wextra -Werror -pedantic 101-main.c 101-sigset_init.c -o 101-sigset_init
alex@~/signals$ ./101-sigset_init
2 (Interrupt)
3 (Quit)
5 (Trace/breakpoint trap)
11 (Segmentation fault)
alex@~/signals$
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `signals`
- File: `101-sigset_init.c`
#
#### 14. Block signals
Write a function that block a given set of signals from being delivered to the current process
- Prototype: `int signals_block(int *signals);`
- Where `signals` is a `0-terminated` array of int, each one being a signal to block
- Your function must return `0` on success, or `-1` on error
- You’re not allowed to use either `signal`(2) nor `sigaction`(2)
```
alex@~/signals$ cat 102-main.c
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include "signals.h"

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    int signals[] = {
        SIGINT,
        SIGQUIT,
        SIGSEGV,
        SIGTRAP,
        0
    };

    if (signals_block(signals) == -1)
    {
        fprintf(stderr, "Failed to block signals\n");
        return (EXIT_FAILURE);
    }
    while (98)
    {
        sleep(5);
    }
    return (EXIT_SUCCESS);
}
alex@~/signals$ gcc -Wall -Wextra -Werror -pedantic 102-main.c 102-signals_block.c -o 102-signals_block
alex@~/signals$ ./102-signals_block
^C^C^C^C^C^C^\^\^C^\^\^\^C^C^\^\^\Terminated
alex@~/signals$
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `signals`
- File: `102-signals_block.c`
#
#### 15. Unblock signals
Write a function that unblock a given set of signals from being delivered to the current process
- Prototype: `int signals_unblock(int *signals);`
- Where `signals` is a `0-terminated` array of int, each one being a signal to unblock
- Your function must return `0` on success, or `-1` on error
- You’re not allowed to use either `signal`(2) nor `sigaction`(2)
```
alex@~/signals$ cat 103-main.c
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include "signals.h"

/* Our function */
int _signals_block(int *signals);

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    int signals_to_block[] = {
        SIGINT,
        SIGQUIT,
        SIGSEGV,
        SIGTRAP,
        0
    };
    int signals_to_unblock[] = {
        SIGINT,
        SIGSEGV,
        SIGTRAP,
        0
    };

    if (_signals_block(signals_to_block) == -1)
    {
        fprintf(stderr, "Failed to block signals\n");
        return (EXIT_FAILURE);
    }
    if (signals_unblock(signals_to_unblock) == -1)
    {
        fprintf(stderr, "Failed to block signals\n");
        return (EXIT_FAILURE);
    }
    while (98)
    {
        sleep(5);
    }
    return (EXIT_SUCCESS);
}
alex@~/signals$ gcc -Wall -Wextra -Werror -pedantic 103-main.c 103-signals_unblock.c _signals_block.c -o 103-signals_unblock
alex@~/signals$ ./103-signals_unblock
^\^\^\^\^\^\^\^C
alex@~/signals$ ./103-signals_unblock
^C
alex@~/signals$
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `signals`
- File: `103-signals_unblock.c`
#
#### 16. Handle pending signals
Write a function that sets up a handler for all the pending signals of the current process
- Prototype: `int handle_pending(void (*handler)(int));`
- Where `handler` is a pointer to the handler function to set for the pending signals
- Your function must return `0` on success, or `-1` on error
- You’re not allowed to use `signal`(2)
```
alex@~/signals$ cat 104-main.c
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include "signals.h"

/* Our functions */
int _signals_block(int *signals);
int _signals_unblock(int *signals);

/**
 * handler - Handler for pending signals
 * @signum: Signal number
 */
void handler(int signum)
{
    printf("\nCaught signal %d\n", signum);
}

/**
 * main - Entry point
 *
 * Return: EXIT_SUCCESS or EXIT_FAILURE
 */
int main(void)
{
    int signals[] = {
        SIGINT,
        SIGQUIT,
        SIGSEGV,
        SIGTRAP,
        0
    };

    if (_signals_block(signals) == -1)
    {
        fprintf(stderr, "Failed to block signals\n");
        return (EXIT_FAILURE);
    }

    sleep(5);
    if (handle_pending(&handler) == -1)
    {
        printf("Failed to set up handler for pending signals\n");
        return (EXIT_FAILURE);
    }

    if (_signals_unblock(signals) == -1)
    {
        fprintf(stderr, "Failed to unblock signals\n");
        return (EXIT_FAILURE);
    }
    return (EXIT_SUCCESS);
}
alex@~/signals$ gcc -Wall -Wextra -Werror -pedantic 104-main.c 104-handle_pending.c _signals_block.c _signals_unblock.c -o 104-handle_pending
alex@~/signals$ ./104-handle_pending
^C^C^C^\^\^C^\^\^C^\^\^C
Caught signal 3

Caught signal 2
alex@~/signals$
```
#
#### Repo:
- GitHub repository: `atlas-system_linux`
- Directory: `signals`
- File: `104-handle_pending.c`
#
