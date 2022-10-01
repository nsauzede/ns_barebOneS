//#include <stdbool.h>
//#include <stddef.h>
//#include <stdint.h>

#include "libc.h"
#include "gdt.h"
#include "idt.h"
#include "vid.h"
#include "ioport.h"
#include "kb.h"
#include "panic.h"
#include "types.h"

#pragma pack(1)
typedef struct {
    uint32_t gs;
    uint32_t fs;
    uint32_t es;
    uint32_t ds;
    uint32_t ss;
    uint16_t filler;
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;
} irq_regs_t;

typedef struct {
    uint32_t ebp;
    uint32_t zero;
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
} irq_ctx_t;
#pragma pack()

static int kbhits = 0;

#define KB_SIZE 20
static unsigned char kb_ring[KB_SIZE];
static int kb_head = 0;
static int kb_tail = 0;
static void kb_handler(int id, void *ptr) {
    id = id; ptr = ptr;
    unsigned char byte;
    byte = inb(KB_PORT);
    kb_ring[kb_head % KB_SIZE] = byte;
    kb_head = (kb_head + 1) % KB_SIZE;
    if (kb_head == kb_tail) {
        panic("kb_handler overflow");
    }
    kbhits++;
}

static int jiffies = 0;
static void timer_handler(int id, void *ptr) {
    irq_regs_t *regs = (irq_regs_t *)(ptr - sizeof(irq_regs_t));
    irq_ctx_t *ctx = (irq_ctx_t *)ptr;
    gotoxy(0, 0);
    printf("id=%d ptr=%p eax=%08x eip=%p efl=%04x kbhits=%d jiffies=%d \n", id, ptr, regs->eax, ctx->eip, ctx->eflags, kbhits, jiffies);
    jiffies++;
}

void schedule() {
    enable();
    while (1) {
        static int count = 0;
        for (int i = 0; i < jiffies*1+10; i++) {
        gotoxy(0, 1);
        printf("hi jiffies=%d count=%d\n", jiffies, count++);
            asm volatile("hlt");
        }
        halt();
    }
}

void kernel_main(void) {
    // it seems like GRUB/multiboot doesn't zero out .bss ?
    extern char stack_bottom[0], stack_top[0];
    memset(stack_bottom, 0, stack_top - stack_bottom); // zero out BSS
    console_init();

    idt_setup();
    irq_setup();
    irq_set_handler(IRQ_TIMER, timer_handler);
    irq_set_handler(IRQ_KB, kb_handler);
    i8254_set_freq(1);
#if 1
    printf("\n\n\n");
    printf("Hello Kernel%s!\n", sizeof(void *) == 8 ? "64" : "32");
    uint16_t cs, ds;
    asm volatile(
        "mov %%cs,%%ax\n"
        "mov %%ax,%0\n"
        : "=g"(cs)
        :
        : "ax"
    );
    asm volatile(
        "mov %%ds,%%ax\n"
        "mov %%ax,%0\n"
        : "=g"(ds)
        :
        : "ax"
    );
    printf("cs=%" PRIx16 " ds=%" PRIx16 "\n", cs, ds);
    extern char stack_bottom[0], stack_top[0];
    printf("stack_bottom=%p stack_top=%p\n", stack_bottom, stack_top);
    printf("\n\n");
#endif
//    printf("%01x\n", 0xa);
    dump_gdt();

    schedule();

    gotoxy(0, 1);
    panic("[toto=%s dead=%" PRIx16 " ffff0=%p ac1dc0d3=%" PRIx32 " 1234=%d]", "toto", 0xdead, 0xffff0, 0xac1dc0d3, 1234);
}
