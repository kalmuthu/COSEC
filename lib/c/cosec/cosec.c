#include <stdarg.h>
#include <cosec/fs.h>

int syscall(int num, ...) {
    va_list vl;
    va_start(vl, num);

    int arg1 = va_arg(vl, int);
    int arg2 = va_arg(vl, int);
    int arg3 = va_arg(vl, int);

    va_end(vl);

    asm(
    "movl %3, %%ebx         \n"
    "movl %2, %%edx         \n"
    "movl %1, %%ecx         \n"
    "movl %0, %%eax         \n"
    "int $0x80              \n"
    ::"m"(num), "m"(arg1), "m"(arg2), "m"(arg3) );
}

int printf(const char *fmt, ...) {
    return syscall(SYS_PRINT, (void **)&fmt, 0, 0);
}

void exit(int status) {
    syscall(SYS_EXIT, status, 0, 0);
}

