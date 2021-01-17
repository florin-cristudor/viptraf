#ifndef VIPTRAF_H
#define VIPTRAF_H

#define EXIT_SUCCESS        0

#define ERROR_PID_FILE      -1
#define ERROR_FORK          -2
#define ERROR_MEMORY        -3
#define ERROR_INIT_VIDEO    -4
#define ERROR_ONLY_ADMIN    -5
#define ERROR_OPTIONS       -6
#define ERROR_DEMONIZE      -7
#define ERROR_GENERAL       -8
#define ERROR_SCREEN_SIZE   -10
#define ERROR_ERRNO         -100

#define exit_program(...) shut_down(__FILE__, __LINE__, __FUNCTION__, __VA_ARGS__)

#endif // VIPTRAF_H
