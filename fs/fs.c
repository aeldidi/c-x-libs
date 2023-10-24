
#if defined(__linux__) || defined(__APPLE__)
// #define _POSIX_C_SOURCE 200809L
#include "./fs_posix.c"
#elif defined(_WIN32)
#error "TODO"
#endif
