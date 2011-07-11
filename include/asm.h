#ifndef __ASM_H
#define __ASM_t

#define thread_hang()   \
    asm volatile ("1:    hlt\n\tjmp 1b\n" ::)
        
#define inb(port, value) \
    asm volatile ("inb %%dx,%%al\n": "=a"(value): "d"(port))

#define outb(port, value) \
    asm volatile ("outb %%al,%%dx\n"::"a" (value),"d" (port))

#define io_wait() \
    asm ("\tjmp 1f\n1:\tjmp 1f\n1:") 

#define inb_p(port, value)  \
    do {                    \
        inb(port, value);   \
        io_wait();          \
    } while (0);        

#define outb_p(port, value) \
    do {                    \
        outb(port, value);  \
        io_wait();          \
    } while (0);

#define intrs_enable() \
    asm ("\t sti \n")

#define intrs_disable() \
    asm ("\t cli \n")                                                       

#define cpu_halt() \
    asm ("\t hlt \n")

#define stack_pointer(p)    \
    asm ("\t movl %%esp, %0 \n" : "=r"(p));

#define eflags(flags) {     \
    asm ("\t pushf \n");    \
    asm ("\t movl (%%esp), %0 \n" : "=r"(flags));   \
    asm ("\t popf \n");     \
}

#endif // __ASM_H
